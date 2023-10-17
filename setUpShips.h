/** @file   setupShips.h
    @author Charlie Porter
    @date   10 October 2023
    @brief  placing ships on the board

*/

#ifndef SETUPSHIPS_H
#define SETUPSHIPS_H

#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "navswitch.h"
#include "button.h"
#include "pio.h"
#include <stdlib.h>

// Constants
#define EMPTY 0
#define OCCUPIED 1 
#define GRID_COLS 5
#define GRID_ROWS 7
#define MAX_SHIP_COUNT 4 // maximum number of ships
#define SHIP_SIZES {4, 3, 2, 1} // Sizes of the ships

// Directions to move ships 
typedef enum {
    NORTH, SOUTH, EAST, WEST
} Direction;

// the Coordinates points of ship
typedef struct {
    int col;
    int row;
} Coordinate;

// ship has array of coordinates and a size 
typedef struct {
    Coordinate* coordinates;  
    int size;  
} Ship;

// Function Prototypes
Ship createShip(int col1, int row1, int col2, int row2);
void moveShip(Ship* ship, Direction direction);
void rotateShip(Ship* ship, int grid[GRID_COLS][GRID_ROWS]);
bool canMoveShip(Ship* ship, Direction direction, int8_t array[5][7]);
bool canRotate(Ship* ship, int grid[GRID_COLS][GRID_ROWS]);
bool placeShips(int8_t array[GRID_COLS][GRID_ROWS]);
bool isOverlapping(Ship* ship, int8_t array[GRID_COLS][GRID_ROWS]);

#endif
