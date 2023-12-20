/**
 * @file joystick.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include <ctype.h>
#include <stdio.h>

/* Analog Pins*/
/* ADD CODE */
#define PIN_ANALOG_JOY_X	P10_6
#define PIN_ANALOG_JOY_Y	P10_7

#define JOYSTICK_THRESH_X_LEFT_2P7V   0xD173
#define JOYSTICK_THRESH_X_RIGHT_0P6V  0x2E8B
#define JOYSTICK_THRESH_Y_UP_2P7V     0xD173
#define JOYSTICK_THRESH_Y_DOWN_0P6V   0x2E8B

/* Custom Data Typtes */
typedef enum {
    JOYSTICK_POS_CENTER,
    JOYSTICK_POS_LEFT,
    JOYSTICK_POS_RIGHT,
    JOYSTICK_POS_UP,
    JOYSTICK_POS_DOWN,
    JOYSTICK_POS_UPPER_LEFT,
    JOYSTICK_POS_UPPER_RIGHT,
    JOYSTICK_POS_LOWER_LEFT,
    JOYSTICK_POS_LOWER_RIGHT
}joystick_position_t;

/* Public Global Variables */

/* Public API */

/** Initialize the ADC channels connected to the Joystick
 *
 * @param - None
 */
void joystick_init(void);

/** Read X direction of Joystick 
 *
 * @param - None
 */
uint16_t  joystick_read_x(void);

/** Read Y direction of Joystick 
 *
 * @param - None
 */
uint16_t  joystick_read_y(void);

/**
 * @brief 
 * Returns the current position of the joystick 
 * @return joystick_position_t 
 */
joystick_position_t joystick_get_pos(void);

/**
 * @brief 
 * Prints out a string to the console based on parameter passed 
 * @param position 
 * The enum value to be printed.
 */
void joystick_print_pos(joystick_position_t position);

#endif /* __JOYSTICK_H__ */