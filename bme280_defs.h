/**
 * Copyright (C) 2016 - 2017 Bosch Sensortec GmbH
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holder nor the names of the
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *
 * The information provided is believed to be accurate and reliable.
 * The copyright holder assumes no responsibility
 * for the consequences of use
 * of such information nor for any infringement of patents or
 * other rights of third parties which may result from its use.
 * No license is granted by implication or otherwise under any patent or
 * patent rights of the copyright holder.
 *
 * @file	bme280_defs.h
 * @date	14 Feb 2018
 * @version	3.3.4
 * @brief
 *
 */

/*! @file bme280_defs.h
    @brief Sensor driver for BME280 sensor */
/*!
 * @defgroup BME280 SENSOR API
 * @brief
 * @{*/
#ifndef BME280_DEFS_H_
#define BME280_DEFS_H_

#include <stdint.h>
#include <stddef.h>

/********************************************************/
/*! @name		Common macros		        */
/********************************************************/

#if !defined(UINT8_C) && !defined(INT8_C)
#define INT8_C(x)       S8_C(x)
#define UINT8_C(x)      U8_C(x)
#endif

#if !defined(UINT16_C) && !defined(INT16_C)
#define INT16_C(x)      S16_C(x)
#define UINT16_C(x)     U16_C(x)
#endif

#if !defined(INT32_C) && !defined(UINT32_C)
#define INT32_C(x)      S32_C(x)
#define UINT32_C(x)     U32_C(x)
#endif

#if !defined(INT64_C) && !defined(UINT64_C)
#define INT64_C(x)      S64_C(x)
#define UINT64_C(x)     U64_C(x)
#endif

/**@}*/

/**\name C standard macros */
#ifndef NULL
#ifdef __cplusplus
#define NULL   0
#else
#define NULL   ((void *) 0)
#endif
#endif
/********************************************************/

#ifndef TRUE
#define TRUE                UINT8_C(1)
#endif
#ifndef FALSE
#define FALSE               UINT8_C(0)
#endif

/**\name I2C addresses */
#define BME280_I2C_ADDR_PRIM	UINT8_C(0x76)
#define BME280_I2C_ADDR_SEC		UINT8_C(0x77)

/**\name BME280 chip identifier */
#define BME280_CHIP_ID  UINT8_C(0x60)

/**\name Register Address */
#define BME280_CHIP_ID_ADDR				UINT8_C(0xD0)
#define BME280_RESET_ADDR				UINT8_C(0xE0)
#define BME280_CTRL_HUM_ADDR			UINT8_C(0xF2)
#define BME280_STATUS_ADDR				UINT8_C(0xF3)
#define BME280_PWR_CTRL_ADDR			UINT8_C(0xF4)
#define BME280_CTRL_MEAS_ADDR			UINT8_C(0xF4)
#define BME280_CONFIG_ADDR				UINT8_C(0xF5)
#define BME280_P_DATA_ADDR				UINT8_C(0xF7)
#define BME280_T_DATA_ADDR				UINT8_C(0xFA)
#define BME280_H_DATA_ADDR				UINT8_C(0xFD)
#define BME280_T_CALIB_DATA_ADDR		UINT8_C(0x88)
#define BME280_P_CALIB_DATA_ADDR		UINT8_C(0x8E)
#define BME280_H_CALIB_DATA_ADDR1		UINT8_C(0xA1)
#define BME280_H_CALIB_DATA_ADDR2		UINT8_C(0xE1)

#define	TEMPERATURE_MIN		INT32_C( -4000 )
#define	TEMPERATURE_MAX 	INT32_C( 8500 )
#define PRESSURE_MIN 		UINT32_C( 30000 )
#define	PRESSURE_MAX		UINT32_C( 110000 )
#define HUMIDITY_MAX		UINT32_C( 102400 )

/**\name API error codes */
typedef enum {
	BME280_OK,
	BME280_E_NULL_PTR,
	BME280_E_DEV_NOT_FOUND,
	BME280_E_INVALID_LEN,
	BME280_E_COMM_FAIL,
	BME280_E_SLEEP_MODE_FAIL,
	BME280_E_CONV,
} bme280_error_t;

