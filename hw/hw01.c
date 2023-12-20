/**
 * @file hw01.c
 * @author Tianqi Shen (tshen46@wisc.edu) & Bowen Quan (bquan2@wisc.edu)
 * @brief
 * @version 0.1
 * @date 2023-09-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "hw01.h"
#include "main.h"

#define TICKS_MS_100 10000000 // 0.1s * 100MHz

/*****************************************************************************/
/*  Function Declaration                                                     */
/*****************************************************************************/

/*****************************************************************************/
/*  Global Variables                                                         */
/*****************************************************************************/
// enumerated type to switch between different modes of the clock
typedef enum {
    RUN_MODE,
    SET_TIME_MODE,
    SET_ALARM_MODE
} hw01_mode;

// interrupts
volatile hw01_timer_alerts_t BLINK_ALERT = HW01_ALERT_NONE; // used to blink the alarm and clock times
volatile hw01_timer_alerts_t TIMER_ALERT = HW01_ALERT_NONE; // used to increment the time of the clock
volatile hw01_timer_alerts_t SW1_ALERT = HW01_ALERT_NONE; // used to check if Button 1 was pressed
volatile hw01_timer_alerts_t SW2_ALERT = HW01_ALERT_NONE; // used to check if Button 2 was pressed
volatile hw01_timer_alerts_t SW3_ALERT = HW01_ALERT_NONE; // used to check if Button 3 was pressed

/** 
 * @brief
 * Initializes the PSoC6 Peripherals used for HW01
 */
void hw01_peripheral_init(void)
{
    /* Initialize the pushbuttons */
    push_buttons_init();

    /* Initalize the LEDs */
    leds_init();

    /* Initialize the LCD */
    ece353_enable_lcd();

    /* Initialize the buzzer */
    pwm_buzzer_init();

    /* Intitialize a timer to generate an interrupt every 100mS*/
    hw01_timer_init(TICKS_MS_100);
}

/**
 * @brief
 * Implements the main application for HW01
 */
