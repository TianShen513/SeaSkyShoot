/**
 * @file hw01_timer.h
 * @author Tianqi Shen (tshen46@wisc.edu) & Bowen Quan (bquan2@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-09-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __HW01_TIMER_H__
#define __HW01_TIMER_H__

#include "main.h"

typedef enum {
    HW01_ALERT_NONE,                /* There is No Event Present*/
    HW01_ALERT_BUTTON_LT_2S,        /* A button was pressed for less than 2 seconds */
    HW01_ALERT_BUTTON_GT_2S,        /* A button was pressed for more than 2 seconds */
    HW01_ALERT_TIME_UPDATE,         /* Update the clock's current time */
    HW01_ALERT_BLINK                /* Blink the Time/Alarm at a rate of 2Hz */
}hw01_timer_alerts_t;

/* Global Variables that will be used to detect when various
 * events have occurred
 */


/**
 * @brief 
 * Used to enable a TCPWM timer with interrupts
 * @param ticks 
 * Period used to generate interrupts.
 */
void hw01_timer_init(uint32_t ticks);

#endif