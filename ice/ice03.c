/**
 * @file ice03.c
 * @author Bowen Quan bquan2@wisc.edu
 * @brief
 * @version 0.1
 * @date 2023-08-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "../main.h"

#if defined(ICE) && (FILE_ID == 3)

/*****************************************************************************/
/* Macros                                                                    */
/*****************************************************************************/

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/
char ICE_DESCRIPTION[] = "ECE353: ICE 03 - PSoC6 IO Ports";

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
    push_buttons_init();
    leds_init();
}

/**
 * @brief
 * This function implements the behavioral requirements for the ICE
 */
void main_app(void)
{
    button_state_t button_state;

     for (;;)
    {
        button_state = get_buttons();

        switch(button_state)
        {
            case BUTTON_INACTIVE:
            {
                /* turn off all LEDS*/
                PORT_RGB_RED->OUT &= ~MASK_RGB_RED;
                PORT_RGB_GRN->OUT &= ~MASK_RGB_GRN;
                PORT_RGB_BLU->OUT &= ~MASK_RGB_BLU;
                break;
            }
            case BUTTON_SW1_PRESSED:
            {
                /* Turn RED LED On*/
                PORT_RGB_RED->OUT |= MASK_RGB_RED;
                /* Turn other LEDs off*/
                PORT_RGB_GRN->OUT &= ~MASK_RGB_GRN;
                PORT_RGB_BLU->OUT &= ~MASK_RGB_BLU;
                break;
            }
            case BUTTON_SW2_PRESSED:
            {
                /* Turn Green LED On*/
                PORT_RGB_GRN->OUT |= MASK_RGB_GRN;
                /* Turn other LEDs off*/
                PORT_RGB_RED->OUT &= ~MASK_RGB_RED;
                PORT_RGB_BLU->OUT &= ~MASK_RGB_BLU;
                break;
            }
            case BUTTON_SW3_PRESSED:
            {
                /* Turn Blue LED On*/
                PORT_RGB_BLU->OUT |= MASK_RGB_BLU;
                /* Turn other LEDs off*/
                PORT_RGB_RED->OUT &= ~MASK_RGB_RED;
                PORT_RGB_GRN->OUT &= ~MASK_RGB_GRN;
                break;
            }
            case BUTTON_SW1_RELEASED:
            {
                printf("SW1 was released\n\r");
                break;
            }
            case BUTTON_SW2_RELEASED:
            {
                printf("SW2 was released\n\r");
                break;
            }
            case BUTTON_SW3_RELEASED:
            {
                printf("SW3 was released\n\r");
                break;
            }
            default:
            {
                /* turn off all LEDS*/
                PORT_RGB_RED->OUT &= ~MASK_RGB_RED;
                PORT_RGB_GRN->OUT &= ~MASK_RGB_GRN;
                PORT_RGB_BLU->OUT &= ~MASK_RGB_BLU;
                printf("Unknown return value\n\r");
                break;
            }
        }
        cyhal_system_delay_ms(50);
    }
}
#endif