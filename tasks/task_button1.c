#include "task_button1.h"

SemaphoreHandle_t Sem_Button1;

void init_task_button1() {
    // create the semaphore
    Sem_Button1 = xSemaphoreCreateBinary();
    // initialize semaphores as available 
    xSemaphoreGive(Sem_Button1);
    // create the task
    BaseType_t rslt = xTaskCreate(task_button1,
            "Button1",
            configMINIMAL_STACK_SIZE,
            NULL,
            2,
            NULL);
    if (rslt == pdPASS) {
        printf("button1 task created\n\r");
    } else {
        printf("button1 task NOT created\n\r");
    }
    
}

// gives the semaphore if button1 is pressed. 
void task_button1(void *pvParameters) {
    /* Allocate any local variables used in this task */
    bool state_button_prev = false;
    bool state_button_curr = false;

    while (1) {
        /* Sleep for 50mS -- DO NOT use any cyhal_ functions to delay */
        vTaskDelay(50);

        /* Check the current state of the button */
        if ((PORT_BUTTONS->IN & SW1_MASK) == 0x00) {
            state_button_curr = true;
        } else {
            state_button_curr = false;
        }

        /* Detect if the button has been pressed */
        if (state_button_curr && !state_button_prev)
        {
            /* Give the semaphore to the state machine task */
            xSemaphoreGive(Sem_Button1);
        }

        /* Update the previous button state */
        state_button_prev = state_button_curr;
    }
}