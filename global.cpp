// test.cpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

//tljasjdajsdjasdalasdljajfdjflasdfakjdfak;sdjfak;sdj;askdg;ajskdg;asjdg;jasg

//#include "src/keys.h"
#include "src/gapBuffer.h"
#include "src/editor.h"

using namespace std;

int main(int argc, char *argv[]) {
    // enableRawMode();
    // if (argc >=2 ) {
    //   editorOpen(argv[1]);
    // }
    // initScreen();
    // editorOpen();

    // while(1) {
    //   updateScreen();
    //   processKey();
    // }


    // //opens an empty text file, you can edit and save the file
    // if (argc > 1) {
    //     // reads from a file and turn on edit and save feature
    //     readAndEditfile(argv[1]);
    // } else {
    //     string fileName;
    //     std::cout << "Enter a name for the file: ";
    //     std::cin >> fileName;
    //     editfileOnCreate(fileName);
    // }

    // char *in = new char[5];
    // char c = 'a';
    // for (int i = 0; i < 5; i++) {
    //   in[i] = c;
    // }

    // GapBuffer gapBuffer(in, 5);
    // gapBuffer.display();
    
    // char c;
    // int position = 0;
    // gapBuffer.display();
    // while(1)  {
    //     std::cout << "Insert char: ";
    //     std::cin >> c;

    //     if (c == 'q') {
    //         break;
    //     }
 
    //     gapBuffer.insert(c, position);
    //     gapBuffer.display();
    //     std::cout << "text length: " << gapBuffer.count() << std::endl;
    //     std::cout << "buffer size: " << gapBuffer.bufferSize() << std::endl;        
    //     position++;
    // }
    return 0;
}



// terminal.h

// // terminal.h

// #ifndef TERMINAL_H
// #define TERMINAL_H

// #include <unistd.h>
// #include <termios.h>
// #include <stdlib.h>
// #include <sys/ioctl.h>
// #include <errno.h>
// #include <stdio.h>
// #include <ctype.h>
// #include <iostream>

// #include "screen.h"
// // If a function raises an error, prints the error message and exit
// // void kill(const char *s) {
// //     // write(STDOUT_FILENO, "\x1b[2J", 4);
// //     // write(STDOUT_FILENO, "\x1b[H", 3);

// //     perror(s);
// //     exit(1);
// // }

// // Changes the state of the terminal its initial state
// void disableRawMode() {
//     // if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &screen.original_state) == -1) {
//     //     kill("tcsetattr");
//     // }
//     tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
//     system("tput rmcup");
// }

// // Changes the state of the terminal to raw mode
// void enableRawMode(struct termios terminal) {
//     // if (tcgetattr(STDIN_FILENO, &screen.original_state) == -1) {
//     //     kill("tcgetattr");
//     // }
//     tcgetattr(STDIN_FILENO, &terminal);
//     atexit(disableRawMode);

//     struct termios raw_state = terminal;

//     raw_state.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
//     raw_state.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
//     raw_state.c_oflag &= ~(OPOST);
//     raw_state.c_cflag |= (CS8);
//     raw_state.c_cc[VMIN] = 0;
//     raw_state.c_cc[VTIME] = 1;
//     system("tput smcup");

//     // if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_state) == -1) {
//     //     kill("tcsetattr");
//     // }
//     tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_state);
// }


// #endif




// SCREEN.H

// // screen.h

// /**
//  * This line explains the purpose of the function
//  * This line explains the purpose of the parameter1
//  * This line explains the purpose of the parameter2
//  * This line explains the return value or the result of the function
//  */

// #ifndef SCREEN_H
// #define SCREEN_H

// #define _BSD_SOURCE

// #include <unistd.h>
// #include <termios.h>
// #include <stdlib.h>
// #include <sys/ioctl.h>
// #include <errno.h>
// #include <stdio.h>
// #include <ctype.h>
// #include <iostream>
// #include <string.h>
// #include <sys/types.h>

// #include "terminal.h"
// #include "screenContainer.h"
// #include "gapBuffer.h"
// //#include "keys.h"

// using namespace std;

// // to do the screen row data structure (gap buffer) 
// typedef struct screen_row {
//     int size;
//     char *chars;
//     int rsize;
//     char *render;
// } screen_row;

// // to expand the configuration of the screen
// struct screen {
//     int cursor_x;
//     int cursor_y;
//     int render_x;
//     int row_off;
//     int col_off;
//     int screen_rows;
//     int screen_cols;
//     int num_rows;
//     screen_row *row;
//     GapBuffer *rows_;
//     struct termios original_state;
// };