/**\name API warning codes */
#define BME280_W_INVALID_OSR_MACRO      INT8_C(1)

/**\name Macros related to size */
#define BME280_T_CALIB_DATA_LEN			UINT8_C(6)
#define BME280_P_CALIB_DATA_LEN			UINT8_C(18)
#define BME280_H_CALIB_DATA_LEN1		UINT8_C(1)
#define BME280_H_CALIB_DATA_LEN2		UINT8_C(8)
#define BME280_T_DATA_LEN				UINT8_C(3)
#define BME280_P_DATA_LEN				UINT8_C(3)
#define BME280_H_DATA_LEN				UINT8_C(2)

/**\name Sensor power modes */
#define	BME280_SLEEP_MODE		UINT8_C(0x00)
#define	BME280_FORCED_MODE		UINT8_C(0x01)
#define	BME280_NORMAL_MODE		UINT8_C(0x03)

/**\name Macro to combine two 8 bit data's to form a 16 bit data */
#define BME280_CONCAT_BYTES(msb, lsb)     (((uint16_t)msb << 8) | (uint16_t)lsb)

#define BME280_SET_BITS(reg_data, bitname, data) \
				((reg_data & ~(bitname##_MSK)) | \
				((data << bitname##_POS) & bitname##_MSK))
#define BME280_SET_BITS_POS_0(reg_data, bitname, data) \
				((reg_data & ~(bitname##_MSK)) | \
				(data & bitname##_MSK))

#define BME280_GET_BITS(reg_data, bitname)  ((reg_data & (bitname##_MSK)) >> \
							(bitname##_POS))
#define BME280_GET_BITS_POS_0(reg_data, bitname)  (reg_data & (bitname##_MSK))

/**\name Macros for bit masking */
#define BME280_SENSOR_MODE_MSK	UINT8_C(0x03)
#define BME280_SENSOR_MODE_POS	UINT8_C(0x00)

#define BME280_CTRL_HUM_MSK		UINT8_C(0x07)
#define BME280_CTRL_HUM_POS		UINT8_C(0x00)

#define BME280_CTRL_PRESS_MSK	UINT8_C(0x1C)
#define BME280_CTRL_PRESS_POS	UINT8_C(0x02)

#define BME280_CTRL_TEMP_MSK	UINT8_C(0xE0)
#define BME280_CTRL_TEMP_POS	UINT8_C(0x05)

#define BME280_FILTER_MSK		UINT8_C(0x1C)
#define BME280_FILTER_POS		UINT8_C(0x02)

#define BME280_STANDBY_MSK		UINT8_C(0xE0)
#define BME280_STANDBY_POS		UINT8_C(0x05)

/**\name Oversampling macros */
#define BME280_NO_OVERSAMPLING		UINT8_C(0x00)
#define BME280_OVERSAMPLING_1X		UINT8_C(0x01)
#define BME280_OVERSAMPLING_2X		UINT8_C(0x02)
#define BME280_OVERSAMPLING_4X		UINT8_C(0x03)
#define BME280_OVERSAMPLING_8X		UINT8_C(0x04)
#define BME280_OVERSAMPLING_16X		UINT8_C(0x05)

/**\name Standby duration selection macros */
#define BME280_STANDBY_TIME_1_MS              (0x00)
#define BME280_STANDBY_TIME_62_5_MS           (0x01)
#define BME280_STANDBY_TIME_125_MS			  (0x02)
#define BME280_STANDBY_TIME_250_MS            (0x03)
#define BME280_STANDBY_TIME_500_MS            (0x04)
#define BME280_STANDBY_TIME_1000_MS           (0x05)
#define BME280_STANDBY_TIME_10_MS             (0x06)
#define BME280_STANDBY_TIME_20_MS             (0x07)

/**\name Filter coefficient selection macros */
#define BME280_FILTER_COEFF_OFF               (0x00)
#define BME280_FILTER_COEFF_2                 (0x01)
#define BME280_FILTER_COEFF_4                 (0x02)
#define BME280_FILTER_COEFF_8                 (0x03)
#define BME280_FILTER_COEFF_16                (0x04)

#endif /* BME280_DEFS_H_ */
/** @}*/
/** @}*/
