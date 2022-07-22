// keys

#ifndef KEYS_H
#define KEYS_H

#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#include "editor.h"


#define CTRL_KEY(k) ((k) & 0x1f)

enum keys {
    BACKSPACE = 127,
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
            //kill("read");
        }
        //cout << c << " " << (int)c << "\r\n";
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
                        case '1': return HOME_KEY;
                        case '3': return DEL_KEY;
                        case '4': return END_KEY;
                        case '5': return PAGE_UP;
                        case '6': return PAGE_DOWN;
                        case '7': return HOME_KEY;
                        case '8': return END_KEY;
                    }
                }
            } else {
                switch (seq[1])
                {
                    case 'A': return ARROW_UP;
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_RIGHT;
                    case 'D': return ARROW_LEFT;
                    case 'H': return HOME_KEY;
                    case 'F': return END_KEY;
                }
            }
        } else if (seq[0] == 'O'){
            switch (seq[1])
            {
                case 'H': return HOME_KEY;
                case 'F': return END_KEY;
            }
        }
        return '\x1b';
    } else {
        return c;
    }
}

// move cursor
void moveCursor(int key, Editor editor) {
    GapBuffer *row = (editor.cursor_y_ >= editor.num_rows_) ? NULL : &editor.rows_[editor.cursor_y_];

    switch(key) {
        case ARROW_LEFT:
            if (editor.cursor_x_ != 0) {
                editor.cursor_x_--;
            } else if (editor.cursor_y_ > 0) {
                editor.cursor_y_--;
                editor.cursor_x_ = editor.rows_[editor.cursor_y_].buffer_size_;
            }
            break;
        case ARROW_RIGHT:
            if (row && editor.cursor_x_ < row->buffer_size_) {
                editor.cursor_x_++;
            } else if (row && editor.cursor_x_ == row->buffer_size_) {
                editor.cursor_y_++;
                editor.cursor_x_ = 0;
            }
            break;
        case ARROW_UP:
            if (editor.cursor_y_ != 0) {
                editor.cursor_y_--;
            }
            break;
        case ARROW_DOWN:
            if (editor.cursor_y_ < editor.num_rows_) {
                editor.cursor_y_++;
            }
            break;
    }

    row = (editor.cursor_y_ >= editor.num_rows_) ? NULL : &editor.rows_[editor.cursor_y_];
    int row_length = row ? row->buffer_size_ : 0;
    if (editor.cursor_x_ > row_length) {
        editor.cursor_x_ = row_length;
    }
}

// process key typed from the user
void processKey(Editor editor) {
    int k = readKey();

    switch (k) {
        case BACKSPACE:
            editor.DeleteChar();
            break;

        case '\r':
            //insertNewline();
            break;

        case CTRL_KEY('q'):
            exit(0);
            break;

        case CTRL_KEY('s'):
            //TO DO
            break;
        case ARROW_LEFT:
        case ARROW_RIGHT:
        case ARROW_UP:
        case ARROW_DOWN:
            moveCursor(k, editor);
            break;
        
        case PAGE_UP:
        case PAGE_DOWN:
            { int times = editor.screen_rows_;
                while(times--) {
                    moveCursor(k == PAGE_UP ? ARROW_UP : ARROW_DOWN, editor);
                }
            }
        case HOME_KEY:
            editor.cursor_x_ = 0;
            break;
        
        case END_KEY:
            if (editor.cursor_y_ < editor.num_rows_) {
                editor.cursor_x_ = editor.rows_[editor.cursor_y_].buffer_size_;
            }
            break;

        //case CTRL_KEY('h'):
        case DEL_KEY:
            if (k == DEL_KEY) {
                moveCursor(ARROW_RIGHT, editor);
            }
            editor.DeleteChar();
            break;

        //case CTRL_KEY('l'):
        // case '\x1b':
        //     break;

        default:
            editor.InsertChar(k);
            break;
    }
}

#endif