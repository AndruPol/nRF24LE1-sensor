#include <stdio.h>
#include <string.h>

#include "rf.h"
#include "main.h"

#if EN_AES
#include "aes.h"
#endif

#define _BV(bit) 		(1 << (bit))

#define RF_DR_LOW		5
#define RF_DR_HIGH		3
#define RF_EN_CRC		3
#define RF_CRCO			2
#define PWR_UP			1
#define RF_PWR_LOW		1
#define RF_PWR_HIGH		2

void radio_init() {
	uint8_t setup;

	rf_spi_configure_enable();

	setup = RF_CONFIG_EN_CRC | RF_CONFIG_CRCO | RF_CONFIG_PWR_UP;		// power up here, CRC 2 bytes
	rf_write_register(RF_CONFIG,&setup, 1) ;

	delay_ms(2);

	setup = RF_SETUP_RETR_ARD_1000 | RF_SETUP_RETR_ARC_3; 		// 1000us, 3 retry
	rf_write_register(RF_SETUP_RETR, &setup, 1);

	setup = RF_SETUP_AW_5BYTES;									// 5 bytes address
	rf_write_register(RF_SETUP_AW, &setup, 1);

	setup = RF_RF_SETUP_RF_PWR_0_DBM;							// 0 dbm power, 1Mhz
	rf_write_register(RF_RF_SETUP, &setup, 1);

	rf_flush_tx();
}


// ------------ setChannel ----------------
void setChannel(unsigned char channel){
	rf_write_register(RF_RF_CH, &channel, 1);
}


// ------------ setDataRate ----------------
void setDataRate(unsigned char speed){
	unsigned char buffer;
	rf_read_register(RF_RF_SETUP, &buffer, 1);

	buffer &= ~(_BV(RF_DR_LOW) | _BV(RF_DR_HIGH)) ;	// default speed=2, 1Mhz

	if(speed == 1)
		buffer |= _BV(RF_DR_LOW);
	else if (speed == 3)
		buffer |= _BV(RF_DR_HIGH);

	rf_write_register(RF_RF_SETUP, &buffer, 1) ;
}


//--------------setAutoAck------------
void setAutoAck(unsigned char enable) {
	unsigned char buffer;

	buffer=0;
	if (enable == 1)
		buffer = RF_EN_AA_ENAA_P0 | RF_EN_AA_ENAA_P1;	//63
	rf_write_register(RF_EN_AA, &buffer, 1);
}


//--------------setCRCLength------------
void setCRCLength(unsigned char length) {
	unsigned char buffer;

	rf_read_register(RF_CONFIG,&buffer, 1);
	
	buffer = buffer & ~( _BV(RF_CRCO) | _BV(RF_EN_CRC)) ;

	if (length == 1)
		buffer |= _BV(RF_EN_CRC);
	else if (length == 2)
		buffer |= _BV(RF_EN_CRC) | _BV( RF_CRCO );

	rf_write_register(RF_CONFIG, &buffer, 1 ) ;
}

void setPALevel(unsigned char level) {
	uint8_t setup;
 
	rf_read_register(RF_RF_SETUP, &setup, 1) ;

	setup &= ~(_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH)) ;
	setup  | (level&3) << RF_PWR_LOW;

	rf_write_register(RF_RF_SETUP, &setup, 1);
}

void openAllPipe(CONFIG_T *config) {
	uint8_t setup;

	setup = RF_EN_RXADDR_ERX_P0 | RF_EN_RXADDR_ERX_P1;
	rf_write_register(RF_EN_RXADDR, &setup, 1);

	rf_write_register(RF_RX_ADDR_P0, config->srvaddr, 5);
	rf_write_register(RF_TX_ADDR, config->srvaddr, 5);

	config->devaddr[4] = config->deviceID;
	rf_write_register(RF_RX_ADDR_P1, config->devaddr, 5);

	setup = MSGLEN;	/* Number of bytes in RX payload		*/
	rf_write_register(RF_RX_PW_P0, &setup, 1);
	rf_write_register(RF_RX_PW_P1, &setup, 1);
}

void rfsend(const MESSAGE_T *msg) {
	uint16_t timeout;
	uint8_t retry = config.maxsend;
#if EN_AES
	MESSAGE_T buf;
	if (config.useaes) {
		aes_encrypt_ecb(&aes_data, (unsigned char *) msg, (unsigned char *) buf);
	}
	else {
		memcpy(&buf, msg, MSGLEN);
	}
#endif
start:
	timeout = 300;	// 300*10us = 3ms
	if (rf_tx_fifo_is_full())
		rf_flush_tx();

	rf_irq_clear_all(); //clear all interrupts in the 24L01
#if EN_AES
	rf_write_tx_payload((uint8_t*) buf, MSGLEN, true); //transmit received char over RF
#else
	rf_write_tx_payload((uint8_t*) msg, MSGLEN, true); //transmit received char over RF
#endif

	//wait until the packet has been sent or the maximum number of retries has been reached
	while(!(rf_irq_pin_active() && (rf_irq_tx_ds_active() || rf_irq_max_rt_active()))) {
		if (timeout-- == 0) { // checking timeout if nothing
			if (retry-- > 0)
				goto start;
			else
				break;
		}
		delay_us(10);	// 10us
	}
	if (rf_irq_pin_active() && rf_irq_max_rt_active()) { // checking max_rt bit
		if (retry-- > 0)
			goto start;
	}

	rf_irq_clear_all(); //clear all interrupts in the 24L01
}

// blocking read NRF24LE1, timeout in 10us intervals
uint8_t rfread(MESSAGE_T *msg, uint16_t timeout) {
	uint8_t status, state = 0;
#if EN_AES
	MESSAGE_T buf;
#endif
	rf_set_as_rx(true); //change the device to an RX to get the character back from the other 24L01
	rf_irq_clear_all(); //clear interrupts again

	while (timeout-- > 0) {
		//wait a while to see if we get the data back (change the loop maximum and the lower if
		//  argument (should be loop maximum - 1) to lengthen or shorten this time frame
		if((rf_irq_pin_active() && rf_irq_rx_dr_active())) {
#if EN_AES
			status = rf_read_rx_payload((uint8_t *) buf, MSGLEN); //get the payload into data
#else
			status = rf_read_rx_payload((uint8_t *) msg, MSGLEN); //get the payload into data
#endif
			if (rf_is_rxed_payload_on_pipe_1_in_status_val(status)) {
				state = 1;
				break;
			}
		}
		delay_us(10);	// 10us
	}
#if EN_AES
	if (state) {
		if (config.useaes) {
			aes_decrypt_ecb(&aes_data, (unsigned char *) buf, (unsigned char *) msg);
		}
		else {
			memcpy(msg, &buf, MSGLEN);
		}
	}
#endif
	rf_irq_clear_all(); //clear interrupts again
	rf_set_as_tx(); //resume normal operation as a TX
	return state;
}

void rfpwrDown(void) {
    rf_irq_clear_all();
    rf_power_down();
}
