/*
 * opt3001.h
 *
 *  Created on: Oct 20, 2020
 *      Author: Joe Krachey
 */

#ifndef IO_EXPANDER_H_
#define IO_EXPANDER_H_

#include <stdint.h>
#include <stdbool.h>
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#include "i2c.h"

/* ADD CODE */
#define LTR_329ALS_01_ADDR                 0x29

/* ADD CODE for IO Expander Register Addresses */
#define LTR_ALS_CONTR_ADDR			             0x80 // ALS operation mode control SW reset
#define LTR_ALS_MEAS_RATE 				         0x85 // ALS measurement rate in active mode
#define LTR_ALS_DATA_CH1_0 					     0x88 // ALS measurement CH1 data, lower byte   Q
#define LTR_ALS_DATA_CH1_1 						 0x89 // ALS measurement CH1 data, upper byte
#define LTR_ALS_DATA_CH0_0                       0x8A // ALS measurement CH0 data, lower byte
#define LTR_ALS_DATA_CH0_1                       0x8B // ALS measurement CH0 data, upper byte
#define LTR_ALS_STATUS                           0x8C // ALS new data status

extern bool ALERT_IO_EXPANDER;
/* ADD CODE */
#define TCA9534_SUBORDINATE_ADDR                 0x20

/* ADD CODE for IO Expander Register Addresses */
#define TCA9534_INPUT_PORT_ADDR					 0x00
#define TCA9534_OUTPUT_PORT_ADDR				 0x01
#define TCA9534_POLARITY_ADDR					 0x02
#define TCA9534_CONFIG_ADDR						 0x03

extern bool ALERT_IO_EXPANDER;

/** Read the value of the input port
 *
 * @param reg The reg address to read
 *
 */
uint8_t io_expander_get_input_port(void);

/** Write the value of the output port
 *
 * @param value The value written
 *
 */
void io_expander_set_output_port(uint8_t value);

/** Write the polarity inversion register
 *
 * @param value The value written
 *
 */
void io_expander_set_polarity_inversion(uint8_t value);

/** Write the value of the configuration Register
 *
 * @param value The value written
 *
 */
void io_expander_set_configuration(uint8_t value);


uint8_t io_expander_get_contr_port(void);
void io_expander_set_contr_port(uint8_t value);
uint8_t io_expander_get_meas_port(void);
void io_expander_set_meas_port(uint8_t value);
uint8_t io_read_reg(uint8_t reg);

void io_expander_led_grn(void);
void io_expander_led_red(void);

#endif /* IO_EXPANDER_H_ */

