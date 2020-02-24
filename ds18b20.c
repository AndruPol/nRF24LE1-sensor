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

// DS18B20 temperature convertation to int16_t
#define BIT2_1		(1<<3)
#define BIT2_2      (1<<2)
#define BIT2_3      (1<<1)
#define BIT2_4      (1<<0)

#define POWER2_1    (5000)
#define POWER2_2    (2500)
#define POWER2_3    (1250)
#define POWER2_4    (0650)

// return DS18B20 temperature * 10
static int16_t tconv(int16_t raw) {
	int16_t frac =  ((raw & BIT2_1) ? POWER2_1 : 0) +
                    ((raw & BIT2_2) ? POWER2_2 : 0) +
                    ((raw & BIT2_3) ? POWER2_3 : 0) +
                    ((raw & BIT2_4) ? POWER2_4 : 0);
    frac = (frac - frac / 1000) >= 500 ? frac + 500 : frac - 500;
    frac = (raw & (1<<15)) ? (-1 * frac / 1000) : (frac / 1000);
    return (10 * (raw >> 4) + frac);
}

static uint8_t OneWireReset(void)
{
	uint8_t r = 1;
	gpio_pin_configure(DSPIN,
			GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT
			| GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_CLEAR
			| GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH
	);
	delay_us(480);
	gpio_pin_configure(DSPIN,
			GPIO_PIN_CONFIG_OPTION_DIR_INPUT
			| GPIO_PIN_CONFIG_OPTION_PIN_MODE_INPUT_BUFFER_ON_NO_RESISTORS
	);
	delay_us(70);
	r = !gpio_pin_val_read(DSPIN);
	delay_us(410);
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
				| GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH
		   );
		   delay_us(1);
		   gpio_pin_configure(DSPIN,
				GPIO_PIN_CONFIG_OPTION_DIR_INPUT
				| GPIO_PIN_CONFIG_OPTION_PIN_MODE_INPUT_BUFFER_ON_NO_RESISTORS
		   );
		   delay_us(50);
		}
		else {
    	  gpio_pin_configure(DSPIN,
    			GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT
				| GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_CLEAR
				| GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH
    	  );
    	  delay_us(50);
    	  gpio_pin_configure(DSPIN,
    			GPIO_PIN_CONFIG_OPTION_DIR_INPUT
				| GPIO_PIN_CONFIG_OPTION_PIN_MODE_INPUT_BUFFER_ON_NO_RESISTORS
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
				| GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH
		);
		delay_us(1);
		gpio_pin_configure(DSPIN,
				GPIO_PIN_CONFIG_OPTION_DIR_INPUT
				| GPIO_PIN_CONFIG_OPTION_PIN_MODE_INPUT_BUFFER_ON_NO_RESISTORS
		);
		delay_us(2);
		b = gpio_pin_val_read(DSPIN);
		delay_us(50);
		d = (d >> 1) | (b << 7);
	}
	return d;
}

// read DS18B20 tempearature in 1/10 C
dserror_t ds18b20_read(int16_t *temp)
{
    uint8_t i, data[9], crc = 0;

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
	delay_ms(DS18B20_WAIT);

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
	*temp = tconv((int16_t)(data[1] << 8) | data[0]);

	return DS_OK;
}

// ask DS18B20 temperature
dserror_t ds18b20_ask(void)
{
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
	return DS_OK;
}

// read DS18B20 tempearature in 1/10 C
dserror_t ds18b20_read_nowait(int16_t *temp)
{
    uint8_t i, data[9], crc = 0;

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
	*temp = tconv((int16_t)(data[1] << 8) | data[0]);

	return DS_OK;
}
