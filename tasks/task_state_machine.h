#ifndef __TASK_STATE_MACHINE_H__
#define __TASK_STATE_MACHINE_H__

#include "main.h"

// max high score that will be displayed or put in EEPROM
#define MAX_HIGH_SCORE 99
// address in EEPROM where we store the high score
#define HIGH_SCORE_ADDRESS 0

// macros for start byte of UART messages
#define SHOT_DATA 0x10
#define SHIP_HIT 0x20 // lets enemy know he won
#define YOUR_SHOT_OUT_OF_BOUNDS 0x30 // send to enemy to let him know he can shoot again
#define SELECT_P1 0x40
#define START_GAME_ACK 0x50
#define RESTART_GAME 0x60
#define NEW_HIGH_SCORE 0x70
#define NO_NEW_HIGH_SCORE 0x80
#define RESET_HIGH_SCORE 0x90

// struct for sending to and receiving from UART
typedef struct {
    uint8_t start_byte;
    int8_t vx;
    int8_t vy;
    int16_t y;
    uint8_t high_score;
} uart_data_format;

// possible states of the state machine
typedef enum {
    TURN_MINE,
    SHOT_MOVING,
    GAME_OVER_STATE,
    INIT_GAME   // select player 1
} project_state;

// return values for move_shot() and move_enemy_shot()
typedef enum {
    SHOT_IN_MY_SIDE,        // default state
    SHOT_IN_ENEMY_SIDE,     // asserted to send shot data to enemy
    SHOT_OUT_OF_BOUNDS,     // shot has hit bottom, right(P1), or left(P2)
    SHIP_COLLISION          // enemy shot hit my ship
} shot_status;


// initializes the task
void init_task_state_machine();

void task_state_machine(void *pvParameters);

#endif