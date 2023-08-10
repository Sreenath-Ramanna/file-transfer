#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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


int main() {
    char *fileListCsv = "DartConfiguration.tcl,CMakeCache.txt,CTestTestfile.cmake,compile_commands.json,cmake_install.cmake,ft_client,CPackConfig.cmake,ft_server,Makefile,CPackSourceConfig.cmake";
    char **fileList = NULL;

    // Call split_string to tokenize the input
    int token_count = split_string(fileListCsv, ',', &fileList);
    if (token_count < 0) {
        fprintf(stderr, "Error splitting string\n");
        return 1;
    }

    // Print and free tokens
    for (int i = 0; i < token_count; i++) {
        printf("%d: %s\n", i + 1, fileList[i]);
        free(fileList[i]);
    }

    // Free the token array
    free(fileList);

    return 0;
}
