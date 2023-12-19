#include "src/editor.cc"
#include "include/raw_mode.h"

// // this is a comment
Editor editor;

// // Changes the state of the terminal its initial state
// void disableRawMode()
// {
//     tcsetattr(STDIN_FILENO, TCSAFLUSH, &editor.original_state_);
//     system("tput rmcup");
// }

// // Changes the state of the terminal to raw mode
// void enableRawMode()
// {
//     tcgetattr(STDIN_FILENO, &editor.original_state_);
//     atexit(disableRawMode);

//     struct termios raw_state = editor.original_state_;

//     raw_state.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
//     raw_state.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
//     raw_state.c_oflag &= ~(OPOST);
//     raw_state.c_cflag |= (CS8);
//     raw_state.c_cc[VMIN] = 0;
//     raw_state.c_cc[VTIME] = 1;
//     system("tput smcup");

//     tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_state);
// }

int main(int argc, char *argv[])
{
    RawMode raw_mode;

    if (argc >= 2)
    {
        editor.OpenFile(argv[1]);
    }
    editor.SetStatusMessage("CTRL+Q = quit  |  CTRL+S = save  |  CTRL+F = search");

    while (1)
    {
        editor.RefreshScreen();
        editor.ProcessKey();
    }

    return 0;
}
