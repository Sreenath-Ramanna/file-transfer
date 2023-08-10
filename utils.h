#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Splits a string into an array of strings based on a delimiter
// Returns the number of tokens found
int split_string(const char *input, char delimiter, char ***tokens);

// Frees memory allocated for the array of tokens
void free_tokens(char **tokens, int count);

#endif // __UTILS_H__
