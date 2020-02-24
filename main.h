/*
 * main.h
 *
 *  Created on: 20/09/2016
 *      Author: andru
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>

#define FIRMWARE		131		// firmware revision
#define BATTERY			1		// battery power
#define DEBUG			0		// debug output via UART
//#define printf			printf_tiny

#define EN_LED			0		// LED enable
#define EN_RF			1		// radio enable
#define EN_DHT			1		// DHT21/22 enable
#define EN_DS18B20		1		// DS18B20 enable
#define EN_VBAT			1		// ADC battery voltage
#define EN_BH1750		0		// BH1750 light sensor enable
#define EN_LIGHT		1		// ADC light sensor enable
#define EN_SLEEP		1		// power save mode enable
#define EN_AES			1		// AES encryption enable
#define EN_BME280		0		// BME280 sensor enable
#define EN_CRC			0		// use CRC8 in radio packets

// NVM configuration data for nRF24LE1
typedef struct CONFIG CONFIG_T;
struct CONFIG {
	uint8_t version;	// configuration version
	uint8_t deviceid;	// device ID
	uint8_t channel;	// radio channel: 0-199
	uint8_t sndaddr[5];	// gateway address to send
	uint8_t rcvaddr[5];	// device address to receive command
	uint8_t maxsend;	// max message send retries
	uint16_t sleeptm;	// wakeup by watchdog timer, S (LSB first)
	uint16_t vbatlow;	// battery low ADC value
#if EN_AES
	uint8_t aeskey[16];	// aes encryption key
#endif
	uint8_t crcbyte;	// CRC8 sizeof(config) - 1
};

// internal address map
#define ADDR_NUM	8
typedef enum {
#if EN_LIGHT
	ADDR_LIGHT = 0,			// ADC light
#endif
#if EN_BH1750
	ADDR_BH1750 = 0,		// BH1750 light
#endif
#if EN_DS18B20
	ADDR_DS18B20 = 1,		// DS18B20 temperature
#endif
#if EN_DHT
	ADDR_DHT_TEMP = 2,		// DHT temperature
	ADDR_DHT_HUM = 3,		// DHT humidity
#endif
#if EN_BME280
	ADDR_BME280_TEMP = 2,		// BME280 temperature
	ADDR_BME280_HUM = 3,		// BME280 humidity
	ADDR_BME280_PRES = 8,		// BME280 pressure
#endif
#if EN_VBAT
	ADDR_VBAT = 4,			// BATTERY voltage (ADC)
	CFG_VBATLOW = 5,		// uint16_t vbatlow;
#endif
	CFG_CHANNEL = 6,		// uint8_t channel;
	CFG_SLEEP = 7,			// uint16_t sleeptm;
} address_t;

// типы передаваемых сообщений
typedef enum {
	MSG_INFO = 0,
	MSG_DATA,
	MSG_ERROR,
	MSG_CMD,
} msgtype_t;

// типы передаваемых данных
typedef enum {
	VAL_ch,
	VAL_i16,
	VAL_i32,
	VAL_fl,
} msgvalue_t;

// команды устройству
typedef enum {
	CMD_CFGREAD = 1,	// read configuration value
	CMD_CFGWRITE,		// write configuration value
	CMD_RESET,			// reset device
	CMD_MSGWAIT,		//remote waiting messages
	CMD_SENSOREAD = 10,	// read sensor value
	CMD_ON = 20,		// ON
	CMD_OFF,			// OFF
	CMD_ONTM,			// ON timeout (S)
	CMD_OFFTM,			// OFF timeout (S)
	CMD_GETREG = 40,	// GET register value
	CMD_SETREG,			// SET register value
} command_t;

// тип отправки сообщения: записывается в cmdparam
typedef enum {
	CMD_NONE,		// command none
	CMD_SEND,		// command send immediately
	CMD_WAIT,		// wait message from device & send command
} devicecmd_t;

// message length
#define MSGLEN		sizeof(MESSAGE_T)

// NRF receive/send message format
typedef struct MESSAGE MESSAGE_T;
struct MESSAGE {
    msgtype_t msgtype;			// (in/out): message type
    uint8_t deviceid;			// (in/out): remote device id
    uint8_t firmware;			// (in/out): remote firmware
    uint8_t addrnum;			// (out): remote device internal address number
    uint8_t address;			// (in/out): on remote device internal address
    command_t command;			// (in): command
    uint8_t error;				// (in): remote error code
    msgvalue_t datatype;		// (in/out): type of data
    uint8_t datapower;			// (in/out): ^10 data power;
	uint8_t datalength;			// (out): data array length
    int16_t cmdparam;			// (in): command parameter
    union {						// (in): value depend of message type
	uint8_t c4[4];				// char value in c4[0]
	int16_t i16[2];				// int16 value in i16[0]
	int32_t	i32;				// int32 value
	float	f;					// float value
    } data;
};

// error codes
typedef enum {
	ERR_VBATLOW = 1,
	ERR_CFGWRITE,
	ERR_CMDPARAM,
} msgerr_t;

extern CONFIG_T config;

#endif /* MAIN_H_ */
