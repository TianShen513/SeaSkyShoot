/**
 * @file ice07.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "../main.h"

#if defined(ICE) && (FILE_ID == 7)

/*****************************************************************************/
/* Macros                                                                    */
/*****************************************************************************/

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/
char ICE_DESCRIPTION[] = "ECE353: ICE 07 - IO Interrupts";

/* ADD CODE */

cyhal_gpio_callback_data_t handler_sw1_cb_data =
{
    .callback     = Handler_SW1,
    .callback_arg = NULL
};

cyhal_gpio_callback_data_t handler_sw2_cb_data =
{
    .callback     = Handler_SW2,
    .callback_arg = NULL
};

/*****************************************************************************/
/* Function Declarations                                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Function Definitions                                                      */
/*****************************************************************************/

/**
 * @brief 
 *  Function used as SW1 Handler
 */
void Handler_SW1(void* handler_arg, cyhal_gpio_event_t event)
{
    CY_UNUSED_PARAMETER(handler_arg);
    CY_UNUSED_PARAMETER(event);

    /* ADD CODE */

}

/**
 * @brief 
 *  Function used as SW2 Handler
 */
void Handler_SW2(void* handler_arg, cyhal_gpio_event_t event)
{
    CY_UNUSED_PARAMETER(handler_arg);
    CY_UNUSED_PARAMETER(event);

    /* ADD CODE */

}

/**
 * @brief 
 * This function will initialize all of the hardware resources for
 * the ICE
 */
void peripheral_init(void)
{
    /* Initialize the Switches as input pins*/
    push_buttons_init();

    /* Register a callback function for SW1 */

    /* Register a callback function for SW2 */

    /* Enable Falling Edge Interrupts on SW1 */

    /* Enable Falling Edge Interrupts on SW2 */

}

/**
 * @brief 
 * This function implements the behavioral requirements for the ICE
 */
void main_app(void)
{
    while(1)
    {
    }
}
#endif