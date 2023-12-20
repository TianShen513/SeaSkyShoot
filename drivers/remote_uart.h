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

#ifndef __REMOTE_UART_H__
#define __REMOTE_UART_H__

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "circular_buffer.h"
#include "remote_uart_rx.h"
#include "remote_uart_tx.h"

#define REMOTE_SCB     SCB2

/* UART (External RJ45) Pins*/
#define PIN_EXT_UART_RX		P3_0
#define PIN_EXT_UART_TX		P3_1

#define DATA_BITS_8     8
#define STOP_BITS_1     1
#define BAUD_RATE       115200

#define STX 2
#define ETX 3
#define ACK 6

/*******************************************************************************
 * Public Global Variables
 ******************************************************************************/

/*******************************************************************************
 * Remote UART Public Functions
 ******************************************************************************/

/**
 * @brief 
 * Prints out a NULL terinated string to the remote UART 
 * @param msg 
 * String to print
 */
void remote_uart_tx_string_polling(uint8_t * msg);

/**
 * @brief 
 * Returns a null ternimated string that was received on the SCB connected to the onboard
 * RJ45 connector 
 * @return char* 
 * The string received
 */
bool remote_uart_rx_string_polling(uint8_t *msg);

/**
 * @brief
 * This function will initialize the SCB connected to the RJ45 jack as a UART
 * running at 115200, 8N1
 */
void remote_uart_init(void);

/**
 * @brief 
 * Enable Rx Interrupts and Initialize the circular buffers for receiving and transmitting data 
 */
void remote_uart_enable_interrupts(void);

#endif