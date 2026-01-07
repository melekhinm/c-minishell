#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include "shell.h"

typedef enum {
    OUTSIDE_QUOTES,
    SINGLE_QUOTES,
    DOUBLE_QUOTES
} parsing_state;

char **check_for_pipeline(char *line) {
    int bufsize = 16;
    int position = 0;
    int length = strlen(line);
    char **pipes = malloc(sizeof(char *) * bufsize);
    if (pipes == NULL) {
        fprintf(stderr, "shell: Memory allocation error\n");
        return NULL;
    }
    char *current_token = NULL;
    parsing_state state = OUTSIDE_QUOTES;

    for (int i = 0; i <= length; i++) {
        char c = line[i];
        switch (state)
        {
        case OUTSIDE_QUOTES:
            if (!isspace(c) && c != '\0' && current_token == NULL) {
                current_token = &line[i];

                if (c == '\'')
                    state = SINGLE_QUOTES;
                if (c == '\"')
                    state = DOUBLE_QUOTES;
            }

            if ((c == '|' || c == '\0') && current_token != NULL) {
                line[i] = '\0';
                pipes[position++] = strdup(current_token);
                current_token = NULL;

                if (position >= bufsize) {
                    bufsize += 16;
                    char **temp = realloc(pipes, sizeof(char *) * bufsize);
                    if (temp == NULL) {
                        return NULL;
                    }
                    pipes = temp;
                }
            }
            break;
        
        case SINGLE_QUOTES:
            if (c == '\'')
                state = OUTSIDE_QUOTES;
            break;
        case DOUBLE_QUOTES:
            if (c == '\"')
                state = OUTSIDE_QUOTES;
        
        default:
            break;
        }
    }

    pipes[position] = NULL;

    for (int i = 0; pipes[i] != NULL; i++) {
        fprintf(stderr, "%s\n", pipes[i]);
    }
    
    return pipes;
}