void hw01_main_app(void) {
    hw01_mode current_mode = SET_TIME_MODE;

    hw01_display_time(0, 0, HW01_LCD_TIME_COLOR);
    hw01_display_alarm(0, 0, HW01_LCD_ALARM_COLOR);

    // variables for the number of minutes and seconds
    uint8_t time_minutes = 0;
    uint8_t time_seconds = 0;
    uint8_t alarm_minutes = 0;
    uint8_t alarm_seconds = 0;

    bool blink_on = true; // whether the time is shown or not, used for blinking
    bool minutes_selected = true; // whether minutes or seconds are selected

    bool alarm_active = false; // whether the alarm is on (ringing) or not
    bool alarm_set = false; // whether the alarm is set (going to ring)

    while (1) {
        switch (current_mode) {
            case RUN_MODE:
                // increment the clock's time every 1s
                if (TIMER_ALERT == HW01_ALERT_TIME_UPDATE) {
                    TIMER_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    // increment minutes if seconds will equal 60 after incrementing
                    if (time_seconds + 1 == 60) {
                        time_minutes = (time_minutes + 1) % 3;
                    }
                    time_seconds = (time_seconds + 1) % 60;

                    // the alarm should ring when the alarm is set and the alarm matches the clock's time
                    if( (alarm_set == true) && (alarm_minutes == time_minutes) && (alarm_seconds == time_seconds) ){
                        alarm_active = true;
                        // start the alarm buzzer
                        pwm_buzzer_start();
                    }

                    // set the clock time's color to red if the alarm is active, green otherwise
                    if (alarm_active) {
                        hw01_display_time(time_minutes, time_seconds, HW01_LCD_ALARM_ACTIVE_COLOR);
                    } else {
                        hw01_display_time(time_minutes, time_seconds, HW01_LCD_TIME_COLOR);
                    }
                }

                // SW3 handler
                if (SW3_ALERT == HW01_ALERT_BUTTON_LT_2S) {
                    SW3_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    // toggle the alarm's on/off state
                    alarm_set = !alarm_set;
                    
                    // draw the bell based on the alarm's new state
                    if (alarm_set) {
                        hw01_draw_bell();
                    } else {
                        hw01_erase_bell();
                    }
                } else if (SW3_ALERT == HW01_ALERT_BUTTON_GT_2S) {
                    SW3_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    // go into SET ALARM MODE if SW3 is held more than 2 seconds
                    current_mode = SET_ALARM_MODE;
                    minutes_selected = true; // default to having minutes selected.   
                }

                // hold SW1 for more than 2s to disable the active alarm
                if(SW1_ALERT == HW01_ALERT_BUTTON_GT_2S){ 
                    SW1_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    alarm_active = false; // disable alarm
                    // disabling the active alarm also turns the alarm off
                    alarm_set = false;
                    // stop the buzzer
                    pwm_buzzer_stop();
                    // erase bell
                    hw01_erase_bell();
                    // reset the color of the clock time (instant effect)
                    hw01_display_time(time_minutes, time_seconds, HW01_LCD_TIME_COLOR);
                }
            break;

            case SET_ALARM_MODE:
                // increment the clock's time every 1s
                if (TIMER_ALERT == HW01_ALERT_TIME_UPDATE) {
                    TIMER_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    // increment minutes if seconds will equal 60 after incrementing
                    if (time_seconds + 1 == 60) {
                        time_minutes = (time_minutes + 1) % 3;
                    }
                    time_seconds = (time_seconds + 1) % 60;
                    // update the clock display
                    hw01_display_time(time_minutes, time_seconds, HW01_LCD_TIME_COLOR); 
                }

                // blink handler 
                if (BLINK_ALERT == HW01_ALERT_BLINK) {
                    BLINK_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    // invert blink so we do the other option next time
                    blink_on = !blink_on;
                }
                // make the time appear if it's not, and vice versa
                if (blink_on) {
                    hw01_display_alarm(alarm_minutes, alarm_seconds, LCD_COLOR_BLACK);
                } else {
                    hw01_display_alarm(alarm_minutes, alarm_seconds, HW01_LCD_ALARM_COLOR);
                }

                // SW3 handler
                if (SW3_ALERT == HW01_ALERT_BUTTON_LT_2S) {
                    SW3_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    // toggle between minutes and seconds if SW3 is pressed
                    minutes_selected = !minutes_selected;
                } else if (SW3_ALERT == HW01_ALERT_BUTTON_GT_2S) {
                    SW3_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    // make sure the alarm is actually shown (not blinking off) before transitioning to other mode
                    hw01_display_alarm(alarm_minutes, alarm_seconds, HW01_LCD_ALARM_COLOR);
                    // go into RUN MODE if SW3 is held
                    current_mode = RUN_MODE;     
                }
                
                // increment alarm when SW1 is pressed
                if (SW1_ALERT == HW01_ALERT_BUTTON_LT_2S || SW1_ALERT == HW01_ALERT_BUTTON_GT_2S) {
                    SW1_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    if (minutes_selected) {
                        alarm_minutes = (alarm_minutes + 1) % 3; // minutes range is [0-2]
                    } else {
                        alarm_seconds = (alarm_seconds + 1) % 60; // seconds range is [0-59]
                    }
                }

                // decrement alarm when SW2 is pressed
                if (SW2_ALERT == HW01_ALERT_BUTTON_LT_2S || SW2_ALERT == HW01_ALERT_BUTTON_GT_2S) {
                    SW2_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    // NOTE: we add the 2nd modulus value (3 and 60) to avoid getting negative numbers when
                    // either time_minutes or time_seconds is 0 
                    if (minutes_selected) {
                        alarm_minutes = (alarm_minutes + 3 - 1) % 3; // minutes range is [0-2]
                    } else {
                        alarm_seconds = (alarm_seconds + 60 - 1) % 60; // seconds range is [0-59]
                    }
                }
                break;
        
            default: // SET_TIME_MODE
                if (BLINK_ALERT == HW01_ALERT_BLINK) {
                    BLINK_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    // invert blink so we do the other option next time
                    blink_on = !blink_on;
                }

                // make the time appear if it's not, and vice versa
                if (blink_on) {
                    hw01_display_time(time_minutes, time_seconds, HW01_LCD_TIME_COLOR);
                } else {
                    hw01_display_time(time_minutes, time_seconds, LCD_COLOR_BLACK);
                }

                // SW3 handler
                if (SW3_ALERT == HW01_ALERT_BUTTON_LT_2S) {
                    SW3_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    // toggle between minutes and seconds if SW3 is pressed
                    minutes_selected = !minutes_selected;
                } else if (SW3_ALERT == HW01_ALERT_BUTTON_GT_2S) {
                    SW3_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    // go into RUN MODE if SW3 is held more than 2 seconds
                    current_mode = RUN_MODE;       
                }

                // increment when SW1 is pressed
                if (SW1_ALERT == HW01_ALERT_BUTTON_LT_2S || SW1_ALERT == HW01_ALERT_BUTTON_GT_2S) {
                    SW1_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    if (minutes_selected) {
                        time_minutes = (time_minutes + 1) % 3; // minutes range is [0-2]
                    } else {
                        time_seconds = (time_seconds + 1) % 60; // seconds range is [0-59]
                    }
                }

                // decrement when SW2 is pressed
                if (SW2_ALERT == HW01_ALERT_BUTTON_LT_2S || SW2_ALERT == HW01_ALERT_BUTTON_GT_2S) {
                    SW2_ALERT = HW01_ALERT_NONE; // acknowledge the alert
                    // NOTE: we add the 2nd modulus value (3 and 60) to avoid getting negative numbers when
                    // either time_minutes or time_seconds is 0 
                    if (minutes_selected) {
                        time_minutes = (time_minutes + 3 - 1) % 3; // minutes range is [0-2]
                    } else {
                        time_seconds = (time_seconds + 60 - 1) % 60; // seconds range is [0-59]
                    }
                }
        }  
    }
}