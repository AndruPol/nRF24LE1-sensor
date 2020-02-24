/*
 * ds18b20.h
 *
 *  Created on: 20/09/2016
 *      Author: andru
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#define SKIP_ROM_CMD	0xcc
#define START_CONV_CMD	0x44
#define READ_SCR_CMD	0xbe
#define WRITE_SCR_CMD	0x4e
#define REG_TH			0x00
#define REG_TL			0xff
#define REG_CONFIG		0x3f	// precision 10bit

#define DS18B20_WAIT	200	 	// wait results (datasheet says max. 187.5ms on 10bit prec)

typedef enum {
	DS_OK,
	DS_NOT_FOUND,
	DS_TIMEOUT,
	DS_CRC_ERROR,
} dserror_t;

dserror_t ds18b20_read(int16_t *temp);
dserror_t ds18b20_ask(void);
dserror_t ds18b20_read_nowait(int16_t *temp);

#endif /* DS18B20_H_ */