// struct screen screen;


// /**
//  * Captures the window size that the terminal is currently
//  * *rows is the data member to modify
//  * *cols is the data member to modify
//  * Return -1 on failure, otherwise modify passed parameters and return 0.
//  */
// int getWindowSize(int *rows, int *cols) {

//     struct winsize window_size;
    
//     if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size) == -1 || window_size.ws_col == 0) {
//        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
//             return -1;
//         }
//         return  -1;
//         //getCursorPosition(rows, cols);
//     } else {
//         *cols = window_size.ws_col;
//         *rows = window_size.ws_row;
//         return 0;
//     }
// }

// // void updateRow(screen_row *row) {
// //     int tabs = 0;
// //     int j;
// //     for (j = 0; j < row->size; j++) {
// //         if (row->chars[j] == '\t') {
// //             tabs++;
// //         }
// //     }
        
// //     free(row->render);
// //     row->render = (char*) malloc(row->size + tabs*3 + 1);

// //     int idx = 0;
// //     for (j = 0; j < row->size; j++) {
// //         if (row->chars[j] == '\t') {
// //             row->render[idx++] = ' ';
// //             while (idx % 4 != 0)
// //             {
// //                 row->render[idx++] = ' ';
// //             }
// //         } else {
// //             row->render[idx++] = row->chars[j];
// //         }
// //     }
// //     row->render[idx] = '\0';
// //     row->rsize = idx;
// // }

// void updateRow(GapBuffer *row) {
//     int tabs = 0;
//     int j;
//     for (j = 0; j < row->buffer_size_; j++) {
//         if (row->buffer_[j] == '\t') {
//             tabs++;
//         }
//     }
//     cout << "test2";
//     free(row->render_);
//     row->render_ = (char*) malloc(row->buffer_size_ + tabs*3 + 1);

//     int idx = 0;
//     for (j = 0; j < row->buffer_size_; j++) {
//         if (row->buffer_[j] == '\t') {
//             row->render_[idx++] = ' ';
//             while (idx % 4 != 0)
//             {
//                 row->render_[idx++] = ' ';
//             }
//         } else {
//             row->render_[idx++] = row->buffer_[j];
//         }
//     }
//     row->render_[idx] = '\0';
//     row->rsize_ = idx;
// }


// void appendRow(char *s, size_t len) {
//     //screen.row = (screen_row*) realloc(screen.row, sizeof(screen.row) * (screen.num_rows+ 1));
//     screen.rows_ = (GapBuffer*) realloc(screen.rows_, sizeof(GapBuffer) * (screen.num_rows + 1));
//     //std::copy(screen.rows_, screen.rows_ + screen.num_rows, temp_rows);

//     int index = screen.num_rows;
//     screen.rows_[index].text_length_ = len;
//     screen.rows_[index].buffer_ = (char*) malloc(len+1);
//     memcpy(screen.rows_[index].buffer_, s, len);
//     screen.rows_[index].buffer_[len] = '\0';

//     // screen.rows_[index].rsize = 0;
//     // screen.rows_[index].render = NULL;
//     updateRow(&screen.rows_[index]);
//     screen.num_rows++;
//     //cout << "num_rows: " << screen.num_rows << endl;
// }

// void appendRoww(GapBuffer &input) {
//     screen.rows_ = (GapBuffer*) realloc(screen.rows_, sizeof(GapBuffer) * (screen.num_rows + 1));

//     int at = screen.num_rows;
//     cout << "is it here?";
//     screen.rows_[at] = input;
    
//     updateRow(&screen.rows_[at]);
//     screen.num_rows++;
// }

// void editorOpen() {
//     // FILE *fp = fopen(filename, "r");

//     // char *line = NULL;
//     // size_t linecap = 0;
//     // ssize_t linelen;
//     // GapBuffer temp_buffer;  

//     // while ((linelen = getline(&line, &linecap, fp))!= -1) {
//     //     while (linelen > 0 && (line[linelen - 1] == '\n' ||
//     //                             line[linelen - 1] == '\r')) {
//     //         linelen--;
//     //     }
//     //     temp_buffer.setBuffer(line, linelen);
//     //     //temp_buffer.display();
//     //     cout << "here?";
//     //     appendRoww(temp_buffer);
//     // }
//     // free(line);
//     // fclose(fp);
// }


// // int getCursorPosition(int *rows, int *cols) {
// //     char buf[32];
// //     unsigned int i = 0;
// //     if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) {
// //         return -1;
// //     }

