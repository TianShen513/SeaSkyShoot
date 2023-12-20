#ifndef __TASK_HIGHSCORE_H__
#define __TASK_HIGHSCORE_H__

#include "main.h"

// initialize the high score task
void init_task_highScore(void);
// enable interrupts for Button 3
void sw3_irq_enable(void);
// calls task_highScore using a task notification when Button 3 is pressed
void sw3_handler(void *handler_arg, cyhal_gpio_event_t event);
// resets high score when Button 3 is pressed
void task_highScore(void *pvParameters);

#endif
