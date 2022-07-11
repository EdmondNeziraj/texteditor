//gapBuffer.h

#include <string.h>
#include <stdlib.h>
#include <iostream>

class GapBuffer{
private:
    char *buffer;
    int gap_left;
    int gap_right;
    int gap_size;
    int size; 
    int length;


public:
    // Constructor
    GapBuffer() {
        std::cout << "Gap buffer" << std::endl; 
        size = 5;
        gap_size = 5;
        buffer = new char[size];

        for(int i = 0; i < size; i++) {
            buffer[i] = '_';
        }
        gap_left = 0;
        gap_right = gap_size - gap_left - 1;
        length = 0;
    }

    // test function
    void display() {
        for(int i = 0; i < size; i++) {
            std::cout << buffer[i] << " ";
        }
        std::cout << std::endl;
    }

    //resize gap when its empty
    void growGap(int k, int position) {
        char * copy_buffer = new char[size + k];

        // copy characters of buffer to copy_buffer[]
        for(int i = 0; i < position; i++) {
            copy_buffer[i] = buffer[i];
        }

        std::cout << "copy_buffer values: " << std::endl;
        for(int i = 0; i < size; i++) {
            std::cout << copy_buffer[i] << std::endl;
        }

        // Insert a gap of K from index position
        // gap is '_'
        for(int i = 0; i < k; i++) {
            copy_buffer[i + position] = '_';
        }

        // only re insert in the case where there is something left to re insert
        if (position <  gap_right) {
            for(int i = 0; i < position + k; i++) {
                copy_buffer[position + k + i] = buffer[i];
            }
        }
        

        buffer = copy_buffer;
        delete[] copy_buffer;
        copy_buffer = NULL;
        
        size += k;
        gap_right += k;

        std::cout << "buffer values: " << std::endl;
        for(int i = 0; i < size; i++) {
            std::cout << buffer[i] << std::endl;
        }
        
       
        
    }

    // move the gap left in the buffer
    void left(int position) {
        while (position < gap_left) {
            gap_left--;
            gap_right--;
            buffer[gap_right + 1] = buffer[gap_left];
            buffer[gap_left] = '_';
        }
    }

    // move gap right in the buffer
    void right(int position) {
        while (position > gap_left)
        {
            gap_left++;
            gap_right++;
            buffer[gap_left - 1] = buffer[gap_right];
            buffer[gap_right]='_';
        }
    }

    // Function to control the movement of gap
    // by checking its position to the point of
    // insertion
    void move_cursor(int position)
    {
        if (position < gap_left)
        {
            left(position);
        }
        else
        {
            right(position);
        }
    }
    
    

    // insert char in the buffer at position
    void insert(char input, int position) {
        std::cout << "position: " << position << std::endl;
        if (position != gap_left) {
            std::cout << "postition != gap_left" << std::endl;
            move_cursor(position);
        }
    
        // Insert characters one by one
    
            // If the gap is empty grow the size
            if (gap_right == gap_left) {
                std::cout << "grow gap " << std::endl;
                int k = gap_size;
                growGap(k, position);
            }
    
            // Insert the character in the gap and
            // move the gap
            buffer[gap_left] = input;
            gap_left++;

        length++;
    }

    // get the total count of the buffer data
    int count() {
        return length;
    }

    // deleter char in the buffer at position
    void deleteChar(int position){
        // if position is not the gap then check and move the cursor to that point
        if (position + 1 != gap_left) {
            move_cursor(position + 1);
        }

        // reduce the gap by 1 position to the left
        gap_left -= 1;
        buffer[gap_left] = '_';
    }
};





