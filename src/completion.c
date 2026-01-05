#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include "shell.h"

char *command_generator(const char *text, int state);

char **my_completion(const char *text, int start, int end) {
    (void) start; (void) end;
    return rl_completion_matches(text, command_generator);
}

char *command_generator(const char *text, int state) {
    static int index, len;
    static DIR *current_dir;
    static char *path_env;
    static char *token;
    char *name;
    struct dirent *file;

    if (state == 0) {
        index = 0;
        len = strlen(text);
        if (path_env)
            free(path_env);
        path_env = strdup(getenv("PATH"));
        token = strtok(path_env, ":");
        if (current_dir) {
            closedir(current_dir);
            current_dir = NULL;
        }
    }

    if (index != -1) {
        while ((name = builtin_names[index++])) {
            if (!strncmp(name, text, len)) {
                return strdup(name);
            }
        }
        index = -1;
    }

    while (token || current_dir) {
        if (current_dir == NULL) {
            if (token == NULL)
                break;
            current_dir = opendir(token);
            token = strtok(NULL, ":");
            if (current_dir == NULL)
                continue; 
        }
        while ((file = readdir(current_dir))) {
            name = file->d_name;

            if (!strncmp(name, text, len)) {
                return strdup(name);
            }
        }
        closedir(current_dir);
        current_dir = NULL;
    }

    free(path_env);
    path_env = NULL;
    return NULL;
}