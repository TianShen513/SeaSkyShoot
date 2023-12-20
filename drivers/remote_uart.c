/**
 * @file remote_uart.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "remote_uart.h"
/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
cyhal_uart_t remote_uart_obj;

const cyhal_uart_cfg_t remote_uart_config =
    {
        .data_bits = DATA_BITS_8,
        .stop_bits = STOP_BITS_1,
        .parity = CYHAL_UART_PARITY_NONE,
        .rx_buffer = NULL,
        .rx_buffer_size = 0};

uint32_t actualbaud;

/*******************************************************************************
 * Function Declarations
 ******************************************************************************/
void remote_uart_event_handler(void *handler_arg, cyhal_uart_event_t event);

/**
 * @brief
 * Prints out a NULL terinated string to the remote UART
 * @param msg
 * String to print
 */
void remote_uart_tx_string_polling(uint8_t *msg)
{
    /* ADD CODE */

    /* check to see that msg pointer is not equal to NULL*/
    if (msg != NULL) {
        /* Use the HAL api to print out 1 character at a time until the NULL charcter is found*/
        while(*msg != 0) {
            cyhal_uart_putc(&remote_uart_obj, *msg);
            msg++;
        }
    }


}

/**
 * @brief
 * Returns a null ternimated string that was received on the SCB connected to the onboard
 * RJ45 connector.
 *
 * The function will continue to add characters to Rx_Buffer until one of the following
 * characters has been received: ETX, \n, or \r
 */
bool remote_uart_rx_string_polling(uint8_t *msg)
{
    static uint8_t temp_buffer[80];
    static uint8_t buffer_index = 0;
    cy_rslt_t rslt;
    bool return_value = false;
    uint8_t c;

    /* Check to see if there is a new character from the console*/
    /* Wait for 1ms if no character has been received */
    rslt = cyhal_uart_getc(&remote_uart_obj, &c, 1);
    if (rslt == CY_RSLT_SUCCESS /* Determine if a character has been receieved */)
    {
        /* Add the current character to the message*/
        temp_buffer[buffer_index] = c;
        /* If the character returned is a \n, copy the string to msg and return true*/
        if (c == '\n' || c == '\r') {
            temp_buffer[buffer_index] = 0; // set last char to null terminating character
            strcpy(msg, temp_buffer);
            buffer_index = 0; // reset buffer index
            return_value = true;
        } else {
            buffer_index++; // increment index
        }
    }

    return return_value;
}

/**
 * @brief
 * Interrupt Handler for the remote UART
 * @param handler_arg
 * @param event
 */
void remote_uart_event_handler(void *handler_arg, cyhal_uart_event_t event)
{
    (void)handler_arg;
    cy_rslt_t status;
    char c;

    if ((event & CYHAL_UART_IRQ_TX_ERROR) == CYHAL_UART_IRQ_TX_ERROR)
    {
        /* An error occurred in Tx */
        /* Insert application code to handle Tx error */
    }
    else if ((event & CYHAL_UART_IRQ_RX_NOT_EMPTY) == CYHAL_UART_IRQ_RX_NOT_EMPTY)
    {
        remote_uart_event_handler_process_rx();
    }
    else if ((event & CYHAL_UART_IRQ_TX_EMPTY) == CYHAL_UART_IRQ_TX_EMPTY)
    {
        remote_uart_event_handler_process_tx();
    }
}

/**
 * @brief
 * Enables UART interrupts, registers a call back function, and initializes the
 * circular buffers.
 */
void remote_uart_enable_interrupts(void)
{
    /* Register the Handler */
    cyhal_uart_register_callback(&remote_uart_obj, remote_uart_event_handler, NULL);

    /* Enable Rx Interrupts and initialize Rx Circular Buffer */
    remote_uart_rx_interrupts_init();

    /* Initialize Tx Circular Buffer, Tx_Empty Interrupts are turned off initially */
    remote_uart_tx_interrupts_init();
}

/**
 * @brief
 * Initialize the SCB connected to the RJ45 jack as a UART running at 115200, 8N1
 */
void remote_uart_init()
{
    cy_rslt_t rslt;

    /* Initialize the SCB for the remote UART */
    rslt = cyhal_uart_init(
        &remote_uart_obj,
        PIN_EXT_UART_TX,      // TX Pin
        PIN_EXT_UART_RX,      // RX Pin
        NC,                   // CTS Pin (No connect)
        NC,                   // RTS Pin (No connect)
        NULL,                 // Clock Source
        &remote_uart_config); // Configuration Structure

    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // Check to see if configuration was successful

    /* Set the baud rate */
    rslt = cyhal_uart_set_baud(
        &remote_uart_obj,
        BAUD_RATE,
        &actualbaud);

    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // Check to see if configuration was successful

    /* Clear the UART */
    cyhal_uart_clear(&remote_uart_obj);
}
