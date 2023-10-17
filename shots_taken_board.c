/** @file   shots_taken_board.c
    @author liam ceelen-thomas lce26
    @author charlie porter cpo57
    @date   10 OCT 2023
    @brief  draw a given board
*/

#include <tinygl.h>
#include <shots_taken_board.h>

/**
    light up all leds on the baord that have a one in a positon of the 2d array
    @param  2d array that is filled with both zeros and ones 
*/
void drawGrid(int8_t array[5][7]) {
    for (int8_t x = 0; x < 5; x++) {
        for (int8_t y = 0; y < 7; y++) {
            if (array[x][y] == 1) {
            // Turn on the LED at (x, y)
            tinygl_pixel_set(tinygl_point (x, y), 1);
            } else {
            // Turn off the LED at (x, y)
            tinygl_pixel_set(tinygl_point (x, y), 0);
            }
        }
    }
}
