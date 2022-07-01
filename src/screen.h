// screen.h

#ifndef SCREEN_H
#define SCREEN_H


#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>

#include "terminal.h"

// // to do the screen row data structure 
// struct screen_row {
//     int size;
//     char buffer[50];
// };

// to expand the configuration of the screen
struct screenConfiguration {
    int cursor_x, cursor_y;
    int screen_rows;
    int screen_cols;
    int num_rows;
    //gapBuffer row;

    struct termios original_state;
};

struct screenConfiguration screen;


// get the size of the terminal window
int getWindowSize(int *rows, int *cols) {
    struct winsize window_size;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size) == -1 || window_size.ws_col == 0) {
       // if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
            return -1;
        //}
        // getCursorPosition(rows, cols);
    } else {
        *cols = window_size.ws_col;
        *rows = window_size.ws_row;
        return 0;
    }
}


void initScreen() {
    screen.cursor_x = 0;
    screen.cursor_y = 0;
    screen.num_rows = 0;
    //screen.row = NULL;

    // if (getWindowSize(&screen.screen_rows, &screen.screen_cols) == -1) {
    //     kill("getWindowSize");
    // }
}

// Prints screen rows
void screenRows() {
    for ( int y = 0; y < screen.screen_rows; y++) {
        write(STDOUT_FILENO, "-\r\n", 3);
    }
}

// Fills the screen with all the rows
void refreshScreen() {
    //write(STDOUT_FILENO, "\x1b[2J", 4);
    //write(STDOUT_FILENO, "\x1b[H", 3);
    getWindowSize(&screen.screen_rows, &screen.screen_cols);
    screenRows();
    // Reposition cursor at location (0,0)
    write(STDOUT_FILENO, "\033[H", 3);
}

#endif