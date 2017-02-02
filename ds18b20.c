/*
 * ds18b20.c
 *
 *  Created on: 20/09/2016
 *      Author: andru
 *
 *      DS18B20 nRF24LE1 driver
 *
 */

// P1.3 - номер пина DS18B20
#define DSPIN	GPIO_PIN_ID_P1_3

#include <stdint.h>

#include "gpio.h"
#include "delay.h"
#include "interrupt.h"

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
	delay_us(60);
	r = !gpio_pin_val_read(DSPIN);
	delay_us(440);
	return r;
}

static void OneWireOutByte(uint8_t d)
{
	uint8_t n;

	for(n = 8; n > 0; n--) {
		if ((d & 0x01) == 1) {
		   gpio_pin_configure(DSPIN,
				   GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT
				   | GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_CLEAR
		   );
		   delay_us(2); //5
		   gpio_pin_configure(DSPIN,
				   GPIO_PIN_CONFIG_OPTION_DIR_INPUT
		   );
		   delay_us(60);
		}
		else {
    	  gpio_pin_configure(DSPIN,
    			  GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT
				  | GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_CLEAR
    	  );
    	  delay_us(60);
    	  gpio_pin_configure(DSPIN,
    			  GPIO_PIN_CONFIG_OPTION_DIR_INPUT
    	  );
		}
		d = d>>1;
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
		delay_us(2); // 5
		gpio_pin_configure(DSPIN,
				GPIO_PIN_CONFIG_OPTION_DIR_INPUT
		);
		delay_us(2); // 5
		b = gpio_pin_val_read(DSPIN);
		delay_us(50);
		d = (d >> 1) | (b << 7);
	}
	return d;
}

dserror_t ds18b20_read(float *temp)
{
    uint8_t SignBit;
    uint8_t data[2];
    int16_t TReading;
    uint16_t convtm;

	gpio_pin_configure(DSPIN,
			GPIO_PIN_CONFIG_OPTION_DIR_INPUT
	);

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
	while (OneWireInByte() != 0xff && --convtm > 0);
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

	data[0] = OneWireInByte();
	data[1] = OneWireInByte();

	gpio_pin_configure(DSPIN,
			GPIO_PIN_CONFIG_OPTION_DIR_INPUT
	);

	TReading = (int16_t)(data[1] << 8) | (int16_t)data[0];
	SignBit = TReading & 0x8000;
	if (SignBit) {
		TReading = (TReading ^ 0xffff) + 1;
	}

	*temp = (float)((6 * TReading) + TReading / 4) / 100;

	return DS_NO_ERROR;
}

