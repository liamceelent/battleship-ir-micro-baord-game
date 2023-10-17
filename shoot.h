/** @file   shoot.h
    @author liam ceelen-thomas lce26
    @author charlie porter cpo57
    @date   10 OCT 2023
    @brief  Set up and shoot target
*/

#ifndef SHOOT_H
#define SHOOT_H

#include <tinygl.h>
#include "pacer.h"
#include "navswitch.h"
#include "system.h"
#include <shots_taken_board.h>


#define EMPTY 0
#define OCCUPIED 1 
#define GRID_COLS 5
#define GRID_ROWS 7

//enum to know the direction moved
typedef enum {
    NORTH, SOUTH, EAST, WEST
} Direction;

// a typedef to know exzact posistion of target
typedef struct {
    int8_t col;
    int8_t row;
} Coordinate;

/**
    the active shhot function taking the users board of all shots they have already taken
    @return returns a unique location on the 2d array
    @param a inital 2d array filled with other previous shots
*/
int8_t shoot(int8_t array[GRID_COLS][GRID_ROWS]);

/**
    checks weather the move is allowed or is off the board
    @return returns a true or false of weather the move is allowed on the baord
    @param  typedef of cooridate to know where abouts the shot it at
    @param  enum of direction to which the target wants to move to
*/
void moveTarget(Coordinate *coordinate, Direction direction);

/**
    actually move the target to the desired loaction
    @param  pointer typedef of cooridate to know where abouts the shot it at
    @param  enum of direction to which the target wants to move to
*/
bool checkPos(Coordinate coordinate, Direction direction);

#endif