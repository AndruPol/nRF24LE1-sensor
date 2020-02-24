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
#include "rtc2.h"
#include "pwr_clk_mgmt.h"
#include "watchdog.h"
#include "interrupt.h"

#include "main.h"
#include "crc8.h"
#include "radio.h"
#if EN_DHT
#include "dht.h"
#endif
#if EN_DS18B20
#include "ds18b20.h"
#endif
#if EN_BH1750
#include "bh1750.h"
#endif
#if EN_BME280
#include "bme280.h"
#endif

#define CMDWAIT			4000	// command wait time in ~20us intervals
#define WDGTIMEOUT		2		// watchdog timeout, sec
#define PWRUPWAIT		19800	// power up DHT sensor ~ 600mS
#define SENSWAIT		6560	// sensors max wait ~200mS

#if DEBUG
#define EN_UART			1	// use UART for debugging
#define UARTTXPIN		GPIO_PIN_ID_P0_3		// P0.3 - UART TX
#define UARTRXPIN		GPIO_PIN_ID_P0_4		// P0.4	- UART RX
#else
#define EN_UART			0	// use UART for debugging
#endif

#define LEDPIN	GPIO_PIN_ID_P0_0		// P0.0 - LED
//#define DHTPIN	GPIO_PIN_ID_P1_4	// P1.4 - DHT21/22
//#define DSPIN	GPIO_PIN_ID_P1_3		// P1.3 - DS18B20
#define VBATCH	ADC_CHANNEL_AIN5		// P0.5 - VBAT
#define LIGHTCH	ADC_CHANNEL_AIN1		// P0.1 - ADC LIGHT
//#define W2SCL	GPIO_PIN_ID_P0_4		// P0.4 - BH1750 2-wire SCL
//#define W2SDA	GPIO_PIN_ID_P0_5		// P0.5 - BH1750 2-wire SDA
#define VENPIN	GPIO_PIN_ID_P0_7		// P0.7 - sensors power enable

#define LEDOFF	gpio_pin_configure(LEDPIN,	\
		GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT	\
		| GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_CLEAR \
		| GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH \
		);

#define LEDON	gpio_pin_configure(LEDPIN,	\
		GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT	\
		| GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET \
		| GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH \
		);

#define VENOFF	gpio_pin_configure(VENPIN,	\
		GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT	\
		| GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_CLEAR \
		| GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH \
		);

#define VENON	gpio_pin_configure(VENPIN,	\
		GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT	\
		| GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET \
		| GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH \
		);

#define NVM_START_ADDRESS	MEMORY_FLASH_NV_STD_END_START_ADDRESS
#define ENVM_START_ADDRESS	MEMORY_FLASH_NV_EXT_END_START_ADDRESS
#define ENVM_PAGE_NUM		MEMORY_FLASH_NV_EXT_END_FIRST_PAGE_NUM

#if EN_UART
#include "uart.h"
#endif

CONFIG_T config;
MESSAGE_T message;

// halt
static void halt(void) {
	while (1) {
#if EN_LED && ! BATTERY
		gpio_pin_val_complement(LEDPIN);
#endif
		delay_ms(250);
	}
}

#define SAMPLES_NUM		16
#define SAMPLES_GOOD	2
#define SAMPLES_SUM		12
static uint16_t samples[SAMPLES_NUM];

static void samples_sort(void) {
	uint8_t i, j;
	for(i = 0 ; i < SAMPLES_NUM - 1; i++) {
		for(j = 0 ; j < SAMPLES_NUM - i - 1 ; j++) {
			if(samples[j] > samples[j+1]) {
				uint16_t tmp = samples[j];
				samples[j] = samples[j+1] ;
				samples[j+1] = tmp;
			}
		}
	}
}

#if EN_VBAT || EN_LIGHT
static uint16_t adc_get(adc_channel_t channel) {
	uint8_t i;
	uint32_t sum = 0;

	for (i = 0; i < SAMPLES_NUM; i++) {
		samples[i] = adc_start_single_conversion_get_value(channel);
	}
	samples_sort();
	for (i = SAMPLES_GOOD; i < SAMPLES_GOOD + SAMPLES_SUM; i++) {
		sum += samples[i];
	}

	return (uint16_t) sum / SAMPLES_SUM;
}
#else
static uint16_t adc_get(adc_channel_t channel) {
	(void) channel;
	return 0;
}
#endif

static void uart_init(void) {
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
}

