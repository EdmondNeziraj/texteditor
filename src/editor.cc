#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>


#include "editor.h"
#include "screen_container.cc" 
#include "search.cc"


#define HL_HL_NUMBERS (1<<0)
#define HL_HL_STRINGS (1<<1)
#define HLDB_ENRIES (sizeof(HLDB) / sizeof(HLDB[0]))
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

struct Syntax {
    char *filetype;
    char **filematch;
    char **keywords;
    char *single_line_comment;
    int flags;
};

char *C_HL_extensions[] = { (char*) ".h", (char*)".cpp", (char*) ".cc",  NULL};
char *C_HL_keywords[] = {
    (char*) "switch", (char*) "if", (char*) "while", (char*) "for", (char*) "break", (char*) "continue", (char*) "return", (char*) "else",
    (char*) "struct", (char*) "union", (char*) "typedef", (char*) "static", (char*) "enum", (char*) "class", (char*) "case",
    (char*) "int|", (char*) "long|", (char*) "double|", (char*) "float|", (char*) "char|", (char*) "unsigned|", (char*) "signed|",
    (char*) "void|", NULL
};

struct Syntax HLDB[] = {
    {
        (char*) ".cpp",
        C_HL_extensions,
        C_HL_keywords,
        (char*) "//",
        HL_HL_NUMBERS | HL_HL_STRINGS
    },
    
};

Editor::Editor() {
    cursor_x_ = 0;
    cursor_y_ = 0;
    row_off_ = 0;
    col_off_ = 0;
    num_rows_ = 0;
    rows_ = NULL;
    filename_ = NULL;
    status_msg_[0] = '\0';
    char status_msg_time_ = 0;
    SetWindowSize(&screen_rows_, &screen_cols_);
    syntax = NULL;
}

// Getters
int Editor::GetBufferCount() {
    return rows_->count();
}

int Editor::GetCoordinateX() {
    return cursor_x_;
}

int Editor::GetCoordinateY() {
    return cursor_y_;
}

int Editor::GetNumRowsOffScreen() {
    return row_off_;
}

int Editor::GetNumColsOffScreen() {
    return col_off_;
}

int Editor::GetNumberOfRows() {
    return num_rows_;
}

GapBuffer *Editor::GetRows() {
    return rows_;
}

char *Editor::GetFilename() {
    return filename_;
}


// Setters
void Editor::SetCoordinateX(int cursor_x) {
    cursor_x = cursor_x;
}
void Editor::SetCoordinateY(int cursor_y) {
    cursor_y_ = cursor_y;
}
void Editor::SetNumRowsOffScreen(int rows_off) {
    row_off_ = rows_off;
}
void Editor::SetNumColsOffScreen(int cols_off) {
    col_off_ = cols_off;
}

void Editor::SetNumberOfRows(int number_of_rows) {
    num_rows_ = number_of_rows;
}
void Editor::SetRows(GapBuffer *new_rows) {
    rows_ = new_rows;
}
void Editor::SetFilename(char *filename) {
    filename_ = filename;
}

void Editor::SetStatusMessage(const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    vsnprintf(status_msg_, sizeof(status_msg_), msg, ap);
    va_end(ap);
    status_msg_time_ = time(NULL);
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
        *rows = window_size.ws_row - 2;
        return 0;
    }
}

void Editor::InsertBuffer(int position, char *buffer, size_t length) {
    if (position < 0 || position > num_rows_) return;

    // allocate memory for the new buffer to insert
    rows_ = (GapBuffer*) realloc(rows_, sizeof(GapBuffer) * (num_rows_ + 1));

    // move all the data for one position, to make room for the new buffer
    memmove(&rows_[position+1], &rows_[position], sizeof(GapBuffer) * (num_rows_ - position));

    // place the new buffer in the position
    rows_[position].InsertLine(buffer, length);
    num_rows_++;
}

