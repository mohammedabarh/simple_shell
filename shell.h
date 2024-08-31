#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <dirent.h>
#include <limits.h>
#include <ctype.h>  // Include for isalpha and isalnum functions

#define BUFFER_SIZE 1024
#define TOKEN_DELIM " \t\r\n\a"
#define COMMAND_DELIM ";&|"
#define HISTORY_SIZE 1000
#define ALIAS_SIZE 100

extern char **environ;
extern alias_t aliases[ALIAS_SIZE];  // Declare extern alias array

// Struct definitions
typedef struct alias {
    char *name;
    char *value;
} alias_t;

typedef struct {
    char **commands;
    char *operators;
} command_t;

// Function prototypes
char *read_line(void);
command_t *split_commands(char *line);
char **split_line(char *line);
int execute(command_t *commands);
int launch(char **args);
char *get_location(char *command);
int is_builtin(char *command);
int execute_builtin(char **args);
int shell_cd(char **args);
int shell_exit(char **args);
int shell_env(char **args);
int shell_setenv(char **args);
int shell_unsetenv(char **args);
int shell_alias(char **args);
int shell_history(char **args);
void print_error(char *command, char *message);
void sigint_handler(int sig);
char *replace_variables(char *str);
void remove_comments(char *str);
int handle_redirection(char **args);
int handle_pipes(char **args);
void add_to_history(char *command);
char *get_history_entry(int index);
void save_history(void);
void load_history(void);
void init_aliases(void);
void add_alias(char *name, char *value);
char *get_alias(char *name);
void save_aliases(void);
void load_aliases(void);

#endif

