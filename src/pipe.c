#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
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
        fprintf(stderr, "shell: pipe: Memory allocation error\n");
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

    if (position <= 1) {
        for (int i = 0; pipes[i] != NULL; i++) {
            free(pipes[i]);
        }
        free(pipes);
        return NULL;
    }

    return pipes;
}

int execute_pipelines(char **pipes, environment_var *origin) {
    int n_pipes = 0;
    while (pipes[n_pipes] != NULL)
        n_pipes++;

    environment_var *env_array[n_pipes];
    for (int i = 0; i < n_pipes; i++) {
        env_array[i] = malloc(sizeof(environment_var));
        if (env_array[i] == NULL) {
            fprintf(stderr, "shell: pipe: Memory allocation error\n");
            return 1;
        }
        env_array[i]->line = pipes[i];
        env_array[i]->path_env = strdup(origin->path_env);
        env_array[i]->home_dir = strdup(origin->home_dir);
        env_array[i]->args = NULL;
        env_array[i]->ofile = NULL;
        env_array[i]->redirection = NOT_REDIRECTING;

        parse_line(env_array[i]);
    }

    int pipefd[2];
    int prev_pipe_read = -1;
    pid_t pipe_pids[n_pipes];

    for (int i = 0; i < n_pipes; i++) {
        if (i < n_pipes - 1) {
            if (pipe(pipefd) == -1) {
                perror("shell: pipe");
                return 1;
            }
        }

        pipe_pids[i] = fork();

        if (pipe_pids[i] == 0) {
            if (prev_pipe_read != -1) {
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }

            if (i < n_pipes - 1) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
                close(pipefd[0]);
            }

            shell_execute(env_array[i]);
            
            exit(0); 
        }

        if (prev_pipe_read != -1) {
            close(prev_pipe_read);
        }

        if (i < n_pipes - 1) {
            prev_pipe_read = pipefd[0];
            close(pipefd[1]);
        }
    }

    for (int i = 0; i < n_pipes; i++) {
        waitpid(pipe_pids[i], NULL, 0);
    }

    for (int i = 0; i < n_pipes; i++) {
        free_env(env_array[i]);
        free(env_array[i]->home_dir);
        free(env_array[i]);
    }
    free(pipes);

    return 1;
}