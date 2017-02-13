/*
 *  Created on: 20/09/2016
 *      Author: andru
 *
 *      nRF24LE1 remote sensors unit
 *      support DHT21/22, DS18B20, BH1750, ADC sensors, AES encryption
 *
 *		based on great nRF24LE1 SDK https://github.com/DeanCording/nRF24LE1_SDK
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "gpio.h"
#include "delay.h"
#include "memory.h"
#include "adc.h"

#include "main.h"
#include "radio.h"
#include "dht.h"
#include "ds18b20.h"
#include "bh1750.h"
#include "crc8.h"

#define CMDTMOUT		18000	// server command wait time in ~20us intervals

#if DEBUG
#define EN_UART			1	// use UART for debugging
#define UARTTXPIN		GPIO_PIN_ID_P0_3		// P0.3 - UART TX
#define UARTRXPIN		GPIO_PIN_ID_P0_4		// P0.4	- UART RX
#else
#define EN_UART			0	// use UART for debugging
#endif

#define LEDPIN	GPIO_PIN_ID_P0_1		// P0.1 - LED
//#define DHTPIN	GPIO_PIN_ID_P1_4		// P1.4 - DHT21/22
//#define DSPIN	GPIO_PIN_ID_P1_3		// P1.3 - DS18B20
#define VBATCH	ADC_CHANNEL_AIN3		// P0.3 - VBAT
#define LIGHTCH	ADC_CHANNEL_AIN0		// P0.0 - ADC LIGHT
//#define W2SCL	GPIO_PIN_ID_P0_4		// P0.4 - BH1750 2-wire SCL
//#define W2SDA	GPIO_PIN_ID_P0_5		// P0.5 - BH1750 2-wire SDA
#define WDGPIN	GPIO_PIN_ID_P1_5		// P1.5 - sleep enable switch

#define NVM_START_ADDRESS	MEMORY_FLASH_NV_STD_END_START_ADDRESS
#define ENVM_START_ADDRESS	MEMORY_FLASH_NV_EXT_END_START_ADDRESS
#define ENVM_PAGE_NUM		MEMORY_FLASH_NV_EXT_END_FIRST_PAGE_NUM

#if EN_UART
#include "uart.h"
#endif

#if EN_SLEEP
#include "watchdog.h"
#include "pwr_clk_mgmt.h"
#endif

CONFIG_T config;

// halt
static void halt(void) {
	while (1) {
#if EN_LED
		gpio_pin_val_complement(LEDPIN);
		delay_ms(250);
#endif
	}
}

// write NVM config to eNVM
static uint8_t write_config(void) {
	uint8_t ret = CRC8((uint8_t *) &config, sizeof(CONFIG_T) - 1);
	config.crcbyte = ret;
	if (memory_flash_erase_page(ENVM_PAGE_NUM) != MEMORY_FLASH_OK)
		return 0;
	if (memory_flash_write_bytes(ENVM_START_ADDRESS, sizeof(CONFIG_T), (uint8_t *) &config) != MEMORY_FLASH_OK)
		return 0;
	return 1;
}

static void read_config(uint16_t addr) {
	uint16_t i;
	memory_movx_accesses_data_memory();
	for (i = 0; i < sizeof(CONFIG_T); i++) {
		*((uint8_t*) &config + i) = *((__xdata uint8_t*) addr + i);
	}
}

static void send_config(uint8_t addr, uint16_t value) {
	MESSAGE_T message;

#if DEBUG
	printf("addr=%d, value=%d\r\n", addr, (uint16_t) value);
#endif
	message.msgType = SENSOR_INFO;
	message.deviceID = config.deviceID;
	message.address = addr;
	message.data.iValue = (uint16_t) value;
	rfsend(&message);
}

static void send_config_err(uint8_t addr, uint8_t errcode) {
	MESSAGE_T message;

#if DEBUG
	printf("addr=%d, config error=%d\r\n", addr, errcode);
#endif
	message.msgType = SENSOR_ERROR;
	message.deviceID = config.deviceID;
	message.address = addr;
	message.error = errcode;
	rfsend(&message);
}


// main
void main(void) {

	// variable definition
	uint8_t ret, cmd;
	uint16_t value;
    MESSAGE_T message;

#if EN_DHT
	int DHTTemp, DHTHum;
#endif
#if EN_DS18B20
    int DSTemp;
#endif

	// program code
#if EN_LED
	gpio_pin_configure(LEDPIN,
		GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT
		| GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_CLEAR
		| GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH
		);
#endif

#if EN_UART
	// Setup UART pins
	gpio_pin_configure(GPIO_PIN_ID_FUNC_RXD,
		GPIO_PIN_CONFIG_OPTION_DIR_INPUT |
		GPIO_PIN_CONFIG_OPTION_PIN_MODE_INPUT_BUFFER_ON_NO_RESISTORS
		);

	gpio_pin_configure(GPIO_PIN_ID_FUNC_TXD,
		GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT |
		GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET |
		GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH
		);

	uart_configure_8_n_1_38400();
#endif

	read_config(NVM_START_ADDRESS);
	ret = CRC8((uint8_t *) &config, sizeof(CONFIG_T)-1);
	if (config.crcbyte != ret) {
		// NVM config wrong stop work
		halt();
	}

	value = config.version;
	read_config(ENVM_START_ADDRESS);
	ret = CRC8((uint8_t *) &config, sizeof(CONFIG_T)-1);
	if (config.crcbyte != ret || config.version != value) {
		read_config(NVM_START_ADDRESS);
		if (!write_config()) {
			// config write error stop work
			halt();
		}
	}

#if EN_ADC_LIGHT || EN_VBAT
	adc_configure((uint16_t)
		  ADC_CONFIG_OPTION_RESOLUTION_10_BITS
		  | ADC_CONFIG_OPTION_REF_SELECT_VDD
		  | ADC_CONFIG_OPTION_RESULT_JUSTIFICATION_RIGHT
		  | ADC_CONFIG_OPTION_ACQ_TIME_12_US
	);
#endif

#if EN_BH1750
    bh1750_init();
#endif

#if EN_RF
 	radio_init();
#endif

	delay_ms(20);

	while(1) {

		message.deviceID = config.deviceID;

#if EN_VBAT
		value = adc_start_single_conversion_get_value(VBATCH);
		message.msgType = SENSOR_DATA;
		message.address = ADDR_VBAT;
		message.sensorType = ADC;
		message.valueType = VOLTAGE;
		message.data.iValue = value;
#if DEBUG
		printf("vbat=%d, vlow=%d\r\n", value, config.vbatlow);
#endif
		rfsend(&message);
		if (config.vbatlow > 0 && value < config.vbatlow) {
			message.msgType = SENSOR_ERROR;
			message.error = VBAT_LOW;
			rfsend(&message);
			goto VBATLOW;				// goto powersave
		}
#endif

WAITCMD:

		// wait command from smarthome gateway
		cmd = rfread(&message, CMDTMOUT);

#if EN_LED
		gpio_pin_val_complement(LEDPIN);
#endif

		if (cmd && message.deviceID == config.deviceID && message.msgType == SENSOR_CMD) {
#if DEBUG
			printf("\r\ncommand: %d\r\n", message.command);
			printf("address: %d\r\n", message.address);
			printf("param: %d\r\n\r\n", (uint16_t) message.data.iValue);
#endif
			// команда чтения/записи конфигурации
			if ((message.command == CMD_CFGREAD || message.command == CMD_CFGWRITE)) {
				switch (message.address) {
				case CFG_MAXSEND:
					if (message.command == CMD_CFGREAD) {
						send_config(CFG_MAXSEND, config.maxsend);
					} else {
						if (message.data.iValue > 50) {
						    send_config_err(CFG_MAXSEND, 2);
						    break;
						}
						config.maxsend = (uint8_t) message.data.iValue;
						if (!write_config())	{
							send_config_err(CFG_MAXSEND, 1);
							break;
						}
						send_config(CFG_MAXSEND, config.maxsend);
					}
					break;
				case CFG_SLEEP:
					if (message.command == CMD_CFGREAD) {
						send_config(CFG_SLEEP, config.sleeptm);
					} else {
						if (message.data.iValue > 512) {
						    send_config_err(CFG_MAXSEND, 2);
						    break;
						}
						config.sleeptm = message.data.iValue;
						if (!write_config())	{
							send_config_err(CFG_SLEEP, 1);
							break;
						}
						send_config(CFG_SLEEP, config.sleeptm);
					}
					break;
#if EN_VBAT
				case CFG_VBATLOW:
					if (message.command == CMD_CFGREAD) {
						send_config(CFG_VBATLOW, config.vbatlow);
					} else {
						if (message.data.iValue > 1020) {
						    send_config_err(CFG_MAXSEND, 2);
						    break;
						}
						config.vbatlow = (uint16_t) message.data.iValue;
						if (!write_config()) {
							send_config_err(CFG_VBATLOW, 1);
							break;
						}
						send_config(CFG_VBATLOW, config.vbatlow);
					}
					break;
#endif
				default:
					break;
				}
			}
			goto WAITCMD;
		}

		message.deviceID = config.deviceID;
#if EN_ADC_LIGHT
		value = adc_start_single_conversion_get_value(LIGHTCH);
		message.msgType = SENSOR_DATA;
		message.address = ADDR_ADC_LIGHT;
		message.sensorType = ADC;
		message.valueType = LIGHT;
		message.data.iValue = value;
		rfsend(&message);
#endif
#if EN_BH1750
		message.msgType = SENSOR_DATA;
		message.address = ADDR_BH1750;
		message.sensorType = BH1750;
		message.valueType = LIGHT;
		ret = bh1750_read(&value);
		if (ret == BH1750_NO_ERROR) {
			message.data.iValue = value;
		} else {
			message.msgType = SENSOR_ERROR;
			message.error = ret;
		}
		rfsend(&message);
#endif
#if EN_DS18B20
		message.msgType = SENSOR_DATA;
		message.address = ADDR_DS18B20;
		message.sensorType = DS1820;
		message.valueType = TEMPERATURE;
		ret = ds18b20_read(&DSTemp);
		if (ret == DS_NO_ERROR) {
			// DS18B20 temperature send
			message.data.iValue = DSTemp;
		} else {
			message.msgType = SENSOR_ERROR;
			message.error = ret;
		}
		rfsend(&message);
#endif	//DS18B20
#if EN_DHT
		// delay after startup & reset need to be > 350ms :(
		message.sensorType = DHT;
		ret = dht_read(&DHTTemp, &DHTHum);
		if (ret != DHT_NO_ERROR) {
			message.error = ret;
			message.msgType = SENSOR_ERROR;
			message.address = ADDR_DHT_TEMP;
			rfsend(&message);
			message.msgType = SENSOR_ERROR;
			message.address = ADDR_DHT_HUM;
			rfsend(&message);
		}
		else {
			// DHT temperature, humidity send
			message.msgType = SENSOR_DATA;
			message.address = ADDR_DHT_TEMP;
			message.valueType = TEMPERATURE;
			message.data.iValue = DHTTemp;
			rfsend(&message);
			message.address = ADDR_DHT_HUM;
			message.valueType = HUMIDITY;
			message.data.iValue = DHTHum;
			rfsend(&message);
		}
#endif	//DHT

// battery low force to powersave
VBATLOW:

#if EN_SLEEP
		if (gpio_pin_val_read(WDGPIN) && (config.sleeptm > 0)) {
			// clear unneeded pins
#if EN_LED
			gpio_pin_val_clear(LEDPIN);
#endif
#if EN_ADC_LIGHT || EN_VBAT
			adc_power_down();
#endif
#if EN_BH1750
			bh1750_stop();
#endif
#if EN_RF
			rfpwrDown();
#endif

			watchdog_setup();
			watchdog_set_wdsv_count(watchdog_calc_timeout_from_sec(config.sleeptm));
			pwr_clk_mgmt_op_mode_configure(
				PWR_CLK_MGMT_OP_MODE_CONFIG_OPTION_RUN_WD_NORMALLY
				| PWR_CLK_MGMT_OP_MODE_CONFIG_OPTION_RETENTION_LATCH_LOCKED
			);

			pwr_clk_mgmt_enter_pwr_mode_memory_ret_tmr_on(); // 1mkA
		}
#endif

   		delay_s(10);

	} // main loop
}
