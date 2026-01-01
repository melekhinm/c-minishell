#ifndef SHELL_CONTROLLER_H
#define SHELL_CONTROLLER_H

typedef struct {
    char *home_dir;
    char *path_env;
    char *line;
    char **args;
} environment_var;


extern const char *builtin_names[];

void parse_line(environment_var *env);
char *locate_executable(char *path_env, char *file);
int shell_execute(environment_var *env);
int execute_builtin(environment_var *env, int id);

#endif