/** @file   game.c
    @author liam Ceelen-Thomas, lce26
    @author charlie porter cpo57
    @date   17 October 2023
    @brief  playing the functional game
*/

#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "button.h"
#include "../fonts/font5x7_1.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "shots_taken_board.h"
#include "setUpShips.h"
#include <math.h>

// our pacer rate to set how fast out timers go
#define PACER_RATE 500
#define MESSAGE_RATE 10 // how fast to display text across screen
#define MAX_HITS 10 // amount of hits needded to win the game
#define CHANGE 1
#define ATK 1
#define DEF 0
#define GRID_ROWS 7 // amount of rows in the array
#define GRID_COLS 5 // amount of colums in the array


// our array storing both prevois shots and placment of ships
int8_t myShips[GRID_COLS][GRID_ROWS] = {{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}};
int8_t shotsTaken[GRID_COLS][GRID_ROWS] = {{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}};

/**
    setting up the visuals for text meassages 
*/
void displaySetup(void)
{
    tinygl_init(PACER_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
}

/**
    the attacking turn for the game function
    @return true or false wheather or not you hit a ship 

*/
bool attackingTurn(void){
    int8_t shootPos = shoot(shotsTaken); 
    int8_t col = floor(shootPos / 7); //getting the correct column and row
    int8_t row = (shootPos % 7);
    char checkHit; 
    shootPos = shootPos + '0'; // coneverting to ascii value
    ir_uart_putc(shootPos);
    // reciving back either a hit or miss char (H or M)
    while(1){
        if (ir_uart_read_ready_p()) {
            checkHit = ir_uart_getc();
            break;
        }
    }
    // checking under what circumstance to update baord
    if(checkHit == 'H'){
        shotsTaken[col][row] = 1;
        PORTC |= (1<<2); // lighting up blue led to indicate hit
        return true;
    }
    if(checkHit == 'M'){
        shotsTaken[col][row] = 0; // could get rid of <
        PORTC &= ~(1<<2); // making sure the led isnt on
        return false;
    }
    return false;
}   

/**
    the defending turn for the fucntion
    @return true or false wheather or not they hit a ship 
*/

bool defendingTurn(void){
    while(1){
        //intalising values
        int8_t enemyShot;
        bool hadTurn = false;
        pacer_wait();

        if (ir_uart_read_ready_p()) {
            enemyShot = ir_uart_getc();
            enemyShot -= '0'; //converting ascii back into int
            hadTurn = true;
        }
        // if other player has taken shot and we recived position
        if (hadTurn == true){
            int8_t col = floor(enemyShot / 7);
            int8_t row = enemyShot % 7;
            //sending either a hit or miss upon seeing placment of our board
            if (myShips[col][row]){
                ir_uart_putc('H');
                return true;
            } else{
                ir_uart_putc('M');
                return false;
            }
        }
        //drawing the past hit shots onto led board
        drawGrid(shotsTaken);
        tinygl_update();
    }
}

/**
 getting initial turns of each player
 @return true or false wheather or not your player 1(true), player2(false)
*/
bool turnSetup(void) {
    // if the player is the first one to complete the placment of ships it starts
    if (!ir_uart_read_ready_p()) {
        ir_uart_putc('1');
        tinygl_text("STAND BY");
        while(ir_uart_read_ready_p() == 0) {
            pacer_wait();
            tinygl_update();
        }
        ir_uart_getc();
        return true; // player starts attacking
    } else {
        while(ir_uart_read_ready_p() == 0) {
            continue;
        }
        ir_uart_getc();
        ir_uart_putc('0');
        return false; // player starts defending
    }
}



/**
 @param myshisp  is the array for where your ships are
*/
void placeMyShips(int8_t myShips[GRID_ROWS][GRID_COLS]) {
    while(1)
    {
        pacer_wait();
        navswitch_update();
        button_update();
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            bool completed = placeShips(myShips);
            if(completed) {
                break;
            }
        }
        tinygl_update();
    }
}

/**
 Game playing loop
*/

bool gamePlay(int8_t player)
{
    int8_t ourHits = 0; // counting both hits to check if they are less than max hits
    int8_t theirHits = 0;
    DDRD |= (1<<7); // initalising the blue led
    while(ourHits != MAX_HITS && theirHits != MAX_HITS) { 
        pacer_wait();
        // if atcking first
        if (player == ATK) {
            if (attackingTurn()) {
                ourHits += 1;
            }
            player = DEF; // change to defending
        } else if(player == DEF) { 
            if(defendingTurn()) {
                theirHits += 1;
            }
            player = ATK; // chnage to attacking
        }
        tinygl_clear();
    }
    return ourHits == MAX_HITS; // returns true if won else false
}

/**
 Winner Display 
*/
void display(bool winner) 
{   
    while (1) {
        pacer_wait();  
        if (winner) {
            tinygl_text("W");
        } else {
            tinygl_text("L");
        }
        tinygl_update();  
    }
}     

/**
 the main game loop
*/
int main (void)
{
    // initialising starting information
    system_init();
    displaySetup();
    navswitch_init();
    pacer_init(PACER_RATE);
    navswitch_update();
    ir_uart_init ();
    button_init();
    tinygl_text("BattleShips");
    placeMyShips(myShips);
    int8_t player;

    if (turnSetup()){
        player = ATK; //setting player to attack first
    } else {
        player = DEF; //setting player to defend first
    }

    bool winner = gamePlay(player); // running main game loop returns true if winner

    display(winner);

    return 0;
}