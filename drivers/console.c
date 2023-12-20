/**
 * @file console.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "console.h"

void console_init(void)
{
    cy_rslt_t rslt;

    /* Initialize retarget-io to use the debug UART port, 8N1 */
    rslt = cy_retarget_io_init(
        PIN_UART_DBG_TX,
        PIN_UART_DBG_RX,
        115200); // Baud Rate

    // If the initialization of the console fails, halt the MCU
    CY_ASSERT(rslt == CY_RSLT_SUCCESS);
}