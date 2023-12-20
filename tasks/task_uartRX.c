#include "task_uartRX.h"

QueueHandle_t Queue_uartRX;
extern SemaphoreHandle_t Sem_Imu;

void init_task_uartRX(){
    // create the queue for joystick position
    Queue_uartRX = xQueueCreate(1, sizeof(uart_data_format));

    // create the task
    BaseType_t rslt = xTaskCreate(task_uartRX,
            "uartRX",
            configMINIMAL_STACK_SIZE,
            NULL,
            2,
            NULL);
            
    if (rslt == pdPASS) {
        printf("uartRX task created\n\r");
    } else {
        printf("uartRX task NOT created\n\r");
    }
}

void task_uartRX(void *pvParameters) {
    uart_data_format rx_data;
    // char buffer to receive via UART
    char buffer[5]; // max number of bytes we receive is 5. We don't receive the '\n' and 0
    while (1) {
        // new RX data is available.
        if (ALERT_UART_RX) {
            ALERT_UART_RX = false;
            remote_uart_rx_data_async(buffer, 7);
            // get start_byte
            rx_data.start_byte = buffer[0];
            // decode message
            if (rx_data.start_byte == NEW_HIGH_SCORE) {
                rx_data.high_score = buffer[1];
            } else if ((rx_data.start_byte & 0xF0) == SHOT_DATA) { // mask out lower 4 bits which contain +1 data
                // subtract 1 from all bytes that had 1 added
                for (uint8_t i = 1; i <= 4; i++) {
                    if (rx_data.start_byte & (2 << (4-1))) {
                        buffer[i] -= 1;
                    }
                }
                rx_data.vx = (int8_t)buffer[1];
                rx_data.vy = (int8_t)buffer[2];
                rx_data.y = (int16_t)((int16_t)(buffer[3] << 8) + (int16_t)buffer[4]);
                rx_data.start_byte = SHOT_DATA;
            }else if(rx_data.start_byte == RESET_HIGH_SCORE){
                // reset the high score in EEPROM to 99
                 xSemaphoreTake(Sem_Imu, portMAX_DELAY);
                eeprom_write_byte(HIGH_SCORE_ADDRESS, MAX_HIGH_SCORE);
                xSemaphoreGive(Sem_Imu);
                // display the high score
                draw_high_score_number(MAX_HIGH_SCORE);
            }
            // send rx_data to queue
            /* Send the current joystick position to the stylus position queue */
            xQueueSend(Queue_uartRX, &rx_data, portMAX_DELAY);
        }
    }
}