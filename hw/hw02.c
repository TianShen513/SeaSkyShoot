/**
 * @file hw02.c
 * @author Tianqi Shen (tshen46@wisc.edu) & Bowen Quan (bquan2@wisc.edu)
 * @brief
 * @version 0.1
 * @date 2023-09-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "main.h"
#include "hw02.h"
#include "hw02-lcd-staff.h"
#include "hw02-images.h"

#define TICKS_MS_100 10000000 // 0.1s * 100MHz

/*****************************************************************************/
/*  Global Variables                                                         */
/*****************************************************************************/
char HW02_DESCRIPTION[] = "ECE353: HW02 Tianqi Shen, Bowen Quan";

// HW02B: states of the game
typedef enum {
    P1_SELECT,
    P1_LETTER_SELECT,
    MY_TURN,
    GAME_OVER,
    OPPONENT_TURN
} state_t;

// alerts for when button 1 or 2 is pressed
volatile events_t HW02_ALERT_SW1 = EVENT_NONE;
volatile events_t HW02_ALERT_SW2 = EVENT_NONE;
volatile events_t HW02_ALERT_100MS = EVENT_NONE;

// possible statuses of a square in the board
typedef enum {
    EMPTY,
    PLAYER_X,
    PLAYER_O
} cell_t;

// playing board. Records where the players' markers have been placed
cell_t board[3][3] = { {EMPTY, EMPTY, EMPTY}, 
                       {EMPTY, EMPTY, EMPTY}, 
                       {EMPTY, EMPTY, EMPTY} };

/* Timer object used */
cyhal_timer_t timer_obj_hw02;
cyhal_timer_cfg_t timer_cfg_hw02;

/*****************************************************************************/
/*  Interrupt Handlers                                                       */
/*****************************************************************************/

// Function that's executed whenever there's an interrupt.
void Handler_HW02_Timer(void *handler_arg, cyhal_timer_event_t event) {
    CY_UNUSED_PARAMETER(handler_arg);
    CY_UNUSED_PARAMETER(event);
    
    // static vars used to make the buttons trigger on release
    static bool sw1_pressed = false;
    static bool sw2_pressed = false;

    // get the current state of push button
    uint32_t reg_val = PORT_BUTTONS->IN;

    // check if SW1 has been pressed (active low, will be 0)
    if ((reg_val & SW1_MASK) == 0) {
        sw1_pressed = true;
    } else if (sw1_pressed) { // only set the alert when the button is released
        HW02_ALERT_SW1 = EVENT_SW1;
        sw1_pressed = false;        
    }
    // check if SW2 has been pressed (active low, will be 0)
    if ((reg_val & SW2_MASK) == 0) {
        sw2_pressed = true;
    } else if (sw2_pressed) { // only set the alert when the button is released
        HW02_ALERT_SW2 = EVENT_SW2;
        sw2_pressed = false;        
    }
    // generate an alert for the joystick every 100ms
    HW02_ALERT_100MS = EVENT_100MS;
}

// Initializes the timer
void timer_init(cyhal_timer_t *timer_obj, cyhal_timer_cfg_t *timer_cfg, uint32_t ticks, void *Handler)
{
    cy_rslt_t rslt;

    timer_cfg->compare_value = 0;
    timer_cfg->period = ticks - 1;
    timer_cfg->direction = CYHAL_TIMER_DIR_UP;
    timer_cfg->is_compare = false;
    timer_cfg->is_continuous = true;
    timer_cfg->value = 0;

    /* Initialize a timer */
    rslt = cyhal_timer_init(timer_obj, NC, NULL);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // If the initialization fails, halt the MCU

    /* Apply timer configuration such as period, count direction, run mode, etc. */
    rslt = cyhal_timer_configure(timer_obj, timer_cfg);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // If the initialization fails, halt the MCU

    /* Set the frequency of timer*/
    rslt = cyhal_timer_set_frequency(timer_obj, 100000000); // 100MHz
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // If the initialization fails, halt the MCU

    /* Assign the ISR to execute on timer interrupt */
    cyhal_timer_register_callback(timer_obj, Handler, NULL);

    /* Set the event on which timer interrupt occurs and enable it */
    cyhal_timer_enable_event(timer_obj, CYHAL_TIMER_IRQ_TERMINAL_COUNT, 3, true);

    /* Start the timer with the configured settings */
    rslt = cyhal_timer_start(timer_obj);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS); // If the initialization fails, halt the MCU   
}

