/**
 * @file ece353.h
 * @author Tianqi Shen (tshen46@wisc.edu) & Bowen Quan (bquan2@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-08-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __MAIN_H__
#define __MAIN_H__

/* Include Infineon BSP Libraries */
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

/* Include Stanard C Libraries*/
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>

/* Include ECE353 Drivers */
#include "drivers/console.h"
#include "drivers/io-sw.h"
#include "drivers/io-leds.h"
#include "drivers/io-lcd.h"
#include "drivers/lcd_images.h"
#include "drivers/joystick.h"
#include "drivers/remote_uart.h"
#include "drivers/remote_uart_tx.h"
#include "drivers/remote_uart_rx.h"
#include "drivers/circular_buffer.h"
#include "drivers/systick.h"
//#include "drivers/timer.h"
#include "drivers/pwm-buzzer.h"
#include "drivers/i2c.h"
#include "drivers/io-expander.h"
#include "drivers/imu.h"
#include "drivers/spi.h"
#include "drivers/lsm6dsm_reg.h"
#include "drivers/eeprom.h"
#include "hw/hw02-lcd-staff.h"

/* FreeRTOS Includes */
#include <FreeRTOS.h>
#include <event_groups.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>
#include <math.h>

/* Task Includes */
#include "tasks/task_joystick.h"
#include "tasks/task_state_machine.h"
#include "tasks/task_lightSensor.h"
#include "tasks/task_imu.h"
#include "tasks/task_button1.h"
#include "tasks/task_uartTX.h"
#include "tasks/task_uartRX.h"
#include "tasks/task_highScore.h"

#include "project-lcd.h"
#include "hw/project.h"

/* This macro is used to determine if we are building an executable for example code or ICE code
 * 
 * To choose ICE code, use the following line
 * #define ICE
 * 
 * To choose EXAMPLE code, use the following line
 * #undef ICE
*/
#undef ICE 
#undef HW01
#undef HW02

/* This macro identifies which ice/example file is being compiled into an executable*/
#define FILE_ID 18

#if ! defined(ICE) 
#define EXAMPLE 
#else
#undef EXAMPLE 
#endif

extern char NAME[];
extern char ICE_DESCRIPTION[];
#endif
