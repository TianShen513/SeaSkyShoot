#include "task_uartTX.h"

QueueHandle_t Queue_uartTX;

void init_task_uartTX() {
    // create the queue for joystick position
    Queue_uartTX = xQueueCreate(1, sizeof(uart_data_format));

    // create the task
    BaseType_t rslt = xTaskCreate(task_uartTX,
            "uartTX",
            configMINIMAL_STACK_SIZE,
            NULL,
            2,
            NULL);
            
    if (rslt == pdPASS) {
        printf("uartTX task created\n\r");
    } else {
        printf("uartTX task NOT created\n\r");
    }
}

void task_uartTX(void *pvParameters) {
    uart_data_format tx_data;
    // char buffer to transmit via UART
    char buffer[7]; // max number of bytes we transmit is 7
    while(1) {
        // wait until something has been put into the queue
        xQueueReceive(Queue_uartTX, &tx_data, portMAX_DELAY);

        // send shot data
        if (tx_data.start_byte == SHOT_DATA) {
            buffer[0] = tx_data.start_byte;
            buffer[1] = (char)tx_data.vx;
            buffer[2] = (char)tx_data.vy;
            buffer[3] = (char)(tx_data.y >> 8); // high byte
            buffer[4] = (char)(tx_data.y); // low byte
            buffer[5] = '\n';
            buffer[6] = 0;
            // for loop to see if we need to add 1 and set bit in start_byte
            for (uint8_t i = 1; i <= 4; i++) {
                //| 2 << (4 - i)
                if (buffer[i] == 0 || buffer[i] == '\n' || buffer[i] == '\r') {
                    buffer[i] += 1;
                    buffer[0] = buffer[0] | 2 << (4 - i); // set bit in start_byte
                }
            }            
        } else if (tx_data.start_byte == NEW_HIGH_SCORE) {
            // send new high score
            buffer[0] = (char)tx_data.start_byte;
            buffer[1] = (char)tx_data.high_score;
            buffer[2] = '\n';
            buffer[3] = 0;
        } else {
            // just send the start byte.
            buffer[0] = (char)tx_data.start_byte;
            buffer[1] = '\n';
            buffer[2] = 0;
        }
        // send data to be transmitted over UART TX
        remote_uart_tx_data_async(buffer);

    }
}