/*
 * main.h
 *
 *  Created on: 20/09/2016
 *      Author: andru
 */

#ifndef MAIN_H_
#define MAIN_H_

#define DEBUG	0	// debug output via UART
#define printf	printf_tiny

#define EN_LED			1		// LED enable
#define EN_RF			1		// radio enable
#define EN_DHT			1		// DHT21/22 enable
#define EN_DS18B20		1		// DS18B20 enable
#define EN_VBAT			0		// ADC battery voltage
#define EN_ADC_LIGHT	1		// ADC based light sensor enable
#define EN_BH1750		0		// BH1750 enable
#define EN_SLEEP		1		// power save mode enable

#define EN_AES	1
#define MSGLEN	sizeof(MESSAGE_T)

#if EN_AES
#include "aes.h"
extern aes_data_t aes_data;
#endif

#include <stdint.h>

// NVM configuration data for nRF24LE1
typedef struct CONFIG CONFIG_T;
struct CONFIG {
	uint8_t version;	// configuration version
	uint8_t deviceID;	// device ID
	uint8_t channel;	// radio channel: 0-199
	uint8_t datarate;	// data rate: 1-3
	uint8_t autoask;	// auto ask
	uint8_t srvaddr[5];	// gateway address to send
	uint8_t devaddr[5];	// device address to receive command
	uint8_t maxsend;	// max message send retries
	uint16_t sleeptm;	// wakeup by watchdog timer, s ~8.5min max (0x1-0x1FF LSB first)
	uint16_t vbatlow;	// battery low ADC value
#if EN_AES
	uint8_t useaes;		// use aes encryption
	uint8_t aeskey[16];	// aes encryption key
#endif
	uint8_t crcbyte;	// CRC8 sizeof(config) - 1
};

typedef enum {
#if EN_ADC_LIGHT
	ADDR_ADC_LIGHT,		// ADC light sensor
#endif
#if EN_BH1750
	ADDR_BH1750,		// BH1750 light
#endif
#if EN_DS18B20
	ADDR_DS18B20,		// DS18B20 temperature
#endif
#if EN_DHT
	ADDR_DHT_TEMP,		// DHT temperature
	ADDR_DHT_HUM,		// DHT humidity
#endif
#if EN_VBAT
	ADDR_VBAT,			// BATTERY voltage (ADC)
	CFG_VBATLOW,		// uint16_t vbatlow;
#endif
	CFG_MAXSEND,		// uint8_t maxsend;
	CFG_SLEEP,			// uint16_t sleeptm;
} address_t;

typedef enum {
	SENSOR_INFO = 0,
	SENSOR_DATA,
	SENSOR_ERROR,
	SENSOR_CMD,
} msgtype_t;

typedef enum {
	DS1820 = 0,
	BH1750,
	DHT,
	BMP085,
	ADC,
	HCSR04,
} sensortype_t;

typedef enum {
	TEMPERATURE = 0,
	HUMIDITY,
	PRESSURE,
	LIGHT,
	VOLTAGE,
	DISTANCE,
} valuetype_t;

typedef enum {
	CMD_CFGREAD = 1,	// read configuration value
	CMD_CFGWRITE,		// write configuration value
	CMD_RESET,			// reset device
	CMD_SENSREAD = 10,	// read sensor value
	CMD_ON = 20,		// ON
	CMD_OFF,			// OFF
	CMD_ONTM,			// ON timeout (S) message.data.iValue
	CMD_OFFTM,			// OFF timeout (S) message.data.iValue
} command_t;

// message format
typedef struct MESSAGE MESSAGE_T;
struct MESSAGE {
	msgtype_t msgType;			// message type
	uint8_t deviceID;			// remote device ID
	sensortype_t sensorType;	// sensor type
	valuetype_t valueType;		// value type
	address_t address;			// internal sensor address
	command_t command;			// command
	uint8_t error;				// error code
	uint8_t notused[5];			//
	union	{					// sensor value depend on sensor type
		float	fValue;			// float value
		int32_t	iValue;			// integer value/command parameter
		uint8_t cValue[4]; 		//
	} data;
};

typedef enum {
	VBAT_OK = 0,				// not used
	VBAT_LOW,
} vbaterr_t;

extern CONFIG_T config;

#endif /* MAIN_H_ */
