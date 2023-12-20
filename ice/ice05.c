/**
 * @file ice05.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "../main.h"

#if defined(ICE) && (FILE_ID == 5)

/*****************************************************************************/
/* Macros                                                                    */
/*****************************************************************************/

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/
char ICE_DESCRIPTION[] = "ECE353: ICE 05 - SysTick";

/*****************************************************************************/
/* Function Declarations                                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Function Definitions                                                      */
/*****************************************************************************/

/**
 * @brief
 * This function will initialize all of the hardware resources for
 * the ICE
 */
void peripheral_init(void)
{
    /* Initialize the LEDs */
    leds_init();

    /* Initialize the push buttons */
    push_buttons_init();

    /* Set the SysTick Timer to that it  expires every 100uS*/
    /* ADD CODE */
}

/**
 * @brief
 * This function implements the behavioral requirements for the ICE
 */
void main_app(void)
{
    button_state_t button_state;
    uint8_t timer_count = 0;
    uint8_t on_threshold = 50;

    for (;;)
    {
        /* Check to see if the systick timer has expired */
        if (systick_expired())
        {
            /* ADD CODE */
        }
        
        /* Determine if the Red LED should be on or off */
        /* ADD CODE */
    }
}
#endif