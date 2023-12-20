/**
 * @file ice06.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "../main.h"

#if defined(ICE) && (FILE_ID == 6)

/*****************************************************************************/
/* Macros                                                                    */
/*****************************************************************************/
typedef enum
{
    COLOR_RED = 0,
    COLOR_GREEN = 1,
    COLOR_BLUE = 2
} active_color_t;

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/
char ICE_DESCRIPTION[] = "ECE353: ICE 06 - SysTick/PWM";

/*****************************************************************************/
/* Function Declarations                                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Function Definitions                                                      */
/*****************************************************************************/

/**
 * @brief
 * This function will initialize all of the hardware resources for
 * the ICE
 */
void peripheral_init(void)
{
    /* Initialize the push buttons */
    push_buttons_init();

    /* Initialize the LEDs using the PWM peripherals*/
    leds_pwm_init();

    /* Set the SysTick Timer to that it  expires every 5ms*/
    systick_init(TICKS_MS_005);
}

/**
 * @brief
 * This function implements the behavioral requirements for the ICE
 */
void main_app(void)
{
    uint8_t duty_cycle_red = 0;
    uint8_t duty_cycle_grn = 0;
    uint8_t duty_cycle_blue = 0;

    for (;;)
    {
        printf("Red %02d, Green %02d, Blue %02d\n\r", duty_cycle_red, duty_cycle_grn, duty_cycle_blue);
    }

}
#endif