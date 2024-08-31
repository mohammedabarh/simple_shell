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

#define BUFFER_SIZE 1024
#define TOKEN_DELIM " \t\r\n\a"

extern char **environ;

// Function prototypes
char *read_line(void);
char **split_line(char *line);
int execute(char **args);
int launch(char **args);
char *get_location(char *command);
int is_builtin(char *command);
int execute_builtin(char **args);
int shell_cd(char **args);
int shell_exit(char **args);
int shell_env(char **args);
void print_error(char *command, char *message);
void sigint_handler(int sig);

#endif
