#ifndef EDITOR_H
#define EDITOR_H

#include <termios.h>
#include <vector>

#include "screen_container.h"
#include "gap_buffer.cc"

using namespace std;

class Editor {   
private: 
    int cursor_x_;                          // cursor position horizontally (in line)
    int cursor_y_;                          // cursor position vertically
    int row_off_;                           // number of rows off the screen
    int col_off_;                           // number of column off the screen
    int screen_rows_;                       // number of rows the screen has
    int screen_cols_;                       // number of columns the screen has
    int num_rows_;                          // total numbers of lines the text has
    GapBuffer *rows_;                       // a pointer of the array of gap buffers
    char *filename_;                        // the filename of the text we are editing
    char status_msg_[100];                  // status message of the editor
    char status_msg_time_;                  // time status message to be displayed
    struct Syntax *syntax;                  // the syntax for the editor
    char *copied_text_;                     // the text copied
    int copied_text_size_;                  // size of copied text

    // undo and redo 
    struct Pos {
        int start_pos;
        int end_pos;
    };
    vector<Pos> undo_stack_;             // stack to store latest commands
    vector<Pos> redo_stack_;             // stack to store latest commands we did undo

public:
    struct termios original_state_;         // the original state of the terminal


    // Getters
    int GetBufferCount();
    int GetCoordinateX();
    int GetCoordinateY();
    int GetNumRowsOffScreen();
    int GetNumColsOffScreen();
    int GetNumberOfRows();
    GapBuffer *GetRows();
    char *GetFilename();

    // Setters
    void SetCoordinateX(int cursor_x);
    void SetCoordinateY(int cursor_y);
    void SetNumRowsOffScreen(int rows_off);
    void SetNumColsOffScreen(int cols_off);
    void SetNumberOfRows(int number_of_rows);
    void SetRows(GapBuffer *new_rows);
    void SetFilename(char *filname);
    void SetStatusMessage(const char *msg, ...);

    /**
     * @brief Constructor of Editor class.
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
    void DrawRows(ScreenContainer *container);

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

    /**
     * @brief Converts all rows into an array of chars. 
     * We do this in order to save the file.
     * 
     * @param buffer_length is the parameter to update with number of chars in the array.
     * @return char* the array of characters.
     */
    char *RowsToString(int *buffer_length);

    /**
     * @brief Opens an existing file.
     * 
     * @param filename is the name of the file we want to open.
     */
    void OpenFile(char *filename);

    /**
     * @brief Saves the file we are modifying currently.
     * 
     */
    void SaveFile();

    /**
     * @brief Displays info such as filename, total number of lines,
     * and current line position of cursor.
     * 
     * @param buffer is the screen container to modify.
     */
    void StatusBar(ScreenContainer *buffer);

    /**
     * @brief Displays the message we have set.
     * 
     * @param container is the screen container to modify.
     */
    void DisplayMessage(ScreenContainer *container);

    /**
     * @brief Cuts a line of text.
     * 
     */
    void Cut();

    /**
     * @brief Copies a part of text.
     * 
     */
    void Copy();

    /**
     * @brief Pastes cut or copied text.
     * 
     */
    void Paste();

    /**
     * @brief Undo the last command or commands.
     * 
     */
    void Undo();

    /**
     * @brief Redo the last command after user pressed undo.
     * 
     */
    void Redo();

    /**
     * @brief Updates the parts of text we need to highlight.
     * 
     * @param row the buffer we want to update.
     */
    void UpdateSyntax(GapBuffer *row);

    /**
     * @brief Updates the value of color for each character.
     * 
     * @param highlight is the type of text we want ot highlight (comment, integer, etc).
     * @return int the value of color we want that char to be.
     */
    int ColorSyntax(int highlight);

    /**
     * @brief Highlights the syntax depending from the filetype.
     * 
     */
    void SelectSyntaxHighlight();

    /**
     * @brief Checks if it is a valid separator between a digit and surrounding chars.
     * 
     * @param character is the character.
     * @return true if it is a valid separator.
     * @return false if it is not a separator.
     */
    bool IsSeparator(int character);

    // keys
    /**
     * @brief Reads a key from the user.
     * 
     * @return int returns an integer value for the key.
     */
    int ReadKey();

    /**
     * @brief Moves the cursor depending from the value of key.
     * 
     * @param key is the value of key.
     */
    void MoveCursor(int key);

    /**
     * @brief Reads a key from the user and processes it.
     * It can be a command or an arrow to navigate the cursor.
     * 
     */
    void ProcessKey();

    /**
     * @brief Prompts the user to type.
     * 
     * @param prompt is the text of the prompt.
     * @return char* is the text the user has typed.
     */
    char *Prompt(char *prompt);

    /**
     * @brief Searches for a pattern within the text file.
     * 
     */
    void Search();
};

#endif // editor