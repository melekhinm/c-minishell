int shell_exit();

int (*builtin_functions[]) (char **) = {
    &shell_exit
};

int execute_builtin(char **args, int id) {
    return (*builtin_functions[id]) (args);
}

int shell_exit() {
    return 0;
}