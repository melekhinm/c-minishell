#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <readline/history.h>
#include "shell.h"

char *get_history_filename(char *home) {
    char *filename = malloc(strlen(home) + strlen(".shell_history") + 2);
    if (filename == NULL) {
        fprintf(stderr, "shell: Memory allocation error\n");
        return NULL;
    }
    sprintf(filename, "%s/%s", home, ".shell_history");

    return filename;
}

void shell_read_history(char *home) {
    char *filename = get_history_filename(home);
    if (read_history(filename) != 0)
        perror("shell");
    free(filename);
}

void shell_write_history(char *home) {
    char *filename = get_history_filename(home);
    if (write_history(filename) != 0)
        perror("shell");
    if (history_truncate_file(filename, 10))
        perror("shell");

    free(filename);
}