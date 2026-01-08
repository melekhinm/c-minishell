#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "shell.h"

char *builtin_names[] = {
    "exit",
    "echo",
    "type",
    "pwd",
    "cd",
    "history",
    NULL
};

int execute_binary(char **args);
int get_original_fd(redirection_state state);
int set_redirection(char *filename, redirection_state state);

int shell_execute(environment_var *env) {
    if (env->args == NULL) {
        return 1;
    }

    int original_fd = get_original_fd(env->redirection);
    int saved_state = set_redirection(env->ofile, env->redirection);

    int result = -1;
    for (int i = 0; builtin_names[i] != NULL; i++) {
        if (!strcmp(env->args[0], builtin_names[i])) {
            result = execute_builtin(env, i);
        }
    }
    if (result == -1)
        result = execute_binary(env->args);

    if (saved_state != -1) {
        dup2(saved_state, original_fd);
        close(saved_state);
    }

    return result;
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

int get_original_fd(redirection_state state) {
    switch (state)
    {
    case NOT_REDIRECTING:
        return -1;
    case WRITING_STDOUT: case APPENDING_STDOUT:
        return STDOUT_FILENO;
    case WRITING_STDERR: case APPENDING_STDERR:
        return STDERR_FILENO;
    default:
        return -1;
    }
}

int set_redirection(char *filename, redirection_state state) {
    int saved_state = -1;
    int file = -1;

    switch (state)
    {
    case NOT_REDIRECTING:
        break;
    case WRITING_STDOUT:
        saved_state = dup(STDOUT_FILENO);
        file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(file, STDOUT_FILENO);
        break;
    case APPENDING_STDOUT:
        saved_state = dup(STDOUT_FILENO);
        file = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
        dup2(file, STDOUT_FILENO);
        break;
    case WRITING_STDERR:
        saved_state = dup(STDERR_FILENO);
        file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(file, STDERR_FILENO);
        break;
    case APPENDING_STDERR:
        saved_state = dup(STDERR_FILENO);
        file = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
        dup2(file, STDERR_FILENO);
        break;
    default:
        break;
    }

    if (file != -1)
        close(file);
    return saved_state;
}