void Editor::UpdateCursor() {
    if (cursor_y_ < row_off_) {
        row_off_ = cursor_y_;
    }
    if (cursor_y_ >= row_off_ + screen_rows_ ) {
        row_off_ = cursor_y_ - screen_rows_ + 1;
    }
    if (cursor_x_ < col_off_) {
        col_off_ = cursor_x_;
    }
    if (cursor_x_ >= col_off_ + screen_cols_) {
        col_off_ = cursor_x_ - screen_cols_ + 1;
    }
}

void Editor::DrawRows(ScreenContainer *container) {
    for ( int y = 0; y < screen_rows_ ; y++) {
        int filerow = y + row_off_;

        // draw tildes (~) in the first positon
        // else if we have text, draw the text that is in the buffers 
        if (filerow >= num_rows_) {
            container->Append((char*)"~", 1);
        } else {
            int length = rows_[filerow].GetBufferSize() - col_off_;
            if (length < 0) {
                length = 0;
            }
            if (length > screen_cols_) {
                length = screen_cols_;
            }
            // below i am trying to make the syntax colored
            UpdateSyntax(&rows_[filerow]);
            char *c = &rows_[filerow].GetBuffer()[col_off_];
            unsigned char *hl = &rows_[filerow].GetHighlight()[col_off_];
            int current_color = -1;

            for (int i = 0; i < length; i++) {
                if (hl[i] == HL_NORMAL) {
                    // make sure we are using default text color before printing it
                    if (current_color != -1 ) {
                        container->Append((char*) "\x1b[39m", 5);
                        current_color = -1;
                    }
                    container->Append(&c[i], 1);
                } else {
                    // color the character if it is not part of normal
                    int color = ColorSyntax(hl[i]);
                    if (color != current_color) {
                        current_color = color;
                        char buf[16];
                        int clen = snprintf(buf, sizeof(buf), "\x1b[%dm", color);
                        container->Append(buf,clen);
                    }
                    container->Append(&c[i], 1);
                }
            } 

            // reset the color to default
            container->Append((char*) "\x1b[39m", 5);
            // above is testing

            //container->Append(&rows_[filerow].GetBuffer()[col_off_], length);
        }
        // clear the screen and add return carriage
        container->Append((char*) "\x1b[K", 3);
        container->Append((char*) "\r\n", 2);
    }
}

void Editor::InsertChar(int character) {
    // insert a character in the buffer at position cursor_x_
    if (cursor_y_ == num_rows_) {
        InsertBuffer(num_rows_, (char*)" ", 1);
    }
    rows_[cursor_y_].move_gap(cursor_x_);
    rows_[cursor_y_].insert(character, cursor_x_);
    cursor_x_++;
}

void Editor::InsertNewLine() {
    // if cursor is at position 0, just add a new line
    if (cursor_x_ == 0) {
        InsertBuffer(cursor_y_, (char*) " ", 1);
    } else {
        // get the pointer of line at cursor_y_
        GapBuffer *row = &rows_[cursor_y_];

        // insert the buffer from on the right of cursor_x_, at position cursor_y_ + 1
        InsertBuffer(cursor_y_ + 1, &row->GetBuffer()[cursor_x_], row->GetBufferSize() - cursor_x_);
        
        // update the row at position cursor_y_
        row = &rows_[cursor_y_];
        row->SetBufferSize(cursor_x_);
    }
    // increment coordinate y, set coordinate x to 0
    cursor_y_++;
    cursor_x_ = 0;
}

void Editor::EmptyRow(GapBuffer *row) {
    // empty the memory the buffer was occupying
    row->EmptyBuffer();
}

void Editor::DeleteRow(int position) {
    // filter out cases when we cannot delete rows
    if (position < 0 || position >= num_rows_) return;

    // empty the content of row in position
    EmptyRow(&rows_[position]);

    // overwrite the deleted row with the rows that come after it
    memmove(&rows_[position], &rows_[position + 1], sizeof(GapBuffer) * (num_rows_ - position - 1));
    num_rows_--;
}

