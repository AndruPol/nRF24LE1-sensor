/*
 * nrf24le1_bme280.h
 *
 *  Created on: 30/07/2018
 *      Author: andru
 */

#include "bme280.h"
#include "bme280_defs.h"

#include "main.h"

#if DEBUG
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#endif

#include "delay.h"
#include "w2.h"

#define BME280_ADDR	BME280_I2C_ADDR_PRIM

// Constant for Pascals to millimeters of mercury conversion
#define BME_MMHG_Q0_20 (uint32_t)7865; // 0.00750061683 in Q0.20 format

static int32_t t_fine = 0;

static void i2c_init(void) {
	w2_configure(W2_CONFIG_OPTION_ENABLE |
				 W2_CONFIG_OPTION_MODE_MASTER |
				 W2_CONFIG_OPTION_CLOCK_FREQ_100_KHZ |
				 W2_CONFIG_OPTION_ALL_INTERRUPTS_ENABLE,
				 0
	);
}

static bme280_error_t i2c_read(uint8_t reg_addr, uint8_t *data, uint16_t len)
{
	uint8_t buf;

	if (w2_master_write_to(BME280_ADDR, &reg_addr, 1, &buf, 0) == W2_NACK_VAL)
		return BME280_E_COMM_FAIL;

	if (w2_master_cur_address_read(BME280_ADDR, data, len) == W2_NACK_VAL)
		return BME280_E_COMM_FAIL;

	return BME280_OK;
}

static bme280_error_t i2c_write(uint8_t reg_addr, uint8_t *data, uint16_t len)
{
	if (w2_master_write_to(BME280_ADDR, &reg_addr, 1, data, len) == W2_NACK_VAL)
		return BME280_E_COMM_FAIL;

	return BME280_OK;
}

/*!
 * @brief This API reads the data from the given register address of the sensor.
 */
bme280_error_t bme280_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
	/* Read the data  */
	if (i2c_read(reg_addr, reg_data, len) != BME280_OK)
		return BME280_E_COMM_FAIL;

	return BME280_OK;
}

/*!
 * @brief This API writes the given data to the register address
 * of the sensor.
 */
bme280_error_t bme280_set_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len)
{
	/* Check for arguments validity */
	if (reg_data == NULL)
		return BME280_E_NULL_PTR;
	if (len == 0)
		return BME280_E_INVALID_LEN;
	if (i2c_write(reg_addr, reg_data, len) == BME280_OK)
		return BME280_OK;

	return BME280_E_COMM_FAIL;
}

/*!
 * @brief This API performs the soft reset of the sensor.
 */
bme280_error_t bme280_soft_reset(void)
{
	/* 0xB6 is the soft reset command */
	uint8_t soft_rst_cmd = 0xB6;

	/* Write the soft reset command in the sensor */
	if (bme280_set_regs(BME280_RESET_ADDR, &soft_rst_cmd, 1) == BME280_OK) {
		/* As per data sheet, startup time is 2 ms. */
		delay_ms(2);
		return BME280_OK;
	}
	return BME280_E_COMM_FAIL;
}

/*!
 *  @brief This API is the entry point.
 *  It reads the chip-id and calibration data from the sensor.
 */
bme280_error_t bme280_init(void)
{
	bme280_error_t rslt;
	/* chip id read try count */
	uint8_t try_count = 5;
	uint8_t chip_id = 0;

	rslt = BME280_OK;
	i2c_init();
	while (try_count) {
		/* Read the chip-id of bme280 sensor */
		rslt = bme280_get_regs(BME280_CHIP_ID_ADDR, &chip_id, 1);
		/* Check for chip id validity */
		if ((rslt == BME280_OK) && (chip_id == BME280_CHIP_ID)) {
			/* Reset the sensor */
			if (bme280_soft_reset() == BME280_OK) {
				return BME280_OK;
			}
		}
		/* Wait for 1 ms */
		delay_ms(1);
		--try_count;
	}
		/* Chip id check failed */
	if (!try_count)
		rslt = BME280_E_DEV_NOT_FOUND;

	return rslt;
}

/*!
 * @brief This internal API writes the power mode in the sensor.
 */
