/*
 * radio.h
 *
 *  Created on: 10/07/2016
 *      Author: andru
 */

#ifndef RADIO_H_
#define RADIO_H_

void radio_init();
void setChannel(unsigned char channel);
void setDataRate(unsigned char speed);
void setAutoAck(unsigned char enable);
void setCRCLength(unsigned char length);
void setPALevel(unsigned char level);
void openAllPipe(CONFIG_T *config);
void rfsend(const MESSAGE_T *msg);
uint8_t rfread(MESSAGE_T *msg, uint16_t timeout);
void rfpwrDown(void);

#endif /* RADIO_H_ */