static void rtc_init(void) {
	// RTC2 init on 1s interval
	pwr_clk_mgmt_clklf_configure(PWR_CLK_MGMT_CLKLF_CONFIG_OPTION_CLK_SRC_RCOSC32K);
	pwr_clk_mgmt_wait_until_clklf_is_ready();

	watchdog_setup();
	watchdog_set_wdsv_count(watchdog_calc_timeout_from_sec(WDGTIMEOUT));

	pwr_clk_mgmt_op_mode_configure(
		PWR_CLK_MGMT_OP_MODE_CONFIG_OPTION_RUN_WD_NORMALLY |
		PWR_CLK_MGMT_WAKEUP_CONFIG_OPTION_WAKEUP_ON_RTC2_TICK_ALWAYS
	);

	rtc2_configure(
		RTC2_CONFIG_OPTION_COMPARE_MODE_0_RESET_AT_IRQ,
		32767);			// 1s
}

static void adc_init(void) {
#if EN_VBAT || EN_LIGHT
		adc_configure((uint16_t)
			ADC_CONFIG_OPTION_RESOLUTION_10_BITS
			| ADC_CONFIG_OPTION_REF_SELECT_VDD
			| ADC_CONFIG_OPTION_RESULT_JUSTIFICATION_RIGHT
			| ADC_CONFIG_OPTION_ACQ_TIME_12_US
		);
		adc_power_up();
#endif
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

static void message_fill(uint8_t addr) {
	memset(&message, 0, MSGLEN);
	message.firmware = FIRMWARE;
	message.addrnum = ADDR_NUM;
	message.cmdparam = CMD_WAIT;
	message.deviceid = config.deviceid;
	message.address = addr;
}

static void send_config(uint8_t addr, uint16_t value) {
#if 0
	printf("cfg: addr=%d, val=%d\r\n", addr, (uint16_t) value);
#endif
	message_fill(addr);
	message.msgtype = MSG_INFO;
	message.datatype = VAL_i32;
	message.datapower = 0;
	message.data.i32 = value;
	rfsend(&message);
}

static void send_config_err(uint8_t addr, uint8_t errcode) {
#if 0
	printf("cfg: addr=%d, error=%d\r\n", addr, errcode);
#endif
	message_fill(addr);
	message.msgtype = MSG_ERROR;
	message.error = errcode;
	message.datapower = 0;
	message.data.i32 = errcode;
	rfsend(&message);
}

#if EN_SLEEP
// register retension sleep 30.51uS * time
void dosleep(uint16_t tm) {
	if (tm == 0)	return;

	rtc2_configure(
		RTC2_CONFIG_OPTION_COMPARE_MODE_0_RESET_AT_IRQ,
		tm);

	watchdog_set_wdsv_count(watchdog_calc_timeout_from_sec(WDGTIMEOUT));

	rtc2_run();
	pwr_clk_mgmt_enter_pwr_mode_register_ret();
	rtc2_stop();
#if DEBUG
	printf("sleep: time=%d\r\n", tm);
#endif
}
#endif


// main
void main(void) {
	// variable definition
	uint8_t ret, cmd;

#if EN_VBAT
	uint16_t vbat;
#endif

#if EN_LIGHT || EN_BH1750
	uint16_t light;
#endif

#if EN_DS18B20
	int16_t DSTemp;
#endif

#if EN_DHT
	int16_t DHTTemp, DHTHum;
#endif

#if EN_BME280
	int32_t val32;
#endif

#if EN_LED
	LEDOFF;
#endif

	read_config(NVM_START_ADDRESS);
	ret = CRC8((uint8_t *) &config, sizeof(CONFIG_T)-1);
	if (config.crcbyte != ret) {
		// NVM config wrong stop work
		halt();
	}

	cmd = config.version;
	read_config(ENVM_START_ADDRESS);
	ret = CRC8((uint8_t *) &config, sizeof(CONFIG_T)-1);
	if (config.crcbyte != ret || config.version != cmd) {
		read_config(NVM_START_ADDRESS);
		if (!write_config()) {
			// config write error stop work
			halt();
		}
	}

	uart_init();

#if 0
	printf("\r\nid=%d\r\ncfgver=%d\r\nchannel=%d\r\n", config.deviceid, config.version, config.channel);
	printf("maxsend=%d\r\nsleep=%d\r\nvbatlow=%d\r\n", config.maxsend, config.sleeptm, config.vbatlow);
	printf("sndaddr=%02X%02X%02X%02X%02X, ", config.sndaddr[0],config.sndaddr[1],config.sndaddr[2],config.sndaddr[3],config.sndaddr[4]);
	printf("rcvaddr=%02X%02X%02X%02X%02X\r\n", config.rcvaddr[0],config.rcvaddr[1],config.rcvaddr[2],config.rcvaddr[3],config.deviceid);
#endif

	rtc_init();

	// main loop
	while (1) {
#if EN_LED
		LEDON
#endif

		adc_init();

#if EN_VBAT
		vbat = adc_get(VBATCH);
		if (config.vbatlow > 0 && vbat < config.vbatlow)
			goto VBATLOW;
#endif

		VENON	// sensors power on

#if EN_LIGHT
        light = adc_get(LIGHTCH);
#endif

#if EN_VBAT || EN_LIGHT
		adc_power_down();
#endif

#if EN_DHT
		dosleep(PWRUPWAIT);		// DHT power up time ~600mS
#endif

#if EN_BH1750
		bh1750_init();
		light = bh1750_ask();
#endif

#if EN_DS18B20
		DSTemp = ds18b20_ask();
#endif	//DS18B20

#if EN_BME280
		bme280_init();
		set_osr_settings(BME280_OVERSAMPLING_1X, BME280_OVERSAMPLING_16X, BME280_OVERSAMPLING_2X);
		set_filter_settings(BME280_FILTER_COEFF_16);
		set_standby_settings(BME280_STANDBY_TIME_1_MS);
		bme280_set_sensor_mode(BME280_FORCED_MODE);
#endif

		dosleep(SENSWAIT);		// max sensors wait time, ~200mS

#if EN_LIGHT
		message_fill(ADDR_LIGHT);
        message.msgtype = MSG_DATA;
		message.datatype = 	VAL_i32;
		message.datapower = 0;
        message.data.i32 = 1023 - light;
        rfsend(&message);
#if DEBUG
		printf("adc light=%d\r\n", 1023 - light);
#endif
#endif

#if EN_BH1750
		message_fill(ADDR_BH1750);
		message.datatype = 	VAL_i32;
		message.datapower = 0;
		if (light == BH1750_OK && (ret = bh1750_read_nowait((uint16_t *) &light)) == BH1750_OK) {
			message.msgtype = MSG_DATA;
			message.data.i32 = light;
#if DEBUG
			printf("bh1750 light=%d\r\n", light);
#endif
		} else {
			message.msgtype = MSG_ERROR;
			message.error = ret;
			message.data.i32 = ret;
#if DEBUG
			printf("bh1750 err=%d\r\n", ret);
#endif
		}
		rfsend(&message);
#endif

#if EN_DS18B20
		message_fill(ADDR_DS18B20);
		message.datatype = 	VAL_i32;
		message.datapower = 1;
		if (DSTemp == DS_OK && (ret = ds18b20_read_nowait((int16_t *) &DSTemp)) == DS_OK) {
			// DS18B20 temperature send
			message.msgtype = MSG_DATA;
			message.data.i32 = DSTemp;
#if DEBUG
			printf("ds18b280 temp=%d\r\n", DSTemp);
#endif
		} else {
			message.msgtype = MSG_ERROR;
			message.error = ret;
			message.data.i32 = ret;
#if DEBUG
			printf("ds18b20 err=%d\r\n", ret);
#endif
		}
		rfsend(&message);
#endif	//DS18B20

#if EN_BME280
		message_fill(ADDR_BME280_TEMP);
		message.msgtype = MSG_DATA;
		message.datatype = 	VAL_i32;
		message.datapower = 2;
		ret = bme280_get_temperature(&val32);
		if (ret == BME280_OK) {
			message.data.i32 = val32;
#if DEBUG
			printf("bme280 temp=%ld\r\n", val32);
#endif
		} else {
			message.msgtype = MSG_ERROR;
			message.error = ret;
#if DEBUG
			printf("bme280 temp err=%d\r\n", ret);
#endif
		}
		rfsend(&message);

#if BME280_PRESSURE
		message_fill(ADDR_BME280_PRES);
		message.msgtype = MSG_DATA;
		message.datatype = 	VAL_i32;
		message.datapower = 3;
		ret = bme280_get_pressure((uint32_t *)&val32);
		if (ret == BME280_OK) {
			message.data.i32 = bme280_Pa_to_mmHg((uint32_t)val32);
#if DEBUG
			printf("bme280 pres=%ld\r\n", bme280_Pa_to_mmHg((uint32_t)val32));
#endif
		} else {
			message.msgtype = MSG_ERROR;
			message.error = ret;
#if DEBUG
			printf("bme280 pres err=%d\r\n", ret);
#endif
		}
		rfsend(&message);
#endif

#if BME280_HUMIDITY
		message_fill(ADDR_BME280_HUM);
		message.msgtype = MSG_DATA;
		message.datatype = 	VAL_i32;
		message.datapower = 3;
		ret = bme280_get_humidity(&val32);
		if (ret == BME280_OK) {
			message.data.i32 = val32;
#if DEBUG
			printf("bme280 hum=%ld\r\n", val32);
#endif
		} else {
			message.msgtype = MSG_ERROR;
			message.error = ret;
#if DEBUG
			printf("bme280 hum err=%d\r\n", ret);
#endif
		}
		rfsend(&message);
#endif
#endif

#if EN_DHT
		// delay after startup & reset need to be > 500ms :(
		ret = dht_read(&DHTTemp, &DHTHum);
		if (ret != DHT_OK) {
			message_fill(ADDR_DHT_TEMP);
			message.msgtype = MSG_ERROR;
			message.error = ret;
			message.data.i32 = ret;
			rfsend(&message);
			message_fill(ADDR_DHT_HUM);
			message.msgtype = MSG_ERROR;
			message.error = ret;
			message.data.i32 = ret;
			rfsend(&message);
#if DEBUG
			printf("dht error=%d\r\n", ret);
#endif
		}
		else {
			// DHT temperature, humidity send
			message_fill(ADDR_DHT_TEMP);
			message.msgtype = MSG_DATA;
			message.datatype = 	VAL_i32;
			message.datapower = 1;
			message.data.i32 = DHTTemp;
			rfsend(&message);
#if DEBUG
			printf("dht temp=%d\r\n", DHTTemp);
#endif
			message_fill(ADDR_DHT_HUM);
			message.msgtype = MSG_DATA;
			message.datatype = 	VAL_i32;
			message.datapower = 1;
			message.data.i32 = DHTHum;
			rfsend(&message);
#if DEBUG
			printf("dht hum=%d\r\n", DHTHum);
#endif
		}
#endif	//DHT

VBATLOW:
		VENOFF

#if EN_VBAT
#if DEBUG
		printf("vbat=%d, vbatlow=%d\r\n", vbat, config.vbatlow);
#endif
		message_fill(ADDR_VBAT);
		if (config.vbatlow > 0 && vbat < config.vbatlow) {
			message.msgtype = MSG_ERROR;
			message.error = ERR_VBATLOW;
			message.data.i32 = ERR_VBATLOW;
#if DEBUG
			printf("vbat low fired\r\n");
#endif
		} else {
			message.msgtype = MSG_DATA;
			message.datatype = 	VAL_i32;
			message.datapower = 0;
			message.data.i32 = vbat;
		}
		rfsend(&message);
#endif

WAITCMD:
		message_fill(0);
		message.msgtype = MSG_CMD;
		message.command = CMD_MSGWAIT;
		rfsend(&message);

		// wait command from smarthome gateway
		memset(&message, 0, MSGLEN);
		cmd = rfread(&message, CMDWAIT);

		if (cmd && message.deviceid == config.deviceid && message.msgtype == MSG_CMD) {
#if DEBUG
			printf("\r\ncommand: %d\r\n", message.command);
			printf("address: %d\r\n", message.address);
			printf("param: %d\r\n\r\n", (uint16_t) message.data.i32);
#endif
			// команда чтения/записи конфигурации
			if ((message.command == CMD_CFGREAD || message.command == CMD_CFGWRITE)) {
				switch (message.address) {
				case CFG_CHANNEL:
					if (message.command == CMD_CFGWRITE) {
						if (message.data.i32 < 0 || message.data.i32 >= 100) {
						    send_config_err(CFG_CHANNEL, ERR_CMDPARAM);
						    break;
						}
						config.channel = (uint8_t) message.data.i32;
						if (!write_config())	{
							send_config_err(CFG_CHANNEL, ERR_CFGWRITE);
							break;
						}
					}
					send_config(CFG_CHANNEL, config.channel);
					break;
				case CFG_SLEEP:
					if (message.command == CMD_CFGWRITE) {
						if (message.data.i32 < 0 || message.data.i32 > 512) {
						    send_config_err(CFG_SLEEP, ERR_CMDPARAM);
						    break;
						}
						config.sleeptm = (uint16_t) message.data.i32;
						if (!write_config())	{
							send_config_err(CFG_SLEEP, ERR_CFGWRITE);
							break;
						}
					}
					send_config(CFG_SLEEP, config.sleeptm);
					break;
#if EN_VBAT
				case CFG_VBATLOW:
					if (message.command == CMD_CFGWRITE) {
						if (message.data.i32 < 0 || message.data.i32 > 1023) {
						    send_config_err(CFG_VBATLOW, ERR_CMDPARAM);
						    break;
						}
						config.vbatlow = (uint16_t) message.data.i32;
						if (!write_config()) {
							send_config_err(CFG_VBATLOW, ERR_CFGWRITE);
							break;
						}
					}
					send_config(CFG_VBATLOW, config.vbatlow);
					break;
#endif
				default:
					break;
				}
			}
			goto WAITCMD;
		}

			rfdown();

#if EN_LED
			LEDOFF
#endif

#if EN_SLEEP
		if (config.sleeptm > 0) {
			watchdog_set_wdsv_count(watchdog_calc_timeout_from_sec(config.sleeptm));
			pwr_clk_mgmt_enter_pwr_mode_memory_ret_tmr_on(); // 1mkA
		}
#else
   		if (config.sleeptm > 0) {
			watchdog_set_wdsv_count(watchdog_calc_timeout_from_sec(config.sleeptm+1));
   			delay_s(config.sleeptm);
   		}
#endif
	} // main loop
}