bme280_error_t write_power_mode(uint8_t sensor_mode)
{
	bme280_error_t rslt;
	/* Variable to store the value read from power mode register */
	uint8_t sensor_mode_reg_val;

	/* Read the power mode register */
	rslt = bme280_get_regs(BME280_PWR_CTRL_ADDR, &sensor_mode_reg_val, 1);
	/* Set the power mode */
	if (rslt == BME280_OK) {
		sensor_mode_reg_val = BME280_SET_BITS_POS_0(sensor_mode_reg_val, BME280_SENSOR_MODE, sensor_mode);
		/* Write the power mode in the register */
		rslt = bme280_set_regs(BME280_PWR_CTRL_ADDR, &sensor_mode_reg_val, 1);
	}

	return rslt;
}

/*!
 * @brief This API gets the power mode of the sensor.
 */
bme280_error_t bme280_get_sensor_mode(uint8_t *sensor_mode)
{
	bme280_error_t rslt;

	/* Read the power mode register */
	rslt = bme280_get_regs(BME280_PWR_CTRL_ADDR, sensor_mode, 1);
	/* Assign the power mode in the device structure */
	*sensor_mode = BME280_GET_BITS_POS_0(*sensor_mode, BME280_SENSOR_MODE);

	return rslt;
}

/*!
 * @brief This API sets the power mode of the sensor.
 */
bme280_error_t bme280_set_sensor_mode(uint8_t sensor_mode)
{
	bme280_error_t rslt;
	uint8_t last_set_mode;

	rslt = bme280_get_sensor_mode(&last_set_mode);
	/* If the sensor is not in sleep mode put the device to sleep
	   mode */
	if ((rslt == BME280_OK) && (last_set_mode != BME280_SLEEP_MODE))
		rslt = write_power_mode(BME280_SLEEP_MODE);
	/* Set the power mode */
	if (rslt == BME280_OK)
		rslt = write_power_mode(sensor_mode);

	return rslt;
}

/*!
 * @brief This API sets the temperature, pressure and humidity oversampling settings
 * in the sensor according to the settings selected by the user.
 */
bme280_error_t set_osr_settings(uint8_t osr_t, uint8_t osr_p, uint8_t osr_h)
{
	bme280_error_t rslt;
	uint8_t reg_data;

	reg_data = osr_h & BME280_CTRL_HUM_MSK;

	/* Write the humidity control value in the register */
	rslt = bme280_set_regs(BME280_CTRL_HUM_ADDR, &reg_data, 1);
	if (rslt != BME280_OK)
		return rslt;

	rslt = bme280_get_regs(BME280_CTRL_MEAS_ADDR, &reg_data, 1);
	if (rslt != BME280_OK)
		return rslt;

	reg_data = BME280_SET_BITS(reg_data, BME280_CTRL_TEMP, osr_t);
	reg_data = BME280_SET_BITS(reg_data, BME280_CTRL_PRESS, osr_p);

	/* Write the oversampling settings in the register */
	rslt = bme280_set_regs(BME280_CTRL_MEAS_ADDR, &reg_data, 1);

	return rslt;
}

/*!
 * @brief This internal API sets the filter settings
 * in the sensor according to the settings selected by the user.
 */
bme280_error_t set_filter_settings(uint8_t filter)
{
	bme280_error_t rslt;
	uint8_t reg_data;

	rslt = bme280_get_regs(BME280_CONFIG_ADDR, &reg_data, 1);

	if (rslt == BME280_OK) {
		reg_data = BME280_SET_BITS(reg_data, BME280_FILTER, filter);
		/* Write the oversampling settings in the register */
		rslt = bme280_set_regs(BME280_CONFIG_ADDR, &reg_data, 1);
	}

	return rslt;
}

/*!
 * @brief This internal API sets the standby duration settings
 * in the sensor according to the settings selected by the user.
 */
bme280_error_t set_standby_settings(uint8_t standby_time)
{
	bme280_error_t rslt;
	uint8_t reg_data;

	rslt = bme280_get_regs(BME280_CONFIG_ADDR, &reg_data, 1);

	if (rslt == BME280_OK) {
		reg_data = BME280_SET_BITS(reg_data, BME280_STANDBY, standby_time);
		/* Write the oversampling settings in the register */
		rslt = bme280_set_regs(BME280_CONFIG_ADDR, &reg_data, 1);
	}

	return rslt;
}

