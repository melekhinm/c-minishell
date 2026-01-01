#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

const char *builtin_names[] = {
    "exit",
    "echo",
    "type",
    NULL
};

int execute_binary(char **args);

int shell_execute(char **args) {
    if (args == NULL) {
        return 1;
    }

    for (int i = 0; builtin_names[i] != NULL; i++) {
        if (!strcmp(args[0], builtin_names[i])) {
            return execute_builtin(args, i);
        }
    }

    return execute_binary(args);
}

int execute_binary(char **args) {
    pid_t pid;
    int status;

    pid = fork();

    if (pid == 0) {
        execvp(args[0], args);
        perror("shell");
        exit(1);
    } else if (pid < 0) {
        perror("shell");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) &&
                 !WIFSIGNALED(status) &&
                 !WIFSTOPPED(status));
    }

    return 1;
}