/* this file is used to consolidate methods to draw various images for the project
 */
#ifndef __PROJECT_LCD_H__
#define __PROJECT_LCD_H__

#include "main.h"

// draws the ship, with the top part's center at input x and y
void draw_ship(int x, int y, bool i_am_p1);

// draws P1's ship for the P1 select screen
void draw_select_screen_ship();

// draws the sky, sea, and bottom black bar
void draw_background();

// draws the sky only
void draw_sky();

// draws the sea only
void draw_sea();

// draws a black_bar 20 pixels tall at the bottom of the screen.
void draw_bottom_black_bar();

// draws a text that says "ANGLE:" and a degree symbol
void draw_angle_text();

// draws the 2 digit angle number
void draw_angle_number(uint8_t angle);

// draws a text that says "STRENGTH:"
void draw_strength_text();

// draws the strength color
void draw_strength_color(uint16_t color);

// draws victory splash screen
void draw_you_win();

// draw loss splash screen
void draw_you_lose();

// draw a text that says HIGH to indicate high score
void draw_highScore_text();

void draw_high_score_number(uint8_t high_score);
#endif