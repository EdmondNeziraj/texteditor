//gapBuffer.h

#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H

#include <string.h>
#include <iostream>

class GapBuffer{
private:
    char *gap_buffer_;
    char *text_buffer_;
    int gap_left_;
    int gap_right_;
    int gap_size_;
    int buffer_size_;
    int text_length_;
    char *copied_text;
    int copied_char_size;
    unsigned char *highlight_;

public:
    // Getters
    char *GetBuffer();
    char *GetTextBuffer();
    int GetGapLeft();
    int GetGapRight();
    int GetGapSize();
    int GetBufferSize();
    int GetTextLength();
    char *GetCopiedText();
    int GetCopiedSize();
    unsigned char *GetHighlight();

    // Setters
    void SetBuffer(char *new_buffer);
    void SetTextBuffer(char *new_text);
    void SetGapLeft(int new_gap_left);
    void SetGapRight(int new_gap_right);
    void SetGapSize(int new_gap_size);
    void SetBufferSize(int new_buffer_size);
    void SetTextLength(int new_text_length);
    void SetHighlight(unsigned char *new_hl);

    /**
     * @brief Construct a new Gap Buffer object
     * 
     */
    GapBuffer();
    
    /**
     * @brief Destroy the Gap Buffer object
     * 
     */
    ~GapBuffer();

    /**
     * @brief Insert a whole line in the buffer.
     * 
     * @param input is the line to insert.
     * @param length is the length of the line to insert.
     */
    void InsertLine(char *input, size_t length);

    /**
     * @brief Display the content of the buffer.
     * 
     */
    void display();

    /**
     * @brief Resizes the gap when it is empty.
     * 
     * @param position is the position where the gap starts.
     */
    void growGap(int position);

    /**
     * @brief Moves the gap to the left.
     * 
     * @param position is position where to move to the left.
     */
    void left(int position);

    /**
     * @brief Move the gap to the right.
     * 
     * @param position is position where to move to the right.
     */
    void right(int position);

    /**
     * @brief Moves the gap.
     * 
     * @param position is where to move the gap.
     */
    void move_gap(int position);
    

    /**
     * @brief Inserts a character in given position.
     * 
     * @param input is the character to insert.
     * @param position is the position where to insert.
     */
    void insert(char input, int position);

    /**
     * @brief Counts the number of characters in the buffer.
     * 
     * @return int the number of characters in the buffer.
     */
    int count();

    /**
     * @brief Deletes a character in the given position.
     * 
     * @param position is the position where to delete the character.
     */
    void deleteChar(int position);

    /**
     * @brief Reallocates memory for the buffer.
     * 
     * @param buffer is the buffer to allocate memory for.
     * @param size is the size of the buffer.
     */
    void ReallocateBufferMemory(char *buffer, size_t size);

    /**
     * @brief Empties the buffer.
     * 
     */
    void EmptyBuffer();

    /**
     * @brief Reallocates memory for the highlight array.
     * 
     * @param hl is the array to allocate memory for.
     * @param size is the size of the array.
     */
    void ReallocateHLMemory(unsigned char *hl, size_t size);

    /**
     * @brief Set the highlight value at index position.
     * 
     * @param index is the index where to modify value.
     * @param value is the value to add in the array.
     */
    void SetHighlightAtIndex(int index, int value);
};

#endif