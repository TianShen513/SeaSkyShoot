/**
 * @file io-lcd.h
 * @author Bowen Quan (bquan2@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-09-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __ECE353_GPIO_LCD_H__
#define __ECE353_GPIO_LCD_H__

#include <stdint.h>
#include <stdbool.h>
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include <stdio.h>

#include "lcd_images.h"

/* IO Pins LCD*/
#define PIN_IO_LCD_INT      P11_1
#define PIN_IO_LCD_CSX      P1_0
#define PIN_IO_LCD_DCX      P1_1
#define PIN_IO_LCD_WRX      P1_2
#define PIN_IO_LCD_D0       P2_0
#define PIN_IO_LCD_D1       P2_1
#define PIN_IO_LCD_D2       P2_2
#define PIN_IO_LCD_D3       P2_3
#define PIN_IO_LCD_D4       P2_4
#define PIN_IO_LCD_D5       P2_5
#define PIN_IO_LCD_D6       P2_6
#define PIN_IO_LCD_D7       P2_7

#define PORT_IO_LCD_DATA				  GPIO_PRT2
#define PORT_IO_LCD_CMD			      GPIO_PRT1

#define MASK_IO_LCD_CSX			(1<<0)
#define MASK_IO_LCD_DCX			(1<<1)
#define MASK_IO_LCD_WRX			(1<<2)


#define LCD_ROWS  320
#define LCD_COLS  240

#define LCD_COLOR_WHITE			0xFFFF
#define LCD_COLOR_BLACK			0x0000
#define LCD_COLOR_RED			  0xF800
#define LCD_COLOR_GREEN			0x07E0
#define LCD_COLOR_GREEN2		0xB723
#define LCD_COLOR_BLUE			0x001F
#define LCD_COLOR_BLUE2			0x051D
#define LCD_COLOR_YELLOW		0xFFE0
#define LCD_COLOR_ORANGE		0xFBE4
#define LCD_COLOR_CYAN			0x07FF
#define LCD_COLOR_MAGENTA		0xA254
#define LCD_COLOR_GRAY			0x7BEF
#define LCD_COLOR_BROWN			0xBBCA


#define LCD_CMD_NOOP                            0x00
#define LCD_CMD_SOFTWARE_RESET                  0x01
#define LCD_CMD_READ_DISPLAY_INFO               0x04
#define LCD_CMD_READ_DISPLAY_STATUS             0x09
#define LCD_CMD_READ_DISPLAY_POWER_MODE         0x0A
#define LCD_CMD_READ_MADCTL                     0x0B
#define LCD_CMD_READ_DISPLAY_PIXEL_FORMAT       0x0C
#define LCD_CMD_READ_DISPLAY_IMAGE_FORMAT       0x0D
#define LCD_CMD_READ_DISPLAY_SIGNAL_FORMAT      0x0E
#define LCD_CMD_READ_DISPLAY_DIAGNOSTICS        0x0F
#define LCD_CMD_ENTER_SLEEP_MODE                0x10
#define LCD_CMD_SLEEP_OUT                       0x11
#define LCD_CMD_PARTIAL_MODE_ON                 0x12
#define LCD_CMD_NORMAL_DISPLAY_MODE_ON          0x13
#define LCD_CMD_DISPLAY_INVERSION_OFF           0x20
#define LCD_CMD_DISPLAY_INVERSION_ON            0x21
#define LCD_CMD_GAMMA_SET                       0x26
#define LCD_CMD_DISPLAY_OFF                     0x28
#define LCD_CMD_DISPLAY_ON                      0x29
#define LCD_CMD_SET_COLUMN_ADDR                 0x2A
#define LCD_CMD_SET_PAGE_ADDR                   0x2B
#define LCD_CMD_MEMORY_WRITE                    0x2C
#define LCD_CMD_COLOR_SET                       0x2D
#define LCD_CMD_MEMORY_READ                     0x2E
#define LCD_CMD_PARTIAL_AREA                    0x30
#define LCD_CMD_VERTICAL_SCROLL_DEFINITION      0x33
#define LCD_CMD_TEARING_EFFECT_LINE_OFF         0x34
#define LCD_CMD_TEARING_EFFECT_LINE_ON          0x35
#define LCD_CMD_MEMORY_ACCESS_CONTROL           0x36
#define LCD_CMD_VERTICAL_SCROLLING_START        0x37
#define LCD_CMD_IDLE_MODE_OFF                   0x38
#define LCD_CMD_IDLE_MODE_ON                    0x39
#define LCD_CMD_PIXEL_FORMAT_SET                0x3A
#define LCD_CMD_WRITE_MEMORY_CONTINUE           0x3C
#define LCD_CMD_READ_MEMORY_CONTINUE        		0x3E
#define LCD_CMD_SET_TEAR_SCANLINE            		0x44
#define LCD_CMD_GET_SCANLINE                		0x45
#define LCD_CMD_WRITE_DISPLAY_BRIGHTNESS        0x51
#define LCD_CMD_READ_DISPLAY_BRIGHTNESS         0x52
#define LCD_CMD_WRITE_CTRL_DISPLAY              0x53
#define LCD_CMD_READ_CTRL_DISPLAY               0x54
#define LCD_CMD_WRITE_CONTENT_ADAPTIVE_STUFF    0x55
#define LCD_CMD_READ_CONTENT_ADAPTIVE_STUFF     0x56
#define LCD_CMD_WRITE_CABC_MINIMUM_BRIGHTNESS   0x5E
#define LCD_CMD_READ_CABC_MINIMUM_BRIGHTNESS    0x5F
#define LCD_CMD_READ_ID1                				0xDA
#define LCD_CMD_READ_ID2                				0xDB
#define LCD_CMD_READ_ID3                				0xDC
#define LCD_CMD_RGB_INTERFACE_CONTROL_SIGNAL    0xB0
#define LCD_CMD_FRAME_CONTROL_NORMAL_MODE       0xB1
#define LCD_CMD_FRAME_CONTROL_IDLE_MODE         0xB2
#define LCD_CMD_FRAME_CONTROL_PARTIAL_MODE      0xB3
#define LCD_CMD_DISPLAY_INVERSION_MODE          0xB4
#define LCD_CMD_BLANKING_PORCH_CONTROL          0xB5
#define LCD_CMD_DISPLAY_FUNCTION_CONTROL        0xB6
#define LCD_CMD_ENTRY_MODE_SET                  0xB7
#define LCD_CMD_BACKLIGHT_CONTROL_1             0xB8
#define LCD_CMD_BACKLIGHT_CONTROL_2             0xB9
#define LCD_CMD_BACKLIGHT_CONTROL_3             0xBA
#define LCD_CMD_BACKLIGHT_CONTROL_4             0xBB
#define LCD_CMD_BACKLIGHT_CONTROL_5             0xBC
#define LCD_CMD_BACKLIGHT_CONTROL_6             0xBD
#define LCD_CMD_BACKLIGHT_CONTROL_7             0xBE
#define LCD_CMD_BACKLIGHT_CONTROL_8             0xBF
#define LCD_CMD_POWER_CONTROL_1                 0xC0
#define LCD_CMD_POWER_CONTROL_2                 0xC1
#define LCD_CMD_VCOMM_CONTROL_1                 0xC5
#define LCD_CMD_VCOMM_CONTROL_2                 0xC7
#define LCD_CMD_NV_MEMORY_WRITE                 0xD0
#define LCD_CMD_NV_MEMORY_PROTECTION_KEY        0xD1
#define LCD_CMD_NV_MEMORY_STATUS_READ           0xD2
#define LCD_CMD_READ_ID4                        0xD3
#define LCD_CMD_POSITIVE_GAMMA_CORRECTION       0xE0
#define LCD_CMD_NEGATIVE_GAMMA_CORRECTION       0xE1
#define LCD_CMD_DIGITAL_GAMMA_CONTROL_1         0xE2
#define LCD_CMD_DIGITAL_GAMMA_CONTROL_2         0xE3
#define LCD_CMD_INTERFACE_CONTROL               0xF6


