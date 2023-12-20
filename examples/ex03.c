/**
 * @file ex03.c
 * @author Bowen Quan bquan2@wisc.edu
 * @brief
 * @version 0.1
 * @date 2023-08-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "../main.h"

#if defined(EXAMPLE) && (FILE_ID == 3)

/*****************************************************************************/
/* Macros                                                                    */
/*****************************************************************************/

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/
char ICE_DESCRIPTION[] = "ECE353: Ex03 - PSoC6 IO Ports";
uint8_t ArrayA[20];
uint8_t ArrayB[20];

/*****************************************************************************/
/* Function Declarations                                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Function Definitions                                                      */
/*****************************************************************************/

/**
 * @brief
 * This function will initialize hardware resources 
 */
void peripheral_init(void)
{
    cy_rslt_t rslt;

    // initialize SW1 P6_3 as an input
    rslt = cyhal_gpio_init(PIN_SW1, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, 0);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS);

    // initialize Red LED P9_0 as an output
    rslt = cyhal_gpio_init(PIN_RGB_RED, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS);
}

/**
 * @brief
 * This function implements the behavioral requirements for the application
 */
void main_app(void)
{
    // local vars that store the current and previous state of SW1
    bool sw1_curr = false;
    bool sw1_prev = false;


    while (1) {
        // check current value of SW1
        // PORT_BUTTONS is GPIO_PRT6
        if ((PORT_BUTTONS->IN & SW1_MASK) == 0) {
            sw1_curr = true;
        } else {
            sw1_curr = false;
        }
        // If the button was pressed (falling edge), turn red LED on
        if (sw1_curr && !sw1_prev) {
            printf("SW1 Pressed\n\r");
            PORT_RGB_RED->OUT |= MASK_RGB_RED;
        }
        // if the button was released, turn red LED off
        if (!sw1_curr && sw1_prev) {
            printf("SW1 Released\n\r");
            PORT_RGB_RED->OUT &= ~MASK_RGB_RED;
        }
        // save the current state to the previous state 
        sw1_prev = sw1_curr;
        cyhal_system_delay_ms(100);
    }
}
#endif