#ifndef SHELL_CONTROLLER_H
#define SHELL_CONTROLLER_H

extern const char *builtin_names[];

char **parse_line(char *line);
int shell_execute(char **args);
int execute_builtin(char **args, int id);

#endif