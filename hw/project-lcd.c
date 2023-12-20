#include "project-lcd.h"

void draw_ship(int x, int y, bool i_am_p1)
{
    if (i_am_p1) {
        lcd_draw_image(x, y, shipTopWidthPixels, shipTopHeightPixels, shipTopBitmaps, LCD_COLOR_RED, LCD_COLOR_BLUE);
        lcd_draw_image(x, y + shipTopHeightPixels, shipBottomWidthPixels, shipBottomHeightPixels, shipBottomBitmaps, LCD_COLOR_BROWN, LCD_COLOR_BLUE);
    } else {
        lcd_draw_image(x, y, p2shipTopWidthPixels, p2shipTopHeightPixels, p2shipTopBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLUE);
        lcd_draw_image(x, y + p2shipTopHeightPixels, p2shipBottomWidthPixels, p2shipBottomHeightPixels, p2shipBottomBitmaps, LCD_COLOR_BROWN, LCD_COLOR_BLUE);
    }
}

void draw_select_screen_ship() {
    lcd_draw_image(SCREEN_X/2, 180, shipTopWidthPixels, shipTopHeightPixels, shipTopBitmaps, LCD_COLOR_RED, LCD_COLOR_BLACK);
    lcd_draw_image(SCREEN_X/2, 180 + shipTopHeightPixels, shipBottomWidthPixels, shipBottomHeightPixels, shipBottomBitmaps, LCD_COLOR_BROWN, LCD_COLOR_BLACK);

}
void draw_background()
{
    lcd_clear_screen(LCD_COLOR_CYAN);                                                 // draw the sky
    lcd_draw_rectangle(0, SCREEN_X, SEA_MIN_Y, SCREEN_Y - SEA_MIN_Y, LCD_COLOR_BLUE); // draw the sea
    draw_bottom_black_bar();
}

void draw_sky() {
    lcd_draw_rectangle(0, SCREEN_X, 0, SEA_MIN_Y, LCD_COLOR_CYAN);
}

void draw_sea() {
    lcd_draw_rectangle(0, SCREEN_X, SEA_MIN_Y, BLACK_BAR_MIN_Y - SEA_MIN_Y, LCD_COLOR_BLUE);
}

void draw_bottom_black_bar()
{
    lcd_draw_rectangle(0, SCREEN_X, BLACK_BAR_MIN_Y, SCREEN_Y - BLACK_BAR_MIN_Y, LCD_COLOR_BLACK);
    // draw a divider between angle and strength
    lcd_draw_rectangle(87, 3, BLACK_BAR_MIN_Y, SCREEN_Y - BLACK_BAR_MIN_Y, LCD_COLOR_GRAY);
}

