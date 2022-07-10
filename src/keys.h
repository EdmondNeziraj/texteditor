// keys

#ifndef KEYS_H
#define KEYS_H

#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#include "terminal.h"


#define CTRL_KEY(k) ((k) & 0x1f)

enum keys {
    ARROW_LEFT = 500,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    PAGE_UP,
    PAGE_DOWN,
    HOME_KEY,
    END_KEY,
    DEL_KEY
};

// read a key input from the user
int readKey() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) {
            kill("read");
        }
        //std::cout << c << " " << (int)c << "\r\n";
    }
    
    if (c == '\x1b') {
        char seq[3];
        
        // <- = \x1b[D     \x1b = ^[
        if (read(STDIN_FILENO, &seq[0], 1) != 1 || 
            read(STDIN_FILENO, &seq[1], 1) != 1) {
            return '\x1b';
        }


        // -> == <esc>[C
        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9'){
                if (read(STDIN_FILENO, &seq[2], 1) != 1) {
                    return '\x1b';
                }
                if (seq[2] == '~') {
                    switch (seq[1]) {
                        case '3': return DEL_KEY;
                        case '5': return PAGE_UP;
                        case '6': return PAGE_DOWN;
                    }
                }
            } else {
                switch (seq[1])
                {
                    case 'A': return ARROW_UP;
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_LEFT;
                    case 'D': return ARROW_RIGHT;
                    case 'H': return HOME_KEY;
                    case 'F': return END_KEY;
                }
            }
        }
        return '\x1b';
    } else {
        return c;
    }
}

// move cursor
void moveCursor(int key) {
    switch(key) {
        case ARROW_LEFT:
            screen.cursor_x--;
            break;
        case ARROW_RIGHT:
            screen.cursor_x++;
            break;
        case ARROW_UP:
            screen.cursor_y--;
            break;
        case ARROW_DOWN:
            screen.cursor_y++;
            break;
    }
}

// process key typed from the user
void processKey() {
    int k = readKey();

    switch (k) {
        case CTRL_KEY('q'):
            exit(0);
            break;

        case ARROW_LEFT:
        case ARROW_RIGHT:
        case ARROW_UP:
        case ARROW_DOWN:
            moveCursor(k);
            break;
        
        case PAGE_UP:
        case PAGE_DOWN:
            { int times = screen.screen_rows;
                while(times--) {
                    moveCursor(k == PAGE_UP ? ARROW_UP : ARROW_DOWN);
                }
            }
        case HOME_KEY:
            screen.cursor_x = 0;
            break;
        
        case END_KEY:
            screen.cursor_x = screen.screen_cols - 1;
            break;
    }
}

#endif