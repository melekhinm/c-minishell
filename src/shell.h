#ifndef SHELL_CONTROLLER_H
#define SHELL_CONTROLLER_H

/* enum specifically to tell parser or shell_execute about current state of
   redirection - telling a shell wheter to redirect output and what output
   to redirect. */
typedef enum {
    NOT_REDIRECTING,
    WRITING_STDOUT,
    WRITING_STDERR,
    APPENDING_STDOUT,
    APPENDING_STDERR
} redirection_state;

/* this is a struct for storing ALL of the info we may need in one place.
   home_dir - user's $HOME directory
   path_env - user's $PATH environment stored as a string
   line - string literal of the current command
   args - tokenized string of arguments to be executed
   ofile - filename of the file where output would be redirected to
   redirection - current state of redirection for this command */
typedef struct {
    char *home_dir;
    char *path_env;
    char *line;
    char **args;
    char *ofile;
    redirection_state redirection;
} environment_var;

/* array of builtin names used for comparing with input, located in execute.c */
extern char *builtin_names[];

/* These functions are supposed to act as connectors between different files. */

/* parser.c
   parse_line - create list of arguments from a string and set it for that env
   locate_executable - parse the path and connect it with executable's name. */
void parse_line(environment_var *env);
char *locate_executable(char *path_env, char *file);

/* execute.c
   shell_execute - get's the list of args and determines whether it is a builtin
   or an actual executable and sending it to the respective execution paths.
   NOTE: it only gets called for a single command. Piping is handled separately.
*/
int shell_execute(environment_var *env);

/* builtins.c
   execute_builtin - just sends env to function with a given ID.
   ID is used to identify a function in an array of function pointers. */
int execute_builtin(environment_var *env, int id);

/* completion.c
   my_completion - only acts as connector to connect readlines' own
   rl_completion_matches with my custom command generator. */
char **my_completion(const char *text, int start, int end);

/* pipe.c
   check_for_piplines - acts the same way as parser, determines the ammount of
   commands to be executed/piped, return NULL if there is only one command.
   execute_pipelines - this is equivalent of shell_execute if there is more than
   one command, a pipe. It works by createing "fake" environment_var for each
   command and then piping output from one into output of the other. */
char **check_for_pipeline(char *line);
int execute_pipelines(char **pipes, environment_var *origin);

/* main.c
   free_env - originally specific to main.c it was added here to be used in
   pipe.c to free every "fake" environment create there.*/
void free_env(environment_var *env);

/* history.c
   shell_read_history - reads history from a file using readline's read_history
   shell_write_history - writes current history to a file and truncates it
   NOTE: shell_read_history does not create file, although it will be created
   after the first exit, when the shell_write_history is called. As a result
   the user will see an error message during the first run. */
void shell_read_history(char *home);
void shell_write_history(char *home);

#endif