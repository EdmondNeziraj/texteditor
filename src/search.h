#ifndef SEARCH_H
#define SEARCH_H

#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

#define NO_OF_CHARS 128

/**
 * @brief get the next state
 * 
 * @param pat - pattern to search
 * @param M - length of pattern
 * @param state - current state
 * @param x - the current num of char
 * @return int - return the state
 */
int NextState(char *pat, int M, int state, int c);
 
/**
 * @brief builds the transition function table that represents
 * the finite automata for the pattern
 * 
 * @param pat - is the pattern
 * @param M - is the length of the pattern
 * @param TF - is the 2D array of transition function
 */
void ComputeTransitionFunc(char *pat, int M, int TF[][NO_OF_CHARS]);
 
/**
 * @brief Searches for the pattern in a text
 * 
 * @param pat - the pattern we are searching
 * @param txt - the text we have
 * @return vector<int> found_at - vector containing 
 * indices of matches in the text
 */
vector<int> SearchDFA(char *pat, char *txt);

#endif