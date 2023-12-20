/**
 * @file systick.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#include <stdint.h>
#include <stdbool.h>

/****************************************************************************/
/* ICE 05 START                                                             */
/****************************************************************************/
#define SYSTICK_CTRL_COUNTFLAG              ( 1 << 16 )
#define SYSTICK_CTRL_CLKSOURCE              ( 1 << 2 )
#define SYSTICK_CTRL_ENABLE_IRQ             ( 1 << 1 )
#define SYSTICK_CTRL_ENABLE                 ( 1 << 0 )

#define TICKS_MS_001                        (100000)
#define TICKS_MS_005                        (500000)
#define TICKS_US_100                        (10000)

/**
 * @brief 
 *  Initilaizes the SysTick Timer for a configurable interval of time. 
 * @param ticks 
 *  The Number clock cycles 
 */
void systick_init(uint32_t ticks);

bool systick_expired(void);

/****************************************************************************/
/* ICE 05 END                                                               */
/****************************************************************************/

#endif
