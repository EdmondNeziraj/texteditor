#include "../include/raw_mode.h"
#include <unistd.h>
#include <cstdlib>

struct termios RawMode::original_state_;

RawMode::RawMode()
{
    tcgetattr(STDIN_FILENO, &original_state_);
    atexit([]
           { restore(); });

    enable();
}

RawMode::~RawMode()
{
    restore();
}

// Changes the state of the terminal to raw mode
void RawMode::enable()
{
    // Clone the original state
    struct termios raw_state = original_state_;

    // Make the terminal raw
    cfmakeraw(&raw_state);

    // raw_state.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    // raw_state.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    // raw_state.c_oflag &= ~(OPOST);
    // raw_state.c_cflag |= (CS8);

    // Optionally modify additional setting if needed
    raw_state.c_cc[VMIN] = 0;
    raw_state.c_cc[VTIME] = 1;

    // Switch to alternate screen mode
    system("tput smcup");

    // Apply the new settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_state);
}

// Restores the state of the terminal its initial state
void RawMode::restore()
{
    // Switch back to normal screen mode
    system("tput rmcup");

    // Restore the original terminal attributes
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_state_);
}