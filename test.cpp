// test.cpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>

//#include "src/keys.h"
#include "src/gap_buffer.h"
#include "src/editor.cc"

using namespace std;

Editor editor;

<<<<<<< HEAD
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
void moveCursor(int key) {
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
=======
    while(1) {
      refreshScreen();
      processKey();
>>>>>>> origin
    }
}

// process key typed from the user
void processKey() {
    int k = readKey();

    switch (k) {
        case BACKSPACE:
            editor.DeleteChar();
            break;

        case '\r':
            editor.InsertNewLine();
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
            moveCursor(k);
            break;
        
        case PAGE_UP:
        case PAGE_DOWN:
            { int times = editor.screen_rows_;
                while(times--) {
                    moveCursor(k == PAGE_UP ? ARROW_UP : ARROW_DOWN);
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
                moveCursor(ARROW_RIGHT);
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

// // Changes the state of the terminal its initial state
void disableRawMode() {
    // if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &screen.original_state) == -1) {
    //     kill("tcsetattr");
    // }
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &editor.original_state_);
    system("tput rmcup");
}

// Changes the state of the terminal to raw mode
void enableRawMode() {
    // if (tcgetattr(STDIN_FILENO, &screen.original_state) == -1) {
    //     kill("tcgetattr");
    // }
    tcgetattr(STDIN_FILENO, &editor.original_state_);
    atexit(disableRawMode);

    struct termios raw_state = editor.original_state_;

    raw_state.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw_state.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw_state.c_oflag &= ~(OPOST);
    raw_state.c_cflag |= (CS8);
    raw_state.c_cc[VMIN] = 0;
    raw_state.c_cc[VTIME] = 1;
    system("tput smcup");

    // if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_state) == -1) {
    //     kill("tcsetattr");
    // }
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_state);
}


void editorOpen(char *filename) {
    FILE *fp = fopen(filename, "r");

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    GapBuffer *temp_buffer;  
    while ((linelen = getline(&line, &linecap, fp))!= -1) {
        while (linelen > 0 && (line[linelen - 1] == '\n' ||
                                line[linelen - 1] == '\r')) {
            linelen--;
        }
        editor.InsertBuffer(editor.num_rows_, line, linelen);
    }
    free(line);
    fclose(fp);
}

int main(int argc, char *argv[]) { 
    enableRawMode();
    if (argc >= 2) {
        editorOpen(argv[1]);
    }

    while(1) {
        editor.RefreshScreen();
        processKey();
    }

    return 0;
}