// //     while ( i < sizeof(buf) - 1) {
// //         if (read(STDIN_FILENO, &buf[i], 1) != 1) {
// //             break;
// //         }
// //         if (buf[i] == 'R') {
// //             break;
// //         }
// //         i++;
// //     }
// //     buf[i] = '\0';

// //     if (buf[0] != '\x1b' || buf[1] != '[') {
// //         return -1;
// //     }
// //     if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) {
// //         return -1;
// //     }

// //     return -1;
// // }


// void initScreen() {
//     screen.cursor_x = 0;
//     screen.cursor_y = 0;
//     screen.render_x = 0;
//     screen.row_off = 0;
//     screen.col_off = 0;
//     screen.num_rows = 0;
//     //screen.row = 0;
//     getWindowSize(&screen.screen_rows, &screen.screen_cols);
// }

// int rowCxToRx (screen_row *row, int cx) {
//     int rx = 0;
//     //int j = 0;
//     for (int j = 0; j < cx; j++) {
//         if (row->chars[j] == '\t') {
//             rx += 3 - (rx % 4);
//         }
//         rx++;
//     }
//     return rx;
// }


// void insertCharInRow(GapBuffer *row, int at, int character) {
//   if (at < 0 || at > row->buffer_size_) at = row->buffer_size_;
//   row->buffer_ = (char*) realloc(row->buffer_, row->buffer_size_ + 2);
//   memmove(&row->buffer_[at + 1], &row->buffer_[at], row->buffer_size_ - at + 1);
//   row->buffer_size_++;
//   row->buffer_[at] = character;
//   updateRow(row);
// }

// void delCharInRow(GapBuffer *row, int at) {
//     if (at < 0 || at >= row->buffer_size_) return;
//     memmove(&row->buffer_[at], &row->buffer_[at+1], row->buffer_size_ - at);
//     row->buffer_size_--;
//     updateRow(row);
// }

// void insertChar(int character) {
//   if (screen.cursor_y == screen.num_rows) {
//     appendRow((char*)' ', 0);
//   }
//   gapbuffer.insertCharInRow(&screen.rows_[screen.cursor_y], screen.cursor_x, character);
//   screen.cursor_x++;
// }

// void insertNewline() {
//   if (screen.cursor_x == 0) {
//     appendRow((char*) " ", 0);
//   } else {
//     GapBuffer *row = &screen.rows_[screen.cursor_y];
//     appendRow(&row->buffer_[screen.cursor_x], row->buffer_size_ - screen.cursor_x);
//     row = &screen.rows_[screen.cursor_y];
//     row->buffer_size_ = screen.cursor_x;
//     row->buffer_[row->buffer_size_] = '\0';
//     updateRow(row);
//   }
//   screen.cursor_y++;
//   screen.cursor_x = 0;
// }

// void freeRow(screen_row *row) {
//   free(row->render);
//   free(row->chars);
// }

// void delRow(int at) {
//   if (at < 0 || at >= screen.num_rows) return;
//   freeRow(&screen.row[at]);
//   memmove(&screen.row[at], &screen.row[at + 1], sizeof(screen_row) * (screen.num_rows - at - 1));
//   screen.num_rows--;
// }

// void appendStringInRow(GapBuffer *row, char *s, size_t length) {
//     row->buffer_ = (char*) realloc(row->buffer_, row->buffer_size_ + length + 1);
//     memcpy(&row->buffer_[row->buffer_size_], s, length);
//     row->buffer_size_ += length;
//     row->buffer_[row->buffer_size_] = '\0';
//     updateRow(row);
// }

// void delChar() {
//     if (screen.cursor_y = screen.num_rows) return;
//     if (screen.cursor_x == 0 && screen.cursor_y == 0) return;

//     GapBuffer *row = &screen.rows_[screen.cursor_y];
//     if (screen.cursor_x > 0) {
//         delCharInRow(row, screen.cursor_x -1);
//         screen.cursor_x--;
//     } else {
//         screen.cursor_x = screen.row[screen.cursor_y - 1].size;
//         appendStringInRow(&screen.rows_[screen.cursor_y - 1], row->buffer_, row->buffer_size_);
//         delRow(screen.cursor_y);
//         screen.cursor_y--;
//     }
// }


