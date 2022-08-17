#include "search.h"

int NextState(char *pat, int M, int state, int c)
{
    // If the character c is same as next character
    // in pattern,then simply increment state
    if (state < M && c == pat[state])
        return state+1;
 
    // next_state stores the result which is next state
    int next_state, i;
 
    // next_state finally contains the longest prefix
    // which is also suffix in "pat[0..state-1]c"
 
    // Start from the largest possible value
    // and stop when you find a prefix which
    // is also suffix
    for (next_state = state; next_state > 0; next_state--)
    {
        if (pat[next_state-1] == c)
        {
            for (i = 0; i < next_state-1; i++)
                if (pat[i] != pat[state-next_state+1+i])
                    break;
            if (i == next_state-1)
                return next_state;
        }
    }
 
    return 0;
}

void ComputeTransitionFunc(char *pat, int M, int TF[][NO_OF_CHARS])
{
    int state, x;
    for (state = 0; state <= M; ++state)
        for (x = 0; x < NO_OF_CHARS; ++x)
            TF[state][x] = NextState(pat, M, state, x);
}
 
vector<int> SearchDFA(char *pat, char *text)
{
    int M = strlen(pat);
    int N = strlen(text);
 
    int TransitionFunc[M+1][NO_OF_CHARS];
 
    ComputeTransitionFunc(pat, M, TransitionFunc);
 
    // Process text over Finite Automata.
    int i, state=0;
    vector<int> found_at;
    if (M == 0 or N == 0) return found_at;
    for (i = 0; i < N; i++)
    {
        state = TransitionFunc[state][text[i]];
        if (state == M)
            found_at.push_back(i-M+1);
    }

    return found_at;
}
