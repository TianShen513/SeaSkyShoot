/**
 * @file hw02.h
 * @author Tianqi Shen (tshen46@wisc.edu) & Bowen Quan (bquan2@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-09-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __HW02_H__
#define __HW02_H__

#include "drivers/joystick.h"

#define SCREEN_X            320
#define SCREEN_Y            240

#define SCREEN_CENTER_COL    ((SCREEN_X/2)-1)
#define SCREEN_CENTER_ROW    ((SCREEN_Y/2)-1)

#define LINE_WIDTH          4
#define LINE_LENGTH         120 

#define SQUARE_SIZE         32

#define PADDING             2

#define LEFT_COL                      (SCREEN_CENTER_COL - SQUARE_SIZE - (2*PADDING) - LINE_WIDTH)
#define CENTER_COL                    (SCREEN_CENTER_COL)
#define RIGHT_COL                     (SCREEN_CENTER_COL + SQUARE_SIZE + (2*PADDING) + LINE_WIDTH)

#define UPPER_ROW                     (SCREEN_CENTER_ROW - SQUARE_SIZE - (2*PADDING) - LINE_WIDTH + 20 )
#define CENTER_ROW                    (SCREEN_CENTER_ROW + 20 )
#define LOWER_ROW                     (SCREEN_CENTER_ROW + SQUARE_SIZE + (2*PADDING) + LINE_WIDTH + 20 )

#define UPPER_HORIZONTAL_LINE_Y     (SCREEN_CENTER_ROW - (SQUARE_SIZE/2) - PADDING - LINE_WIDTH/2 + 20)
#define LOWER_HORIZONTAL_LINE_Y     (SCREEN_CENTER_ROW + (SQUARE_SIZE/2) + PADDING + LINE_WIDTH/2 + 20)

#define LEFT_VERTICAL_LINE_X      (SCREEN_CENTER_COL - (SQUARE_SIZE/2) - PADDING - LINE_WIDTH/2)
#define RIGHT_VERTICAL_LINE_X     (SCREEN_CENTER_COL + (SQUARE_SIZE/2) + PADDING + LINE_WIDTH/2)

#define LEFT_HORIZONTAL_LINE_X      (SCREEN_CENTER_COL - (SQUARE_SIZE/2) - PADDING - LINE_WIDTH/2)
#define RIGHT_HORIZONTAL_LINE_X     (SCREEN_CENTER_COL + (SQUARE_SIZE/2) + PADDING + LINE_WIDTH/2)

#define FG_COLOR_X                LCD_COLOR_YELLOW
#define BG_COLOR_X                LCD_COLOR_BLACK
#define FG_COLOR_O                LCD_COLOR_CYAN
#define BG_COLOR_O                LCD_COLOR_BLACK

#define FG_COLOR_CLAIMED          LCD_COLOR_BLACK
#define BG_COLOR_CLAIMED          LCD_COLOR_RED

#define FG_COLOR_UNCLAIMED        LCD_COLOR_BLACK
#define BG_COLOR_UNCLAIMED        LCD_COLOR_GREEN

#define MESSAGE_PLAYER1_SELECTION 0x5A
#define MESSAGE_ACK               0xF0
#define MESSAGE_NACK              0xE0
#define MESSAGE_X                 0x58
#define MESSAGE_O                 0x4F
#define MESSAGE_RESTART           0x30

typedef enum
{
    EVENT_NONE,
    EVENT_100MS,
    EVENT_SW1,
    EVENT_SW2
} events_t;


extern char HW02_DESCRIPTION[];
extern volatile events_t HW02_ALERT_100MS;
extern volatile events_t HW02_ALERT_SW1;
extern volatile events_t HW02_ALERT_SW2;

/* Timer object used */
extern cyhal_timer_t hw02_timer_obj;
extern cyhal_timer_cfg_t hw02_timer_cfg;


/**
 * @brief 
 * Initializes the PSoC6 Peripherals used for HW02 
 */
void hw02_peripheral_init(void);

/**
 * @brief 
 * Implements the main application for HW02 
 */
void hw02_main_app(void);

#endif