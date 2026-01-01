#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "shell.h"

int shell_exit(environment_var *env);
int shell_echo(environment_var *env);
int shell_type(environment_var *env);
int shell_pwd(environment_var *env);
int shell_cd(environment_var *env);

int (*builtin_functions[]) (environment_var *env) = {
    &shell_exit,
    &shell_echo,
    &shell_type,
    &shell_pwd,
    &shell_cd
};

int execute_builtin(environment_var *env, int id) {
    return (*builtin_functions[id]) (env);
}

int shell_exit(environment_var *env) {
    (void) env;
    return 0;
}

int shell_echo(environment_var *env) {
    for (int i = 1; env->args[i] != NULL; i++) {
        if (i > 1)
            printf(" ");
        printf("%s", env->args[i]);
    }
    printf("\n");

    return 1;
}

int shell_type(environment_var *env) {
    if (env->args[1] == NULL)
        return 1;

    for (int i = 0; builtin_names[i] != NULL; i++) {
        if (!strcmp(builtin_names[i], env->args[1])) {
            printf("%s is a shell builtin\n", env->args[1]);
            return 1;
        }
    }

    char *full_path = locate_executable(env->path_env, env->args[1]);
    if (full_path != NULL) {
        printf("%s is %s\n", env->args[1], full_path);
        free(full_path);
        return 1;
    }

    printf("%s: not found\n", env->args[1]);

    return 1;
}

int shell_pwd(environment_var *env) {
    (void)env;

    char *pwd = getcwd(NULL, 0);

    printf("%s\n", pwd);

    free(pwd);
    return 1;
}

int shell_cd(environment_var *env) {
    if (env->args[1] == NULL) {
        if (chdir(env->home_dir) != 0) {
            perror("cd");
        }
        return 1;
    }

    struct stat statbuf;
    char *path = NULL;

    if (env->args[1][0] == '~') {
        path = malloc(strlen(env->home_dir) + strlen(env->args[1]));
        if (path == NULL) {
            fprintf(stderr, "shell: Memory allocation error\n");
            return 1;
        }

        sprintf(path, "%s%s", env->home_dir, &env->args[1][1]);
        fprintf(stderr, "%s\n", path);
    } else {
        path = env->args[1];
    }

    if (stat(path, &statbuf) != 0) {
        perror("cd");
        return 1;
    }

    if (S_ISDIR(statbuf.st_mode) == 0) {
        printf("cd: %s: No such file or directory\n", env->args[1]);
        return 1;
    }


    if (chdir(path) != 0) {
        perror("cd");
    }

    if (!strcmp(path, env->args[1]))
        return 1;

    free(path);
    return 1;
}
