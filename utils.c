#include "utils.h"

int split_string(const char *input, char delimiter, char ***tokens) {
    // Count the number of tokens
    int token_count = 1;
    for (const char *c = input; *c; c++) {
        if (*c == delimiter) {
            token_count++;
        }
    }

    // Allocate memory for the array of token pointers
    *tokens = (char **)malloc(token_count * sizeof(char *));
    if (!(*tokens)) {
        perror("split_string: Memory allocation failed");
        return -1;
    }

    char *copy = strdup(input); // Create a copy of the input string
    if (!copy) {
        perror("split_string: Memory allocation failed");
        free(*tokens);
        return -1;
    }

    int count = 0;
    char *token = strtok(copy, &delimiter);
    while (token) {
        (*tokens)[count] = strdup(token);
        if (!(*tokens)[count]) {
            perror("split_string: Memory allocation failed");
            free(copy);
            for (int i = 0; i < count; i++) {
                free((*tokens)[i]);
            }
            free(*tokens);
            return -1;
        }
        count++;
        token = strtok(NULL, &delimiter);
    }

    free(copy);
    return count;
}

// Frees memory allocated for the array of tokens
void free_tokens(char **tokens, int count) {
    for (int i = 0; i < count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}