/*!
 * @brief This internal API is used to compensate the raw temperature data and
 * return the compensated temperature data in integer data type.
 *
 * Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
 * t_fine carries fine temperature as global value
 */
static bme280_error_t compensate_temperature(int32_t uncomp_t, int32_t *temperature)
{
	bme280_error_t rslt;
	uint16_t dig_T1;
	int16_t dig_T2, dig_T3;

	uint8_t calib_data[BME280_T_CALIB_DATA_LEN] = {0};

	/* Read the calibration data from the sensor */
	rslt = bme280_get_regs(BME280_T_CALIB_DATA_ADDR, calib_data, BME280_T_CALIB_DATA_LEN);

	if (rslt != BME280_OK)
		return rslt;

	dig_T1 = BME280_CONCAT_BYTES(calib_data[1], calib_data[0]);
	dig_T2 = (int16_t) BME280_CONCAT_BYTES(calib_data[3], calib_data[2]);
	dig_T3 = (int16_t) BME280_CONCAT_BYTES(calib_data[5], calib_data[4]);

	// (!) t_fine assigned here
	t_fine = ((((uncomp_t>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
	t_fine += (((((uncomp_t>>4) - ((int32_t)dig_T1)) * ((uncomp_t>>4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;

	*temperature = (t_fine * 5 + 128) >> 8;

	if (*temperature < TEMPERATURE_MIN)
		*temperature = TEMPERATURE_MIN;
	if (*temperature > TEMPERATURE_MAX)
		*temperature = TEMPERATURE_MAX;

	return rslt;
}

/*!
 * @brief This API reads the temperature data from the
 * sensor, compensates the data and store it in the temperature parameter.
 */
bme280_error_t bme280_get_temperature(int32_t *temperature)
{
	bme280_error_t rslt;
	uint8_t reg_data[BME280_T_DATA_LEN] = {0};
	int32_t uncomp_t;

	/* Read the temperature data from the sensor */
	rslt = bme280_get_regs(BME280_T_DATA_ADDR, reg_data, BME280_T_DATA_LEN);
	if (rslt == BME280_OK) {
		uncomp_t = (uint32_t) reg_data[0] << 12 | (uint32_t) reg_data[1] << 4 | (uint32_t) reg_data[2] >> 4;
		/* Compensate the temperature data from the sensor */
		rslt = compensate_temperature(uncomp_t, temperature);
	}

	return rslt;
}

#if BME280_PRESSURE
// Fixed point Pa to mmHg conversion (Pascals to millimeters of mercury)
// 1 Pa = 0.00750061683 mmHg
// input:
//    PQ24_8 - pressure in pascals (Q24.8 format, result of BME280_CalcP function)
// return:
//    pressure in millimeter of mercury
// note: return value of '746225' represents 746.225 mmHg
// note: maximum input value is 131071 Pa
// source: https://github.com/LonelyWolf/stm32/blob/master/bme280/bme280.c
uint32_t bme280_Pa_to_mmHg(uint32_t pressure)
{
	// Truncate the pressure value to Q24.2 format and multiply by Q0.20 constant (~0.00750061683)
	// The multiply product will be Q10.22 pressure value in mmHg
	uint32_t p_mmHg = ((pressure << 8) >> 6) * BME_MMHG_Q0_20;

	// (p_mmHg >> 22) -> integer part from Q10.22 value
	// ((uint32_t)p_mmHg << 10) >> 18 -> fractional part truncated down to 14 bits
	// (XXX * 61039) / 1000000 is rough integer equivalent of float (XXX / 16383.0) * 1000
	return ((p_mmHg >> 22) * 1000) + ((((p_mmHg << 10) >> 18) * 61039) / 1000000);
}

/*!
 * @brief This internal API is used to compensate the raw pressure data and
 * return the compensated pressure data in integer data type.
 *
 * Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
 * Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
 */
static bme280_error_t compensate_pressure(uint32_t uncomp_p, uint32_t *pressure)
{
	bme280_error_t rslt;
	uint16_t dig_P1;
	int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
	int32_t var1, var2;

	uint8_t calib_data[BME280_P_CALIB_DATA_LEN] = {0};

	if (t_fine == 0)
		return BME280_E_CONV;

	/* Read the calibration data from the sensor */
	rslt = bme280_get_regs(BME280_P_CALIB_DATA_ADDR, calib_data, BME280_P_CALIB_DATA_LEN);

	if (rslt != BME280_OK)
		return rslt;

	dig_P1 = BME280_CONCAT_BYTES(calib_data[1], calib_data[0]);
	dig_P2 = (int16_t) BME280_CONCAT_BYTES(calib_data[3], calib_data[2]);
	dig_P3 = (int16_t) BME280_CONCAT_BYTES(calib_data[5], calib_data[4]);
	dig_P4 = (int16_t) BME280_CONCAT_BYTES(calib_data[7], calib_data[6]);
	dig_P5 = (int16_t) BME280_CONCAT_BYTES(calib_data[9], calib_data[8]);
	dig_P6 = (int16_t) BME280_CONCAT_BYTES(calib_data[11], calib_data[10]);
	dig_P7 = (int16_t) BME280_CONCAT_BYTES(calib_data[13], calib_data[12]);
	dig_P8 = (int16_t) BME280_CONCAT_BYTES(calib_data[15], calib_data[14]);
	dig_P9 = (int16_t) BME280_CONCAT_BYTES(calib_data[17], calib_data[16]);

	var1 = (((int32_t)t_fine)>>1) - (int32_t)64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)dig_P6);
	var2 = var2 + ((var1*((int32_t)dig_P5))<<1);
	var2 = (var2>>2)+(((int32_t)dig_P4)<<16);
	var1 = (((dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)dig_P2) * var1)>>1))>>18;
	var1 =((((32768+var1))*((int32_t)dig_P1))>>15);
	if (var1 == 0) {
		return BME280_E_CONV; // avoid exception caused by division by zero
	}
	*pressure = (((uint32_t)(((int32_t)1048576)-uncomp_p)-(var2>>12)))*3125;
	if (*pressure < 0x80000000)	{
		*pressure = (*pressure << 1) / ((uint32_t)var1);
	} else {
		*pressure = (*pressure / (uint32_t)var1) * 2;
	}
	var1 = (((int32_t)dig_P9) * ((int32_t)(((*pressure>>3) * (*pressure>>3))>>13)))>>12;
	var2 = (((int32_t)(*pressure>>2)) * ((int32_t)dig_P8))>>13;
	*pressure = (uint32_t)((int32_t)*pressure + ((var1 + var2 + dig_P7) >> 4));

	if (*pressure < PRESSURE_MIN)
		*pressure = PRESSURE_MIN;
	if (*pressure > PRESSURE_MAX)
		*pressure = PRESSURE_MAX;

	return rslt;
}

/*!
 * @brief This API reads the pressure data from the
 * sensor, compensates the data and store it in the pressure parameter.
 */
bme280_error_t bme280_get_pressure(uint32_t *pressure)
{
	bme280_error_t rslt;
	uint8_t reg_data[BME280_P_DATA_LEN] = {0};
	int32_t uncomp_p;

	/* Read the pressure data from the sensor */
	rslt = bme280_get_regs(BME280_P_DATA_ADDR, reg_data, BME280_P_DATA_LEN);
	if (rslt == BME280_OK) {
		uncomp_p = (uint32_t) reg_data[0] << 12 | (uint32_t) reg_data[1] << 4 | (uint32_t) reg_data[2] >> 4;
		/* Compensate the pressure data from the sensor */
		rslt = compensate_pressure(uncomp_p, pressure);
	}

	return rslt;
}
#endif

#if BME280_HUMIDITY
/*!
 * @brief This internal API is used to compensate the raw temperature data and
 * return the compensated temperature data in integer data type.
 *
 * Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
 * Output value of “47445” represents 47445/1024 = 46.333 %RH
 */
static bme280_error_t compensate_humidity(int32_t uncomp_h, int32_t *humidity)
{
	bme280_error_t rslt;
	uint8_t dig_H1, dig_H3;
	int16_t dig_H2, dig_H4, dig_H5;
	int8_t dig_H6;
	int32_t v_x1_u32r;

	/* Array to store calibration data */
	uint8_t calib_data[BME280_H_CALIB_DATA_LEN2] = {0};

	if (t_fine == 0)
		return BME280_E_CONV;

	/* Read the calibration data from the sensor */
	rslt = bme280_get_regs(BME280_H_CALIB_DATA_ADDR1, calib_data, BME280_H_CALIB_DATA_LEN1);

	if (rslt != BME280_OK)
		return rslt;

	dig_H1 = (uint8_t) calib_data[0];

	/* Read the calibration data from the sensor */
	rslt = bme280_get_regs(BME280_H_CALIB_DATA_ADDR2, calib_data, BME280_H_CALIB_DATA_LEN2);

	if (rslt != BME280_OK)
		return rslt;

	dig_H2 = (int16_t) BME280_CONCAT_BYTES(calib_data[1], calib_data[0]);
	dig_H3 = (uint8_t) calib_data[2];
	dig_H4 = (int16_t) calib_data[3] << 4 | (int16_t) calib_data[4] & 0x0F;
	dig_H5 = (int16_t) calib_data[5] << 4 | (int16_t) calib_data[4] >> 4;
	dig_H6 = (int8_t) calib_data[6];

	v_x1_u32r = (t_fine - ((int32_t)76800));
	v_x1_u32r = (((((uncomp_h << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1_u32r)) +
			((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)dig_H6)) >> 10) * (((v_x1_u32r *
			((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)dig_H2) + 8192) >> 14));
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
	*humidity = (uint32_t)(v_x1_u32r>>12);

	if (*humidity > HUMIDITY_MAX)
		*humidity = HUMIDITY_MAX;

	return rslt;
}

/*!
 * @brief This API reads the humidity data from the
 * sensor, compensates the data and store it in the humidity parameter.
 */
bme280_error_t bme280_get_humidity(uint32_t *humidity)
{
	bme280_error_t rslt;
	uint8_t reg_data[BME280_H_DATA_LEN] = {0};
	int32_t uncomp_h;

	/* Read the humidity data from the sensor */
	rslt = bme280_get_regs(BME280_H_DATA_ADDR, reg_data, BME280_H_DATA_LEN);
	if (rslt == BME280_OK) {
		uncomp_h = (uint32_t) reg_data[0] << 8 | (uint32_t) reg_data[1];
		/* Compensate the humidity data from the sensor */
		rslt = compensate_humidity(uncomp_h, humidity);
	}

	return rslt;
}
#endif

#if DEBUG
bme280_error_t stream_sensor_data_forced_mode(void) {
  bme280_error_t rslt;
  int32_t temperature = 0;
  uint32_t pressure = 0;
  uint32_t humidity = 0;

  /* Recommended mode of operation: Indoor navigation */
  set_osr_settings(BME280_OVERSAMPLING_1X, BME280_OVERSAMPLING_16X, BME280_OVERSAMPLING_2X);
  set_filter_settings(BME280_FILTER_COEFF_16);
  set_standby_settings(BME280_STANDBY_TIME_1_MS);

#if DEBUG
  printf("Temperature, Pressure, Humidity\r\n");
#endif
  /* Continuously stream sensor data */
  rslt = bme280_set_sensor_mode(BME280_FORCED_MODE);

  /* Wait for the measurement to complete and print data @25Hz */
  delay_ms(40);

  if (bme280_get_temperature(&temperature) == BME280_OK) {
#if DEBUG
	  printf("temp=%ld\r\n", temperature);
#endif
  }

#if BME280_PRESSURE
  if (bme280_get_pressure(&pressure) == BME280_OK) {
#if DEBUG
	  printf("pres=%ld\r\n", bme280_Pa_to_mmHg(pressure));
#endif
  }
#endif

#if BME280_HUMIDITY
  if (bme280_get_humidity(&humidity) == BME280_OK) {
#if DEBUG
	  printf("hum=%ld\r\n", humidity);
#endif
  }
#endif

  return rslt;
}
#endif
