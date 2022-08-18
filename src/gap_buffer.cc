
#include <string.h>
#include <iostream>

#include "gap_buffer.h"

enum highlight {
    HL_NORMAL = 0,
    HL_COMMENT,
    HL_KEYWORD,
    HL_DATA_TYPE,
    HL_STRING,
    HL_NUMBER,
    HL_FOUND
};

// Constructor
GapBuffer::GapBuffer() {
    buffer_size_ = 5;
    gap_size_ = 5;
    gap_buffer_ = new char[buffer_size_];

    for(int i = 0; i < buffer_size_; i++) {
        gap_buffer_[i] = ' ';
    }
    gap_left_ = 0;
    gap_right_ = gap_size_ - gap_left_ - 1;
    text_length_ = 0;
    text_buffer_ = new char[text_length_];
} 

// Destructor
GapBuffer::~GapBuffer() {
    delete[] gap_buffer_;
    gap_buffer_ = NULL;

    delete[] text_buffer_;
    text_buffer_ = NULL;
}

// Getters
char *GapBuffer::GetBuffer() {
    return gap_buffer_;
}

char *GapBuffer::GetTextBuffer() {
    for (int i = 0; i < gap_left_; i++) {
        text_buffer_[i] = gap_buffer_[i];
    }
    for (int i = gap_right_; i < buffer_size_; i++) {
        text_buffer_[i - gap_size_] = gap_buffer_[i];
    }

    return text_buffer_;
}

int GapBuffer::GetGapLeft() {
    return gap_left_;
}

int GapBuffer::GetGapRight() {
    return gap_right_;
}

int GapBuffer::GetGapSize() {
    return gap_size_;
}

int GapBuffer::GetBufferSize() {
    return buffer_size_;
}

int GapBuffer::GetTextLength() {
    return text_length_;
}

char *GapBuffer::GetCopiedText() {
    return copied_text;
}

int GapBuffer::GetCopiedSize() {
    return copied_char_size;
}

unsigned char *GapBuffer::GetHighlight() {
    return highlight_;
}

// Setters
void GapBuffer::SetBuffer(char *new_buffer) {
    gap_buffer_ = new_buffer;
}

void GapBuffer::SetTextBuffer(char *new_text) {
    text_buffer_ = new_text;
}

void GapBuffer::SetGapLeft(int new_gap_left) {
    gap_left_ = new_gap_left;
}

void GapBuffer::SetGapRight(int new_gap_right) {
    gap_right_ = new_gap_right;
}

void GapBuffer::SetGapSize(int new_gap_size) {
    gap_size_ = new_gap_size;
}

void GapBuffer::SetBufferSize(int new_buffer_size) {
    buffer_size_ = new_buffer_size;
}

void GapBuffer::SetTextLength(int new_text_length) {
    text_length_ = new_text_length;
}

void GapBuffer::SetHighlight(unsigned char *new_hl) {
    highlight_ = new_hl;
}

void GapBuffer::InsertLine(char *input, size_t length) {
    // allocate enough memory for the input
    gap_buffer_ = new char[length];

    // copy the input in the buffer
    memcpy(gap_buffer_, input, length);

    // update the data members
    buffer_size_ = length;
    text_length_ = length;
    gap_left_ = length;
    gap_right_ = length;
    gap_size_ = 0;
    //gap_buffer_[length] = ' ';

    text_buffer_ = gap_buffer_;
}

// Display buffer content
void GapBuffer::display() {
    for(int i = 0; i < buffer_size_; i++) {
        std::cout << gap_buffer_[i];
    }
    std::cout << std::endl;
}

//resize gap when its empty
void GapBuffer::growGap(int position) {
    char *temp_buffer = new char[buffer_size_];
    std::copy(gap_buffer_, gap_buffer_+buffer_size_, temp_buffer);

    gap_buffer_ = (char*) realloc(gap_buffer_, buffer_size_+5);

    // copy characters of buffer to temp_buffer[]

    for (int i = 0; i < position; i++) {
        gap_buffer_[i] = temp_buffer[i];
    }

    for (int i = position; i < position + 5; i++) {
        gap_buffer_[i] = ' ';
    }

    for (int i = position; i < buffer_size_; i++) {
        gap_buffer_[i+5] = temp_buffer[i];
    }

    delete[] temp_buffer;
    temp_buffer = NULL;
    
    buffer_size_ += 5;
    gap_right_ += 5;
    gap_size_ = 5;

}

// move the gap left in the buffer
void GapBuffer::left(int position) {
    while (position < gap_left_) {
        gap_left_--;
        gap_right_--;
        gap_buffer_[gap_right_ + 1] = gap_buffer_[gap_left_];
        gap_buffer_[gap_left_] = ' ';
    }
}

// move gap right in the buffer
void GapBuffer::right(int position) {
    while (position > gap_left_)
    {
        gap_left_++;
        gap_right_++;
        gap_buffer_[gap_left_ - 1] = gap_buffer_[gap_right_];
        gap_buffer_[gap_right_] = ' ';
    }
}

// Function to control the movement of gap
// by checking its position to the point of
// insertion
void GapBuffer::move_gap(int position)
{
    if (position < gap_left_)
    {
        left(position);
    }
    else
    {
        right(position);
    }
}



// insert char in the buffer at position
void GapBuffer::insert(char input, int position) {
    //std::cout << "position: " << position << std::endl;
    if (position != gap_left_) {
        //std::cout << "postition != gap_left" << std::endl;
        move_gap(position);
    }

    // Insert characters one by one

    // If the gap is empty grow the size
    if (gap_right_ <= gap_left_) {
        //std::cout << "grow gap " << std::endl;
        //int k = gap_size_;
        growGap(position);
    }

    // Insert the character in the gap and
    // move the gap
    gap_buffer_[gap_left_] = input;
    gap_left_++;
    text_length_++;
    gap_size_--;
}

// get the total count of the buffer data
int GapBuffer::count() {
    return text_length_;
}

// deleter char in the buffer at position
void GapBuffer::deleteChar(int position){
    // if position is not the gap then check and move the cursor to that point
    if (position + 1 != gap_left_) {
        move_gap(position + 1);
    }

    // reduce the gap by 1 position to the left
    gap_left_ -= 1;
    gap_buffer_[gap_left_] = ' ';
    gap_size_++;
    text_length_--;
}


void GapBuffer::ReallocateBufferMemory(char *buffer, size_t size) {
    gap_buffer_ = (char*) realloc(buffer, size);
}

void GapBuffer::EmptyBuffer() {
    delete[] gap_buffer_;
    gap_buffer_ = nullptr;
    gap_left_ = 0;
    gap_right_ = 0;
    gap_size_ = 0;
    buffer_size_ = 0;
    text_length_ = 0;
    delete[] copied_text;
    copied_text = nullptr;
    copied_char_size = 0;
    delete[] highlight_;
    highlight_ = nullptr;
}

void GapBuffer::ReallocateHLMemory(unsigned char *hl, size_t size) {
    highlight_ = (unsigned char*) realloc(hl, size);
}

void GapBuffer::SetHighlightAtIndex(int index, int value) {
    highlight_[index] = value;
}