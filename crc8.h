/*
 * crc8.h
 *
 *  Created on: 20/09/2016
 *      Author: andru
 */

#ifndef CRC8_H_
#define CRC8_H_

// use CRC8 source table (increase program size ~100bytes)
#define USECRC8TBL		1

#include <stdint.h>

uint8_t CRC8(uint8_t *addr, uint8_t len);

#endif /* CRC8_H_ */