/*******************************************************************************
* Function Name: lcd_set_pos
********************************************************************************
* Summary: Sets the boundries of the active portion of the screen.  When data
*          is written to the LCD, it will be written at addr of x0,y0.
*
* Return:
*  Nothing
*******************************************************************************/
void lcd_set_pos(
  uint16_t x0,    // X coordinate for the start of the box
  uint16_t x1,    // X coordinate for the end of the box
  uint16_t y0,    // Y coordinate for the start of the box
  uint16_t y1     // Y coordinate for the end of the box
);

/*******************************************************************************
* Function Name: lcd_clear_screen
********************************************************************************
* Summary: clears the entire screen with the provided color.
*
* Return:
*  Nothing
*******************************************************************************/
void lcd_clear_screen(
  uint16_t bColor   //  Color to paint the entire screen with
);



/*******************************************************************************
* Function Name: lcd_draw_image
********************************************************************************
* Summary: Prints an image centered at the coordinates set by x_start, y_start
* Returns:
*  Nothing
*******************************************************************************/
void lcd_draw_image(
  uint16_t x_start,                 // X coordinate starting address
  uint16_t y_start,                 // Y coordinate starting address
  uint16_t image_width_bits,        // image width
  uint16_t image_height_pixels,     // image height
  const uint8_t *image,             // bitmap of the image
  uint16_t fColor,                  // foreground color
  uint16_t bColor                   // background color
);


/*******************************************************************************
* Function Name: lcd_draw_rectangle
********************************************************************************
* Summary: Draws a rectangle starting at x_start,y_start.
* Returns:
*  Nothing
*******************************************************************************/
void lcd_draw_rectangle
  (
  uint16_t x_start,
  uint16_t x_len,
  uint16_t y_start,
  uint16_t y_len,
  uint16_t fg_color
);

  /*******************************************************************************
* Function Name: lcd_config_gpio
********************************************************************************
* Summary: Configures the GPIO pins used to implement the 8080 interface on the
*          LCD display
*
* Parameters
*
* Return:
*  Nothing
*******************************************************************************/
cy_rslt_t lcd_config_gpio(void);

/*******************************************************************************
* Function Name: lcd_config_screen
********************************************************************************
* Summary: After the configuration of the gpio pins, this function configures
*          the internal LCD controller chip.
* Returns:
*  Nothing
*******************************************************************************/
void lcd_config_screen(void);

/*******************************************************************************
* Function Name:ece353_enable_lcd
********************************************************************************
* Summary: Configures the LCD screen and displays a default image              *
* Parameters
* Return:
*  Nothing
*******************************************************************************/
void ece353_enable_lcd(void);

#endif /* ECE353__GPIO_LCD_H_ */
