/*
 * io-lcd.c
 *
 *  Created on: Mar 7, 2023
 *      Author: Joe Krachey
 */

#include "io-lcd.h"

/*******************************************************************************
* Function Name: lcd_write_cmd_u8
********************************************************************************
* Summary: Writes an 8-bit command used to identify which characteristic of the
*          LCD will be modified
* Return:
*  Nothing
*******************************************************************************/
__STATIC_INLINE void lcd_write_cmd_u8(uint8_t DL)
{
  // ADD CODE

  // Start a transaction to the LCD by setting LCD_CSX low
  // PORT_IO_LCD_CMD->OUT = PORT_IO_LCD_CMD->OUT & ~MASK_IO_LCD_CSX; // This is slow because read and write
  PORT_IO_LCD_CMD->OUT_CLR = MASK_IO_LCD_CSX; // sets CSX to 0 without needing a read

  // Indicate this is an instruction by setting the value on the LCD_DCX GPIO Pin
  PORT_IO_LCD_CMD->OUT_CLR = MASK_IO_LCD_DCX;

  // Send the 8 bits of the command
  PORT_IO_LCD_DATA->OUT = DL;

  // Set the write signal LCD_WRX low
  PORT_IO_LCD_CMD->OUT_CLR = MASK_IO_LCD_WRX;

  // Set the write signal LCD_WRX high
  PORT_IO_LCD_CMD->OUT_SET = MASK_IO_LCD_WRX;

  // Default back to sending a data packet by LCD_DCX GPIO Pin
  PORT_IO_LCD_CMD->OUT_SET = MASK_IO_LCD_DCX;

  // End the transaction to the LCD by setting LCD_CSX high
  PORT_IO_LCD_CMD->OUT_SET = MASK_IO_LCD_CSX;

}

/*******************************************************************************
* Function Name: lcd_write_data_u8
********************************************************************************
* Summary: Writes 8-bits of data to the specified LCD characteristic
* Return:
*  Nothing
*******************************************************************************/
__STATIC_INLINE void  lcd_write_data_u8 (uint8_t x)
{
  // ADD CODE

  // Start a transaction to the LCD by setting LCD_CSX low
  PORT_IO_LCD_CMD->OUT_CLR = MASK_IO_LCD_CSX; 

  // Send the 8 bits of data
  //LCD_DATA  = x;
  PORT_IO_LCD_DATA->OUT = x;

  // Set the write signal LCD_WRX low
  PORT_IO_LCD_CMD->OUT_CLR = MASK_IO_LCD_WRX;

  // Set the write signal LCD_WRX high
  PORT_IO_LCD_CMD->OUT_SET = MASK_IO_LCD_WRX;

  // End the transaction to the LCD by setting LCD_CSX high
  PORT_IO_LCD_CMD->OUT_SET = MASK_IO_LCD_CSX; 
}

/*******************************************************************************
* Function Name: lcd_write_data_u16
********************************************************************************
* Summary:
* Return:
*  Nothing
*******************************************************************************/
//write  data word
__STATIC_INLINE void  lcd_write_data_u16(uint16_t y)
{
  // ADD CODE

  uint8_t DH = y>>8;      // Bits 15-8 of the pixel color
  uint8_t DL = y;        // Bits 7-0 of the pixel color

  // Start a transaction to the LCD by setting LCD_CSX low
  PORT_IO_LCD_CMD->OUT_CLR = MASK_IO_LCD_CSX; 

  // Send the upper 8 bits of the current pixel's color
  PORT_IO_LCD_DATA->OUT = DH;

  // Set the write signal LCD_WRX low
  PORT_IO_LCD_CMD->OUT_CLR = MASK_IO_LCD_WRX;

  // Set the write signal LCD_WRX high
  PORT_IO_LCD_CMD->OUT_SET = MASK_IO_LCD_WRX;

  // Send the lower 8 bits of the current pixel's color
  PORT_IO_LCD_DATA->OUT = DL;

  // Set the write signal LCD_WRX low
  PORT_IO_LCD_CMD->OUT_CLR = MASK_IO_LCD_WRX;

  // Set the write signal LCD_WRX high
  PORT_IO_LCD_CMD->OUT_SET = MASK_IO_LCD_WRX;

  // End the transaction to the LCD by setting LCD_CSX high
  PORT_IO_LCD_CMD->OUT_SET = MASK_IO_LCD_CSX;

}

