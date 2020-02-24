# nRF24LE1-sensor
==============


##nRF24LE1 remote sensor

- support DHT21/22, DS18B20, BME280, BH1750, photo resistor as light sensor, AES encryption

- works with my custom nRF24 - MODBUS gateway

- settings stored in nRF24LE1 NVM memory

based on great project [SDK for the Nordic nRF24LE1](https://github.com/DeanCording/nRF24LE1_SDK)

#####WARNING: BME280 with AES encryption cause linker error: Insufficient ROM/EPROM/FLASH memory

=== PINOUT ===

P0.1 - LED

P0.3 - Battery voltage ADC value

P0.0 - Photo resistor ADC value

P0.4 - BH1750/BME280 I2C SCL

P0.5 - BH1750/BME280 I2C SDA

P1.4 - DHT21/22

P1.3 - DS18B20
