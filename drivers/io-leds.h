/**
 * @file io-leds.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __IO_LEDS_H__
#define __IO_LEDS_H__

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include <ctype.h>
#include <stdio.h>

/*****************************************************************************/
/* ICE 03 START */
/*****************************************************************************/
void leds_init(void);

/* Push Button (SW) Pins */
#define PIN_RGB_RED                 P9_0        // MODIFY
#define PIN_RGB_GRN                 P9_2        // MODIFY
#define PIN_RGB_BLU                 P8_0        // MODIFY

/* Bit Masks for Push Buttons */
#define MASK_RGB_RED                (1 << 0)    // MODIFY
#define MASK_RGB_GRN                (1 << 2)    // MODIFY
#define MASK_RGB_BLU                (1 << 0)    // MODIFY

/* Define a Macro for accessing the Port used to control
*  each of the LEDs
*/
#define PORT_RGB_RED                GPIO_PRT9   // MODIFY 
#define PORT_RGB_GRN                GPIO_PRT9   // MODIFY 
#define PORT_RGB_BLU                GPIO_PRT8   // MODIFY

/*****************************************************************************/
/* ICE 03 END */
/*****************************************************************************/

/*****************************************************************************/
/* ICE 06 START                                                              */
/*****************************************************************************/

/* 
* Provide external access to the PWM Handles for the 3 different IO pins.
*/
extern cyhal_pwm_t pwm_red_obj;
extern cyhal_pwm_t pwm_green_obj;
extern cyhal_pwm_t pwm_blue_obj;

/**
 * @brief 
 *  Initializes the RGB LED pins to be controlled by thier PWM peripherals 
 */
void leds_pwm_init(void);

/*****************************************************************************/
/* ICE 06 END                                                                */
/*****************************************************************************/


#endif