#include "task_state_machine.h"

// queues from other tasks
extern QueueHandle_t Queue_Joystick_Position;
extern QueueHandle_t Queue_Imu_Acceleration;
extern QueueHandle_t Queue_LightSensor_Meas;
extern QueueHandle_t Queue_uartTX;
extern QueueHandle_t Queue_uartRX;
extern QueueHandle_t Queue_HighScore_Info;

// semaphores from other tasks
extern SemaphoreHandle_t Sem_Button1;
extern SemaphoreHandle_t Sem_Imu;
extern SemaphoreHandle_t Sem_I2c;

// whether we are player 1 or 2
bool i_am_p1;

// velocities used to move the ship
int8_t ship_vx;
int8_t ship_vy;
// position of the middle of the ship top
uint16_t ship_x;
uint16_t ship_y;

// position and velocities used to move shot
#define GRAVITY 1 // amount to decrement vy by
#define GRAVITY_INTERVAL 15 // how often gravity should be applied
#define SHOT_SIZE 7
#define CEILING -250    // max vertical position of shots
int8_t shot_vx;
int8_t shot_vy;
uint16_t shot_x;
int16_t shot_y;
bool shot_in_screen;  // whether the shot is in our screen.
bool shot_in_my_side; // NOTE: it's possible for the shot to be in my side and not be in the screen (above ceiling)

// position and velocities used to move enemy shot
int8_t enemy_shot_vx;
int8_t enemy_shot_vy;
uint16_t enemy_shot_x;
int16_t enemy_shot_y;
bool enemy_shot_in_screen;
bool enemy_shot_in_my_side;

// light sensor data used for initial velocity of shot
uint16_t strength_step;
uint8_t strength;
uint8_t strength_past;

void init_task_state_machine() {
    // create the task
    BaseType_t rslt = xTaskCreate(task_state_machine,
                                  "State_Machine",
                                  256,
                                  NULL,
                                  2,
                                  NULL);
    if (rslt == pdPASS) {
        printf("state machine task created\n\r");
    }
    else {
        printf("state machine task NOT created\n\r");
    }
}

/* Returns absolute value of an int8_t
 */
int8_t abs_val(int8_t val) {
    if (val < 0) {
        val = val * -1;
    }
    return val;
}

/* Updates the ship's x and y velocity based on the tilt of the board
 * Velocity is capped at a magnitude of 8.
 * Tilt is determined by the IMU's accelerometer.
 */
void update_velocities(imu_acceleration_t accel) {
    // scale the accelerometer readings to usable values
    ship_vx = accel.x / 1024;
    // clamp the velocity
    if (ship_vx > 8) {
        ship_vx = 8;
    }
    else if (ship_vx < -8){
        ship_vx = -8;
    }
    ship_vy = accel.y / 1024;
    if (ship_vy > 8) {
        ship_vy = 8;
    }
    else if (ship_vy < -8) {
        ship_vy = -8;
    }
}

/* Moves the ship based on its velocities.
 */
void move_ship() {
    static int8_t count_x = 0;
    static int8_t count_y = 0;

    bool ship_moved = false; // whether the position of the ship changed.

    // only move if x velocity is nonzero
    if (ship_vx != 0) {
        count_x--; // decrement count

        // move the ship if the count is 0
        if (count_x <= 0) {
            if (ship_vx < 0 && ship_x < SHIP_MAX_X) {
                ship_x++;
            } else if (ship_vx > 0 && ship_x > SHIP_MIN_X) {
                ship_x--;
            }
            count_x = 8 - abs_val(ship_vx);
            ship_moved = true;
        }
    }

    // only move if y velocity is nonzero
    if (ship_vy != 0) {
        count_y--; // decrement count

        // move the ship if the count is 0
        if (count_y <= 0) {
            // move ship
            if (ship_vy < 0 && ship_y > SHIP_MIN_Y) {
                ship_y--;
            } else if (ship_vy > 0 && ship_y < SHIP_MAX_Y) {
                ship_y++;
            }
            // reset the count
            count_y = 8 - abs_val(ship_vy);
            ship_moved = true;
        }
    }

    if (ship_moved) {
        // draw ship top and ship bottom after moving
        draw_ship(ship_x, ship_y, i_am_p1);
    }
}

