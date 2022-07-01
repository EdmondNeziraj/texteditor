// terminal.h

#ifndef TERMINAL_H
#define TERMINAL_H

//#define _DEFAULT_SOURCE
//#define _BSD_SOURCE
//#define _GNU_SOURCE

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>

#include "screen.h"

#define TXTED_VERSION "1.0.0"

// If a function raises an error, prints the error message and exit
void kill(const char *s) {
    // write(STDOUT_FILENO, "\x1b[2J", 4);
    // write(STDOUT_FILENO, "\x1b[H", 3);

    perror(s);
    exit(1);
}

// Changes the state of the terminal its initial state
void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &screen.original_state) == -1) {
        kill("tcsetattr");
    }
    system("tput rmcup");
}

// Changes the state of the terminal to raw mode
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &screen.original_state) == -1) {
        kill("tcgetattr");
    }
    atexit(disableRawMode);

    struct termios raw_state = screen.original_state;
    raw_state.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw_state.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw_state.c_oflag &= ~(OPOST);
    raw_state.c_cflag |= (CS8);
    raw_state.c_cc[VMIN] = 0;
    raw_state.c_cc[VTIME] = 1;

    system("tput smcup");

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_state) == -1) {
        kill("tcsetattr");
    }
}


#endif