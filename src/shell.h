#ifndef SHELL_CONTROLLER_H
#define SHELL_CONTROLLER_H

char **parse_line(char *line);
int shell_execute(char **args);

#endif