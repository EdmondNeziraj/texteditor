#ifndef EDITOR_H
#define EDITOR_H

#include <termios.h>

#include "screen_container.h"
#include "gap_buffer.h"

using namespace std;

class Editor {    

public:
    int cursor_x_;
    int cursor_y_;
    int row_off_;
    int col_off_;
    int screen_rows_;
    int screen_cols_;
    int num_rows_;
    GapBuffer *rows_;
    struct termios original_state_;

    /**
     * @brief Construct a new Editor object.
     */
    Editor();

    /**
     * @brief Captures the window size that the terminal is currently.
     * and sets the screen_rows and screen_cols private members to that size.
     * 
     * @param rows is the member variable to set.
     * @param cols is the member variable to set.
     * @return -1 on failure, otherwise modify parameters passed and return 0.
     */
    int SetWindowSize(int *rows, int *cols);

    /**
     * @brief Appends a new row in the editor.
     * 
     * @param string is the string to append.
     * @param length is the length of the string.
     */
    void AppendRow(char *string, size_t len);

    /**
     * @brief Inserts a new buffer (text) in the editor.
     * 
     * @param position is the starting position where to make insertion.
     * @param buffer is the buffer to insert.
     * @param length is the length of the buffer.
     */
    void InsertBuffer(int at, char *buffer, size_t len);

    /**
     * @brief Fill screen container with all the data and escape sequences.
     * 
     * @param container is the screen container that will hold all the data.
     */
    void DisplayRows(ScreenContainer *container);

    /**
     * @brief Updates the container with recent changes and calls write().
     * 
     */
    void RefreshScreen();

    /**
     * @brief Updates cursor coordinates and row_off & col_off data memebers.
     * 
     */
    void UpdateCursor();

    /**
     * @brief Inserts a character in the buffer.
     * 
     * @param character is the character to insert.
     */
    void InsertChar(int character);

    /**
     * @brief Inserts a new line or buffer.
     * 
     */
    void InsertNewLine();

    /**
     * @brief Deletes a character from the buffer.
     * 
     */
    void DeleteChar();

    /**
     * @brief Empties the content of a buffer.
     * 
     * @param row is the buffer to be emptied.
     */
    void EmptyRow(GapBuffer *row);

    /**
     * @brief Deletes a row or buffer.
     * 
     * @param position is the position of row to be deleted.
     */
    void DeleteRow(int position);

    /**
     * @brief Appends a string in a row.
     * 
     * @param row is the row where the string will be appended.
     * @param string is the string to be appended.
     * @param length is the length of the string to be appended.
     */
    void AppendStringInRow(GapBuffer *row, char *string, size_t length);
};

#endif // editor