/* Updates the strength (initial velocity) of the shot based on
 * the value of the light sensor.
 * More dark = less strength
 */
void update_strength(uint16_t light_sensor_data) {
    strength_past = strength;
    // min strength is 10
    strength = light_sensor_data / strength_step + 10;
    // max strength is 42
    if (strength > 42) {
        strength = 42;
    }
    // only update the strength color if it has changed
    if (strength_past != strength) {
        draw_strength_color(((0x1F * (strength - 10)) / 32) << 11);
    }
}

/* Physics for moving the shot and boundary detection
 */
shot_status move_shot() {
    static int8_t shot_count_x = 0;
    static int8_t shot_count_y = 0;
    static int8_t gravity_count = 0;

    // whether the position of the shot changed
    bool shot_moved_x = false;
    bool shot_moved_y = false;

    // should the shot be transferred to enemy screen?
    if ((i_am_p1 && shot_x <= 0) || (!i_am_p1 && shot_x >= SCREEN_X - SHOT_SIZE)) {
        printf("transfer shot to enemy screen\n\r");
        return SHOT_IN_ENEMY_SIDE;
    }

    // move shot in x direction
    if (shot_vx != 0) {
        shot_count_x--;
        // move the shot if the count is 0
        if (shot_count_x <= 0) {
            if (shot_vx > 0 && shot_x < SCREEN_X - SHOT_SIZE) {
                shot_x++;
            }
            else if (shot_vx < 0 && shot_x > 0) {
                shot_x--;
            }
            shot_count_x = 42 / abs_val(shot_vx);
            shot_moved_x = true;
        }
    }

    shot_count_y--;
    // move the shot if the count is 0
    if (shot_count_y <= 0) {
        // move shot in y direction if it had a downward velocity
        if (shot_vy != 0) {
            if (shot_vy > 0) {
                shot_y--;
            }
            else if (shot_vy < 0 && shot_y < SCREEN_Y - SHOT_SIZE) {
                shot_y++;
            }
            shot_moved_y = true;
            shot_count_y = 42 / abs_val(shot_vy);
        }
    }

    // prevent shot_y from experiencing underflow.
    if (shot_y <= CEILING) {
        shot_y = CEILING + 1;
        shot_vy = 0;
    }
    // after moving in y, check if it has gone above the ceiling.
    if (shot_in_screen && shot_y <= 1) {
        shot_in_screen = false;
        // redraw sky to erase shot
        draw_sky();
    } else if (!shot_in_screen && shot_y >= 1) { // account for cases where shot drops back into screen
        shot_in_screen = true;
    }

    // after moving in x and y, check if it has gone out of bounds
    // only need to check if shot is at the bottom since it will never go in reverse.
    if (shot_y >= BLACK_BAR_MIN_Y - SHOT_SIZE) {
        printf("shot went out of bounds\n\r");
        return SHOT_OUT_OF_BOUNDS;
    }

    // apply gravity at a fixed interval
    gravity_count--;
    if (gravity_count <= 0) {
        shot_vy -= GRAVITY;
        if (shot_vy < -42) {
            shot_vy = -42; // terminal downward velocity is -42
        }
        gravity_count = GRAVITY_INTERVAL;
    }

    // only redraw if shot moved and it's in the screen
    if ((shot_moved_x || shot_moved_y) && shot_in_screen) {
        // X DRAW
        uint16_t draw_shot = 0;
        uint16_t draw_sea_sky = 0;
        // shot moved left
        if (shot_vx <= 0) {
            draw_shot = shot_x;
            draw_sea_sky = shot_x + SHOT_SIZE;
        }
        // shot moved right
        else if (shot_vx > 0) {
            draw_shot = shot_x + SHOT_SIZE - 1;
            draw_sea_sky = shot_x - 1;
        }

        // draw 1 pixel column of shot
        lcd_draw_rectangle(draw_shot, 1, shot_y, SHOT_SIZE, LCD_COLOR_BLACK);

        // draw 1 pixel column of sea/sky
        // shot is in sea, draw 1 pixel column of sea
        if (shot_y >= SEA_MIN_Y) {
            lcd_draw_rectangle(draw_sea_sky, 1, shot_y, SHOT_SIZE + 1, LCD_COLOR_BLUE);
        }
        // shot is in sky, draw 1 pixel column of sky
        else if (shot_y <= SEA_MIN_Y - SHOT_SIZE) {
            lcd_draw_rectangle(draw_sea_sky, 1, shot_y, SHOT_SIZE + 1, LCD_COLOR_CYAN);
        }
        // shot is in between sky and sea
        else {
            // draw sky pixels
            lcd_draw_rectangle(draw_sea_sky, 1, shot_y, SEA_MIN_Y - shot_y, LCD_COLOR_CYAN);
            // draw sea pixels
            lcd_draw_rectangle(draw_sea_sky, 1, SEA_MIN_Y, SEA_MIN_Y + SHOT_SIZE + 1 - shot_y, LCD_COLOR_BLUE);
        }

        // Y DRAW
        int16_t draw_shot_y = 0;
        int16_t draw_sea_sky_y = 0;
        uint16_t replace_color = 0;

        // shot is moving down
        if (shot_vy <= 0) {
            draw_shot_y = shot_y + SHOT_SIZE - 1; // -1 because we move INTO the square
            draw_sea_sky_y = shot_y - 1;
            replace_color = LCD_COLOR_CYAN; // sky color
        }
        // shot is moving up
        else if (shot_vy > 0) {
            draw_shot_y = shot_y;
            draw_sea_sky_y = shot_y + SHOT_SIZE;
            replace_color = LCD_COLOR_BLUE; // sea color
        }
        // draw shot row
        lcd_draw_rectangle(shot_x, SHOT_SIZE, draw_shot_y, 1, LCD_COLOR_BLACK);

        // draw 1 pixel column of sea/sky
        // shot is in sea, draw 1 pixel row of sea
        if (shot_y > SEA_MIN_Y) {
            lcd_draw_rectangle(shot_x - 1, SHOT_SIZE + 2, draw_sea_sky_y, 1, LCD_COLOR_BLUE);
        }
        // shot is in sky, draw 1 pixel column of sky
        else if (shot_y <= SEA_MIN_Y - SHOT_SIZE - 1) {
            lcd_draw_rectangle(shot_x - 1, SHOT_SIZE + 2, draw_sea_sky_y, 1, LCD_COLOR_CYAN);
        }
        // shot is in between sky and sea
        else {
            lcd_draw_rectangle(shot_x - 1, SHOT_SIZE + 2, draw_sea_sky_y, 1, replace_color);
        }
    }

    // default return
    return SHOT_IN_MY_SIDE;
}

