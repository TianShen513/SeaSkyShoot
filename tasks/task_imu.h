#ifndef __TASK_IMU_H__
#define __TASK_IMU_H__

#include "main.h"

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} imu_acceleration_t;

// initializes the queue and the task
void init_task_imu();

void task_imu(void *pvParameters);

#endif