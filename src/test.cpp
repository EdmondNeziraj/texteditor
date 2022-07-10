#include <iostream>
#include "gapBuffer.h"

int main() {
    GapBuffer gapBuffer = GapBuffer();
    
    char c;
    int position = 0;
    gapBuffer.display();
    while(1)  {
        std::cin >> c;

        if (c == 'q') {
            break;
        }
 
        gapBuffer.insert(c, position);
        gapBuffer.display();
        position++;
    }

   

    return 0;
}