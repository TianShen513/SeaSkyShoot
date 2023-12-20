/**
 * @file hw02-lcd-staff.h
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-10-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __HW02_LCD_STAFF_H__
#define __HW02_LCD_STAFF_H__

#include "drivers/io-lcd.h"
#include "hw02.h"
#include "hw02-images.h"

void tic_tac_toe_example_board(void);
void lcd_X_wins(void);
void lcd_O_wins(void);
void lcd_tie(void);
void lcd_select_player1(void);
void lcd_wait_for_other_player(void);
void lcd_clear_other_player(void);
void tic_tac_toe_example_board(void);

void lcd_draw_rectangle_centered(
  uint16_t x,
  uint16_t y,
  uint16_t width_pixels,
  uint16_t height_pixels,
  uint16_t fColor
);


#endif