void Editor::AppendStringInRow(GapBuffer *row, char *string, size_t length) {
    // reallocate memory for the additional string that we are appending
    row->ReallocateBufferMemory(row->GetBuffer(), row->GetBufferSize() + length);

    // copy the passed string into the end of the array
    memcpy(&row->GetBuffer()[row->GetBufferSize()], string, length);

    // update the length of the line
    row->SetBufferSize(row->GetBufferSize() + length);

    //updateRow(row);
}

void Editor::DeleteChar() {
    // filter out cases when we cannot delete characters
    // if (cursor_y_ = num_rows_) return;
    if (cursor_x_ == 0 && cursor_y_ == 0) return;

    GapBuffer *row = &rows_[cursor_y_];
    
    // this checks is the character we want to delete
    // is within the line or in the beginning of the line
    // if the char is within the line, we just delete it
    // else, we will move that line to the previous line and then delete the row we were 
    if (cursor_x_ > 0) {
        row->deleteChar(cursor_x_ - 1);
        cursor_x_--;
    } else {
        cursor_x_ = row[cursor_y_ - 1].GetBufferSize();
        AppendStringInRow(&rows_[cursor_y_ - 1], row->GetBuffer(), row->GetBufferSize());
        DeleteRow(cursor_y_);
        cursor_y_--;
    }
}