/* Physics for moving the ENEMY's shot, boundary detection, and collision with ship detection
 * Copy of move_shot() but with variables replaced for enemy shot.
 */
shot_status move_enemy_shot() {
    static int8_t shot_count_x = 0;
    static int8_t shot_count_y = 0;
    static uint8_t gravity_count = 0;

    // whether the position of the shot changed
    bool shot_moved_x = false;
    bool shot_moved_y = false;

    // move shot in x direction
    if (enemy_shot_vx != 0) {
        shot_count_x--;
        // move the shot if the count is 0
        if (shot_count_x <= 0) {
            if (enemy_shot_vx > 0 && enemy_shot_x < SCREEN_X - SHOT_SIZE) {
                enemy_shot_x++;
            } else if (enemy_shot_vx < 0 && enemy_shot_x > 0) {
                enemy_shot_x--;
            }
            shot_count_x = 42 / abs_val(enemy_shot_vx);
            shot_moved_x = true;
        }
    }

    shot_count_y--;
    // move the shot if the count is 0
    if (shot_count_y <= 0) {
        // move shot in y direction if it had a downward velocity
        if (enemy_shot_vy != 0) {
            if (enemy_shot_vy > 0) {
                enemy_shot_y--;
            } else if (enemy_shot_vy < 0 && enemy_shot_y < SCREEN_Y - SHOT_SIZE) {
                enemy_shot_y++;
            }
            shot_moved_y = true;
            shot_count_y = 42 / abs_val(enemy_shot_vy);
        }
    }

    // prevent enemy_shot_y from experiencing underflow.
    if (enemy_shot_y <= CEILING) {
        enemy_shot_y = CEILING + 1;
        enemy_shot_vy = 0;
    }
    // after moving in y, check if it has gone above the ceiling.
    if (enemy_shot_in_screen && enemy_shot_y <= 1) {
        enemy_shot_in_screen = false;
        // redraw sky to erase shot
        draw_sky();
    }
    else if (!enemy_shot_in_screen && enemy_shot_y >= 1) { // account for cases where shot drops back into screen
        enemy_shot_in_screen = true;
    }

    // after moving in x and y, check if it has gone out of bounds
    // P1 check right wall, P2 check left wall
    if (enemy_shot_y >= BLACK_BAR_MIN_Y - SHOT_SIZE || (i_am_p1 && enemy_shot_x >= SCREEN_X - SHOT_SIZE) || (!i_am_p1 && enemy_shot_x <= 0)) {
        printf("enemy shot went out of bounds\n\r");
        return SHOT_OUT_OF_BOUNDS;
    }
    
    // check enemy shot collision with ship
    if (!(enemy_shot_x > (ship_x + shipTopWidthPixels/2)
            || (enemy_shot_x + SHOT_SIZE) < (ship_x - shipTopWidthPixels/2) 
            || enemy_shot_y < (ship_y - shipTopHeightPixels/2)
            || enemy_shot_y > (ship_y + shipTopHeightPixels/2 + shipBottomHeightPixels))) {
        return SHIP_COLLISION;
    }

    // apply gravity at a fixed interval
    gravity_count--;
    if (gravity_count == 0) {
        enemy_shot_vy -= GRAVITY;
        if (enemy_shot_vy < -42) {
            enemy_shot_vy = -42; // terminal downward velocity is -42
        }
        gravity_count = GRAVITY_INTERVAL;
    }

    // only redraw if shot moved and it's in the screen
    if ((shot_moved_x || shot_moved_y) && enemy_shot_in_screen) {
        // X DRAW
        uint16_t draw_shot = 0;
        uint16_t draw_sea_sky = 0;
        // shot moved left
        if (enemy_shot_vx <= 0) {
            draw_shot = enemy_shot_x;
            draw_sea_sky = enemy_shot_x + SHOT_SIZE;
        }
        // shot moved right
        else if (enemy_shot_vx > 0) {
            draw_shot = enemy_shot_x + SHOT_SIZE - 1;
            draw_sea_sky = enemy_shot_x - 1;
        }

        // draw 1 pixel column of shot
        lcd_draw_rectangle(draw_shot, 1, enemy_shot_y, SHOT_SIZE, LCD_COLOR_BLACK);

        // draw 1 pixel column of sea/sky
        // shot is in sea, draw 1 pixel column of sea
        if (enemy_shot_y >= SEA_MIN_Y) {
            lcd_draw_rectangle(draw_sea_sky, 1, enemy_shot_y, SHOT_SIZE + 1, LCD_COLOR_BLUE);
        }
        // shot is in sky, draw 1 pixel column of sky
        else if (enemy_shot_y <= SEA_MIN_Y - SHOT_SIZE) {
            lcd_draw_rectangle(draw_sea_sky, 1, enemy_shot_y, SHOT_SIZE + 1, LCD_COLOR_CYAN);
        }
        // shot is in between sky and sea
        else {
            // draw sky pixels
            lcd_draw_rectangle(draw_sea_sky, 1, enemy_shot_y, SEA_MIN_Y - enemy_shot_y, LCD_COLOR_CYAN);
            // draw sea pixels
            lcd_draw_rectangle(draw_sea_sky, 1, SEA_MIN_Y, SEA_MIN_Y + SHOT_SIZE + 1 - enemy_shot_y, LCD_COLOR_BLUE);
        }

        // Y DRAW
        int16_t draw_enemy_shot_y = 0;
        int16_t draw_sea_sky_y = 0;
        uint16_t replace_color = 0;

        // shot is moving down
        if (enemy_shot_vy <= 0) {
            draw_enemy_shot_y = enemy_shot_y + SHOT_SIZE - 1; // -1 because we move INTO the square
            draw_sea_sky_y = enemy_shot_y - 1;
            replace_color = LCD_COLOR_CYAN; // sky color
        }
        // shot is moving up
        else if (enemy_shot_vy > 0) {
            draw_enemy_shot_y = enemy_shot_y;
            draw_sea_sky_y = enemy_shot_y + SHOT_SIZE;
            replace_color = LCD_COLOR_BLUE; // sea color
        }
        // draw shot row
        lcd_draw_rectangle(enemy_shot_x, SHOT_SIZE, draw_enemy_shot_y, 1, LCD_COLOR_BLACK);

        // draw 1 pixel column of sea/sky
        // shot is in sea, draw 1 pixel row of sea
        if (enemy_shot_y > SEA_MIN_Y) {
            lcd_draw_rectangle(enemy_shot_x - 1, SHOT_SIZE + 2, draw_sea_sky_y, 1, LCD_COLOR_BLUE);
        }
        // shot is in sky, draw 1 pixel column of sky
        else if (enemy_shot_y <= SEA_MIN_Y - SHOT_SIZE - 1) {
            lcd_draw_rectangle(enemy_shot_x - 1, SHOT_SIZE + 2, draw_sea_sky_y, 1, LCD_COLOR_CYAN);
        }
        // shot is in between sky and sea
        else {
            lcd_draw_rectangle(enemy_shot_x - 1, SHOT_SIZE + 2, draw_sea_sky_y, 1, replace_color);
        }
    }

    // default return
    return SHOT_IN_MY_SIDE;
}

