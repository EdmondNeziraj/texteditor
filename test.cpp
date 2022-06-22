// test.cpp


#include <iostream>
#include "src/terminal.cpp"

int main() {
    enableRawMode();

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        std::cout << "char: " << c << "  ASCII: " << (int)c << std::endl; 
    }

    return 1;
}