/**
 * @file i2c.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "i2c.h"

cyhal_i2c_t i2c_monarch_obj;

// Define the I2C monarch configuration structure
cyhal_i2c_cfg_t i2c_monarch_config =
{
    CYHAL_I2C_MODE_MASTER,
    0, // address is not used for master mode
    I2C_MONARCH_FREQUENCY
};

/** Initialize the I2C bus to the specified module site
 *
 * @param - None
 */
cy_rslt_t i2c_init(void)
{
	cy_rslt_t rslt;

    // Initialize I2C monarch, set the SDA and SCL pins and assign a new clock
	rslt = cyhal_i2c_init(&i2c_monarch_obj, PIN_I2C_SDA, PIN_I2C_SCL, NULL);

	if(rslt != CY_RSLT_SUCCESS)
	{
	    CY_ASSERT(0);

	    while(1){};
	}

    // Configure the I2C resource to be monarch
	rslt = cyhal_i2c_configure(&i2c_monarch_obj, &i2c_monarch_config) ;

	if(rslt != CY_RSLT_SUCCESS)
	{
	    CY_ASSERT(0);

	    while(1){};
	}

	return CY_RSLT_SUCCESS;
}
