//gapBuffer.h

#include <string.h>
#include <stdlib.h>

class gapBuffer {
    char buffer[50];
    int gap_size = 10;
    int gap_left = 0;
    int gap_right = gap_size - gap_left - 1;
    int size = 10;
    };

//resize gap when its empty
void growGap(int k, int position) {}

// move the gap left in the buffer
void left(int positon) {}

// move gap right in the buffer
void right(int position) {}

// insert char in the buffer at position
void insert(char input, int position) {}

// deleter char in the buffer at position
void deleteChar(int position){}