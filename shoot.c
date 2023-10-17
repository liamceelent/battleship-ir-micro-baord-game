/** @file   shoot.c
    @author liam ceelen-thomas lce26
    @author charlie porter cpo57
    @date   10 OCT 2023
    @brief  Set up and shoot target
*/

#include <tinygl.h>
#include <shoot.h>
#include "navswitch.h"
#include "pacer.h"
#include "system.h"
#include <shots_taken_board.h>

// defining constants to be used in the baord
#define EMPTY 0
#define OCCUPIED 1 
#define GRID_COLS 5
#define GRID_ROWS 7

/**
    the active shhot function taking the users board of all shots they have already taken
    @return returns a unique location on the 2d array
    @param a inital 2d array filled with other previous shots
*/

int8_t shoot(int8_t array[GRID_COLS][GRID_ROWS]) 
{   
    //intialising the shot to the top left of the board
    Coordinate coordinate;
    coordinate.col = 0;
    coordinate.row = 0;
    //main loop of selecting where on the led the shoot is
    while(1) {
        navswitch_update();
        pacer_wait();
        if (navswitch_push_event_p(NAVSWITCH_NORTH))
        {   
            if (checkPos(coordinate, NORTH)) {
                moveTarget(&coordinate, NORTH);
            }
        } else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            if (checkPos(coordinate, SOUTH)) {
                moveTarget(&coordinate, SOUTH);
            }
        } else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
            if (checkPos(coordinate, EAST)) {
                moveTarget(&coordinate, EAST);
            }

        } else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
            if (checkPos(coordinate, WEST)) {
                moveTarget(&coordinate, WEST);
            }
        } else if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            return (coordinate.col * 7) + coordinate.row; // getting a unique number for eahc element in the 2d array
        }
        tinygl_clear();
        drawGrid(array);
        tinygl_pixel_set(tinygl_point (coordinate.col, coordinate.row), 1); // seting the target pixel location on
        tinygl_update();
    }
}

/**
    checks weather the move is allowed or is off the board
    @return returns a true or false of weather the move is allowed on the baord
    @param  typedef of cooridate to know where abouts the shot it at
    @param  enum of direction to which the target wants to move to
*/

bool checkPos(Coordinate coordinate, Direction direction)
{
    // different cases on which differnt actions are taken
    switch(direction) {
        case NORTH:
            coordinate.row -= 1;
            if (coordinate.row >= 0) {
                return true;
            }
            break;
        case SOUTH:
            coordinate.row  += 1;
            if (coordinate.row <= 6) {
                return true;
            }
            break;
        case EAST:
            coordinate.col += 1;
            if (coordinate.col <= 4) {
                return true;
            }
            break;
        case WEST:
            coordinate.col-= 1;
            if (coordinate.col >=0 ) {
                return true;
            }
            break;
    }
    return false;
}

/**
    actually move the target to the desired loaction
    @param  pointer typedef of cooridate to know where abouts the shot it at
    @param  enum of direction to which the target wants to move to
*/
void moveTarget(Coordinate *coordinate, Direction direction) {
    switch(direction) {
        case NORTH:
            coordinate->row -= 1;
            break;
        case SOUTH:
            coordinate->row  += 1;
            break;
        case EAST:
            coordinate->col += 1;
            break;
        case WEST:
            coordinate->col-= 1;
            break;
    }
}
