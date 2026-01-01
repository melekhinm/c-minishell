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

int shell_execute(environment_var *env) {
    if (env->args == NULL) {
        return 1;
    }

    fprintf(stderr, "checking for builtins: %s\n", env->args[0]);
    for (int i = 0; builtin_names[i] != NULL; i++) {
        if (!strcmp(env->args[0], builtin_names[i])) {
            return execute_builtin(env, i);
        }
    }

    fprintf(stderr, "found none\n");
    return execute_binary(env->args);
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