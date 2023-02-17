# texteditor

A terminal-based text editor for Linux operating system. Support basic features such as creating a new file, opening an existing file, and modifying and saving changes. Users can copy, cut, and paste lines of text. Moreover, users can search for a word.

The project is built in C++ with two underlying data structures to organize and store the text. The main editor’s data structure is a dynamic array with routines that support inserting, appending, and deleting lines of text. Gap buffer data structure is used to store characters within a text line. It supports inserting, appending, and deleting characters. The search feature is implemented using a NFA-based algorithm for pattern matching.

