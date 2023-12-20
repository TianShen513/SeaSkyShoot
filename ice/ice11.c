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

#if defined(ICE) && (FILE_ID == 11)

char ICE_DESCRIPTION[] = "ECE353: ICE 11 - UART Rx Interrupts";

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

    // Enable interrupts and circular buffers from remote board.
    remote_uart_enable_interrupts();
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
    uint8_t remote_rx_message[80];

    /* Clear out the array holding the message */
    memset(remote_rx_message,0,80);

    
    cyhal_uart_putc(&cy_retarget_io_uart_obj, '\r');

    while (1)
    {
        /* Check to see if the user has entered a string into the console*/
        if (console_rx_string(console_msg))
        {
            printf("Tx message -> %s\n\r", console_msg);

            /* Send the String to the remote board */
             remote_uart_tx_string_polling(console_msg);

        }
        if (ALERT_UART_RX)
        {
            /* Set the alert back to false*/
            ALERT_UART_RX = false;

            /* Initialize the array to all 0*/
            memset(remote_rx_message, 0, 80);

            if (remote_uart_rx_data_async(remote_rx_message, 80))
            {
                /* print out the message */
                printf("Rx Asynch message <- %s\n\r", remote_rx_message);
            }
        }
    }
}

#endif