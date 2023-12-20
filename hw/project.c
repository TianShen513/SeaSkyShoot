/**
 * @file project.c
 * @author Tianqi Shen (tshen46@wisc.edu) & Bowen Quan (bquan2@wisc.edu)
 * @brief
 * @version 0.1
 * @date 2023-09-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "project.h"

TaskHandle_t Task_Handle_State_Machine;
TaskHandle_t Task_Handle_joystick;
TaskHandle_t Task_Handle_lightSensor;

void task_joystick(void *pvParameters);
void task_lightSensor(void *pvParameters);
void task_state_machine(void *pvParameters);


/**
 * @brief
 * Initializes the PSoC6 Peripherals used for project
 */
void project_peripheral_init(void)
{
    // initialize SPI
    spi_init();

    /* Initialize the joystick*/
    joystick_init();

    /* Initialize the I2C protocol*/
    i2c_init();

    /* Initialize the buttons */
    push_buttons_init();

    /* Initialize the remote UART */
    remote_uart_init();

    /* Enable interrupts and circular buffers from remote board */ 
    remote_uart_enable_interrupts();

    /* Initialize the LCD */
    ece353_enable_lcd();

    eeprom_cs_init();

    sw3_irq_enable();

    io_expander_set_configuration(0x80); 	// Set bit 7 as input, bits 6-0 as outputs 
	io_expander_set_output_port(0x00); 		// Turn OFF all LEDs


}

/**
 * @brief
 * Implements the main application for project
 * Initializes all the tasks and starts the scheduler
 */
void project_main_app(void) { 
     init_task_highScore();
    // initialize joystick task
    init_task_joystick();

    // initialize state machine task
    init_task_state_machine();

    // initialize light sensor task
    init_task_lightSensor();

    // initialize imu task
    init_task_imu();

    // initialize button1 task
    init_task_button1();

    // initialize UART
    init_task_uartTX();
    init_task_uartRX();

    /* Start the Scheduler */
    vTaskStartScheduler();
    
    while(1) {}
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while (1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    // Handle FreeRTOS Stack Overflow
    printf("stack overflow: %s\n\r", pcTaskName);
    while (1)
    {
    }
}