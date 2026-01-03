#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "shell.h"

#define ARGS_SIZE 16

typedef enum {
    REGULAR_PARSING,
    PARSING_SINGLE,
    PARSING_DOUBLE,
    ESCAPING_REGULAR,
    ESCAPING_DOUBLE
} parsing_state;

typedef enum {
    REMOVING_NOTHING,
    REMOVING_SINGLE,
    REMOVING_DOUBLE
} removal_state;

typedef struct {
    const char *redir_string;
    redirection_state state;
} redirection_map;

const redirection_map redir_array[] = {
    {">", WRITING_STDOUT},
    {"1>", WRITING_STDOUT},
    {"2>", WRITING_STDERR},
    {">>", APPENDING_STDOUT},
    {"1>>", APPENDING_STDOUT},
    {"2>>", APPENDING_STDERR},
    {NULL, NOT_REDIRECTING}
};

void remove_special_characters(char *string);

void parse_line(environment_var *env) {
    int bufsize = ARGS_SIZE;
    int length = strlen(env->line);
    env->args = malloc(sizeof(char *) * bufsize);
    if (env->args == NULL) {
        fprintf(stderr, "shell: Memory allocation error\n");
        return;
    }
    int position = 0;
    char *current_token = NULL;
    parsing_state current_state = REGULAR_PARSING;
    redirection_state current_redirection = NOT_REDIRECTING;

    for (int i = 0; i <= length; i++) {
        char c = env->line[i];

        switch (current_state) {
        case REGULAR_PARSING:
            if (!isspace(c) && c != '\0' && current_token == NULL) {
                current_token = &env->line[i];
                if (c == '\'')
                    current_state = PARSING_SINGLE;
                else if (c == '\"')
                    current_state = PARSING_DOUBLE;
            }



            else if ((isspace(c) || c == '\0') && current_token != NULL) {
                env->line[i] = '\0';

                /*
                * NOTE:
                * Consider implementing support for more than one redirect.
                * For now, assumption is of redirecting once, and that outfile
                * is the argument after the LAST redirection symbol. If looking
                * at bash, the desirable effect seems to be creation of every
                * file, but the output is indeed going to the last one ONLY.
                */

                if (current_redirection == NOT_REDIRECTING) {
                    env->args[position++] = strdup(current_token);

                    for (int i = 0; redir_array[i].redir_string != NULL; i++) {
                        if (!strcmp(redir_array[i].redir_string,
                            env->args[position - 1])) {
                                current_redirection = redir_array[i].state;
                                free(env->args[--position]);
                            }
                    }
                
                    if (current_redirection == NOT_REDIRECTING)
                        remove_special_characters(env->args[position - 1]);
                    current_token = NULL;
                }

                else {
                    env->ofile = strdup(current_token);
                    env->redirection = current_redirection;
                    current_redirection = NOT_REDIRECTING;
                    remove_special_characters(env->ofile);
                    current_token = NULL;
                }

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

            else if (c == '\'')
                current_state = PARSING_SINGLE;

            else if (c == '\"')
                current_state = PARSING_DOUBLE;

            else if (c == '\\')
                current_state = ESCAPING_REGULAR;

            break;
        
        case PARSING_SINGLE:
            if (c == '\'')
                current_state = REGULAR_PARSING;
            break;

        case PARSING_DOUBLE:
            if (c == '\"')
                current_state = REGULAR_PARSING;
            else if (c == '\\')
                current_state = ESCAPING_DOUBLE;
            break;
        
        case ESCAPING_REGULAR:
            current_state = REGULAR_PARSING;
            break;
        
        case ESCAPING_DOUBLE:
            current_state = PARSING_DOUBLE;
            break;

        default:
            break;
        }

    }

    if (current_state != REGULAR_PARSING) {
        env->args[position++] = strdup(current_token);
        current_token = NULL;
    }

    env->args[position] = NULL;
}

void remove_special_characters(char *string) {
    removal_state current_state = REMOVING_NOTHING;

    char *reader = string;
    char *writer = string;

    while (*reader != '\0') {
        switch (current_state)
        {
        case REMOVING_NOTHING:
            if (*reader == '\'')
                current_state = REMOVING_SINGLE;
            else if (*reader == '\"')
                current_state = REMOVING_DOUBLE;
            else if (*reader == '\\') {
                reader++;
                *writer = *reader;
                writer++;
            }
            else {
                *writer = *reader;
                writer++;
            }
            break;
        
        case REMOVING_SINGLE:
            if (*reader == '\'')
                current_state = REMOVING_NOTHING;
            else {
                *writer = *reader;
                writer++;
            }
            break;
        
        case REMOVING_DOUBLE:
            if (*reader == '\"')
                current_state = REMOVING_NOTHING;
            else if (*reader == '\\') {
                switch (*(reader + 1))
                {
                case '\\': case '\"': case '$': case '`':
                    reader++;
                    *writer = *reader;
                    writer++;
                    break;
                default:
                    *writer = *reader;
                    writer++;
                    break;
                }
            }
            else {
                *writer = *reader;
                writer++;
            }
            break;

        default:
            break;
        }

        reader++;
    }
    *writer = '\0';
} 

char *locate_executable(char *path_env, char *file) {
    if (path_env == NULL || file == NULL) {
        return NULL;
    }

    char *path_string = strdup(path_env);
    char *dir = strtok(path_string, ":");
    char *path_buffer = malloc(strlen(file) + strlen(path_string) + 2);

    while (dir != NULL) {
        sprintf(path_buffer, "%s/%s", dir, file);

        if (access(path_buffer, F_OK | X_OK) == 0) {
            free(path_string);
            return path_buffer;
        }

        dir = strtok(NULL, ":");
    }

    free(path_buffer);
    free(path_string);
    return NULL;
}