// Resets the position and velocity of the ship
// Redraws the sky, sea, ship
void restart_redraw() {
    // reset velocity and position
    ship_vx = 0;
    ship_vy = 0;
    ship_x = (SCREEN_X / 2) - 1; // middle of x
    ship_y = 159;                // 100 + (120/2) - 1. Middle of y coordinates of sea.

    draw_sky();
    draw_sea();
    // draw ship top and ship bottom
    draw_ship(ship_x, ship_y, i_am_p1); 

    // don't draw shots anymore
    shot_in_screen = false;
    enemy_shot_in_screen = false;
}

// turns the green LED off and the orange LED on to signify that the player's shot is in motion
// and that the player cannot move 
void green_off_orange_on() {
    // take semaphore to prevent contention with light sensor for I2C.
    xSemaphoreTake(Sem_I2c, portMAX_DELAY);
    io_expander_set_output_port(0x00); 	
    io_expander_led_red();
    // give semaphore back to allow light sensor to use I2C. 
    xSemaphoreGive(Sem_I2c);
}

// turns the green LED on and the orange LED off to signify that the player can move and shoot again
void green_on_orange_off() {
    // take semaphore to prevent contention with light sensor for I2C.
    xSemaphoreTake(Sem_I2c, portMAX_DELAY);
    io_expander_set_output_port(0x00); 	
    io_expander_led_grn();
    // give semaphore back to allow light sensor to use I2C. 
    xSemaphoreGive(Sem_I2c);
}


