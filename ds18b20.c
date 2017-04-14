/*
 * ds18b20.c
 *
 *  Created on: 20/09/2016
 *      Author: andru
 *
 *      DS18B20 nRF24LE1 driver
 *
 */

// P1.3 - DS18B20 pin
#define DSPIN	GPIO_PIN_ID_P1_3

#include <stdint.h>

#include "gpio.h"
#include "delay.h"
#include "interrupt.h"

#include "crc8.h"
#include "ds18b20.h"

static uint8_t OneWireReset(void)
{
	uint8_t r = 1;
	gpio_pin_configure(DSPIN,
			GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT
			| GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_CLEAR
	);
	delay_us(480);
	gpio_pin_configure(DSPIN,
			GPIO_PIN_CONFIG_OPTION_DIR_INPUT
	);
	delay_us(80);
	r = !gpio_pin_val_read(DSPIN);
	delay_us(400);
	return r;
}

static void OneWireOutByte(uint8_t d)
{
	uint8_t n;

	for(n = 8; n > 0; n--) {
		if (d & 0x01) {
		   gpio_pin_configure(DSPIN,
				   GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT
				   | GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_CLEAR
		   );
		   delay_us(1);
		   gpio_pin_configure(DSPIN,
				   GPIO_PIN_CONFIG_OPTION_DIR_INPUT
		   );
		   delay_us(49);
		}
		else {
    	  gpio_pin_configure(DSPIN,
    			  GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT
				  | GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_CLEAR
    	  );
    	  delay_us(49);
    	  gpio_pin_configure(DSPIN,
    			  GPIO_PIN_CONFIG_OPTION_DIR_INPUT
    	  );
    	  delay_us(1);
		}
		d = d >> 1;
	}
}

static uint8_t OneWireInByte(void)
{
	uint8_t d = 0, n, b = 0;

	for (n = 0; n < 8; n++) {
		gpio_pin_configure(DSPIN,
				GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT
				| GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_CLEAR
		);
		delay_us(1);
		gpio_pin_configure(DSPIN,
				GPIO_PIN_CONFIG_OPTION_DIR_INPUT
		);
		delay_us(2);
		b = gpio_pin_val_read(DSPIN);
		delay_us(50);
		d = (d >> 1) | (b << 7);
	}
	return d;
}

// read DS18B20 tempearature in 1/10 C
dserror_t ds18b20_read(int *temp)
{
    uint8_t i, data[9], crc = 0;
    uint16_t trow, convtm;
    float decimal;

	if (!OneWireReset()) {
		return DS_NOT_FOUND;
	}
	// 0xCC skip ROM
	OneWireOutByte(SKIP_ROM_CMD);

	// setting precision 10bit
	OneWireOutByte(WRITE_SCR_CMD);
	OneWireOutByte(REG_TH);
	OneWireOutByte(REG_TL);
	OneWireOutByte(REG_CONFIG);

	if (!OneWireReset()) {
		return DS_NOT_FOUND;
	}
	// 0xCC skip ROM
	OneWireOutByte(SKIP_ROM_CMD);
	// 0x44 start conversion
	OneWireOutByte(START_CONV_CMD);

	// wait while temperature value not ready
	convtm = 400;	// ~200ms
	while (!gpio_pin_val_read(DSPIN) && --convtm > 0);
	// conversion timeout
	if (convtm == 0) {
		return DS_TIMEOUT;
	}

	if (!OneWireReset()) {
		return DS_NOT_FOUND;
	}

	OneWireOutByte(SKIP_ROM_CMD);
	// 0xbe get temperature from ram
	OneWireOutByte(READ_SCR_CMD);

	for (i = 0; i < 9; i++) {
		/* Read byte by byte */
		data[i] = OneWireInByte();
	}

	if (CRC8(data, 8) != data[8]) {
		return DS_CRC_ERROR;
	}

	/* First two bytes of scratchpad are temperature values */
	trow = ((int16_t) data[1] << 8) | data[0];

	/* Check if temperature is negative */
	i = 0;
	if (trow & 0x8000) {
		/* Two's complement, temperature is negative */
		trow = ~trow + 1;
		i = 1;
	}

	decimal = (float)((int16_t) trow >> 2) / 4;	// 10bit
	if (i) {
		decimal = 0 - decimal;
	}

	*temp = (int) (decimal * 10);

	return DS_NO_ERROR;
}