void Editor::StatusBar(ScreenContainer *container) {
    // append the escape sequence "\x1b[7m" to make the row white
    container->Append((char*) "\x1b[7m", 4);

    // initialize arrays that will hold the status of the file
    // it will display the name of the file and the total lines the file has
    // it will be displayed in the left side of the terminal
    char status[80];

    // it will display the line the cursor is slash the total number of lines
    // this will be displayed in the right side of the terminal
    char r_status[80];

    // get the length of the array to be displayed on the left side
    int length = snprintf(status, sizeof(status), "%.20s - %d lines",
        filename_ ? filename_ : "New file", num_rows_);
    
    // get the length of the array to be displayed on the right side
    int r_length = snprintf(r_status, sizeof(r_status), "%d/%d",
                                        cursor_y_ + 1, num_rows_);

    // set the length of the array as wide as the terminal width, if it is wider
    if (length > screen_cols_) {
        length = screen_cols_;
    }

    container->Append(status, length);

    while (length < screen_cols_) {
        // if we have the exact ro0m for the second array, append it
        // else add empty chars
        if ( screen_cols_- length == r_length) {
            container->Append(r_status, r_length);
            break;
        } else {
            container->Append((char*) " ", 1);
            length++;
        }
    }

    container->Append((char*) "\x1b[m", 3);
    container->Append((char*) "\r\n", 2);
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
    DrawRows(&container);
    StatusBar(&container);
    DisplayMessage(&container);

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

char *Editor::RowsToString(int *buffer_length) {
    int total_length = 0;

    // find the total length of the text file
    // it addes the size of the buffer for each row
    for (int i = 0; i < num_rows_; i++) {
        total_length += rows_[i].GetBufferSize() + 1;
    }

    // save the total length of buffer in our passed parameter
    *buffer_length = total_length;

    // allocate memory for char array of the length of all
    char *buffer = (char*) malloc(total_length);
    char *p = buffer;

    // copy the content of each row into our array and then return the array
    for (int i = 0; i < num_rows_; i++) {
        memcpy(p, rows_[i].GetBuffer(), rows_[i].GetBufferSize());
        p += rows_[i].GetBufferSize();
        *p = '\n';
        p++;
    }

    return buffer;
}

void Editor::OpenFile(char *filename) {
    filename_ = filename;
    SelectSyntaxHighlight();

    // get the file pointer from the text file, read mode
    FILE *file_pointer = fopen(filename_, "r");

    // initialize the character array for the lines to read
    char *line = NULL;
    size_t linecap = 0;
    ssize_t line_length;

    // read in each line from the file, and insert it into the rows_ buffer
    while ((line_length = getline(&line, &linecap, file_pointer))!= -1) {
        while (line_length > 0 && (line[line_length - 1] == '\n' ||
                                line[line_length - 1] == '\r')) {
            line_length--;
        }
        InsertBuffer(num_rows_, line, line_length);
    }
    delete line;
    line = NULL;
    fclose(file_pointer);
}


void Editor::SaveFile() {
    if (filename_ == NULL) {
        filename_ = Prompt((char*)"Filename to save as: %s (ESC to cancel)");
    if (filename_ == NULL) {
      SetStatusMessage("Save cancelled!");
      return;
    }
    SelectSyntaxHighlight();
  }

    int length;
    // read the data from rows and set the length of the buffer
    char *buffer = RowsToString(&length);

    // if filename is not null, open file to read and write (ORDWR)
    // if file does not exist, create a file (O_CREAT)
    int file = open(filename_, O_RDWR | O_CREAT, 0644);

    // set file size to length
    ftruncate(file, length);

    // write the content of the buffer in file
    if (write(file, buffer, length) == length) {
        close(file);
        SetStatusMessage("Saved! Wrote %d lines", num_rows_);
        return;
    }
    delete buffer;
    buffer = NULL;
}

void Editor::Copy() {

    // saving the character from start position to end position in a seperate array
    int size = rows_[cursor_y_].GetBufferSize();
    copied_text_ = new char[size];
    for (int i = 0; i < size; i++) {
        copied_text_[i] = rows_[cursor_y_].GetBuffer()[i];
    }
    copied_text_size_ = size;
}

void Editor::Cut() {

    // saving the character from start position to end position in a seperate array
    int size = rows_[cursor_y_].GetBufferSize();
    copied_text_ = new char[size];
    for (int i = 0; i < size; i++) {
        copied_text_[i] = rows_[cursor_y_].GetBuffer()[i];
    }
    copied_text_size_ = size;

    DeleteRow(cursor_y_);
    cursor_x_ = 0;
}

void Editor::Paste() {
    // insert the character starting from this startPostion into the gap buffer 
    // for that we need to make gap inisde gap buffer if there are elements in that buffer 
    // this is already handled by the insertion function we wrote above
    
    for (int i = 0; i < copied_text_size_; i++) {
        //insert(copied_text[index], i);
        InsertChar(copied_text_[i]);
    }
}

// undo and redo unfinished.

// void Editor::Undo() {
//     Pos to_remove = undo_stack_.pop_back();
    
//     redo_stack_.push_back(to_remove);
// }

// void Editor::Redo() {
//     Pos old_element = redo_stack_.pop_back();
//     undo_stack_.push_back(old_element);

// }


void Editor::UpdateSyntax(GapBuffer *row) {
    // reallocate memory for the highlight array
    row->ReallocateHLMemory(row->GetHighlight(), row->GetBufferSize());

    // set all characters to HL_NORMAL by default
    memset(row->GetHighlight(), HL_NORMAL, row->GetBufferSize());

    if (syntax == NULL) return;

    char **keywords = syntax->keywords;

    char *scs = syntax->single_line_comment;
    int scs_length = scs ? strlen(scs) : 0;

    int prev_sep = 1;
    int in_string = 0;
 
    // set all digits to HL_NUMBER
    char c;
    int i = 0;
    while (i < row->GetBufferSize()) {
        c = row->GetBuffer()[i];
        unsigned char prev_hl = (i > 0) ? row->GetHighlight()[i - 1] : HL_NORMAL;

        // highlights comments
        if (scs_length && !in_string) {
            if (!strncmp(&row->GetBuffer()[i], scs, scs_length)) {
                memset(&row->GetHighlight()[i], HL_COMMENT, row->GetBufferSize() - i);
                break;
            }
        }
        
        // highlights strings
        if (syntax->flags & HL_HL_STRINGS) {
            if (in_string) {
                rows_->SetHighlightAtIndex(i, HL_STRING);
                if (c == '\\' && i + 1 < rows_->GetBufferSize()) {
                    rows_->SetHighlightAtIndex(i+1, HL_STRING);
                    i += 2;
                    continue;
                }
                if (c == in_string) {
                    in_string = 0;
                }
                i++;
                prev_sep = 1;
                continue;
            } 
            // this is not working for some reason
            /* else {
                if (c == '\'' || c == '\"') {
                    in_string = c;
                    rows_->SetHighlightAtIndex(i, HL_STRING);
                    i++;
                    continue;
                } 
            } */
        }

        // highlights digits/numbers
        if (syntax->flags & HL_HL_NUMBERS) {
            if ((isdigit(c) && (prev_sep || prev_hl == HL_NUMBER)) || 
                (c == '.' && prev_hl == HL_NUMBER)) {
                    
                row->SetHighlightAtIndex(i, HL_NUMBER);
                i++;
                prev_sep = 0;
                continue;
            }
        }

        // highlights datatypes or keywords
        if (prev_sep) {
        int j;
        for (j = 0; keywords[j]; j++) {
            int klen = strlen(keywords[j]);
            int kw2 = keywords[j][klen - 1] == '|';
            if (kw2) klen--;
            if (!strncmp(&row->GetBuffer()[i], keywords[j], klen) &&
                IsSeparator(row->GetBuffer()[i + klen])) {
                memset(&row->GetHighlight()[i], kw2 ? HL_DATA_TYPE : HL_KEYWORD, klen);
                i += klen;
                break;
            }
        }
        if (keywords[j] != NULL) {
            prev_sep = 0;
            continue;
        }
        }
        
        prev_sep = IsSeparator(c);
        i++;
    }
}

//
int Editor::ColorSyntax(int highlight) {
    switch (highlight)
    {
        case HL_COMMENT: return 32;
        case HL_KEYWORD: return 95;
        case HL_DATA_TYPE: return 34;
        case HL_STRING: return 93;
        case HL_NUMBER: return 33;
        case HL_FOUND: return 91;
        default: return 37;
    }
}

void Editor::DisplayMessage(ScreenContainer *container) {
    // first we erase everything in that line
    container->Append((char*) "\x1b[K", 3);

    int mgs_length = strlen(status_msg_);

    // depending from the length of the message
    // we display either all the message or as wide as the screen is
    if (mgs_length > screen_cols_) {
        mgs_length = screen_cols_;
    }
    container->Append(status_msg_, mgs_length);
}

void Editor::SelectSyntaxHighlight() {
    syntax = NULL;
    if (filename_ == NULL) return;

    char *ext = strrchr(filename_, '.');

    for (unsigned int i = 0; i < HLDB_ENRIES; i++) {
        struct Syntax *s = &HLDB[i];
        unsigned int j = 0;
        // depending from the file type
        // we highlight the syntax
        while (s->filematch[j]) {
            int is_ext = (s->filematch[j][0] == '.');
            if ((is_ext && ext && !strcmp(ext, s->filematch[j])) ||
                (!is_ext && strstr(filename_, s->filematch[j]))) {
                    syntax = s;
                    
                    int filerow;
                    for (filerow = 0; filerow < num_rows_; filerow++) {
                        UpdateSyntax(&rows_[filerow]);
                    }

                    return;
            }
            j++;
        }
    }
}

bool Editor::IsSeparator(int c) {
    // 
    return isspace(c) || c == '\0' || strchr(",.()+-/*=~%<>[];", c) != NULL;
}

// read a key input from the user
int Editor::ReadKey() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {}
    if (c == '\x1b') {
        char seq[3];

        if (read(STDIN_FILENO, &seq[0], 1) != 1 || 
            read(STDIN_FILENO, &seq[1], 1) != 1) {
            return '\x1b';
        }

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
void Editor::MoveCursor(int key) {
    GapBuffer *row = (cursor_y_ >= num_rows_) ? NULL : &rows_[cursor_y_];

    switch(key) {
        case ARROW_LEFT:
            if (cursor_x_ != 0) {
                cursor_x_--;

            } else if (cursor_y_ > 0) {
                cursor_y_--;
                cursor_x_ = rows_[cursor_y_].GetBufferSize();
            }
            break;
        case ARROW_RIGHT:
            if (row && cursor_x_ < row->GetBufferSize()) {
                cursor_x_++;
            } else if (row && cursor_x_ == row->GetBufferSize()) {
                cursor_y_++;
                cursor_x_ = 0;
            }
            break;
        case ARROW_UP:
            if (cursor_y_ != 0) {
                cursor_y_--;
            }
            break;
        case ARROW_DOWN:
            if (cursor_y_ < num_rows_ - 1) {
                cursor_y_++;
            }
            break;
    }

    row = (cursor_y_ >= num_rows_) ? NULL : &rows_[cursor_y_];
    int row_length = row ? row->GetBufferSize() : 0;
    if (cursor_x_ > row_length) {
        cursor_x_ = row_length;
    }
}

// process key typed from the user
void Editor::ProcessKey() {
    int k = ReadKey();

    switch (k) {
        case '\r':
            InsertNewLine();
            break;

        case CTRL_KEY('q'):
            exit(0);
            break;

        case CTRL_KEY('s'):
            SaveFile();
            break;
        case ARROW_LEFT:
        case ARROW_RIGHT:
        case ARROW_UP:
        case ARROW_DOWN:
            MoveCursor(k);
            break;
        
        case PAGE_UP:
        case PAGE_DOWN:
            { int times = screen_rows_;
                while(times--) {
                    MoveCursor(k == PAGE_UP ? ARROW_UP : ARROW_DOWN);
                }
            }
        case HOME_KEY:
            cursor_x_ = 0;
            break;
        
        case END_KEY:
            if (cursor_y_ < num_rows_) {
                cursor_x_ = rows_[cursor_y_].GetBufferSize();
            }
            break;

        case CTRL_KEY('f'):
            Search();
            break;

        case CTRL_KEY('x'):
            Cut();
            break;
        case CTRL_KEY('c'):
            Copy();
            break;
        case CTRL_KEY('v'):
            Paste();
            break;

        case BACKSPACE:
        case CTRL_KEY('h'):
        case DEL_KEY:
            if (k == DEL_KEY) {
                MoveCursor(ARROW_RIGHT);
            }
            DeleteChar();
            break;


        case CTRL_KEY('l'):
        case '\x1b':
            break;

        default:
            InsertChar(k);
            break;
    }
}

char *Editor::Prompt(char *prompt) {
    size_t bufsize = 128;
    char *buffer = (char*) malloc(bufsize);

    size_t buflen = 0;
    buffer[0] = '\0';

    while (1) {
        SetStatusMessage(prompt, buffer);
        RefreshScreen();

        int c = ReadKey();
        if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
            if (buflen != 0) buffer[--buflen] = '\0';
        } else if (c == '\x1b') {
            SetStatusMessage("");
            delete[] buffer;
            buffer = nullptr;
            return NULL;
        } else if (c == '\r') {
            if (buflen != 0) {
                SetStatusMessage("");
                return buffer;
            }  
        } else if (!iscntrl(c) && c < 128) {
            if (buflen == bufsize - 1) {
            bufsize *= 2;
            buffer = (char*) realloc(buffer, bufsize);
        }
        buffer[buflen++] = c;
        buffer[buflen] = '\0';
    }

  }
}


void Editor::Search() {
    char *query = Prompt((char*) "Search: %s (ESC to cancel)");
    if (query == NULL) return;
    int i;
    for (i = 0; i < num_rows_; i++) {
        GapBuffer *row = &rows_[i];
        vector<int> match = SearchDFA(query, row->GetBuffer());
        if (match.size()) {
            cursor_y_ = i;
            cursor_x_ = match[0];
            row_off_ = num_rows_;

            memset(&row->GetHighlight()[match[0]], HL_FOUND, strlen(query));
            break;
        }
    }
    delete[] query;
    query = nullptr;
}