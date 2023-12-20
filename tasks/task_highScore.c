#include "../main.h"



/*****************************************************************************/
/*  FreeRTOS Handles
/*****************************************************************************/
/* Create Task Handles for High Score reset task*/
TaskHandle_t Task_Handle_HighScore;
extern SemaphoreHandle_t Sem_Imu;
extern QueueHandle_t Queue_uartTX;
/*****************************************************************************/
/* Function Declarations                                                     */
/*****************************************************************************/
void task_highScore(void *pvParameters);

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/
cyhal_gpio_callback_data_t sw3_callback_data;

/*****************************************************************************/
/* Interrupt Handlers                                                        */
/*****************************************************************************/
/* Interrupt handler callback function */
void sw3_handler(void *handler_arg, cyhal_gpio_event_t event)
{
    BaseType_t xHigherPriorityTaskWoken;

    /* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
     it will get set to pdTRUE inside the interrupt safe API function if a
     context switch is required. */
    xHigherPriorityTaskWoken = pdFALSE;

    /* ADD CODE */
    /* Send a notification directly to the task to which interrupt processing
     * is being deferred.
     */
    vTaskNotifyGiveFromISR(Task_Handle_HighScore, &xHigherPriorityTaskWoken);

    /* ADD CODE */
    /* Call the function that will force the task that was running prior to the
     * interrupt to yield*/
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}



/*****************************************************************************/
/* Peripheral Initialization                                                  */
/*****************************************************************************/

// enable interrupts for Button 3
void sw3_irq_enable(void)
{
    sw3_callback_data.callback = sw3_handler;
    cyhal_gpio_register_callback(PIN_SW3, &sw3_callback_data);
    cyhal_gpio_enable_event(PIN_SW3, CYHAL_GPIO_IRQ_FALL, 3, true);
}


/**
 * Initialize the high score task
 */
void init_task_highScore(void)
{  
    BaseType_t rslt;
    rslt = xTaskCreate(task_highScore,
                "High Score",
                configMINIMAL_STACK_SIZE,
                NULL,
                4,
                &Task_Handle_HighScore);
    if (rslt == pdPASS)
    {
        printf("High score task created\n\r");
    }
    else
    {
        printf("High task NOT created\n\r");
    }
}

// resets the high score to 99 in EEPROM and sends a UART message to make the other board reset high score too
void task_highScore(void *pvParameters)
{       
    uart_data_format send;
    while (1)
    {
        /* Wait indefinitely for a Task Notification */
        ulTaskNotifyTake(true, portMAX_DELAY);
        send.start_byte = RESET_HIGH_SCORE;
        xQueueSend(Queue_uartTX, &send, portMAX_DELAY);
        // reset the high score in EEPROM to 99
        xSemaphoreTake(Sem_Imu, portMAX_DELAY);
        eeprom_write_byte(HIGH_SCORE_ADDRESS, MAX_HIGH_SCORE);
        xSemaphoreGive(Sem_Imu);
        // display the high score
        draw_high_score_number(MAX_HIGH_SCORE);
    }
}



