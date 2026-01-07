#ifndef SHELL_CONTROLLER_H
#define SHELL_CONTROLLER_H

typedef enum {
    NOT_REDIRECTING,
    WRITING_STDOUT,
    WRITING_STDERR,
    APPENDING_STDOUT,
    APPENDING_STDERR
} redirection_state;

typedef struct {
    char **args;
    char *ofile;
    redirection_state redirection;
    struct Command *next_command;
} Command;

typedef struct {
    char *home_dir;
    char *path_env;
    char *line;
    char **args;
    char *ofile;
    redirection_state redirection;
} environment_var;


extern char *builtin_names[];

void parse_line(environment_var *env);
char *locate_executable(char *path_env, char *file);
int shell_execute(environment_var *env);
int execute_builtin(environment_var *env, int id);
char **my_completion(const char *text, int start, int end);
char **check_for_pipeline(char *line);

#endif