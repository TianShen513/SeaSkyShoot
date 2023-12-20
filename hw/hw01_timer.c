/**
 * @file hw01_timer.c
 * @author Tianqi Shen (tshen46@wisc.edu) & Bowen Quan (bquan2@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-09-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "hw01_timer.h"

/*****************************************************************************/
/*  Function Declaration                                                     */
/*****************************************************************************/
void Handler_HW01_Timer(void *handler_arg, cyhal_timer_event_t event);

/*****************************************************************************/
/*  Global Variables                                                         */
/*****************************************************************************/

/* Timer object and timer_cfg object */
cyhal_timer_t timer_obj;

cyhal_timer_cfg_t timer_cfg =
    {
        .compare_value = 0,              /* Timer compare value, not used */
        .period = 0,                     /* number of timer ticks */
        .direction = CYHAL_TIMER_DIR_UP, /* Timer counts up */
        .is_compare = false,             /* Don't use compare mode */
        .is_continuous = true,           /* Run timer indefinitely */
        .value = 0                       /* Initial value of counter */
};

/*****************************************************************************/
/*  Interrupt Handlers                                                       */
/*****************************************************************************/
// extern variables to access the interrupts
extern hw01_timer_alerts_t SW1_ALERT;
extern hw01_timer_alerts_t SW2_ALERT;
extern hw01_timer_alerts_t SW3_ALERT;
extern hw01_timer_alerts_t BLINK_ALERT;
extern hw01_timer_alerts_t TIMER_ALERT;

/**
 * @brief
 *  Function used as Timer Handler
 */
void Handler_HW01_Timer(void *handler_arg, cyhal_timer_event_t event)
{
    CY_UNUSED_PARAMETER(handler_arg);
    CY_UNUSED_PARAMETER(event);

    // static variables to track status of interrupts
    static uint8_t blink_count = 0;
    static uint8_t timer_count = 0;
    static uint8_t sw1_count = 0;
    static uint8_t sw2_count = 0;
    static uint8_t sw3_count = 0;

    // get the status of the buttons
    uint32_t reg_val = PORT_BUTTONS->IN;

    // toggle the blinking every 0.5s (5 * 100 ms)
    blink_count = (blink_count + 1) % 5;
    if (blink_count == 0) {
        BLINK_ALERT = HW01_ALERT_BLINK;
    }

    // make the clock increment every 1s (10 * 100ms)
    timer_count = (timer_count + 1) % 10;
    if (timer_count == 0) {
        TIMER_ALERT = HW01_ALERT_TIME_UPDATE;
    }

    // check if SW1 has been pressed (active low, will be 0)
    if ((reg_val & SW1_MASK) == 0) {
        // check if SW1 has been pressed for more than 2s (20 * 100ms)
        if (sw1_count < 20) {
            sw1_count++; 
        } 
    } else { // button is not pressed, so reset the count
        if (sw1_count < 20 && sw1_count > 0) {
            SW1_ALERT = HW01_ALERT_BUTTON_LT_2S;
        } else if (sw1_count >= 20){
            SW1_ALERT = HW01_ALERT_BUTTON_GT_2S;
        }
        sw1_count = 0;
    }

    // check if SW2 has been pressed (active low, will be 0)
    if ((reg_val & SW2_MASK) == 0) {
        // check if SW2 has been pressed for more than 2s (20 * 100ms)
        if (sw2_count < 20) {
            sw2_count++; 
        } 
    } else { // button is not pressed, so reset the count
        if (sw2_count < 20 && sw2_count > 0) {
            SW2_ALERT = HW01_ALERT_BUTTON_LT_2S;
        } else if (sw2_count >= 20){
            SW2_ALERT = HW01_ALERT_BUTTON_GT_2S;
        }
        sw2_count = 0;
    }

    // check if SW3 has been pressed (active low, will be 0)
    if ((reg_val & SW3_MASK) == 0) {
        // check if SW3 has been pressed for more than 2s (20 * 100ms)
        if (sw3_count < 20) {
            sw3_count++; 
        } 
    } else { // button is not pressed, so reset the count
        if (sw3_count < 20 && sw3_count > 0) {
            SW3_ALERT = HW01_ALERT_BUTTON_LT_2S;
        } else if (sw3_count >= 20){
            SW3_ALERT = HW01_ALERT_BUTTON_GT_2S;
        }
        sw3_count = 0;
    }
}

/*****************************************************************************/
/*  TCPWM Initialization Function                                            */
/*****************************************************************************/
void hw01_timer_init(uint32_t ticks)
{
    timer_cfg.period = ticks; // set the ticks for the configuration object
    cy_rslt_t rslt;

    // initialize the timer without a pin output or a clock source
    rslt = cyhal_timer_init(&timer_obj, NC, NULL);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // stop the MCU if initialization error

    // configure the timer
    rslt = cyhal_timer_configure(&timer_obj, &timer_cfg); 
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // stop the MCU if initialization error

    // set frequency of timer (10MHz)
    rslt = cyhal_timer_set_frequency(&timer_obj, 100000000); 
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // stop the MCU if initialization error

    // make ISR execute on timer interrupt
    cyhal_timer_register_callback(&timer_obj,Handler_HW01_Timer,NULL);
    
    // set event on which timer interrupt occurs and enable it
    cyhal_timer_enable_event(&timer_obj, CYHAL_TIMER_IRQ_TERMINAL_COUNT,3, true);

    // start timer with configured settings
    rslt = cyhal_timer_start(&timer_obj);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // stop the MCU if initialization error
}
