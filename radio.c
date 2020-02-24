#include <stdio.h>
#include <string.h>

#include "rf.h"
#include "interrupt.h"
#include "main.h"
#include "crc8.h"

#if EN_AES
#include "tiny-AES128/include/aes.h"
#endif

///////////////////////////////////////////
// Inline function definitions
///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// static __INLINE uint8_t enc_dec_accel_galois_multiply(uint8_t a, uint8_t b) __reentrant
//
// Description:
//  Performs a GF(2^8) mutliplication in hardware and returns the result
//
// Parameters:
//  uint8_t a - multiplier in the operation
//  uint8_t b - multiplicand in the operation
//
// Return value:
//  Result of the GF(2^8) multiplication
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//See enc_dec_accel_galois_multiply.c for function details (function body is the same)
unsigned char enc_dec_accel_galois_multiply(unsigned char a, unsigned char b)
{
	//Load the hardware registers for the operators
	CCPDATIA = a;
	CCPDATIB = b;

	//Return the result of the hardware GF(2^8) multiplication
	return CCPDATO;
}

void rfinit() {
	uint8_t setup;

	rf_spi_configure_enable();

	setup = RF_CONFIG_EN_CRC;									// power up here, CRC 1 bytes
	rf_power_up_param(false, setup);

	setup = RF_SETUP_RETR_ARD_750 | RF_SETUP_RETR_ARC_3; 		// 750us, 3 retry
	rf_write_register(RF_SETUP_RETR, &setup, 1);

	setup = RF_SETUP_AW_5BYTES;									// 5 bytes address
	rf_write_register(RF_SETUP_AW, &setup, 1);

	setup = RF_EN_RXADDR_ERX_P0 | RF_EN_RXADDR_ERX_P1;			// enable pipe0 & pipe1
	rf_write_register(RF_EN_RXADDR, &setup, 1);

	rf_write_register(RF_RX_ADDR_P0, config.sndaddr, 5);
	rf_write_register(RF_TX_ADDR, config.sndaddr, 5);

	config.rcvaddr[4] = config.deviceid;
	rf_write_register(RF_RX_ADDR_P1, config.rcvaddr, 5);

	setup = MSGLEN;												/* Number of bytes in RX payload		*/
	rf_write_register(RF_RX_PW_P0, &setup, 1);
	rf_write_register(RF_RX_PW_P1, &setup, 1);

	setup = RF_EN_AA_ENAA_P0 | RF_EN_AA_ENAA_P1;			// set autoask
	rf_write_register(RF_EN_AA, &setup, 1);

	rf_set_rf_channel(config.channel);

	rf_set_output_power(RF_RF_SETUP_RF_PWR_0_DBM);

	setup &= ~(RF_RF_SETUP_RF_DR_LOW | RF_RF_SETUP_RF_DR_HIGH);	// default datarate 1Mhz
	rf_write_register(RF_RF_SETUP, &setup, 1);

	interrupt_control_rfirq_enable();
}

void rfdown(void) {
    rf_irq_clear_all();
	rf_power_down();
}

void rfsend(const MESSAGE_T *msg) {
	uint16_t timeout;
	uint8_t retry = config.maxsend;
	uint8_t buf[MSGLEN], out[MSGLEN];

	if (!(rf_read_register_1_byte(RF_CONFIG) & RF_CONFIG_PWR_UP))
		rfinit();

	memcpy(&buf, msg, MSGLEN);
#if EN_CRC
	buf[MSGLEN-1] = CRC8((uint8_t *) msg, MSGLEN-1);
#endif

#if EN_AES
	AES128_ECB_encrypt((uint8_t*) buf, config.aeskey, out);
#else
	memcpy(&out, &buf, MSGLEN);
#endif

start:
	if (rf_tx_fifo_is_full())
		rf_flush_tx();

	rf_irq_clear_all(); //clear all interrupts in the 24L01
	rf_set_as_tx(); //resume normal operation as a TX

	rf_write_tx_payload(out, MSGLEN, true); //transmit received char over RF

	//wait until the packet has been sent or the maximum number of retries has been reached
	timeout = 90;	// 90*26us = 2.3ms > 750uS * 3
	while(! rf_irq_pin_active()) {
		if (timeout-- == 0) { // checking timeout if nothing
			if (retry-- > 0) goto start;
			break;
		}
		delay_us(10);	// 10us
	}
	if (rf_irq_pin_active() && rf_irq_max_rt_active()) { // checking max_rt bit
		if (retry-- > 0) goto start;
	}

    rf_irq_clear_all();
}

// blocking read NRF24LE1, timeout in 10us intervals
uint8_t rfread(MESSAGE_T *msg, uint16_t timeout) {
	uint8_t status, state = 0;
	uint8_t buf[MSGLEN], in[MSGLEN];

	if (!(rf_read_register_1_byte(RF_CONFIG) & RF_CONFIG_PWR_UP))
		rfinit();

	rf_set_as_rx(true); //change the device to an RX to get the character back from the other 24L01
	rf_irq_clear_all(); //clear interrupts again

	while (timeout-- > 0) {
		//wait a while to see if we get the data back (change the loop maximum and the lower if
		//  argument (should be loop maximum - 1) to lengthen or shorten this time frame
		if(rf_irq_pin_active() && rf_irq_rx_dr_active()) {
			status = rf_read_rx_payload(buf, MSGLEN); //get the payload into data
			if (rf_is_rxed_payload_on_pipe_1_in_status_val(status)) {
				state = 1;
				break;
			}
		}
		delay_us(10);	// 10us
	}

    rf_irq_clear_all();

	if (!state)
		return state;

#if EN_AES
	AES128_ECB_decrypt(buf, config.aeskey, in);
#else
	memcpy(&in, &buf, MSGLEN);
#endif
#if EN_CRC
	if (in[MSGLEN-1] != CRC8(in, MSGLEN-1)) {
		return 0;
	}
#endif
	memcpy(msg, &in, MSGLEN);

	return state;
}
