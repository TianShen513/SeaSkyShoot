#ifndef __TASK_LIGHTSENSOR_H__
#define __TASK_LIGHTSENSOR_H__

#include "main.h"

// initializes the queue and the task
void init_task_lightSensor(void);

void task_lightSensor(void *pvParameters);

#endif