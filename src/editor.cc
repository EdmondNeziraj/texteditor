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

#include "editor.h"
#include "screen_container.cc"
//#include "keys.h"

Editor::Editor() {
    cursor_x_ = 0;
    cursor_y_ = 0;
    row_off_ = 0;
    col_off_ = 0;
    num_rows_ = 0;
    rows_ = NULL;
    SetWindowSize(&screen_rows_, &screen_cols_);
}

int Editor::SetWindowSize(int *rows, int *cols) {

    struct winsize window_size;
    
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size) == -1 || window_size.ws_col == 0) {
       if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
            return -1;
        }
        return  -1;
    } else {
        *cols = window_size.ws_col;
        *rows = window_size.ws_row;
        return 0;
    }
}

void Editor::AppendRow(char *string, size_t length) {
    rows_ = (GapBuffer*) realloc(rows_, sizeof(GapBuffer) * (num_rows_ + 1));

    int index = num_rows_;
    rows_[index].buffer_size_ = length;
    rows_[index].buffer_ = (char*) malloc(length+1);
    memcpy(rows_[index].buffer_, string, length);
    rows_[index].buffer_[length] = '\0';
    num_rows_++;
}

void Editor::InsertBuffer(int position, char *buffer, size_t length) {
    if (position < 0 || position > num_rows_) return;

    rows_ = (GapBuffer*) realloc(rows_, sizeof(GapBuffer) * (num_rows_ + 1));
    memmove(&rows_[position+1], &rows_[position], sizeof(GapBuffer) * (num_rows_ - position));
    rows_[position].setBuffer(buffer, length);
    num_rows_++;
}

void Editor::UpdateCursor() {
    if (cursor_y_ < row_off_) {
        row_off_ = cursor_y_;
    }
    if (cursor_y_ >= row_off_ + screen_rows_) {
        row_off_ = cursor_y_ - screen_rows_ + 1;
    }
    if (cursor_x_ < col_off_) {
        col_off_ = cursor_x_;
    }
    if (cursor_x_ >= col_off_ + screen_cols_) {
        col_off_ = cursor_x_ - screen_cols_ + 1;
    }
}

void Editor::DisplayRows(ScreenContainer *container) {
    for ( int y = 0; y < screen_rows_; y++) {
        int filerow = y + row_off_;
        if (filerow >= num_rows_) {
            container->Append((char*)"~", 1);
        } else {
            int len = rows_[filerow].buffer_size_ - col_off_;
            if (len < 0) {
                len = 0;
            }
            if (len > screen_cols_) {
                len = screen_cols_;
            }
            container->Append(&rows_[filerow].buffer_[col_off_], len);
        }

        container->Append((char*) "\x1b[K", 3);
        if (y < screen_rows_ - 1) {
            container->Append((char*) "\r\n", 2);
        }
    }
}

void Editor::InsertChar(int character) {
    if (cursor_y_ == num_rows_) {
        InsertBuffer(num_rows_, (char*)"", 0);
    }
    rows_[cursor_y_].insert(character, cursor_x_);
    cursor_x_++;
}

void Editor::InsertNewLine() {
  if (cursor_x_ == 0) {
    InsertBuffer(cursor_y_, (char*) "", 0);
  } else {
    GapBuffer *row = &rows_[cursor_y_];
    InsertBuffer(cursor_y_ + 1, &row->buffer_[cursor_x_], row->buffer_size_ - cursor_x_);
    row = &rows_[cursor_y_];
    row->buffer_size_ = cursor_x_;
    row->buffer_[row->buffer_size_] = '\0';
    //updateRow(row);
  }
  cursor_y_++;
  cursor_x_ = 0;
}

void Editor::EmptyRow(GapBuffer *row) {
    delete[] row->buffer_;
    row->buffer_ = NULL;
}

void Editor::DeleteRow(int position) {
  if (position < 0 || position >= num_rows_) return;
  EmptyRow(&rows_[position]);
  memmove(&rows_[position], &rows_[position + 1], sizeof(GapBuffer) * (num_rows_ - position - 1));
  num_rows_--;
}

void Editor::AppendStringInRow(GapBuffer *row, char *string, size_t length) {
    row->buffer_ = (char*) realloc(row->buffer_, row->buffer_size_ + length + 1);
    memcpy(&row->buffer_[row->buffer_size_], string, length);
    row->buffer_size_ += length;
    row->buffer_[row->buffer_size_] = '\0';
    //updateRow(row);
}

void Editor::DeleteChar() {
    if (cursor_y_ = num_rows_) return;
    if (cursor_x_ == 0 && cursor_y_ == 0) return;

    GapBuffer *row = &rows_[cursor_y_];
    if (cursor_x_ > 0) {
        //void deleteChar(int position);
        row->deleteChar(cursor_x_ -1);
        cursor_x_--;
    } else {
        cursor_x_ = row[cursor_y_ - 1].buffer_size_;
        AppendStringInRow(&rows_[cursor_y_ - 1], row->buffer_, row->buffer_size_);
        DeleteRow(cursor_y_);
        cursor_y_--;
    }
}
 
void Editor::RefreshScreen() {
    // update data member for cursor position
    UpdateCursor();

    // initialize screen contaienr
    ScreenContainer container;

    // hide the cursor while updating screen
    container.Append((char*) "\x1b[?25l", 6);

    // position cursor at the top-left corner
    container.Append((char*) "\x1b[H", 3);

    // update screen rows and columns
    SetWindowSize(&screen_rows_, &screen_cols_);
    
    // append in container the data
    DisplayRows(&container);

    // update the position of the cursor on screen
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (cursor_y_ - row_off_) + 1, 
                                              (cursor_x_ - col_off_) + 1);
    container.Append(buf, strlen(buf));

    // redisplay the cursor
    container.Append((char*)"\x1b[?25h", 6);

    // write the container in terminal screen
    write(STDOUT_FILENO, container.GetData(), container.GetSize());
}