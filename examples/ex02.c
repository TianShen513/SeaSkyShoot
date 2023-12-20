/**
 * @file ex01.c
 * @author Bowen Quan bquan2@wisc.edu
 * @brief
 * @version 0.1
 * @date 2023-08-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "../main.h"

#if defined(EXAMPLE) && (FILE_ID == 2)

/*****************************************************************************/
/* Macros                                                                    */
/*****************************************************************************/

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/
char ICE_DESCRIPTION[] = "ECE353: Ex02 - Memory Mapped IO";


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
   push_buttons_init();
}

/**
 * @brief
 * This function implements the behavioral requirements for the application
 */
void main_app(void)
{
    uint32_t reg_val;
    /* Enter Infinite Loop*/
    while (1)
    {
        reg_val = REG_PUSH_BUTTON_IN;

        if ((reg_val & SW1_MASK) == 0) {
            printf("SW1 Pressed\n\r");
        }

        cyhal_system_delay_ms(250);
    }
}
#endif