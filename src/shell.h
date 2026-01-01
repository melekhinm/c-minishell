#ifndef SHELL_CONTROLLER_H
#define SHELL_CONTROLLER_H

typedef struct {
    char **path_dirs;
    char *home_dir;
    char *line;
    char **args;
} environment_var;


extern const char *builtin_names[];

void parse_line(environment_var *env);
int shell_execute(environment_var *env);
int execute_builtin(environment_var *env, int id);

#endif