/*****************************************************************************/
/*  HW02 Functions                                                           */
/*****************************************************************************/

/* Checks if PLAYER_X or PLAYER_O has won the game by getting 3 of their markers in the same row, column, or diagonal
 */
cell_t check_winner(cell_t board[3][3]) {
    int i, j;

    // Check rows
    for (i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != EMPTY) {
            return board[i][0];
        }
    }

    // Check columns
    for (j = 0; j < 3; j++) {
        if (board[0][j] == board[1][j] && board[1][j] == board[2][j] && board[0][j] != EMPTY) {
            return board[0][j];
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY) {
        return board[0][0];
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY) {
        return board[0][2];
    }

    return EMPTY;  // No winner yet
}

/* Returns true if the board is full (no EMPTY squares), false otherwise.
 */
bool is_board_filled(cell_t board[3][3]) {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                return false; // Found an empty cell, board isn't full yet
            }
        }
    }
    return true; // No empty cells found, board is full
}

/* Draws a square based on whether it is currently selected and if it has been claimed or not
 * @param row specifies row of square. Ranges from 0 to 2
 * @param col specifies column of square. Ranges from 0 to 2
 * @param player_turn whose turn it currently is. Either PLAYER_X or PLAYER_O
 * @param selected if the square is currently selected
 */
void draw_square(int row, int col, cell_t player_turn, bool selected) {
    // get the player at the input square in the board
    cell_t player_in_board = board[row][col];
    // check if the board is claimed at the input square location
    bool claimed = (player_in_board != EMPTY);

    // get the vertical position at which to draw
    int y = 0;
    switch (row) {
        case 0: y = UPPER_ROW; break;
        case 1: y = CENTER_ROW; break;
        case 2: y = LOWER_ROW; break;
        default: return;
    }
    // get the horizontal position at which to draw 
    int x = 0;
    switch (col) {
        case 0: x = LEFT_COL; break;
        case 1: x = CENTER_COL; break;
        case 2: x = RIGHT_COL; break;
        default: return;
    }

    // unclaimed AND unselected means draw a black square.
    if (!claimed && !selected) {
        lcd_draw_rectangle_centered(x, y, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_BLACK);
    }
    
    // set foreground and background color based on whether the square is selected and claimed
    // (to visualize, draw 2 two parameter K-maps)
    uint16_t foreground_color = (selected || !claimed) ? LCD_COLOR_BLACK: 
                                (player_in_board == PLAYER_X)? LCD_COLOR_YELLOW:
                                LCD_COLOR_CYAN;
    uint16_t background_color = (!selected) ? LCD_COLOR_BLACK: 
                                claimed ? LCD_COLOR_RED: 
                                LCD_COLOR_GREEN;
    
    // draw background square
    lcd_draw_rectangle_centered(x, y, SQUARE_SIZE, SQUARE_SIZE, background_color);
    // draw player in the foreground
    // if the square is claimed, draw the player whose marker is in the board.
    // if the square isn't claimed, draw the player whose turn it is.
    if(player_in_board == PLAYER_X || (!claimed && player_turn == PLAYER_X)) {
        lcd_draw_image(x, y, X_WIDTH, X_HEIGHT, Bitmaps_X, foreground_color, background_color);
    } else if(player_in_board == PLAYER_O || (!claimed && player_turn == PLAYER_O)) {
        lcd_draw_image(x, y, O_WIDTH, O_HEIGHT, Bitmaps_O, foreground_color, background_color);
    } 
}

