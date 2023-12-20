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

#if defined(EXAMPLE) && (FILE_ID == 6)

/*****************************************************************************/
/*  Custom Data Types                                                        */
/*****************************************************************************/
typedef enum {
    TIMER_ALERT_NONE,
    TIMER_ALERT_TOGGLE_RED,
    TIMER_ALERT_TOGGLE_BLUE
}timer_alerts_t;

/*****************************************************************************/
/*  Global Variables                                                         */
/*****************************************************************************/
char ICE_DESCRIPTION[] = "ECE353: Ex06 - Timer Interrupts";

const cyhal_timer_cfg_t ex_timer_cfg =
    {
        .compare_value = 0,              /* Timer compare value, not used */
        .period = 10000000,              /* number of timer ticks */
        .direction = CYHAL_TIMER_DIR_UP, /* Timer counts up */
        .is_compare = false,             /* Don't use compare mode */
        .is_continuous = true,           /* Run timer indefinitely */
        .value = 0                       /* Initial value of counter */
};

/* Timer object used */
cyhal_timer_t ex_timer_obj;

/*****************************************************************************/
/*  Interrupt Handlers                                                       */
/*****************************************************************************/
/**
 * @brief
 *  Function used as Timer Handler
 */
void Handler_Timer(void *handler_arg, cyhal_timer_event_t event)
{
    /*
     * The IRQ occurs every 100mS.  
     *
     * Toggle the RED LED every second requires 10 IRQs
     * 
     * Toggle the BLUE LED every 5 seconds requires 50 IRQs 
     * 
    */

}

/*****************************************************************************/
/* Peripheral Initilization                                                  */
/*****************************************************************************/

/**
 * @brief
 * This function will initialize all of the hardware resources for
 * the ICE
 */
void peripheral_init(void)
{
    cy_rslt_t rslt;

    /* Initialize leds */
    leds_init();

    /* Initialize a timer */
    rslt = cyhal_timer_init(&ex_timer_obj, NC, NULL);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // If the initialization fails, halt the MCU

    /* Apply timer configuration such as period, count direction, run mode, etc. */
    rslt = cyhal_timer_configure(&ex_timer_obj, &ex_timer_cfg); 
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // If the initialization fails, halt the MCU

    /* Set the frequency of timer*/
    rslt = cyhal_timer_set_frequency(&ex_timer_obj,100000000); 
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // If the initialization fails, halt the MCU

    /* Assign the ISR to execute on timer interrupt */
    cyhal_timer_register_callback(&ex_timer_obj,Handler_Timer,NULL);

    /* Set the event on which timer interrupt occurs and enable it */
    cyhal_timer_enable_event(&ex_timer_obj, CYHAL_TIMER_IRQ_TERMINAL_COUNT,3, true);

    /* Start the timer with the configured settings */
    rslt = cyhal_timer_start(&ex_timer_obj);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // If the initialization fails, halt the MCU
}

/*****************************************************************************/
/* Application Code                                                          */
/*****************************************************************************/
/**
 * @brief
 * This function implements the behavioral requirements for the ICE
 */
void main_app(void)
{
    /* Enter Infinite Loop*/
    while (1)
    {
    }
}
#endif