// функции для настройки nRF24LE1
// Автор MaksMS 
// http://homes-smart.ru/index.php/oborudovanie/bez-provodov-2-4-ggts/opisanie-radiomodulya-nrf24l01

#include "rf.h"

#define _BV(bit) (1 << (bit))

#define RF_DR_LOW	5
#define RF_DR_HIGH	3
#define RF_EN_CRC	3
#define RF_CRCO		2
#define PWR_UP		1
#define RF_PWR_LOW	1
#define RF_PWR_HIGH	2
#define RF_ERX_P0	0

void radiobegin() {
	uint8_t setup;

	rf_spi_configure_enable();

	setup = RF_CONFIG_EN_CRC | RF_CONFIG_CRCO | RF_CONFIG_PWR_UP;		// power up here, CRC 2 bytes
	rf_write_register(RF_CONFIG,&setup,1) ;

	delay_ms(2);

	setup = RF_SETUP_RETR_ARD_1000 | RF_SETUP_RETR_ARC_3; 		// 1000us, 3 retry
	rf_write_register(RF_SETUP_RETR, &setup, 1);

	setup = RF_SETUP_AW_5BYTES;									// 5 bytes address
	rf_write_register(RF_SETUP_AW, &setup, 1);

	setup = RF_RF_SETUP_RF_PWR_0;								// 0 dbm power, 1Mhz
	rf_write_register(RF_RF_SETUP, &setup, 1);

	rf_flush_tx();
}


// ------------ setChannel ----------------
void setChannel(unsigned char channel){
	rf_write_register(RF_RF_CH, &channel, 1);
}


// ------------ setDataRate ----------------
void setDataRate(unsigned char speed){
	unsigned char buffer;
	rf_read_register(RF_RF_SETUP,&buffer,1);

	buffer &= ~(_BV(RF_DR_LOW) | _BV(RF_DR_HIGH)) ;	// default speed=2, 1Mhz

	if(speed == 1)
		buffer |= _BV(RF_DR_LOW);
	else if (speed == 3)
		buffer |= _BV(RF_DR_HIGH);

	rf_write_register(RF_RF_SETUP,&buffer,1) ;
}


//--------------setAutoAck------------
void setAutoAck(unsigned char enable) {
	unsigned char buffer;

	if (enable==1)
		buffer=RF_EN_AA_ENAA_P0;	//63
	else
		buffer=0;
	rf_write_register(RF_EN_AA, &buffer,1);
}


//--------------setCRCLength------------
void setCRCLength(unsigned char length) {
	unsigned char buffer;

	rf_read_register(RF_CONFIG,&buffer,1);
	
	buffer = buffer & ~( _BV(RF_CRCO) | _BV(RF_EN_CRC)) ;

	if (length==1)
		buffer |= _BV(RF_EN_CRC);
	else if (length==2)
		buffer |= _BV(RF_EN_CRC) | _BV( RF_CRCO );

	rf_write_register(RF_CONFIG,&buffer,1 ) ;
}

void setPALevel(unsigned char level) {
	uint8_t setup;
 
	rf_read_register(RF_RF_SETUP,&setup,1) ;

	setup &= ~(_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH)) ;
	setup  | (level&3)<<RF_PWR_LOW;

	rf_write_register(RF_RF_SETUP,&setup,1);
}

//------------------------------------------------
// замена openWritingPipe и openReadingPipe:
#if 1

void openAllPipe() {
	uint8_t setup;
	unsigned char buffer[5] = {
			config.srvaddr[0],
			config.srvaddr[1],
			config.srvaddr[2],
			config.srvaddr[3],
			config.srvaddr[4]
	};

	setup = RF_EN_RXADDR_ERX_P0; // en_rxaddr ,биты включения каналов B00000001 на прием
	rf_write_register(RF_EN_RXADDR, &setup, 1);

	rf_write_register(RF_RX_ADDR_P0,buffer,5);
	rf_write_register(RF_TX_ADDR,buffer,5);

	setup = MSGLEN;	/* Number of bytes in RX payload		*/
	rf_write_register(RF_RX_PW_P0,&setup,1);
}
#endif
//------------------------------------------------

#if 0
void openWritingPipe(uint8_t value)
{
  uint8_t setup;
  unsigned char data[5];
  data[0]=value;
  data[1]=adr1;
  data[2]=adr2;
  data[3]=adr3;
  data[4]=adr4;
  
  
  rf_write_register(RF_RX_ADDR_P0, (uint8_t*)(&data), 5);
  rf_write_register(RF_TX_ADDR, (uint8_t*)(&data), 5);
  
  setup=32;
  rf_write_register(RF_RX_PW_P0,&setup,1);
}

void openReadingPipe(uint8_t child, uint8_t address)
{
uint8_t setup;
unsigned char data[5];


  
  // если openReadingPipe(0,хх) будет равно нулю,то включить:
 // if (child == 0) pipe0_reading_address = address;

  if (child <= 6)
  { 
    // For pipes 2-5, only write the LSB
    if ( child < 2 ) {
      
  data[0]=address;
  data[1]=adr1;
  data[2]=adr2;
  data[3]=adr3;
  data[4]=adr4;
  
      rf_write_register(RF_RX_ADDR_P0 + child, (const uint8_t*)(&data), 5);
    
    } else

      rf_write_register(RF_RX_ADDR_P0 + child, (const uint8_t*)(&address), 1);
      
    setup=32;
    rf_write_register(RF_RX_PW_P0+child,&setup,1);
    
// 
rf_read_register(RF_EN_RXADDR,&setup,1);

setup | _BV(RF_ERX_P0+child);

rf_write_register(RF_EN_RXADDR,&setup ,1);
  }
}
//

#endif
