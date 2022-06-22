// terminal.cpp
// enableRawMode() - allows us to enter in terminal raw mode
// disableRawMode() - restores terminal's initial properties


#include <unistd.h>
#include <termios.h>
#include <stdlib.h>


struct termios original_state;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_state);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &original_state);
    atexit(disableRawMode);

    struct termios raw_state = original_state;
    raw_state.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_state);
}