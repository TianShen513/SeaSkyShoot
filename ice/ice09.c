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

#if defined(ICE) && (FILE_ID == 9)

char ICE_DESCRIPTION[] = "ECE353: ICE 09 - UART Polling";

extern cyhal_uart_t cy_retarget_io_uart_obj;

/*****************************************************************************/
/*  Function Declarations                                                    */
/*****************************************************************************/

/*****************************************************************************/
/*  Global Variables                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* Peripheral Initilization                                                  */
/*****************************************************************************/

/**
 * @brief
 * This function will initialize all of the hardware resources for
 * the ICE
 */
void peripheral_init(void)
{
    // Initialize push buttons
    push_buttons_init();

    // Initialize the remote UART port
    remote_uart_init();
}

/*****************************************************************************/
/* Application Code                                                          */
/*****************************************************************************/
/**
 * @brief
 * This function implements the behavioral requirements for the ICE
 */
void main_app(void)
{
    uint8_t console_msg[80];
    uint8_t remote_rx_msg[80];

    /* Clear out the arrays holding the messages */
    memset(remote_rx_msg, 0, 80);
    memset(console_msg, 0, 80);

    cyhal_uart_putc(&cy_retarget_io_uart_obj, '\r');

    while (1)
    {

        /*
         * If the user enters a string at the serial console, send
         * the string to the other board using the remote uart
         */
        if (console_rx_string(console_msg))
        {
            printf("Tx message -> %s\n\r", console_msg);

            /* Send the String to the remote board */
            remote_uart_tx_string_polling(console_msg);

            /* clear the array for the next message */
            memset(console_msg, 0, 80);
        }

        /*
         * If you receive a string from the remote uart, print
         * the string to the console
         */
        if (remote_uart_rx_string_polling(remote_rx_msg))
        {
            /* print out the message */
            printf("Rx message <- %s\n\r", remote_rx_msg);

            /* clear the array for the next message */
            memset(remote_rx_msg, 0, 80);
        }
    }
}

#endif