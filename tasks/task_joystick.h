#ifndef __TASK_JOYSTICK_H__
#define __TASK_JOYSTICK_H__

#include "main.h"

// initializes the queue and the task
void init_task_joystick();

void task_joystick(void *pvParameters);

#endif