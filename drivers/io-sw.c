/**
 * @file io-sw.c
 * @author Bowen Quan bquan2@wisc.edu
 * @brief 
 * @version 0.1
 * @date 2023-09-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "io-sw.h"

/*****************************************************************************/
/* ICE 02 START */
/*****************************************************************************/
void push_buttons_init(void)
{
    cy_rslt_t rslt;

    rslt = cyhal_gpio_init(PIN_SW1, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, 0);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS);
    rslt = cyhal_gpio_init(PIN_SW2, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, 0);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS);
    rslt = cyhal_gpio_init(PIN_SW3, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, 0);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS);
    //printf("buttons initialized\n\r");
} 
/*****************************************************************************/
/* ICE 02 END */
/*****************************************************************************/


/*****************************************************************************/
/* ICE 03 START */
/*****************************************************************************/
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
button_state_t get_buttons(void)
{
    // static variables for the previous value of the buttons
    // true if the button is not pressed
    // false if button is pressed 
    // this is opposite to how it was done in ex03.c
    static bool button1_prev = true;
    static bool button2_prev = true;
    static bool button3_prev = true;

    uint32_t buttons_in = PORT_BUTTONS->IN; // get input register of buttons

    // get current values of buttons
    bool button1 = true;
    bool button2 = true;
    bool button3 = true;
    if ((buttons_in & SW1_MASK) == 0) {
        button1 = false;
    } 
    if ((buttons_in & SW2_MASK) == 0) {
        button2 = false;
    } 
    if ((buttons_in & SW3_MASK) == 0) {
        button3 = false;
    } 

    // check if buttons were released, in the order of 3, 2, 1
    if (button3 && !button3_prev) {
        // set previous button values to current values
        button3_prev = button3;
        button2_prev = button2;
        button1_prev = button1;
        return BUTTON_SW3_RELEASED;
    }
    if (button2 && !button2_prev) {
        // set previous button values to current values
        button3_prev = button3;
        button2_prev = button2;
        button1_prev = button1;
        return BUTTON_SW2_RELEASED;
    }
    if (button1 && !button1_prev) {
        // set previous button values to current values
        button3_prev = button3;
        button2_prev = button2;
        button1_prev = button1;
        return BUTTON_SW1_RELEASED;
    }
    
    // check if buttons were pressed, in the order of 3, 2, 1
    if (!button3) {
        // set previous button values to current values
        button3_prev = button3;
        button2_prev = button2;
        button1_prev = button1;
        return BUTTON_SW3_PRESSED;
    }
    if (!button2) {
        // set previous button values to current values
        button3_prev = button3;
        button2_prev = button2;
        button1_prev = button1;
        return BUTTON_SW2_PRESSED;
    }
    if (!button1) {
        // set previous button values to current values
        button3_prev = button3;
        button2_prev = button2;
        button1_prev = button1;
        return BUTTON_SW1_PRESSED;
    }

    return BUTTON_INACTIVE; // default return if no buttons are pressed or released.
}

/*****************************************************************************/
/* ICE 03 END */
/*****************************************************************************/