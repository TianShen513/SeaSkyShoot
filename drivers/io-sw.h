/**
 * @file io-sw.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __IO_SW_H__
#define __IO_SW_H__

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include <ctype.h>
#include <stdio.h>

/*****************************************************************************/
/* ICE 02 START */
/*****************************************************************************/

/* Push Button (SW) Pins */
#define PIN_SW1                 P6_3       // MODIFY
#define PIN_SW2                 P6_4        // MODIFY
#define PIN_SW3                 P6_5        // MODIFY

/* Bit Masks for Push Buttons */
#define SW1_MASK                (1 << 3)    // MODIFY
#define SW2_MASK                (1 << 4)    // MODIFY
#define SW3_MASK                (1 << 5)    // MODIFY

/* Memory address for Push Button IO Port IN register */
#define ADDR_REG_PUSH_BUTTON_IN     0x40310310      // MODIFY
#define REG_PUSH_BUTTON_IN          (*(volatile uint32_t *)(ADDR_REG_PUSH_BUTTON_IN))

#define PORT_BUTTONS                GPIO_PRT6       // MODIFY

typedef enum {
    BUTTON_INACTIVE,
    BUTTON_SW1_PRESSED,
    BUTTON_SW2_PRESSED,
    BUTTON_SW3_PRESSED,
    BUTTON_SW1_RELEASED,
    BUTTON_SW2_RELEASED,
    BUTTON_SW3_RELEASED,
}button_state_t;

/**
 * @brief 
 * Initializes the 3 push buttons on the ECE353 Dev Board 
 */
void push_buttons_init(void);

/**
 * @brief
 * Returns which button has been pressed and then released. 
 *  
 * This function should examine the current state of each button and
 * compare it with the previous state of the button.  If the current
 * state is not-pressed and the previous state was pressed, return that
 * the button has been released.
 * 
 * If none of the buttons have been released, check to see if the button is 
 * currently being pressed.  If it is, return that the button is being pressed
 * 
 * If multiple buttons are pressed/released, return the button state of the button
 * with the highest SW number (SW3 has highest priority, then SW2, then SW1)
 * 
 * This function should only read the IN register once.
 * 
 * @return button_state_t 
 * Returns which of the buttons is currently being pressed.  
 */
button_state_t get_buttons(void);

/*****************************************************************************/
/* ICE 02 END */
/*****************************************************************************/

#endif