/*******************************************************************************
* Function Name: lcd_set_pos
********************************************************************************
* Summary: Sets the boundries of the active portion of the screen.  When data
*          is written to the LCD, it will be written at addr of x0,y0.
*
* Return:
*  Nothing
*******************************************************************************/
void lcd_set_pos(uint16_t x0,uint16_t x1,uint16_t y0,uint16_t y1)
{
   lcd_write_cmd_u8(LCD_CMD_SET_COLUMN_ADDR);
  lcd_write_data_u16(x0);
  lcd_write_data_u16(x1);
  lcd_write_cmd_u8(LCD_CMD_SET_PAGE_ADDR);
  lcd_write_data_u16(y0);
  lcd_write_data_u16(y1);
  lcd_write_cmd_u8(LCD_CMD_MEMORY_WRITE);//LCD_WriteCMD(GRAMWR);
}

/*******************************************************************************
* Function Name: lcd_clear_screen
********************************************************************************
* Summary: clears the entire screen with the provided color.
*
* Return:
*  Nothing
*******************************************************************************/
void lcd_clear_screen(uint16_t bColor)
{
  uint16_t i,j;
  lcd_set_pos(0,LCD_ROWS - 1, 0,LCD_COLS - 1);

  for (i=0;i< LCD_COLS ;i++)
  {
        for(j= 0; j < LCD_ROWS; j++)
        {
            lcd_write_data_u16(bColor);
        }
  }
}


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
)
{
  uint16_t i,j;
  lcd_set_pos(x_start,x_start + x_len - 1, y_start, y_start + y_len - 1);

  for (i=0;i< y_len ;i++)
  {
    for(j= 0; j < x_len; j++)
    {
      lcd_write_data_u16(fg_color);
    }
  }
}


/*******************************************************************************
* Function Name: lcd_draw_rectangle
********************************************************************************
* Summary: Draws a rectangle starting at x_start,y_start.
* Returns:
*  Nothing
*******************************************************************************/
void lcd_draw_pixel
(
  uint16_t x,
  uint16_t y,
  uint16_t fg_color
)
{
  lcd_set_pos(x,x, y, y);
  lcd_write_data_u16(fg_color);
}


/*******************************************************************************
* Function Name: lcd_draw_image
********************************************************************************
* Summary: Prints an image centered at the coordinates set by x_start, y_start
* Returns:
*  Nothing
*******************************************************************************/
void lcd_draw_image(
  uint16_t x_start,
  uint16_t y_start,
  uint16_t image_width_bits,
  uint16_t image_height_pixels,
  const uint8_t *image,
  uint16_t fColor,
  uint16_t bColor
)
{
  uint16_t i,j;
  uint8_t data;
  uint16_t byte_index;
  uint16_t bytes_per_row;
  uint16_t x0;
  uint16_t x1;
  uint16_t y0;
  uint16_t y1;

  x0 = x_start - (image_width_bits/2);
  x1 = x_start + (image_width_bits/2);
  if( (image_width_bits & 0x01) == 0x00)
  {
    x1--;
  }

  y0 = y_start  - (image_height_pixels/2);
  y1 = y_start  + (image_height_pixels/2) ;
  if( (image_height_pixels & 0x01) == 0x00)
  {
    y1--;
  }

  lcd_set_pos(x0, x1, y0, y1);

  bytes_per_row = image_width_bits / 8;
  if( (image_width_bits % 8) != 0)
  {
    bytes_per_row++;
  }

  for (i=0;i< image_height_pixels ;i++)
  {
        for(j= 0; j < image_width_bits; j++)
        {
            if( (j %8) == 0)
            {
              byte_index = (i*bytes_per_row) + j/8;
              data = image[byte_index];
            }
            if ( data & 0x80)
            {
                lcd_write_data_u16(fColor);
            }
            else
            {
                lcd_write_data_u16(bColor);
            }
            data  = data << 1;
        }
  }
}

/*******************************************************************************
* Function Name: lcd_config_screen
********************************************************************************
* Summary: After the configuration of the gpio pins, this function configures
*          the internal LCD controller chip.
* Returns:
*  Nothing
*******************************************************************************/
void lcd_config_screen(void)

