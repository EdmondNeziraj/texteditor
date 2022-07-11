#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
using namespace std;

// when we type a character in terminal, this function will read that character and returns 
char keypress() {
  // stty --> Print or change terminal characteristics.
  system ("/bin/stty raw"); // The raw setting means that the input and output is not processed, just sent straight through.
  char c;
  system("/bin/stty -echo"); // dash means disable the setting and typing is not echoed back to us
  c = getc(stdin);
  system("/bin/stty echo"); // enables echo again
  system ("/bin/stty cooked"); // cannonical form, returns one line of data at a time from the terminal device.
  return c;
}


void editFile(string filename, string text) {
    ofstream file(filename);  
    bool saved = true;
    // use data structure here 
    string save;
    
  do {
    std::cout << "\033[2J\033[1;1H"; //clear

    if (saved) {
      save = "  |  File is saved";
    } else {
      save = "  |  File has unsaved changes";
    }
    std::cout << filename << "  |  Press ctrl + X to exit" << save << "\n____________________________________________________________________________\n" << text; //display text and header

    system("/bin/stty -raw"); // disables raw mode, the terminal device driver returns one character at a time without assembling lines of data. Also, special characters are not processed in this mode
    system("/bin/stty raw"); //change input
    char key = keypress(); //take input
    if (key == 24) {
         std::cout << "\033[2J\033[1;1H";
         break;
    }
    if (key == 96) {
      std::cout << "\033[2J\033[1;1H";
      break;
    } else if(key == 127) {
         text = text.substr(0, text.length() - 1);
    } else { //add char
      text += key;
      saved = false;
    }
  } while (true);
}

void editfileOnCreate(string filename) {
  std::cout << "\033[2J\033[1;1H";
  string text = "";
  editFile(filename, text); 
}

void readAndEditfile(string filename) {
  std::cout << "\033[2J\033[1;1H"; //clear
  ifstream readFile(filename); //open file to read
  string text;
  std::cout << filename << "  |  Read only mode  |  Press enter to exit \n___________________________________________________\n"; //header
  while (getline (readFile, text)) {
    cout << text << "\n"; //display
  }
  // allows to edit and save the file 
  editFile(filename, text);
}

int mains(int argc, char *argv[]) {
    // opens an empty text file, you can edit and save the file
    if (argc > 1) {
        // reads from a file and turn on edit and save feature
        readAndEditfile(argv[1]);
    } else {
        string fileName;
        std::cout << "Enter a name for the file: ";
        std::cin >> fileName;
        editfileOnCreate(fileName);
    }
    return 0;
}