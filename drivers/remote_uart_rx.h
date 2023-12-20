/**
 * @file remote_uart.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __REMOTE_UART_RX_H__
#define __REMOTE_UART_RX_H__

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "circular_buffer.h"

#define RX_BUFFER_SIZE 256
#define RX_IRQ_PRIORITY 3

/*******************************************************************************
 * Public Global Variables
 ******************************************************************************/
extern volatile bool ALERT_UART_RX;
extern Circular_Buffer *Rx_Circular_Buffer;

/*******************************************************************************
 * Remote UART Public Functions
 ******************************************************************************/
/**
 * @brief 
 * Enables Rx Interrupts for the remote UART.  
 * Receiveing of data will utilize circular buffers to help minimize the amount
 * of time spent waiting for the UART interface.
 */
void remote_uart_rx_interrupts_init(void);

/**
 * @brief 
 * Return a message from the circular buffer 
 * @param msg 
 * Starting address to place the message
 * @param max_size 
 * Max number of characters to copy
 * @return true 
 * Message copied successfully
 * @return false 
 * No message was found in circular buffer
 */
bool remote_uart_rx_data_async(char *msg, uint16_t max_size);

/**
 * @brief 
 * Used to process data received by the UART and then placed into a 
 * circular buffer 
 */
void remote_uart_event_handler_process_rx(void);
#endif