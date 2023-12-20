/**
 * @file hw01_draw.h
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-09-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __HW01_DRAW_H__
#define __HW01_DRAW_H__

#include "../drivers/io-lcd.h"
#include "hw01_font.h"
#include "hw01_images.h"

#define COORD_TIME_Y 120
#define COORD_TIME_X_MIN_MSB    80 
#define COORD_TIME_X_MIN_LSB    120 
#define COORD_TIME_X_COLON      160 
#define COORD_TIME_X_SEC_MSB    200 
#define COORD_TIME_X_SEC_LSB    240 

#define COORD_ALARM_Y            50 
#define COORD_ALARM_X_MIN_MSB    200 
#define COORD_ALARM_X_MIN_LSB    220 
#define COORD_ALARM_X_COLON      240 
#define COORD_ALARM_X_SEC_MSB    260 
#define COORD_ALARM_X_SEC_LSB    280 

#define COORD_BELL_X            75 
#define COORD_BELL_Y            50 

#define HW01_LCD_ALARM_COLOR        LCD_COLOR_BLUE2
#define HW01_LCD_TIME_COLOR         LCD_COLOR_GREEN
#define HW01_LCD_ALARM_ACTIVE_COLOR  LCD_COLOR_RED

/**
 * @brief 
 * Draws the alarm bell 
 */
void hw01_draw_bell(void);

/**
 * @brief 
 * Erases the alarm bell 
 */
void hw01_erase_bell(void);

/**
 * @brief 
 * Displayes the current time 
 * @param minutes
 * Decimal number representing the minutes
 * @param seconds 
 * Decimal number representing the seconds
 * @param color
 * Foreground color of the time
 */
void hw01_display_time(uint8_t minutes, uint8_t seconds, uint16_t color);

/**
 * @brief 
 * Displayes the current alarm
 * @param minutes
 * Decimal number representing the minutes
 * @param seconds 
 * Decimal number representing the seconds
 * @param color
 * Foreground color of the time
 */

void hw01_display_alarm(uint8_t minutes, uint8_t seconds, uint16_t color);

#endif