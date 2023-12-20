/**
 * @file hw01.h
 * @author Tianqi Shen (tshen46@wisc.edu) & Bowen Quan (bquan2@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-09-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __HW01_H__
#define __HW01_H__

#include "hw01_draw.h"
#include "hw01_timer.h"

/**
 * @brief 
 * Initializes the PSoC6 Peripherals used for HW01 
 */
void hw01_peripheral_init(void);

/**
 * @brief 
 * Implements the main application for HW01 
 */
void hw01_main_app(void);


#endif