void task_state_machine(void *pvParameters)
{
    // current state
    project_state state = INIT_GAME;

    // status of readings from queues and semaphores
    BaseType_t joystick_status;
    BaseType_t imu_status;
    BaseType_t lightSensor_status;
    BaseType_t button1_status;
    BaseType_t uart_rx_status;
    uart_data_format uart_msg;

    // number of shots a player has taken in a round
    uint8_t eeprom_num_shots = 0;

    // take button1 semaphore to prevent pre-firing
    xSemaphoreTake(Sem_Button1, portMAX_DELAY); // portMAX_DELAY = forever

    //////////////////////////////////////////
    // accelerometer: ship movement control //
    //////////////////////////////////////////
    imu_acceleration_t accel;
    ship_vx = 0;
    ship_vy = 0;

    // starting position of the ship
    ship_x = (SCREEN_X / 2) - 1; // middle of x
    ship_y = 159;                // 100 + (120/2) - 1. Middle of y coordinates of sea.

    /////////////////////////////
    // joystick: angle control //
    /////////////////////////////
    joystick_position_t pos = JOYSTICK_POS_CENTER;
    uint8_t angle = 45;
    uint8_t angle_counter = 0; // used to slow down the rate at which we change the angle

    ////////////////////////////////////
    // light sensor: strength control //
    ////////////////////////////////////
    strength = 0;
    strength_past = 0;

    // initialize the step value based on max value from the light sensor
    uint16_t light_sensor_data = 0;
    lightSensor_status = xQueueReceive(Queue_LightSensor_Meas, &light_sensor_data, portMAX_DELAY);
    strength_step = light_sensor_data / 32;
    if (strength_step == 0) {
        strength_step = 1;
    }  

    // draw player 1 select screen
    lcd_select_player1();

    // draw player 1's ship on select screen
    draw_select_screen_ship();

    while(1) {
        // when in state where the game is played, check for messages from enemy and move his shot.
        if (state == TURN_MINE || state == SHOT_MOVING) {
            uart_rx_status = xQueueReceive(Queue_uartRX, &uart_msg, 0); // poll UART_RX queue
            if (uart_rx_status == pdPASS) {
                switch (uart_msg.start_byte) {
                    case YOUR_SHOT_OUT_OF_BOUNDS:
                        // now we can shoot again.
                        green_on_orange_off();
                        state = TURN_MINE;
                        break;
                    case SHOT_DATA:
                        // start moving the enemy shot
                        enemy_shot_in_my_side = true;
                        // copy over the velocity and position data
                        enemy_shot_vx = uart_msg.vx;
                        enemy_shot_vy = uart_msg.vy;
                        enemy_shot_y = uart_msg.y;
                        // set initial x based on P1 or P2
                        enemy_shot_x = (i_am_p1)? 0 : SCREEN_X - SHOT_SIZE - 1;
                        // shot is in the screen if its y is not higher than 1
                        enemy_shot_in_screen = !(enemy_shot_y <= 1);
                        break;
                    case SHIP_HIT:
                        printf("we hit his ship\n\r");
                        // draw victory splash screen because we hit his ship
                        draw_you_win();

                        // stop moving and drawing enemy's shot
                        enemy_shot_in_my_side = false;

                        // 1. read high score from EEPROM
                        xSemaphoreTake(Sem_Imu, portMAX_DELAY);
                        uint8_t high_score = eeprom_read_byte(HIGH_SCORE_ADDRESS);
                        xSemaphoreGive(Sem_Imu);
                        // 2. check if we got a better high score
                        if (eeprom_num_shots < high_score) {
                            printf("<<<my new high score: %d\n\r", eeprom_num_shots);
                            // let opponent know about new high score
                            uart_msg.start_byte = NEW_HIGH_SCORE;
                            uart_msg.high_score = eeprom_num_shots;
                            xQueueSend(Queue_uartTX, &uart_msg, portMAX_DELAY);

                            // write new high score to EEPROM
                            xSemaphoreTake(Sem_Imu, portMAX_DELAY);
                            eeprom_write_byte(HIGH_SCORE_ADDRESS, eeprom_num_shots);
                            xSemaphoreGive(Sem_Imu);

                            // draw the new high score
                            draw_high_score_number(eeprom_num_shots);
                        }else{ // if no better high socre 
                            uart_msg.start_byte = NO_NEW_HIGH_SCORE;
                            xQueueSend(Queue_uartTX, &uart_msg, portMAX_DELAY);
                        }


                        // game is over
                        state = GAME_OVER_STATE;
                        break;
                    default:
                        break;
                }
            }

            if (enemy_shot_in_my_side) {
                switch (move_enemy_shot()) {
                    case SHOT_OUT_OF_BOUNDS:
                        // send UART message to let enemy know he can shoot again.
                        uart_msg.start_byte = YOUR_SHOT_OUT_OF_BOUNDS;
                        xQueueSend(Queue_uartTX, &uart_msg, 5);
                        enemy_shot_in_my_side = false;
                        // redraw sky, sea, ship, shot if it's in my screen
                        draw_sky();
                        draw_sea();
                        draw_ship(ship_x, ship_y, i_am_p1);
                        break;
                    case SHIP_COLLISION:
                        // draw loss screen because our ship was hit
                        draw_you_lose();

                        // send UART message to let enemy know he won
                        uart_msg.start_byte = SHIP_HIT;
                        xQueueSend(Queue_uartTX, &uart_msg, portMAX_DELAY);
                        enemy_shot_in_my_side = false;

                        // wait 100ms for a new high score message from enemy
                        // Note: he won't send one if there isn't a new high score
                        uart_rx_status = xQueueReceive(Queue_uartRX, &uart_msg, portMAX_DELAY);
                        if (uart_rx_status == pdPASS && uart_msg.start_byte == NEW_HIGH_SCORE) {
                            // draw the new high score
                            draw_high_score_number(uart_msg.high_score);
                            printf("<<<ENEMY new high score: %d\n\r", uart_msg.high_score);
                            xSemaphoreTake(Sem_Imu, portMAX_DELAY);
                            eeprom_write_byte(HIGH_SCORE_ADDRESS, uart_msg.high_score);
                            xSemaphoreGive(Sem_Imu);

                        }
                        // game is over
                        state = GAME_OVER_STATE;
                        break;
                    default:
                        break;
                }
            }
        }

        switch (state) {
            case TURN_MINE:
                vTaskDelay(3); // slow down if there's an enemy shot
                // read from joystick queue
                joystick_status = xQueueReceive(Queue_Joystick_Position, &pos, 0); // 1 tick is 1ms
                if (joystick_status == pdPASS) {
                    // change angle if the joystick is in up or down position
                    switch (pos) {
                        case JOYSTICK_POS_UP:
                            angle_counter = (angle_counter + 1) % 10;
                            if (angle_counter == 0) {
                                if (angle < 90){
                                    angle++;
                                }
                                draw_angle_number(angle);
                            }
                            break;
                        case JOYSTICK_POS_DOWN:
                            angle_counter = (angle_counter + 1) % 10;
                            if (angle_counter == 0) {
                                if (angle > 0) {
                                    angle--;
                                }
                                draw_angle_number(angle);
                            }
                            break;
                        default:
                            break;
                    }
                }

                // move ship based on tilt of the board (from IMU)
                imu_status = xQueueReceive(Queue_Imu_Acceleration, &accel, 0);
                if (imu_status == pdPASS) {
                    update_velocities(accel);
                    move_ship();
                }

                // update strength based on light sensor
                lightSensor_status = xQueueReceive(Queue_LightSensor_Meas, &light_sensor_data, 0);
                if (lightSensor_status == pdPASS) {
                    update_strength(light_sensor_data);
                }

                // check to see if we should fire a shot
                button1_status = xSemaphoreTake(Sem_Button1, 0); // poll the button
                if (button1_status == pdTRUE) {
                    // initialize shot x velocity and position (depends on which player you are)
                    if (i_am_p1) {
                        shot_vx = (int8_t)(strength * cos((float)angle / 360 * 6.28)) * -1; // shoot left
                        shot_x = ship_x - shipTopWidthPixels / 2 - SHOT_SIZE;
                    } else {
                        shot_vx = (int8_t)(strength * cos((float)angle / 360 * 6.28)); // shoot right
                        shot_x = ship_x + shipTopWidthPixels / 2;
                    }
                    shot_vy = (int8_t)(strength * sin((float)angle / 360 * 6.28));
                    shot_y = ship_y - shipTopHeightPixels / 2 - SHOT_SIZE;

                    // draw shot
                    lcd_draw_rectangle(shot_x, SHOT_SIZE, shot_y, SHOT_SIZE, LCD_COLOR_BLACK);

                    // make sure shot gets drawn in move_shot()
                    shot_in_screen = true;
                    // make sure we move the shot
                    shot_in_my_side = true;

                    // increment the number of shots we have taken
                    if (eeprom_num_shots < MAX_HIGH_SCORE) {
                        eeprom_num_shots++;
                    }
                    // go to SHOT_MOVING state
                    green_off_orange_on(); // let player know he can't move
                    state = SHOT_MOVING;
                }
                break;
            case SHOT_MOVING:
                vTaskDelay(3);                                   // slow the movement of the shot down.
                button1_status = xSemaphoreTake(Sem_Button1, 0); // poll the button to prevent it from being used

                if (shot_in_my_side) {
                    switch (move_shot()) {
                        // shot hit bottom boundary
                        case SHOT_OUT_OF_BOUNDS:
                            // redraw sea
                            draw_sea();
                            // redraw ship
                            draw_ship(ship_x, ship_y, i_am_p1);
                            green_on_orange_off(); // let player know he can move
                            // go back to ship moving state
                            state = TURN_MINE;
                            break;
                        case SHOT_IN_ENEMY_SIDE:
                            // redraw sea, sky
                            draw_sea();
                            draw_sky();
                            // redraw ship
                            draw_ship(ship_x, ship_y, i_am_p1);
                            // send shot data over UART
                            uart_msg.start_byte = SHOT_DATA;
                            uart_msg.vx = shot_vx;
                            uart_msg.vy = shot_vy;
                            uart_msg.y = shot_y;
                            xQueueSend(Queue_uartTX, &uart_msg, 5);

                            // set bool so we don't keep trying to move our shot. VERY IMPORTANT
                            shot_in_my_side = false;
                            break;
                        // SHOT_IN_MY_SIDE, don't need to do anything.
                        default:
                            break;
                    }
                }
                break;
            case GAME_OVER_STATE:
                // check to see if button 1 was pressed to restart the game
                button1_status = xSemaphoreTake(Sem_Button1, 0); // poll the button
                if (button1_status == pdTRUE) {
                    // let the opponent's board know the game has restarted. 
                    uart_msg.start_byte = RESTART_GAME;
                    xQueueSend(Queue_uartTX, &uart_msg, 5);

                    // redraw assets and reset ship properties
                    restart_redraw();
                    // reset number of shots
                    eeprom_num_shots = 0;

                    green_on_orange_off(); // let player know he can move
                    // go back to game run state
                    state = TURN_MINE;
                }
                // check to see if opponent has restarted the game
                uart_rx_status = xQueueReceive(Queue_uartRX, &uart_msg, 0);
                if (uart_rx_status == pdPASS && uart_msg.start_byte == RESTART_GAME) {
                    // redraw assets and reset ship properties
                    restart_redraw();
                    // reset number of shots
                    eeprom_num_shots = 0;
                    green_on_orange_off(); // let player know he can move
                    // go back to game run state
                    state = TURN_MINE;
                }
                break;
            // init game state
            default: 
                // check rx queue to see if enemy has chosen to be player 1
                uart_rx_status = xQueueReceive(Queue_uartRX, &uart_msg, 0); // poll UART_RX queue
                if (uart_rx_status == pdPASS) {
                    if (uart_msg.start_byte == SELECT_P1) {
                        // opponent is P1
                        i_am_p1 = false;
                        // draw the sky, sea, black bar
                        draw_background();
                        // draw ship top and ship bottom
                        draw_ship(ship_x, ship_y, i_am_p1); 
                        // draw angle
                        draw_angle_text();
                        draw_angle_number(angle);
                        // draw strength
                        draw_strength_text();
                        draw_strength_color(LCD_COLOR_BLACK);
                        // draw high score
                        draw_highScore_text();
                        // read EEPROM to draw high score number
                        xSemaphoreTake(Sem_Imu, portMAX_DELAY);
                        uint8_t high_score = eeprom_read_byte(HIGH_SCORE_ADDRESS);
                        xSemaphoreGive(Sem_Imu);
                        draw_high_score_number(high_score);

                        green_on_orange_off(); // let player know he can move
                        // go to game running state
                        state = TURN_MINE;
                    }
                }
                // check if button 1 has been pressed to select ourselves as player 1
                button1_status = xSemaphoreTake(Sem_Button1, 0); // poll the button
                if (button1_status == pdTRUE) {
                    // I am P1 because I pressed button 1
                    i_am_p1 = true;
                    // send uart msg to let enemy know we are player 1
                    uart_msg.start_byte = SELECT_P1;
                    xQueueSend(Queue_uartTX, &uart_msg, 5);
                    
                    // draw the sky, sea, black bar
                    draw_background();
                    // draw ship top and ship bottom
                    draw_ship(ship_x, ship_y, i_am_p1); 
                    // draw angle
                    draw_angle_text();
                    draw_angle_number(angle);
                    // draw strength
                    draw_strength_text();
                    draw_strength_color(LCD_COLOR_BLACK);
                    // draw high score
                    draw_highScore_text();
                    // read EEPROM to draw high score number
                    xSemaphoreTake(Sem_Imu, portMAX_DELAY);
                    uint8_t high_score = eeprom_read_byte(HIGH_SCORE_ADDRESS);
                    xSemaphoreGive(Sem_Imu);
                    draw_high_score_number(high_score);

                    green_on_orange_off(); // let player know he can move
                    // go to game running state
                    state = TURN_MINE;
                }
                break;
        }
    }
}
