/**
 * @file project.h
 * @author Tianqi Shen (tshen46@wisc.edu) & Bowen Quan (bquan2@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-09-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __PROJECT_H__
#define __PROJECT_H__

#include "main.h"

#define SCREEN_X            320
#define SCREEN_Y            240
#define SHIP_MIN_X          (shipTopWidthPixels/2)
#define SHIP_MAX_X          319 - (shipTopWidthPixels/2)
#define SHIP_MIN_Y          100 + (shipTopHeightPixels/2)
#define SHIP_MAX_Y          (219 - (shipTopHeightPixels)/2 - shipBottomHeightPixels)

#define SEA_MIN_Y           100
#define BLACK_BAR_MIN_Y     220 // 240 - 220 = 20 pixels of height


#define LCD_MIN_X (shipTopWidthPixels/2)
#define LCD_MAX_X (319 - (shipTopWidthPixels/2))
#define LCD_MIN_Y (shipTopHeightPixels/2)
#define LCD_MAX_Y (239 - (shipTopHeightPixels)/2 - shipBottomHeightPixels)

/**
 * @brief 
 * Initializes the PSoC6 Peripherals used for HW02 
 */
void project_peripheral_init(void);

/**
 * @brief 
 * Implements the main application for HW02 
 */
void project_main_app(void);

#endif