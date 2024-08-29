#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define TOKEN_DELIM " \t\r\n\a"

/* Function prototypes */
char **split_line(char *line);
int execute(char **args);
int launch(char **args);
char *get_location(char *command);
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);
int shell_env(char **args);

/* Built-in command functions */
extern char **environ;

#endif /* SHELL_H */
