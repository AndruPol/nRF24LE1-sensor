/*
 * radio.h
 *
 *  Created on: 10/07/2016
 *      Author: andru
 */

#ifndef RADIO_H_
#define RADIO_H_

#include "main.h"

#if EN_RF
void rfinit();
void rfsend(const MESSAGE_T *msg);
uint8_t rfread(MESSAGE_T *msg, uint16_t timeout);
void rfdown(void);
#else
#define rfinit() {};
#define rfsend(msg) {};
#define rfread(msg, timeout) (0)
#define rfdown() {};
#endif

#endif /* RADIO_H_ */
