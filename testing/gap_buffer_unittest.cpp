#include <iostream>
#include "../src/editor.cc"
#include "../src/gap_buffer.h"
#include <string.h>
using namespace std;

// Assertion functions
// assert true function for unit test
void assertTrue(int value, int expectedValue, string error) {
    if (value == expectedValue) {
        cout <<  "Test case passed: True" << endl;
    } else {
        cout << "Test case Failed: " << " " << error << " " << endl;
    }
}

void assertTrue(string value, string expectedValue, string error) {
    if (value == expectedValue) {
        cout <<  "Test case passed: True" << endl;
    } else {
        cout << "Test case Failed: "  << " " << error << " " << endl;
    }
}



void assertTrue(float value, float expectedValue, string error) {
    if (value == expectedValue) {
        cout <<  "Test case Passed" << endl;
    } else {
        cout << "Test case Failed: "  << " " << error << " "  << endl;
    }
}
// Assertion function ends


// Test Functions
/**
 * @brief 
 * The following test functions test the behavior of the GapBuffer.
 * There is a test for insertion, deletion, cursor and grow gap.
 * Insertion Test
 *  -- we insert the character and assert it with the count of the gap buffer
 * 
 * Deletion Test
 *  -- we insert and delete the character and assert it with the count of the gap buffer
 * 
 * Cursor Test
 *  -- we insert the character at different position to check if the cursor is one unit above the position
 * 
 * Grow Gap Test
 *  -- we check if the gap is resizing itself by 5 unit   
 */

/**
 * @Test 
 * when a character is inserted to gap buffer, size should increase by 1
 */
void testInsertCharFunction() {
    Editor editor;// reference
    editor.InsertChar('a');
    cout << editor.GetRows()->GetBuffer();
    cout << editor.GetBufferCount();

    std::cout << "testInsertCharFunction: ";
    assertTrue(editor.GetBufferCount(), 1, "Editor did not insert the character.");
    
}

/**
 * @Test 
 * when a character is inserted, gap buffer sizes increases to 1
 * and after deletion the size should be 0.
 * the function test this behavior
 */
void testDeleteCharFunction() {
    Editor editor;
    editor.InsertChar('a');
    cout << editor.GetBufferCount();


    // [a], length = 1
    
    //std::cout << editor.getBufferCount() << std::endl;
    editor.DeleteChar();

    // [], length = 0
    //std::cout << editor.getBufferCount() << std::endl;
    cout << editor.GetBufferCount();

    // 0 != 1
    // 1 == 1
    std::cout << "testDeleteCharFunction: ";
    assertTrue(editor.GetBufferCount(), 0, "Editor did not delete the character.");
}

/**
 * @Test 
 * the cursor in the gap buffer increment as we start to insert character
 * insert char 'a' at position 0 --> after insertion cursor will move to position 1
 * insert char 'z' at position 3 --> after insertion will move to position 4
 * notice cursor moves  
 */
void testCursorInGapBuffer() {

    GapBuffer gapbuffer;
    gapbuffer.insert('a', 0);
    gapbuffer.insert('b', 2);
    
    std::cout << "testCursorInGapBuffer: ";
    assertTrue(gapbuffer.GetGapLeft(), 3, "Cursor at wrong position");

    gapbuffer.insert('s', 4);
    std::cout << "testCursorInGapBuffer: ";
    assertTrue(gapbuffer.GetGapLeft(), 5, "Cursor at wrong position");
}

/**
 * @Test 
 * when gap becomes full, gap buffer resizs iteself
 * initially gap buffer size is 5
 * after it becomes full, it resizes itself by 5
 * hence, the test should increase gap buffer size to 10
 */
void testGrowGapBuffer() {
    GapBuffer gapbuffer;
    gapbuffer.insert('a',0);
    gapbuffer.insert('a',1);
    gapbuffer.insert('a',2);
    gapbuffer.insert('a',3);
    gapbuffer.insert('a',4);
    gapbuffer.insert('a',5);

    //std::cout << gapbuffer.buffer_size_ << std::endl;
    std::cout << "testGrowGapBuffer: ";
    assertTrue(gapbuffer.GetBufferSize(), 10, "Gap buffer size did not increased.");
    
}


int main() {
    testInsertCharFunction();
    testDeleteCharFunction();
    testCursorInGapBuffer();
    testGrowGapBuffer();
   
    return 0;
}