#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "shell.h"

#define ARGS_SIZE 16

void parse_line(environment_var *env) {
    int bufsize = ARGS_SIZE;
    int length = strlen(env->line);
    env->args = malloc(sizeof(char *) * bufsize);
    if (env->args == NULL) {
        fprintf(stderr, "shell: Memory allocation error\n");
        return;
    }
    int position = 0;
    env->args[position++] = strdup(env->line);

    // Parsing loop
    for (int i = 0; i < length; i++) {
        // If met a whitespace, we can place a NULL terminator
        if (isspace(env->line[i])) {
            env->line[i] = '\0';
            // If the second char isn't whitespace, we add an argument
            if (!isspace(env->line[i + 1])) {
                env->args[position++] = strdup(&env->line[i + 1]);

                // Overflow handling
                if (position >= bufsize) {
                    bufsize += ARGS_SIZE;
                    char **temp = realloc(env->args, sizeof(char *) * bufsize);
                    if (temp == NULL) {
                        fprintf(stderr, "shell: Memory allocation error");
                        free(env->args);
                        env->args = NULL;
                        return;
                    }
                    env->args = temp;
                }
            }
        }
    }

    env->args[position] = NULL;
}