void draw_angle_text()
{
    lcd_draw_image(angleTextWidthPixels / 2, 230, angleTextWidthPixels, angleTextHeightPixels, angleTextBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
    // draw a colon
    lcd_draw_rectangle(angleTextWidthPixels, 3, 227, 3, LCD_COLOR_WHITE);
    lcd_draw_rectangle(angleTextWidthPixels, 3, 232, 3, LCD_COLOR_WHITE);
    // draw a degree sign
    lcd_draw_rectangle(angleTextWidthPixels + 37, 6, 222, 6, LCD_COLOR_WHITE);
    lcd_draw_rectangle(angleTextWidthPixels + 39, 2, 224, 2, LCD_COLOR_BLACK);
}

void draw_angle_number(uint8_t angle)
{
    if (angle <= 90 && angle >= 0)
    {
        uint8_t tens = angle / 10;
        uint8_t ones = angle % 10;

        uint16_t y = 230;
        uint16_t tens_x = angleTextWidthPixels + 13;      // +13 to make room for colon
        uint16_t ones_x = angleTextWidthPixels + 13 + 15; // +15 to make room for tens digit

        switch (tens)
        {
        case 0:
            lcd_draw_image(tens_x, y, zeroWidthPixels, zeroHeightPixels, zeroBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 1:
            lcd_draw_image(tens_x, y, oneWidthPixels, oneHeightPixels, oneBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 2:
            lcd_draw_image(tens_x, y, twoWidthPixels, twoHeightPixels, twoBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 3:
            lcd_draw_image(tens_x, y, threeWidthPixels, threeHeightPixels, threeBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 4:
            lcd_draw_image(tens_x, y, fourWidthPixels, fourHeightPixels, fourBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 5:
            lcd_draw_image(tens_x, y, fiveWidthPixels, fiveHeightPixels, fiveBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 6:
            lcd_draw_image(tens_x, y, sixWidthPixels, sixHeightPixels, sixBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 7:
            lcd_draw_image(tens_x, y, sevenWidthPixels, sevenHeightPixels, sevenBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 8:
            lcd_draw_image(tens_x, y, eightWidthPixels, eightHeightPixels, eightBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 9:
            lcd_draw_image(tens_x, y, nineWidthPixels, nineHeightPixels, nineBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        default:
            break;
        }

        switch (ones)
        {
        case 0:
            lcd_draw_image(ones_x, y, zeroWidthPixels, zeroHeightPixels, zeroBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 1:
            lcd_draw_image(ones_x, y, oneWidthPixels, oneHeightPixels, oneBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 2:
            lcd_draw_image(ones_x, y, twoWidthPixels, twoHeightPixels, twoBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 3:
            lcd_draw_image(ones_x, y, threeWidthPixels, threeHeightPixels, threeBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 4:
            lcd_draw_image(ones_x, y, fourWidthPixels, fourHeightPixels, fourBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 5:
            lcd_draw_image(ones_x, y, fiveWidthPixels, fiveHeightPixels, fiveBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 6:
            lcd_draw_image(ones_x, y, sixWidthPixels, sixHeightPixels, sixBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 7:
            lcd_draw_image(ones_x, y, sevenWidthPixels, sevenHeightPixels, sevenBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 8:
            lcd_draw_image(ones_x, y, eightWidthPixels, eightHeightPixels, eightBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 9:
            lcd_draw_image(ones_x, y, nineWidthPixels, nineHeightPixels, nineBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        default:
            break;
        }
    }
}

void draw_strength_text()
{
    // draw text
    lcd_draw_image(95 + strengthTextWidthPixels / 2, 230, strengthTextWidthPixels, strengthTextHeightPixels, strengthTextBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
    // draw a colon
    lcd_draw_rectangle(95 + strengthTextWidthPixels, 3, 227, 3, LCD_COLOR_WHITE);
    lcd_draw_rectangle(95 + strengthTextWidthPixels, 3, 232, 3, LCD_COLOR_WHITE);
}

void draw_strength_color(uint16_t color)
{
    lcd_draw_rectangle(95 + strengthTextWidthPixels + 5, 16, 222, 16, LCD_COLOR_WHITE);
    lcd_draw_rectangle(95 + strengthTextWidthPixels + 7, 12, 224, 12, color);
}

void draw_you_win() {
    lcd_draw_image(SCREEN_X/2, 50, winWidthPixels, winHeightPixels, winBitmaps, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
}

void draw_you_lose() {
    lcd_draw_image(SCREEN_X/2, 50, loseWidthPixels, loseHeightPixels, loseBitmaps, LCD_COLOR_MAGENTA, LCD_COLOR_BLACK);
}

void draw_highScore_text(){
    lcd_draw_image(265, 230, highScoreWidthPixels, highScoreHeightPixels, highScoreBitmaps ,LCD_COLOR_RED, LCD_COLOR_BLACK);
}

void draw_high_score_number(uint8_t high_score)
{
    uint8_t tens = high_score / 10;
    uint8_t ones = high_score % 10;
    uint16_t tens_x = 297;
    uint16_t ones_x = 297 + 15;
    uint16_t y = 230;

      switch (tens)
        {
        case 0:
            lcd_draw_image(tens_x, y, zeroWidthPixels, zeroHeightPixels, zeroBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 1:
            lcd_draw_image(tens_x, y, oneWidthPixels, oneHeightPixels, oneBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 2:
            lcd_draw_image(tens_x, y, twoWidthPixels, twoHeightPixels, twoBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 3:
            lcd_draw_image(tens_x, y, threeWidthPixels, threeHeightPixels, threeBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 4:
            lcd_draw_image(tens_x, y, fourWidthPixels, fourHeightPixels, fourBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 5:
            lcd_draw_image(tens_x, y, fiveWidthPixels, fiveHeightPixels, fiveBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 6:
            lcd_draw_image(tens_x, y, sixWidthPixels, sixHeightPixels, sixBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 7:
            lcd_draw_image(tens_x, y, sevenWidthPixels, sevenHeightPixels, sevenBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 8:
            lcd_draw_image(tens_x, y, eightWidthPixels, eightHeightPixels, eightBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 9:
            lcd_draw_image(tens_x, y, nineWidthPixels, nineHeightPixels, nineBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        default:
            break;
        }

        switch (ones)
        {
        case 0:
            lcd_draw_image(ones_x, y, zeroWidthPixels, zeroHeightPixels, zeroBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 1:
            lcd_draw_image(ones_x, y, oneWidthPixels, oneHeightPixels, oneBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 2:
            lcd_draw_image(ones_x, y, twoWidthPixels, twoHeightPixels, twoBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 3:
            lcd_draw_image(ones_x, y, threeWidthPixels, threeHeightPixels, threeBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 4:
            lcd_draw_image(ones_x, y, fourWidthPixels, fourHeightPixels, fourBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 5:
            lcd_draw_image(ones_x, y, fiveWidthPixels, fiveHeightPixels, fiveBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 6:
            lcd_draw_image(ones_x, y, sixWidthPixels, sixHeightPixels, sixBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 7:
            lcd_draw_image(ones_x, y, sevenWidthPixels, sevenHeightPixels, sevenBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 8:
            lcd_draw_image(ones_x, y, eightWidthPixels, eightHeightPixels, eightBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        case 9:
            lcd_draw_image(ones_x, y, nineWidthPixels, nineHeightPixels, nineBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
            break;
        default:
            break;
        }
}
