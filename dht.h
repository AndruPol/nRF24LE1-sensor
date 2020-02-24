/*
 * dht.h
 *
 *  Created on: 20/09/2016
 *      Author: andru
 */

#ifndef DHT_H_
#define DHT_H_

typedef enum {
	DHT_OK,
	DHT_NO_RESPONSE,
	DHT_TIMEOUT_ERROR,
	DHT_CHECKSUM_ERROR,
	DHT_NO_VALUE,
} dhterror_t;

void dht_init(void);
dhterror_t dht_read(int16_t *temp, uint16_t *hum);

#endif /* DHT_H_ */
