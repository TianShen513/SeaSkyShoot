/**
 * @file io-leds.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "io-leds.h"


/*****************************************************************************/
/* ICE 03 START */
/*****************************************************************************/
void leds_init(void)
{
    cy_rslt_t rslt;
    
    // initialize Red LED P9_0 as an output
    rslt = cyhal_gpio_init(PIN_RGB_RED, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS);
    // initialize Green LED P9_2 as an output
    rslt = cyhal_gpio_init(PIN_RGB_GRN, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS);
    // initialize Blue LED P8_0 as an output
    rslt = cyhal_gpio_init(PIN_RGB_BLU, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS);
}
/*****************************************************************************/
/* ICE 03 END */
/*****************************************************************************/



/*****************************************************************************/
/* ICE 06 START                                                              */
/*****************************************************************************/

/*
* Global variables that are used to interact with the PWM peripherals for
* the 3 pins that control the RGB LED.
*/

/**
 * @brief 
 *  Initializes the RGB LED pins to be controlled by thier PWM peripherals 
 */
void leds_pwm_init(void)
{
    cy_rslt_t rslt;

}
/*****************************************************************************/
/* ICE 06 END                                                                */
/*****************************************************************************/