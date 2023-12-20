#include "task_imu.h"

QueueHandle_t Queue_Imu_Acceleration;
SemaphoreHandle_t Sem_Imu;

void init_task_imu() {
    // create the queue for joystick position
    Queue_Imu_Acceleration = xQueueCreate(1, sizeof(imu_acceleration_t));
    Sem_Imu = xSemaphoreCreateBinary();
    xSemaphoreGive(Sem_Imu);

    // create the task
    BaseType_t rslt = xTaskCreate(task_imu,
            "Imu",
            256,
            NULL,
            2,
            NULL);
    if (rslt == pdPASS) {
        printf("IMU task created\n\r");
    } else {
        printf("IMU task NOT created\n\r");
    }
    
}

/**
 * @brief 
 * This function will monitor the tilt of the board using the accelerometer of the IMU
 * @param pvParameters 
 */
void task_imu(void *pvParameters) {
    
    /* Allocate any local variables used in this task */
    //imu_angular_velocity_t curr_velocity = {0, 0, 0};
    imu_acceleration_t accel = {0, 0, 0};
    bool init_once = true;
    while (1) {
        if (init_once) {
            // run this function to do some initialization for the IMU
            lsm6dsm_orientation();
            printf("imu inited\n\r");
            init_once = false;
        }
       
        vTaskDelay(5);
        // take semaphore to make sure EEPROM can't use SPI
        xSemaphoreTake(Sem_Imu, portMAX_DELAY);
        get_acceleration(&accel);
        xSemaphoreGive(Sem_Imu);
        xQueueSend(Queue_Imu_Acceleration, &accel, portMAX_DELAY);
    }
}