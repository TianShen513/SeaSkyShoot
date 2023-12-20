/**
 * @file hw01_draw.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-09-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "hw01_draw.h"


void hw01_draw_bell(void)
{
    lcd_draw_image(
        COORD_BELL_X,
        COORD_BELL_Y,
        bellWidthPixels,
        bellHeightPixels,
        bellBitmaps,
        LCD_COLOR_BLUE2,
        LCD_COLOR_BLACK);
}

void hw01_erase_bell(void)
{
    lcd_draw_image(
        COORD_BELL_X,
        COORD_BELL_Y,
        bellWidthPixels,
        bellHeightPixels,
        bellBitmaps,
        LCD_COLOR_BLACK,
        LCD_COLOR_BLACK);
}

void hw01_display_time(uint8_t minutes, uint8_t seconds, uint16_t color)
{
    uint8_t minutes_msb = minutes / 10;
    uint8_t minutes_lsb = minutes % 10;
    uint8_t seconds_msb = seconds / 10;
    uint8_t seconds_lsb = seconds % 10;

    uint8_t *minutes_msb_bitmap = (uint8_t *)&brussels_48ptBitmaps[brussels_48ptOffsets[minutes_msb]];
    uint8_t *minutes_lsb_bitmap = (uint8_t *)&brussels_48ptBitmaps[brussels_48ptOffsets[minutes_lsb]];
    uint8_t *colon_bitmap = (uint8_t *)&brussels_48ptBitmaps[brussels_48ptOffsets[10]];
    uint8_t *seconds_msb_bitmap = (uint8_t *)&brussels_48ptBitmaps[brussels_48ptOffsets[seconds_msb]];
    uint8_t *seconds_lsb_bitmap = (uint8_t *)&brussels_48ptBitmaps[brussels_48ptOffsets[seconds_lsb]];

    /* minutes MSBit*/
    lcd_draw_image(
        COORD_TIME_X_MIN_MSB,
        COORD_TIME_Y,
        FONT_TIME_WIDTH,
        FONT_TIME_HEIGHT,
        minutes_msb_bitmap,
        color,
        LCD_COLOR_BLACK);

    /* minutes LSBit*/
    lcd_draw_image(
        COORD_TIME_X_MIN_LSB,
        COORD_TIME_Y,
        FONT_TIME_WIDTH,
        FONT_TIME_HEIGHT,
        minutes_lsb_bitmap,
        color,
        LCD_COLOR_BLACK);

    /* Colon */
    lcd_draw_image(
        COORD_TIME_X_COLON,
        COORD_TIME_Y,
        FONT_TIME_WIDTH,
        FONT_TIME_HEIGHT,
        colon_bitmap,
        color,
        LCD_COLOR_BLACK);

    /* Min MSBit*/
    lcd_draw_image(
        COORD_TIME_X_SEC_MSB,
        COORD_TIME_Y,
        FONT_TIME_WIDTH,
        FONT_TIME_HEIGHT,
        seconds_msb_bitmap,
        color,
        LCD_COLOR_BLACK);

    /* Min LSBit*/
    lcd_draw_image(
        COORD_TIME_X_SEC_LSB,
        COORD_TIME_Y,
        FONT_TIME_WIDTH,
        FONT_TIME_HEIGHT,
        seconds_lsb_bitmap,
        color,
        LCD_COLOR_BLACK);
}

void hw01_display_alarm(uint8_t minutes, uint8_t seconds, uint16_t color)
{
    uint8_t minutes_msb = minutes / 10;
    uint8_t minutes_lsb = minutes % 10;
    uint8_t seconds_msb = seconds / 10;
    uint8_t seconds_lsb = seconds % 10;

    uint8_t *minutes_msb_bitmap = (uint8_t *)&brussels_24ptBitmaps[brussels_24ptOffsets[minutes_msb]];
    uint8_t *minutes_lsb_bitmap = (uint8_t *)&brussels_24ptBitmaps[brussels_24ptOffsets[minutes_lsb]];
    uint8_t *colon_bitmap = (uint8_t *)&brussels_24ptBitmaps[brussels_24ptOffsets[10]];
    uint8_t *seconds_msb_bitmap = (uint8_t *)&brussels_24ptBitmaps[brussels_24ptOffsets[seconds_msb]];
    uint8_t *seconds_lsb_bitmap = (uint8_t *)&brussels_24ptBitmaps[brussels_24ptOffsets[seconds_lsb]];

    /* minutes MSBit*/
    lcd_draw_image(
        COORD_ALARM_X_MIN_MSB,
        COORD_ALARM_Y,
        FONT_ALARM_WIDTH,
        FONT_ALARM_HEIGHT,
        minutes_msb_bitmap,
        color,
        LCD_COLOR_BLACK);

    /* minutes LSBit*/
    lcd_draw_image(
        COORD_ALARM_X_MIN_LSB,
        COORD_ALARM_Y,
        FONT_ALARM_WIDTH,
        FONT_ALARM_HEIGHT,
        minutes_lsb_bitmap,
        color,
        LCD_COLOR_BLACK);

    /* Colon */
    lcd_draw_image(
        COORD_ALARM_X_COLON,
        COORD_ALARM_Y,
        FONT_ALARM_WIDTH,
        FONT_ALARM_HEIGHT,
        colon_bitmap,
        color,
        LCD_COLOR_BLACK);

    /* Min MSBit*/
    lcd_draw_image(
        COORD_ALARM_X_SEC_MSB,
        COORD_ALARM_Y,
        FONT_ALARM_WIDTH,
        FONT_ALARM_HEIGHT,
        seconds_msb_bitmap,
        color,
        LCD_COLOR_BLACK);

    /* Min LSBit*/
    lcd_draw_image(
        COORD_ALARM_X_SEC_LSB,
        COORD_ALARM_Y,
        FONT_ALARM_WIDTH,
        FONT_ALARM_HEIGHT,
        seconds_lsb_bitmap,
        color,
        LCD_COLOR_BLACK);
}