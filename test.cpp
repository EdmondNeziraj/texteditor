// test.cpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

#include "src/terminal.h"
#include "src/keys.h"
#include "src/file.h"

using namespace std;

int main() {
    enableRawMode();
    initScreen();


    while(1) {
      refreshScreen();
      processKey();
    }


    // //opens an empty text file, you can edit and save the file
    // if (argc > 1) {
    //     // reads from a file and turn on edit and save feature
    //     readAndEditfile(argv[1]);
    // } else {
    //     string fileName;
    //     std::cout << "Enter a name for the file: ";
    //     std::cin >> fileName;
    //     editfileOnCreate(fileName);
    // }


    return 0;
}