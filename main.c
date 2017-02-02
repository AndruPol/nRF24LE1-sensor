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

#define CMDTMOUT		50000	// server command wait time in 10us intervals

#if DEBUG
#define EN_UART			1	// use UART for debugging
#define UARTTXPIN		GPIO_PIN_ID_P0_3		// P0.3 - UART TX
#define UARTRXPIN		GPIO_PIN_ID_P0_4		// P0.4	- UART RX
#else
#define EN_UART			0	// use UART for debugging
#endif

#define LEDPIN	GPIO_PIN_ID_P0_1		// P0.1 - номер пина LED
#define DHTPIN	GPIO_PIN_ID_P1_4		// P1.4 - номер пина DHT21/22
//#define DSPIN	GPIO_PIN_ID_P1_3		// P1.3 - номер пина DS18B20
//#define VBATCH	ADC_CHANNEL_AIN0		// P0.0 - номер пина напряжение батареи
#define LIGHTCH	ADC_CHANNEL_AIN0		// P0.0 - номер пина датчика освещенности
//#define W2SCL	GPIO_PIN_ID_P0_4		// P0.4 - номер пина BH1750 2-wire SCL
//#define W2SDA	GPIO_PIN_ID_P0_5		// P0.5 - номер пина BH1750 2-wire SDA
#define WDGPIN	GPIO_PIN_ID_P1_5		// P1.5 - номер пина sleep enable switch

__xdata __at(0xFC00) CONFIG_T nvmconfig;
__xdata __at(0xFA00) CONFIG_T envmconfig;

CONFIG_T config;

#if EN_UART
#include "uart.h"
#endif

#if EN_SLEEP
#include "watchdog.h"
#include "pwr_clk_mgmt.h"
#endif

#if EN_AES
#include "aes/include/aes.h"
#include "aes/include/aes_user_options.h"
#include "aes/include/enc_dec_accel.h"
aes_data_t aes_data;
#endif

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
static uint8_t write_config(uint8_t * data) {
	if (memory_flash_erase_page(MEMORY_FLASH_NV_EXT_END_FIRST_PAGE_NUM) != MEMORY_FLASH_OK)
		return 0;
	if (memory_flash_write_bytes(MEMORY_FLASH_NV_EXT_END_START_ADDRESS, sizeof(CONFIG_T), data) != MEMORY_FLASH_OK)
		return 0;
	return 1;
}

void send_config(uint8_t addr, uint16_t value) {
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

void send_config_err(uint8_t addr, uint8_t errcode) {
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
	static int DHTTemp, DHTHum;
#endif

#if EN_DS18B20
    static float DSTemp;
#endif

	// program code
	ret = pwr_clk_mgmt_get_reset_reason();
	if (pwr_clk_mgmt_was_prev_reset_watchdog(ret))
		pwr_clk_mgmt_open_retention_latches();

#if EN_LED
	// настроим порт LED
	gpio_pin_configure(LEDPIN,
		GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT |
		GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET |
		GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH
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

	// check NVRAM config
	ret = CRC8((uint8_t *) &nvmconfig, sizeof(CONFIG_T) -1);
	if (nvmconfig.crcbyte != ret) {
		// NVM config wrong stop work
		halt();
	}

	// check extended endurance NVRAM config
	ret = CRC8((uint8_t *) &envmconfig, sizeof(CONFIG_T) -1);
	if (envmconfig.crcbyte != ret) {
		if (!write_config((uint8_t *) &nvmconfig)) {
			// config write error stop work
			halt();
		}
	}

	// new version of NVRAM config detected
	if (nvmconfig.version != envmconfig.version) {
		if (!write_config((uint8_t *) &nvmconfig)) {
			// config write error stop work
			halt();
		}
	}

	memcpy(&config, &envmconfig, sizeof(CONFIG_T));

#if EN_ADC_LIGHT || EN_VBAT
	adc_configure((uint16_t)
		  ADC_CONFIG_OPTION_RESOLUTION_10_BITS |
		  ADC_CONFIG_OPTION_REF_SELECT_VDD |
		  ADC_CONFIG_OPTION_ACQ_TIME_12_US |
		  ADC_CONFIG_OPTION_RESULT_JUSTIFICATION_RIGHT
	);
#endif

#if EN_BH1750
    bh1750init();
#endif

#if EN_DHT
    dht_init();
#endif

#if EN_RF
 	radio_init();
	openAllPipe(&config);
	setAutoAck(config.autoask);
	setChannel(config.channel);
	setDataRate(config.datarate);
#if EN_AES
	if (config.useaes) {
		aes_initialize(&aes_data, AES_KEY_LENGTH_128_BITS, config.aeskey, NULL);
	}
#endif
#endif

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
		rfsend(&message, &config);
		if (value < config.vbatlow) {
			message.msgType = SENSOR_ERROR;
			message.error = VBAT_LOW;
			rfsend(&message, &config);
			goto VBATLOW;				// goto powersave
		}
#endif

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
		ret = bh1750read(&value);
		if (ret == BH1750_NO_ERROR) {
			message.data.iValue = (uint32_t)value;
		} else {
			message.msgType = SENSOR_ERROR;
			message.error = ret;
		}
		rfsend(&message, &config);
#endif

#if EN_DS18B20
		message.msgType = SENSOR_DATA;
		message.address = ADDR_DS18B20;
		message.sensorType = DS1820;
		message.valueType = TEMPERATURE;
		ret = ds18b20_read(&DSTemp);
		if (ret == DS_NO_ERROR) {
			// DS18B20 temperature send
			message.data.fValue = DSTemp;
		} else {
			message.msgType = SENSOR_ERROR;
			message.error = ret;
		}
		rfsend(&message);
#endif	//DS

#if EN_DHT
		// TODO: sensor read error on startup
		delay_ms(350);

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
						config.maxsend = (uint8_t) message.data.iValue;
						ret = CRC8((uint8_t *) &config, sizeof(CONFIG_T) - 1);
						config.crcbyte = ret;
						if (!write_config((uint8_t *) &config))	{
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
						config.sleeptm = (uint16_t) message.data.iValue;
						ret = CRC8((uint8_t *) &config, sizeof(CONFIG_T) - 1);
						config.crcbyte = ret;
						if (!write_config((uint8_t *) &config))	{
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
						config.vbatlow = (uint16_t) message.data.iValue;
						ret = CRC8((uint8_t *) &config, sizeof(CONFIG_T) - 1);
						config.crcbyte = ret;
						if (!write_config((uint8_t *) &config)) {
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

// battery low force to powersave
VBATLOW:

#if EN_SLEEP
		if (gpio_pin_val_read(WDGPIN) && (config.sleeptm > 0)) {
			// clear unneeded pins
			gpio_pin_val_clear(LEDPIN);
			adc_power_down();
			rfpwrDown();

			watchdog_setup();
			watchdog_set_wdsv_count(watchdog_calc_timeout_from_sec(config.sleeptm));
			pwr_clk_mgmt_op_mode_configure(
				PWR_CLK_MGMT_OP_MODE_CONFIG_OPTION_RUN_WD_NORMALLY
				| PWR_CLK_MGMT_OP_MODE_CONFIG_OPTION_RETENTION_LATCH_LOCKED
			);
			pwr_clk_mgmt_clear_reset_reasons();
			pwr_clk_mgmt_enter_pwr_mode_memory_ret_tmr_on(); // 1mkA
		}
#endif

   		delay_s(30);

	} // main loop
}