/* Sets all squares in the board to EMPTY and redraws an empty board.
 */
void clear_board() {
    // set all squares in the board to EMPTY
    memset(board, EMPTY, sizeof(board)); 
    // redraw an empty board
    lcd_clear_screen(LCD_COLOR_BLACK);
    lcd_draw_rectangle_centered(SCREEN_CENTER_COL, UPPER_HORIZONTAL_LINE_Y, LINE_LENGTH, LINE_WIDTH, LCD_COLOR_BLUE);
    lcd_draw_rectangle_centered(SCREEN_CENTER_COL, LOWER_HORIZONTAL_LINE_Y, LINE_LENGTH, LINE_WIDTH, LCD_COLOR_BLUE);
    lcd_draw_rectangle_centered(LEFT_VERTICAL_LINE_X, SCREEN_CENTER_ROW + 20, LINE_WIDTH, LINE_LENGTH, LCD_COLOR_BLUE);
    lcd_draw_rectangle_centered(RIGHT_VERTICAL_LINE_X, SCREEN_CENTER_ROW + 20, LINE_WIDTH, LINE_LENGTH, LCD_COLOR_BLUE);
}

/* Redraws the current board with its Xs and Os.
 * Does NOT draw the current square as selected. 
 */
void redraw_board(){
    // redraw an empty board
    lcd_clear_screen(LCD_COLOR_BLACK);
    lcd_draw_rectangle_centered(SCREEN_CENTER_COL, UPPER_HORIZONTAL_LINE_Y, LINE_LENGTH, LINE_WIDTH, LCD_COLOR_BLUE);
    lcd_draw_rectangle_centered(SCREEN_CENTER_COL, LOWER_HORIZONTAL_LINE_Y, LINE_LENGTH, LINE_WIDTH, LCD_COLOR_BLUE);
    lcd_draw_rectangle_centered(LEFT_VERTICAL_LINE_X, SCREEN_CENTER_ROW + 20, LINE_WIDTH, LINE_LENGTH, LCD_COLOR_BLUE);
    lcd_draw_rectangle_centered(RIGHT_VERTICAL_LINE_X, SCREEN_CENTER_ROW + 20, LINE_WIDTH, LINE_LENGTH, LCD_COLOR_BLUE);

    // draw all the Xs and Os in the board
    for (int row = 0; row < 3;row ++) {
        for (int col = 0; col < 3;col ++) {
            draw_square(row, col, board[row][col], false);
        }
    }
}

/**
 * @brief
 * Initializes the PSoC6 Peripherals used for HW01
 */
void hw02_peripheral_init(void)
{
    /* Initialize the pushbuttons */
    push_buttons_init();

    /* Initialize the LCD */
    ece353_enable_lcd();

    /* Initialize the joystick*/
    joystick_init();

    /* Initialize the remote UART */
    remote_uart_init();

    /* Enable interrupts and circular buffers from remote board */ 
    remote_uart_enable_interrupts();

    /* Initialize Timer to generate interrupts every 100mS*/
    timer_init(&timer_obj_hw02, &timer_cfg_hw02, TICKS_MS_100, Handler_HW02_Timer);
}

/* Returns a char from the UART_rx line if there is one.
 * If there isn't, it returns the default char 0x80 (chosen because it wasn't used by anything)
 */
uint8_t get_uart_rx_byte() {
    char remote_rx_message[1] = {0x80}; // default char if there is no message
    if (ALERT_UART_RX) {
        ALERT_UART_RX = false; // set alert back to false
        // try to read a 1 byte message
        remote_uart_rx_data_async(remote_rx_message, 1);
    } 
    return remote_rx_message[0];
}

/* Sends a char to the UART_tx line
 */
void send_uart_tx_byte(uint8_t c) {
    char remote_tx_message[3] = {c, '\n', 0};
    // Send the String to the remote board
    remote_uart_tx_data_async(remote_tx_message);
}

