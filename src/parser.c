#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "shell.h"

#define ARGS_SIZE 16

char **parse_line(char *line) {
    int bufsize = ARGS_SIZE;
    int length = strlen(line);
    char **args = malloc(sizeof(char *) * bufsize);
    if (args == NULL) {
        fprintf(stderr, "shell: Memory allocation error\n");
        return args;
    }
    int position = 0;
    args[position++] = line;

    // Parsing loop
    for (int i = 0; i < length; i++) {
        // If met a whitespace, we can place a NULL terminator
        if (isspace(line[i])) {
            line[i] = '\0';
            // If the second char isn't whitespace, we add an argument
            if (!isspace(line[i + 1])) {
                args[position++] = &line[i + 1];

                // Overflow handling
                if (position >= bufsize) {
                    bufsize += ARGS_SIZE;
                    char **temp = realloc(args, sizeof(char *) * bufsize);
                    if (temp == NULL) {
                        fprintf(stderr, "shell: Memory allocation error");
                        free(args);
                        return NULL;
                    }
                    args = temp;
                }
            }
        }
    }

    args[position] = NULL;

    return args;
}