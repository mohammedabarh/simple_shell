#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>

#define BUFFER_SIZE 1024
#define TOKEN_DELIM " \t\r\n\a"
#define MAX_ALIASES 100

extern char **environ;
extern int status;

// Function prototypes
char *read_line(void);
char **split_line(char *line, char *delim);
int execute(char **args);
int launch(char **args);
void expand_variables(char **args);
char **split_logical_ops(char *line);
void process_file(char *filename);

// Builtin function prototypes
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);
int shell_env(char **args);
int shell_setenv(char **args);
int shell_unsetenv(char **args);
int shell_alias(char **args);

// Alias function prototypes
void add_alias(char *name, char *value);
char *get_alias(char *name);
void print_aliases(void);

typedef struct {
    char *name;
    char *value;
} Alias;

extern Alias aliases[MAX_ALIASES];
extern int alias_count;

#endif
