#include "task_joystick.h"

QueueHandle_t Queue_Joystick_Position;

void init_task_joystick() {
    // create the queue for joystick position
    Queue_Joystick_Position = xQueueCreate(1, sizeof(joystick_position_t));
    // create the task
    BaseType_t rslt = xTaskCreate(task_joystick,
            "Joystick",
            configMINIMAL_STACK_SIZE,
            NULL,
            2,
            NULL);
    if (rslt == pdPASS) {
        printf("joystick task created\n\r");
    } else {
        printf("joystick task NOT created\n\r");
    }
    
}

/**
 * @brief 
 * This function will monitor the current position of the joystick.  If the joystick
 * if not in the center postion, send a message to the LCD task with the current postion
 * of the joystick.  This will be done using a queue. 
 * @param pvParameters 
 */
void task_joystick(void *pvParameters) {
    /* Allocate any local variables used in this task */
    joystick_position_t cur_pos = JOYSTICK_POS_CENTER;

    while (1) {
        vTaskDelay(5);

        cur_pos = joystick_get_pos();

        /* Check to see what the current position of the
         * joystick is.  If it is not in the center, send a message
         * to the stylus position queue
         */
        if (cur_pos != JOYSTICK_POS_CENTER)
        {
            /* ADD CODE */
            /* Send the current joystick position to the stylus position queue */
            xQueueSend(Queue_Joystick_Position, &cur_pos, portMAX_DELAY);
        }
    }
}