{
  lcd_write_cmd_u8(LCD_CMD_SOFTWARE_RESET); //software reset
  Cy_SysLib_Delay(50);

  lcd_write_cmd_u8(LCD_CMD_SLEEP_OUT);
  Cy_SysLib_Delay(120);
   lcd_write_cmd_u8(0xCF);
  lcd_write_data_u8(0x00);
  lcd_write_data_u8(0x83);
  lcd_write_data_u8(0X30);

   lcd_write_cmd_u8(0xED);
  lcd_write_data_u8(0x64);
  lcd_write_data_u8(0x03);
  lcd_write_data_u8(0X12);
  lcd_write_data_u8(0X81);

   lcd_write_cmd_u8(0xE8);
  lcd_write_data_u8(0x85);
  lcd_write_data_u8(0x01);
  lcd_write_data_u8(0x79);

   lcd_write_cmd_u8(0xCB);
  lcd_write_data_u8(0x39);
  lcd_write_data_u8(0x2C);
  lcd_write_data_u8(0x00);
  lcd_write_data_u8(0x34);
  lcd_write_data_u8(0x02);

   lcd_write_cmd_u8(0xF7);
  lcd_write_data_u8(0x20);

   lcd_write_cmd_u8(0xEA);
  lcd_write_data_u8(0x00);
  lcd_write_data_u8(0x00);

   lcd_write_cmd_u8(LCD_CMD_POWER_CONTROL_2);    //Power control
  lcd_write_data_u8(0x11);   //SAP[2:0];BT[3:0]

   lcd_write_cmd_u8(LCD_CMD_VCOMM_CONTROL_1);    //VCM control 1
  lcd_write_data_u8(0x34);
  lcd_write_data_u8(0x3D);

   lcd_write_cmd_u8(LCD_CMD_VCOMM_CONTROL_2);    //VCM control 2
  lcd_write_data_u8(0xC0);

   lcd_write_cmd_u8(LCD_CMD_MEMORY_ACCESS_CONTROL);    // Memory Access Control
  lcd_write_data_u8(0x68);

   lcd_write_cmd_u8(LCD_CMD_PIXEL_FORMAT_SET);      // Pixel format
  lcd_write_data_u8(0x55);  //16bit

   lcd_write_cmd_u8(LCD_CMD_FRAME_CONTROL_NORMAL_MODE);        // Frame rate
  lcd_write_data_u8(0x00);
  lcd_write_data_u8(0x1D);  //61Hz

   lcd_write_cmd_u8(0xB6);    // Display Function Control
  lcd_write_data_u8(0x0A);
  lcd_write_data_u8(0xA2);
  lcd_write_data_u8(0x27);
  lcd_write_data_u8(0x00);

  lcd_write_cmd_u8(LCD_CMD_ENTRY_MODE_SET); //Entry mode
  lcd_write_data_u8(0x07);


   lcd_write_cmd_u8(LCD_CMD_DISPLAY_FUNCTION_CONTROL);    // 3Gamma Function Disable
  lcd_write_data_u8(0x08);

   lcd_write_cmd_u8(LCD_CMD_GAMMA_SET);    //Gamma curve selected
  lcd_write_data_u8(0x01);


  lcd_write_cmd_u8(LCD_CMD_POSITIVE_GAMMA_CORRECTION); //positive gamma correction
  lcd_write_data_u8(0x1f);
  lcd_write_data_u8(0x1a);
  lcd_write_data_u8(0x18);
  lcd_write_data_u8(0x0a);
  lcd_write_data_u8(0x0f);
  lcd_write_data_u8(0x06);
  lcd_write_data_u8(0x45);
  lcd_write_data_u8(0x87);
  lcd_write_data_u8(0x32);
  lcd_write_data_u8(0x0a);
  lcd_write_data_u8(0x07);
  lcd_write_data_u8(0x02);
  lcd_write_data_u8(0x07);
  lcd_write_data_u8(0x05);
  lcd_write_data_u8(0x00);

  lcd_write_cmd_u8(LCD_CMD_NEGATIVE_GAMMA_CORRECTION); //negamma correction
  lcd_write_data_u8(0x00);
  lcd_write_data_u8(0x25);
  lcd_write_data_u8(0x27);
  lcd_write_data_u8(0x05);
  lcd_write_data_u8(0x10);
  lcd_write_data_u8(0x09);
  lcd_write_data_u8(0x3a);
  lcd_write_data_u8(0x78);
  lcd_write_data_u8(0x4d);
  lcd_write_data_u8(0x05);
  lcd_write_data_u8(0x18);
  lcd_write_data_u8(0x0d);
  lcd_write_data_u8(0x38);
  lcd_write_data_u8(0x3a);
  lcd_write_data_u8(0x1f);

   lcd_write_cmd_u8(LCD_CMD_SLEEP_OUT);    //Exit Sleep
   Cy_SysLib_Delay(120);
   lcd_write_cmd_u8(LCD_CMD_DISPLAY_ON);    //Display on
   Cy_SysLib_Delay(50);

}

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
cy_rslt_t lcd_config_gpio(void)
{
    cy_rslt_t rslt;
  rslt = cyhal_gpio_init(PIN_IO_LCD_CSX, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);

	if( rslt != CY_RSLT_SUCCESS)
	{
        return rslt;
	}

  rslt = cyhal_gpio_init(PIN_IO_LCD_DCX, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);

	if( rslt != CY_RSLT_SUCCESS)
	{
        return rslt;
	}

  rslt = cyhal_gpio_init(PIN_IO_LCD_WRX, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);

	if( rslt != CY_RSLT_SUCCESS)
	{
        return rslt;
	}

  rslt = cyhal_gpio_init(PIN_IO_LCD_D0, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);

	if( rslt != CY_RSLT_SUCCESS)
	{
        return rslt;
	}

  rslt = cyhal_gpio_init(PIN_IO_LCD_D1, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);

	if( rslt != CY_RSLT_SUCCESS)
	{
        return rslt;
	}

  rslt = cyhal_gpio_init(PIN_IO_LCD_D2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);

	if( rslt != CY_RSLT_SUCCESS)
	{
        return rslt;
	}

  rslt = cyhal_gpio_init(PIN_IO_LCD_D3, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);

	if( rslt != CY_RSLT_SUCCESS)
	{
        return rslt;
	}

  rslt = cyhal_gpio_init(PIN_IO_LCD_D4, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);

	if( rslt != CY_RSLT_SUCCESS)
	{
		return rslt;
	}

  rslt = cyhal_gpio_init(PIN_IO_LCD_D5, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);

	if( rslt != CY_RET_SUCCESS)
	{
		return rslt;
	}

  rslt = cyhal_gpio_init(PIN_IO_LCD_D6, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);

	if( rslt != CY_RSLT_SUCCESS)
	{
		return rslt;
	}

  rslt = cyhal_gpio_init(PIN_IO_LCD_D7, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);


	return rslt;
}