// // Prints screen rows
// void screenRows(screenContainer *container) {
//     for ( int y = 0; y < screen.screen_rows; y++) {
//         int filerow = y + screen.row_off;
//         if (filerow >= screen.num_rows) {
//             container->Append(container, (char*)"~", 1);
//         } else {
//             int len = screen.row[filerow].rsize - screen.col_off;
//             if (len < 0) {
//                 len = 0;
//             }
//             if (len > screen.screen_cols) {
//                 len = screen.screen_cols;
//             }
//             container->Append(container, &screen.rows_[filerow].render_[screen.col_off], len);
//         }

//         container->Append(container, (char*) "\x1b[K", 3);
//         if (y < screen.screen_rows-1) {
//             container->Append(container, (char*) "\r\n", 2);
//         }
//     }
// }


// void scroll() {
//     screen.render_x = 0;
//     if (screen.cursor_y < screen.num_rows) {
//         screen.render_x = rowCxToRx(&screen.row[screen.cursor_y], screen.cursor_x);
//     }

//     if (screen.cursor_y < screen.row_off) {
//         screen.row_off = screen.cursor_y;
//     }
//     if (screen.cursor_y >= screen.row_off + screen.screen_rows) {
//         screen.row_off = screen.cursor_y - screen.screen_rows;
//     }
//     if (screen.render_x < screen.col_off) {
//         screen.col_off = screen.render_x;
//     }
//     if (screen.render_x >= screen.col_off + screen.screen_cols) {
//         screen.col_off = screen.render_x - screen.screen_cols;
//     }
// }

// // Fills the screen with all the rows
// void updateScreen() {
//     scroll();
//     getWindowSize(&screen.screen_rows, &screen.screen_cols);

//     //struct abuf ab = ABUF_INIT;
//     screenContainer container;

//     container.Append(&container, (char*) "\x1b[?25l", 6);
//     container.Append(&container, (char*) "\x1b[H", 3);
    
//     screenRows(&container);

//     char buf[32];
//     snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (screen.cursor_y - screen.row_off) + 1, 
//                                                 (screen.render_x - screen.col_off) + 1);
//     container.Append(&container, buf, strlen(buf));
//     container.Append(&container, (char*)"\x1b[?25h", 6);

//     write(STDOUT_FILENO, container.getData(), container.getLength());
// }
































// #define CTRL_KEY(k) ((k) & 0x1f)

// enum keys {
//     BACKSPACE = 127,
//     ARROW_LEFT = 500,
//     ARROW_RIGHT,
//     ARROW_UP,
//     ARROW_DOWN,
//     PAGE_UP,
//     PAGE_DOWN,
//     HOME_KEY,
//     END_KEY,
//     DEL_KEY
// };

// // read a key input from the user
// int readKey() {
//     int nread;
//     char c;
//     while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
//         if (nread == -1 && errno != EAGAIN) {
//             //kill("read");
//         }
//         //cout << c << " " << (int)c << "\r\n";
//     }
//     if (c == '\x1b') {
//         char seq[3];

//         // <- = \x1b[D     \x1b = ^[
//         if (read(STDIN_FILENO, &seq[0], 1) != 1 || 
//             read(STDIN_FILENO, &seq[1], 1) != 1) {
//             return '\x1b';
//         }


//         // -> == <esc>[C
//         if (seq[0] == '[') {
//             if (seq[1] >= '0' && seq[1] <= '9'){
//                 if (read(STDIN_FILENO, &seq[2], 1) != 1) {
//                     return '\x1b';
//                 }
//                 if (seq[2] == '~') {
//                     switch (seq[1]) {
//                         case '3': return DEL_KEY;
//                         case '5': return PAGE_UP;
//                         case '6': return PAGE_DOWN;
//                     }
//                 }
//             } else {
//                 switch (seq[1])
//                 {
//                     case 'A': return ARROW_UP;
//                     case 'B': return ARROW_DOWN;
//                     case 'C': return ARROW_RIGHT;
//                     case 'D': return ARROW_LEFT;
//                     case 'H': return HOME_KEY;
//                     case 'F': return END_KEY;
//                 }
//             }
//         }
//         return '\x1b';
//     } else {
//         return c;
//     }
// }

// // move cursor
// void moveCursor(int key) {
//     screen_row *row = (screen.cursor_y >= screen.num_rows) ? NULL : &screen.row[screen.cursor_y];

