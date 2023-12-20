/**
 * @file remote_uart_tx.h
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-09-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __REMOTE_UART_TX_H__
#define __REMOTE_UART_TX_H__

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "circular_buffer.h"

#define TX_BUFFER_SIZE  256
#define TX_IRQ_PRIORITY 3

extern Circular_Buffer *Tx_Circular_Buffer;

/*******************************************************************************
 * Remote UART Public Functions
 ******************************************************************************/

/**
 * @brief 
 * Enables Tx Interrupts for the remote UART. Transmitting and 
 * receiveing of data will utilize circular buffers to help minimize the amount
 * of time spent waiting for the UART interface.
 */
void remote_uart_tx_interrupts_init(void);

/**
 * @brief
 * Send a character to the remote UART
 * @param 
 * Character to Send
 */
void remote_uart_tx_char_async(char c);

/**
 * @brief
 * Prints out a NULL terinated string to the remote UART
 * @param msg
 * String to print
 */
void remote_uart_tx_data_async(char *msg);

/**
 * @brief 
 * Used the UART handler to facilitate the transmission of data using
 * a circular buffer 
 */
void remote_uart_event_handler_process_tx(void);

#endif