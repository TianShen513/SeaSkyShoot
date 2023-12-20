/*
 * i2c.h
 *
 *  Created on: Jan 21, 2022
 *      Author: Joe Krachey
 */

#ifndef I2C_H_
#define I2C_H_

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

/* I2C Pins*/
#define PIN_I2C_SCL			P0_2
#define PIN_I2C_SDA			P0_3

/* Macros */
#define I2C_MONARCH_FREQUENCY 100000u

/* Public Global Variables */
extern cyhal_i2c_t i2c_monarch_obj;

/* Public API */

/** Initialize the I2C bus to the specified module site
 *
 * @param - None
 */
cy_rslt_t i2c_init(void);

#endif /* I2C_H_ */
