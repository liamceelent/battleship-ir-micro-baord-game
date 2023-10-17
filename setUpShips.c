/** @file   setUpShips
    @author charlie porter cpo57
    @author liam ceelen-thomas lce26
    @date   10 OCT 2023
    @brief  SET UP battle ships 
*/
#include "setUpShips.h"
#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "navswitch.h"
#include "button.h"
#include "pio.h"
#include "button.h"
#include <stdlib.h>

bool placeShips(int8_t array[GRID_COLS][GRID_ROWS]) {
    int shipSizes[] = SHIP_SIZES;
    int numShips = sizeof(shipSizes) / sizeof(shipSizes[0]);
    bool allShipsPlaced = true;  

    for (int s = 0; s < numShips; s++) { // loop through ships

        int currentShipSize = shipSizes[s];
        Ship ship = createShip(0, 0, 0, currentShipSize);  // make ship of size currentShip Size

        bool shipPlaced = false;
        while (!shipPlaced) {
            navswitch_update();
            button_update();
            pacer_wait();
            // move ship logic 
            if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
                if (canMoveShip(&ship, NORTH, array)) {
                    moveShip(&ship, NORTH);
                }
            } else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
                if (canMoveShip(&ship, SOUTH, array)) {
                    moveShip(&ship, SOUTH);
                }
            } else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
                if (canMoveShip(&ship, EAST, array)) {
                    moveShip(&ship, EAST);
                }
            } else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
                if (canMoveShip(&ship, WEST, array)) {
                    moveShip(&ship, WEST);
                }

            } else if (button_push_event_p(0)) {
                if (canRotate(&ship, array)) { 
                    rotateShip(&ship, array);
                }
            } else if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                if (!isOverlapping(&ship, array)) { // check if overlapping Coordinates before adding to board
                    for (int i = 0; i < ship.size; i++) {
                        array[ship.coordinates[i].col][ship.coordinates[i].row] = OCCUPIED; // add ship 
                    }
                    shipPlaced = true;
                } else {
                
                }
            }

            tinygl_clear();
            drawGrid(array);
            for (int i = 0; i < ship.size; i++) {
                tinygl_pixel_set(tinygl_point(ship.coordinates[i].col, ship.coordinates[i].row), 1);
            }
            tinygl_update();
        }
 
        if (!shipPlaced) {  // loop back to smaller ships if till all placed 
            allShipsPlaced = false;
            break; 
        }

        free(ship.coordinates);
    }

    tinygl_update();
    return allShipsPlaced;
}


bool canMoveShip(Ship* ship, Direction direction, int8_t array[GRID_COLS][GRID_ROWS]) { // check if ship still in bounds 
    for (int i = 0; i < ship->size; i++) {
        Coordinate newCoordinate = ship->coordinates[i];
        switch(direction) {
            case NORTH:
                newCoordinate.row -= 1;
                break;
            case SOUTH:
                newCoordinate.row += 1;
                break;
            case EAST:
                newCoordinate.col += 1;
                break;
            case WEST:
                newCoordinate.col -= 1;
                break;
        }

        if (newCoordinate.row < 0 || newCoordinate.row >= GRID_ROWS || 
            newCoordinate.col < 0 || newCoordinate.col >= GRID_COLS) {
            return false;
        }
    }
    return true;
}

bool isOverlapping(Ship* ship, int8_t array[GRID_COLS][GRID_ROWS]) { // check if ships are overlapping 
    for (int i = 0; i < ship->size; i++) {
        if (array[ship->coordinates[i].col][ship->coordinates[i].row] == OCCUPIED) {
            return true;
        }
    }
    return false;
}

void moveShip(Ship* ship, Direction direction) { // relocate ships
    for (int i = 0; i < ship->size; i++) {
        switch(direction) {
            case NORTH:
                ship->coordinates[i].row -= 1;
                break;
            case SOUTH:
                ship->coordinates[i].row += 1;
                break;
            case EAST:
                ship->coordinates[i].col += 1;
                break;
            case WEST:
                ship->coordinates[i].col -= 1;
                break;
        }
    }
}
bool canRotate(Ship* ship, int grid[GRID_COLS][GRID_ROWS]) {

    Coordinate pivot = ship->coordinates[0]; 

    for (int i = 0; i < ship->size; i++) {
        int relativeRow = ship->coordinates[i].row - pivot.row;
        int relativeCol = ship->coordinates[i].col - pivot.col;

        Coordinate newCoordinate;
        newCoordinate.row = pivot.row + relativeCol;
        newCoordinate.col = pivot.col - relativeRow;

        if (newCoordinate.row < 0 || newCoordinate.row >= GRID_ROWS || // check all conditions 
            newCoordinate.col < 0 || newCoordinate.col >= GRID_COLS || 
            grid[newCoordinate.col][newCoordinate.row] != EMPTY) {
            return false;
        }
    }
    return true;
}

void rotateShip(Ship* ship, int grid[GRID_COLS][GRID_ROWS]) {
    if (canRotate(ship, grid)) {
        Coordinate pivot = ship->coordinates[0];

        // Perform a 90-degree clockwise rotation for each coordinate
        for (int i = 0; i < ship->size; i++) {
            int relativeRow = ship->coordinates[i].row - pivot.row;
            int relativeCol = ship->coordinates[i].col - pivot.col;

            ship->coordinates[i].row = pivot.row + relativeCol;
            ship->coordinates[i].col = pivot.col - relativeRow;
        }
    }
}


Ship createShip(int col1, int row1, int col2, int row2) {
    Ship ship;
    int size;
    
    if(col1 == col2) {
        size = abs(row2 - row1);
    } else if(row1 == row2) {
        size = abs(col2 - col1);
    } else {

        exit(1);  
    }
    ship.size = size;
    ship.coordinates = (Coordinate*) malloc(size * sizeof(Coordinate));
    if (ship.coordinates == NULL) {
        
        exit(1);
    }

    if(col1 == col2) {
        int min_row = row1 < row2 ? row1 : row2;
        for(int i = 0; i < size; i++) {
            ship.coordinates[i].col = col1;
            ship.coordinates[i].row = min_row + i;
        }
    } else {
       
        int min_col = col1 < col2 ? col1 : col2;
        for(int i = 0; i < size; i++) {
            ship.coordinates[i].col = min_col + i;
            ship.coordinates[i].row = row1;
        }

    }
    
    return ship;
}
