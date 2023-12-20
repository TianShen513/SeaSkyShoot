/**
 * @file ice08.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "../main.h"

#if defined(ICE) && (FILE_ID == 8)

/*****************************************************************************/
/* Macros                                                                    */
/*****************************************************************************/
#define IRQ_PRIORITY_ADC_PIN 3
#define TICKS_MS_010 (100000)
#define TICKS_MS_020 (200000)

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/
char ICE_DESCRIPTION[] = "ECE353: ICE 08 - ADC";

volatile bool ALERT_TIMER_010_MS = false; /* ADD CODE */

/* Timer object used */
cyhal_timer_t timer_obj;

const cyhal_timer_cfg_t timer_cfg =
    {
        .compare_value = 0,                /* Timer compare value, not used */
        .period = TICKS_MS_010,            /* Timer period set to a large enough value
                                            * compared to event being measured */
        .direction = CYHAL_TIMER_DIR_UP,   /* Timer counts up */
        .is_compare = false,               /* Don't use compare mode */
        .is_continuous = true,             /* Run timer indefinitely */
        .value = 0                         /* Initial value of counter */
};


/*****************************************************************************/
/* Function Declarations                                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Function Definitions                                                      */
/*****************************************************************************/

/**
 * @brief
 *  Function used as SW1 Handler
 */
void Handler_Timer_010_MS(void *handler_arg, cyhal_timer_event_t event)
{
    CY_UNUSED_PARAMETER(handler_arg);
    CY_UNUSED_PARAMETER(event);
    ALERT_TIMER_010_MS = true; /* Add CODE */
}
/**
 * @brief
 * This function will initialize all of the hardware resources for
 * the ICE
 */
void peripheral_init(void)
{
    cy_rslt_t rslt;

    /* Initialize the X and Y directions of the joystick to be ADC pins*/
    joystick_init();

    /* Initialize a timer that generates an interrupt every 10mS */
    /* ADD CODE */
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // If the initialization, halt the MCU

    /* Apply timer configuration such as period, count direction, run mode, etc. */
    /* ADD CODE */
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // If the initialization, halt the MCU

    /* Assign the ISR to execute on timer interrupt */
    /* ADD CODE */
  
    /* Set the event on which timer interrupt occurs and enable it */
    /* ADD CODE */
    
    /* Start the timer with the configured settings */
    /* ADD CODE */
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // If the initialization, halt the MCU
}

/**
 * @brief
 * This function implements the behavioral requirements for the ICE
 */
void main_app(void)
{
    joystick_position_t curr_position = JOYSTICK_POS_CENTER;
    joystick_position_t prev_position = JOYSTICK_POS_CENTER;

    while (1)
    {
        if (ALERT_TIMER_010_MS)
        {
            /* Set Interrupt flag back to false*/
            ALERT_TIMER_010_MS = false;

            /* Save the previous position */
            prev_position = curr_position;

            /* Get the current position */
            curr_position = joystick_get_pos();

            /* If the joystick was moved, print its new position*/
            if (curr_position != prev_position)
            {
                printf("Joystick now in ");
                joystick_print_pos(curr_position);
                printf(" position\n\r");
            }
        }
    }
}
#endif