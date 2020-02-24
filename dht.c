/*
 * dht.c
 *
 *  Created on: 20/09/2016
 *      Author: andru
 *
 *      DHT21/22 nRF24LE1 driver
 */

#include <stdint.h>

#include "gpio.h"
#include "delay.h"

#include "dht.h"

#define DHTUSEP0	0	// P0.X
#define DHTUSEP1	1	// P1.X

#define PIN0XVAL(p) ((P0 & (1 << (p % 8))) > 0 ? 1 : 0)		// read P0.X
#define PIN1XVAL(p) ((P1 & (1 << (p % 8))) > 0 ? 1 : 0)		// read P1.X

// P1.4 - номер пина DHT21/22
#define DHTPIN		GPIO_PIN_ID_P1_4

static uint8_t waitpin(uint8_t val)
{
	uint8_t readtm = 80;
#if DHTUSEP0
	while ((PIN0XVAL(DHTPIN) != val) && (--readtm > 0));	// for P0.X only
#endif
#if DHTUSEP1
	while ((PIN1XVAL(DHTPIN) != val) && (--readtm > 0));	// for P1.X only
#endif
	if (readtm == 0) {
		return 0;
	}
	return 1;
}

void dht_init(void) {
	gpio_pin_configure(DHTPIN,
			GPIO_PIN_CONFIG_OPTION_DIR_INPUT
			| GPIO_PIN_CONFIG_OPTION_PIN_MODE_INPUT_BUFFER_ON_NO_RESISTORS
	);
}

/* temperature in 1/10 deg C, humidity in 1/10 % */
dhterror_t dht_read(int16_t *temp, uint16_t *hum) {
	uint8_t j, i;
	uint8_t datadht[5] = {0,0,0,0,0};
	uint8_t crcdata = 0;

	//pin as output and set 0
	gpio_pin_configure(DHTPIN,
			GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT
			| GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_CLEAR
			| GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH
	);

	delay_ms(1);	// reset 1-20ms

	//pin as input
	gpio_pin_configure(DHTPIN,
			GPIO_PIN_CONFIG_OPTION_DIR_INPUT
			| GPIO_PIN_CONFIG_OPTION_PIN_MODE_INPUT_BUFFER_ON_NO_RESISTORS
	);

	//=============check DHT response
	if (!waitpin(0)) {
		return DHT_NO_RESPONSE;
	}
	if (!waitpin(1)) {
		return DHT_NO_RESPONSE;
	}
	//===============receive 40 data bits
	if (!waitpin(0)) {
		return DHT_NO_RESPONSE;
	}

	for (j = 0; j < 5; j++) {
		for(i = 0; i < 8; i++) {
			if (!waitpin(1)) {
				return DHT_TIMEOUT_ERROR;
			}
			delay_us(30);
			if (gpio_pin_val_read(DHTPIN))
				datadht[j] |= 1 << (7-i);
			if (!waitpin(0)) {
				return DHT_TIMEOUT_ERROR;
			}
        }
    }

	if (datadht[0] == 0 && datadht[1] == 0 && datadht[2] == 0 && datadht[3] == 0) {
    	return DHT_NO_VALUE;
    }

	for(i = 0; i < 4; i++) {
		crcdata += datadht[i];
	}
    if ((crcdata & 0xff) != datadht[4]) {	// CRC check
    	return DHT_CHECKSUM_ERROR;
    }

    if ((datadht[1] == 0) && (datadht[3] == 0)) {
    	// dht11
	    *hum = datadht[2]*10;
	    *temp = datadht[0]*10;
    }
    else {
    	// dht22
    	*hum = ((uint16_t) datadht[0] << 8) | (uint16_t) datadht[1];
    	*temp = (((uint16_t) datadht[2] & 0x7F) << 8) | (uint16_t) datadht[3];
    	if (datadht[2] & 0x80) *temp *= -1;
	}

	return DHT_OK;
}
