#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "shell.h"

char *command_generator(const char *text, int state);

char **my_completion(const char *text, int start, int end) {
    return rl_completion_matches(text, command_generator);
}

char *command_generator(const char *text, int state) {
    static int index, len;
    char *name;

    if (state == 0) {
        index = 0;
        len = strlen(text);
    }

    while ((name = builtin_names[index++])) {
        if (!strncmp(name, text, len))
            return strdup(name);
    }

    return NULL;
}