/*******************************************************************************
* Function Name:ece353_enable_lcd
********************************************************************************
* Summary: Configures the LCD screen and displays a default image              *
* Parameters
*   print_debug  - if true, debug messages will be printed out to the console
*
* Return:
*  Nothing
*******************************************************************************/
void ece353_enable_lcd(void)
{
  cy_rslt_t result;

  result = lcd_config_gpio();

  if(result == CY_RSLT_SUCCESS)
  {
    lcd_config_screen();
	  lcd_clear_screen(LCD_COLOR_BLACK);
  }
}

// from HW2
/*******************************************************************************
* Function Name: lcd_draw_rectangle_centered
********************************************************************************
* Summary: draws a filled rectangle centered at the coordinates set by x, y
* Returns:
*  Nothing
*******************************************************************************/
void lcd_draw_rectangle_centered(
  uint16_t x,
  uint16_t y,
  uint16_t width_pixels,
  uint16_t height_pixels,
  uint16_t fColor
)
{
    uint16_t i,j;
    uint16_t x0;
    uint16_t x1;
    uint16_t y0;
    uint16_t y1;

    // Set the left Col to be the center point minus half the width
    x0 = x - (width_pixels/2);

    // Set the Right Col to be the center point plus half the width
    x1 = x + (width_pixels/2);

    // Account for a width that is an even number
    if( (width_pixels & 0x01) == 0x00)
    {
        x1--;
    }

    // Set the upper Row to be the center point minus half the height
    y0 = y  - (height_pixels/2);

    // Set the upper Row to be the center point minus half the height
    y1 = y  + (height_pixels/2) ;

    // Account for a height that is an  number
    if( (height_pixels & 0x01) == 0x00)
    {
        y1--;
    }

    // Set the boundry of the image to draw
    lcd_set_pos(x0, x1, y0, y1);

    // Write out the image data
    for(i = 0; i < height_pixels; i++)
    {
        for(j=0; j < width_pixels; j++)
        {
            lcd_write_data_u16(fColor);
        }
    }

}