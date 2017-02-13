/*
 * bh1750.c
 *
 *  Created on: 21/09/2016
 *      Author: andru
 *
 *      BH1750 - nRF24LE1 light sensor driver
 *
 */

#define BH1750_ADDR 		0x23 // device address
#define BH1750_PWR_DOWN		0x0	 // No active state.
#define BH1750_CONT_HMODE	0x10 // Continuously H-Resolution Mode
#define BH1750_DELAY		200	 // wait results (datasheet says max. 180ms)

#include "delay.h"
#include "w2.h"

#include "bh1750.h"

void bh1750_init(void) {
	w2_configure(W2_CONFIG_OPTION_ENABLE |
				 W2_CONFIG_OPTION_MODE_MASTER |
				 W2_CONFIG_OPTION_CLOCK_FREQ_100_KHZ |
				 W2_CONFIG_OPTION_ALL_INTERRUPTS_ENABLE,
				 0
				 );
}

void bh1750_stop(void) {
	w2_disable();
}

bh1750error_t bh1750_read(uint16_t *light) {
	uint8_t addr, txbuf=0, rxbuf[2];

	addr = BH1750_CONT_HMODE;
	if (w2_master_write_to(BH1750_ADDR, &addr, 1, &txbuf, 0) == W2_NACK_VAL)
		return BH1750_TIMEOUT;

	delay_ms(BH1750_DELAY);

	if (w2_master_cur_address_read(BH1750_ADDR, rxbuf, 2) == W2_NACK_VAL)
		return BH1750_ERROR;

	addr = BH1750_PWR_DOWN;
	w2_master_write_to(BH1750_ADDR, &addr, 1, &txbuf, 0);

	*light = (uint32_t) ((rxbuf[0] << 8) + rxbuf[1]) * 10 / 12;

	return BH1750_NO_ERROR;
}