/**
 * @brief
 * Implements the main application for HW02
 */
void hw02_main_app(void)
{
    joystick_position_t position = JOYSTICK_POS_CENTER; 
    joystick_position_t past_position = JOYSTICK_POS_CENTER; 
    
    // variables to hold current and previous position selected
    int current_row = 1;
    int current_col = 1; 
    int prev_row = 1;
    int prev_col = 1;

    // state variable
    state_t state = P1_SELECT; // start off by selecting a player.

    bool i_am_player1 = false; // used to keep track of who has the 1st turn
    cell_t my_letter = EMPTY; // letter of this player. NEVER changes, even upon reset
    cell_t opponent_letter = EMPTY; // used to draw opponent's selections
    cell_t p1_letter_selection = PLAYER_X; // letter selection in P1_LETTER_SELECT always begins with X
    cell_t winner = EMPTY; // winner of the match 
    uint8_t message; // used to store char received from UART_rx
    
    // draw Select Player 1 screen
    lcd_select_player1();

    while(1)
    {  
        // HW02B: switch statement for the state machine
        // we are using a state machine because the buttons have different functionalities based on
        // the state of the game.
        switch (state) {
            // P1 selects his letter while P2 waits 
            case P1_LETTER_SELECT:
                // select letter if we are P1, otherwise wait for P1's selection
                if (i_am_player1) {
                    // button 1: toggles letter
                    if (HW02_ALERT_SW1 == EVENT_SW1) {
                        HW02_ALERT_SW1 = EVENT_NONE; // clear the alert
                        // swap letter selection
                        if (p1_letter_selection == PLAYER_X) {
                            p1_letter_selection = PLAYER_O;
                        } else  {
                            p1_letter_selection = PLAYER_X;
                        }
                        // redraw center square 
                        draw_square(current_row, current_col, p1_letter_selection, true);
                    }

                    // button 2: selects letter
                    if (HW02_ALERT_SW2 == EVENT_SW2) {
                        HW02_ALERT_SW2 = EVENT_NONE; // clear the alert
                        // set my_letter to my selection
                        my_letter = p1_letter_selection;
                        // send my letter to UART
                        // and set opponent's letter
                        if (my_letter == PLAYER_X) {
                            send_uart_tx_byte(MESSAGE_X);
                            opponent_letter = PLAYER_O;
                        } else {
                            send_uart_tx_byte(MESSAGE_O);
                            opponent_letter = PLAYER_X;
                        }
                        // wait indefinitely for an acknowledge
                        while (get_uart_rx_byte() != MESSAGE_ACK) {}
                        // transition to MY_TURN state
                        state = MY_TURN;
                    }     
                } else { // I am not P1
                    // check for P1's letter selection message
                    message = get_uart_rx_byte();
                    if (message == MESSAGE_O || message == MESSAGE_X) {
                        // set my letter to be opposite of opponent's
                        if (message == MESSAGE_O) {
                            my_letter = PLAYER_X;
                            opponent_letter = PLAYER_O;
                        } else if (message == MESSAGE_X) {
                            my_letter = PLAYER_O;
                            opponent_letter = PLAYER_X;
                        }

                        // send ACK byte
                        send_uart_tx_byte(MESSAGE_ACK);
                        // transition to OPPONENT_TURN state
                        state = OPPONENT_TURN;
                    }

                    // ignore button presses if we aren't selecting a letter
                    if (HW02_ALERT_SW1 == EVENT_SW1) {
                        HW02_ALERT_SW1 = EVENT_NONE;
                    }
                    if (HW02_ALERT_SW2 == EVENT_SW2) {
                        HW02_ALERT_SW2 = EVENT_NONE;
                    }
                }
                break;
            
            // A player has won or it's a tie
            case GAME_OVER:
                // Button 2: restarts the game and I have the first turn
                if (HW02_ALERT_SW2 == EVENT_SW2) {
                    HW02_ALERT_SW2 = EVENT_NONE; // clear the alert

                    send_uart_tx_byte(MESSAGE_RESTART);
                    // wait indefinitely for an acknowledge
                    while (get_uart_rx_byte() != MESSAGE_ACK) {}
                    // reset winner
                    winner = EMPTY;

                    // clear the board and redraw it
                    clear_board();
                    // it's my turn first, so draw the center square as selected with my letter
                    current_row = 1;
                    current_col = 1;
                    draw_square(current_row, current_col, my_letter, true);
                    state = MY_TURN;
                }  

                // opponent pressed SW2, so it's his turn
                if(get_uart_rx_byte() == MESSAGE_RESTART) {
                    send_uart_tx_byte(MESSAGE_ACK);

                    // reset winner
                    winner = EMPTY;
                    // clear the board and redraw it
                    clear_board();
                    // My opponent starts first, so I wait
                    lcd_wait_for_other_player();
                    state = OPPONENT_TURN;
                } 

                // ignore button 1 presses when game is over
                if (HW02_ALERT_SW1 == EVENT_SW1) {
                    HW02_ALERT_SW1 = EVENT_NONE;
                }
                break;

            case MY_TURN:
                // Joystick 100ms alert
                if (HW02_ALERT_100MS == EVENT_100MS) {
                    HW02_ALERT_100MS = EVENT_NONE; // clear the alert
                    // set past and current position of the joystick
                    past_position = position;
                    position = joystick_get_pos();

                    // only allow the player to move if the joystick has been released since its last move
                    if (past_position == JOYSTICK_POS_CENTER && position != JOYSTICK_POS_CENTER) {
                        // check for changes in the column (left or right movement)
                        if (position == JOYSTICK_POS_LEFT) {
                            current_col = (current_col + 3 - 1) % 3; // add 3 to prevent underflow
                        } else if (position == JOYSTICK_POS_RIGHT) {
                            current_col = (current_col + 1) % 3;
                        }
                        // check for changes in the row (up or down movement)
                        if (position == JOYSTICK_POS_UP) {
                            current_row = (current_row + 3 - 1) % 3; // add 3 to prevent underflow
                        } else if (position == JOYSTICK_POS_DOWN) {
                            current_row = (current_row + 1) % 3;
                        }
                    }

                    // only draw the current and previous square if the joystick has moved
                    if (prev_row != current_row || prev_col != current_col) {
                        // current square is selected
                        draw_square(current_row, current_col, my_letter, true);
                        // previous square is not currently selected by us since it's different from current square
                        draw_square(prev_row, prev_col, my_letter, false);

                        // update previous row and column variables
                        prev_row = current_row;
                        prev_col = current_col;
                    }
                }


                // Button 1: claims square for player
                if (HW02_ALERT_SW1 == EVENT_SW1) {
                    HW02_ALERT_SW1 = EVENT_NONE; // clear the alert
                    // squares can only be claimed if they are EMPTY
                    if (board[current_row][current_col] == EMPTY) {
                        // claim it for the current player
                        board[current_row][current_col] = my_letter;
                        // draw the square to show that the current player has claimed it
                        draw_square(current_row, current_col, my_letter, true);

                        // send which square we have claimed to opponent 
                        uint8_t send_message = (current_row << 4) | current_col;
                        send_uart_tx_byte(send_message | 0x40);
            
                        // Wait indefinitely for an ACK from the opponent's board
                        while (get_uart_rx_byte() != MESSAGE_ACK) {}

                        // check if my selection ended the game (a player has won or it's a draw)
                        winner = check_winner(board);
                        if (winner == PLAYER_X) {
                            lcd_X_wins();
                            state = GAME_OVER;
                        } else if (winner == PLAYER_O) {
                            lcd_O_wins();
                            state = GAME_OVER;
                        } else if (is_board_filled(board)) {
                            lcd_tie();
                            state = GAME_OVER;
                        } else {
                            // no winner and board isn't filled, so the game continues and we go to OPPONENT_TURN
                            lcd_wait_for_other_player();
                            state = OPPONENT_TURN;
                        }
                    }
                }

                // ignore button 2 presses while in this state
                if (HW02_ALERT_SW2 == EVENT_SW2) {
                    HW02_ALERT_SW2 = EVENT_NONE;
                }
                break;

            case OPPONENT_TURN: 
                // wait for opponent's message of selected square 
                message = get_uart_rx_byte();
                // check if there was actually a message
                if (message != 0x80) {
                    // derive row and col from message
                    uint8_t temp_row = (message >> 4) & (0xB); // row is upper 4 bits
                    uint8_t temp_col = message & 0x0F; // col is bottom 4 bits
                    // check if row and col are within [0,2]
                    if (temp_row <= 2 && temp_row >= 0 && temp_col <= 2 && temp_col >= 0) {
                        // update current row and col
                        current_row = temp_row;
                        current_col = temp_col;
                        // claim opponent's selection on the board (MUST be done before drawing)
                        board[current_row][current_col] = opponent_letter;
                        // redraw the board to clear the waiting scene
                        redraw_board();
                        // draw the opponent's selection
                        draw_square(current_row, current_col, opponent_letter, true);
                        // send ACK byte
                        send_uart_tx_byte(MESSAGE_ACK);
                        // check if opponent's selection has ended the game (a player has won or it's a draw)
                        winner = check_winner(board);
                        if (winner == PLAYER_X) {
                            lcd_X_wins();
                            state = GAME_OVER;
                        } else if (winner == PLAYER_O) {
                            lcd_O_wins();
                            state = GAME_OVER;
                        } else if (is_board_filled(board)) {
                            lcd_tie();
                            state = GAME_OVER;
                        } else {
                            // game isn't over, so transition to MY_TURN
                            state = MY_TURN;
                        }
                    }
                }

                // ignore button presses when we are waiting
                if (HW02_ALERT_SW1 == EVENT_SW1) {
                    HW02_ALERT_SW1 = EVENT_NONE;
                }
                if (HW02_ALERT_SW2 == EVENT_SW2) {
                    HW02_ALERT_SW2 = EVENT_NONE;
                }
                break;

            // default is P1_SELECT
            // in this state, one player presses SW2 to be selected as PLAYER 1
            default:
                // check UART_rx for messages
                if (get_uart_rx_byte() == MESSAGE_PLAYER1_SELECTION) {
                    // opponent is player 1
                    i_am_player1 = false;
                    // send ACK byte
                    send_uart_tx_byte(MESSAGE_ACK);

                    // transition to P1_LETTER_SELECT state
                    state = P1_LETTER_SELECT;
                    // draw empty board
                    clear_board();
                    // draw Waiting For Player 1 screen
                    lcd_wait_for_other_player();
                }

                // button 2 was pressed 
                if (HW02_ALERT_SW2 == EVENT_SW2) {
                    HW02_ALERT_SW2 = EVENT_NONE; // clear the alert
                    // set myself as player 1
                    i_am_player1 = true;
                    // send PLAYER1_SELECTION byte on UART
                    send_uart_tx_byte(MESSAGE_PLAYER1_SELECTION);
                    // wait indefinitely for an acknowledge
                    while (get_uart_rx_byte() != MESSAGE_ACK) {}

                    // transition to P1_LETTER_SELECT state
                    state = P1_LETTER_SELECT;
                    // draw empty board
                    clear_board();
                    // draw center square (since I am selecting)
                    draw_square(current_row, current_col, p1_letter_selection, true);
                }
                
                // ignore button 1 presses when we are selecting player 1
                if (HW02_ALERT_SW1 == EVENT_SW1) {
                    HW02_ALERT_SW1 = EVENT_NONE;
                }
                break;
        }

    }

    
    
}