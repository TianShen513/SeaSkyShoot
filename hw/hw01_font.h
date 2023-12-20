/**
 * @file hw01_font.h
 * @author  Joe Krachey jkrachey@wisc.edu
 * @brief 
 * @version 0.1
 * @date 2023-09-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __HW01_FONT_H__
#define __HW01_FONT_H__

#include <stdint.h>

#define FONT_TIME_WIDTH     37 
#define FONT_TIME_HEIGHT    47 

#define FONT_ALARM_WIDTH     18 
#define FONT_ALARM_HEIGHT    23 


extern const uint8_t brussels_48ptBitmaps[];
extern const uint16_t brussels_48ptOffsets[];
extern const uint8_t brussels_24ptBitmaps[];
extern const uint16_t brussels_24ptOffsets[];
#endif