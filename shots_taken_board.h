/** @file   shots_taken_board.h
    @author liam ceelen-thomas lce26
    @author charlie porter cpo57
    @date   10 OCT 2023
    @brief  draw a given board
*/

#ifndef SHOTS_TAKEN_BOARD_H
#define SHOTS_TAKEN_BOARD_H

#include <tinygl.h>

/**
    light up all leds on the baord that have a one in a positon of the 2d array
    @param  2d array that is filled with both zeros and ones 
*/
void drawGrid(int8_t array[5][7]);

#endif