//     switch(key) {
//         case ARROW_LEFT:
//             if (screen.cursor_x != 0) {
//                 screen.cursor_x--;
//             } else if (screen.cursor_y > 0) {
//                 screen.cursor_y--;
//                 screen.cursor_x = screen.row[screen.cursor_y].size;
//             }
//             break;
//         case ARROW_RIGHT:
//             if (row && screen.cursor_x < row->size) {
//                 screen.cursor_x++;
//             } else if (row && screen.cursor_x == row->size) {
//                 screen.cursor_y++;
//                 screen.cursor_x = 0;
//             }
//             break;
//         case ARROW_UP:
//             if (screen.cursor_y != 0) {
//                 screen.cursor_y--;
//             }
//             break;
//         case ARROW_DOWN:
//             if (screen.cursor_y < screen.screen_rows) {
//                 screen.cursor_y++;
//             }
//             break;
//     }

//     row = (screen.cursor_y >= screen.num_rows) ? NULL : &screen.row[screen.cursor_y];
//     int row_length = row ? row->size : 0;
//     if (screen.cursor_x > row_length) {
//         screen.cursor_x = row_length;
//     }
// }

// // process key typed from the user
// void processKey() {
//     int k = readKey();

//     switch (k) {
//         case BACKSPACE:
//             delChar();
//             break;

//         case '\r':
//             insertNewline();
//             break;

//         case CTRL_KEY('q'):
//             exit(0);
//             break;

//         case CTRL_KEY('s'):
//             //TO DO
//             break;
//         case ARROW_LEFT:
//         case ARROW_RIGHT:
//         case ARROW_UP:
//         case ARROW_DOWN:
//             moveCursor(k);
//             break;
        
//         case PAGE_UP:
//         case PAGE_DOWN:
//             { int times = screen.screen_rows;
//                 while(times--) {
//                     moveCursor(k == PAGE_UP ? ARROW_UP : ARROW_DOWN);
//                 }
//             }
//         case HOME_KEY:
//             screen.cursor_x = 0;
//             break;
        
//         case END_KEY:
//             screen.cursor_x = screen.screen_cols - 1;
//             break;

//         //case CTRL_KEY('h'):
//         case DEL_KEY:
//             if (k == DEL_KEY) {
//                 moveCursor(ARROW_RIGHT);
//             }
//             delChar();
//             break;

//         case CTRL_KEY('l'):
//         // case '\x1b':
//         //     break;

//         default:
//             insertChar(k);
//             break;
//     }
// }
// #endif



// FILE.H

#include <iostream>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string>
// #include <fstream>
// using namespace std;

// // when we type a character in terminal, this function will read that character and returns 
// char keypress() {
//   // stty --> Print or change terminal characteristics.
//   system ("/bin/stty raw"); // The raw setting means that the input and output is not processed, just sent straight through.
//   char c;
//   system("/bin/stty -echo"); // dash means disable the setting and typing is not echoed back to us
//   c = getc(stdin);
//   system("/bin/stty echo"); // enables echo again
//   system ("/bin/stty cooked"); // cannonical form, returns one line of data at a time from the terminal device.
//   return c;
// }

// void editFile(string filename, string text) {
//     ofstream file(filename);  
//     bool saved = true;
//     // use data structure here 
//     string save;
    
//   do {
//     std::cout << "\033[2J\033[1;1H"; //clear

//     if (saved) {
//       save = "  |  File is saved";
//     } else {
//       save = "  |  File has unsaved changes";
//     }
//     std::cout << filename << "  |  Press ctrl + X to exit" << save << "\n____________________________________________________________________________\n" << text; //display text and header

//     system("/bin/stty -raw"); // disables raw mode, the terminal device driver returns one character at a time without assembling lines of data. Also, special characters are not processed in this mode
//     system("/bin/stty raw"); //change input
//     char key = keypress(); //take input
//     if (key == 24) {
//          std::cout << "\033[2J\033[1;1H";
//          break;
//     }
//     if (key == 96) {
//       std::cout << "\033[2J\033[1;1H";
//       break;
//     } else if(key == 127) {
//          text = text.substr(0, text.length() - 1);
//     } else { //add char
//       text += key;
//       saved = false;
//     }
//   } while (true);
// }

// void editfileOnCreate(string filename) {
//   std::cout << "\033[2J\033[1;1H";
//   string text = "";
//   editFile(filename, text); 
// }

// void readAndEditfile(string filename) {
//   std::cout << "\033[2J\033[1;1H"; //clear
//   ifstream readFile(filename); //open file to read
//   string text;
//   std::cout << filename << "  |  Read only mode  |  Press enter to exit \n___________________________________________________\n"; //header
//   while (getline (readFile, text)) {
//     cout << text << "\n"; //display
//   }
//   // allows to edit and save the file 
//   editFile(filename, text);
// }
