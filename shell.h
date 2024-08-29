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
#include <limits.h>
#include <errno.h>

#define TOKEN_DELIM " \t\r\n\a"
#define BUFFER_SIZE 1024

typedef struct alias_s {
    char *name;
    char *value;
    struct alias_s *next;
} alias_t;

typedef struct {
    char **env;
    alias_t *aliases;
    int last_status;
} shell_data_t;

/* Function prototypes */
char *read_line(void);
char **split_line(char *line, char *delim);
char **split_logical_ops(char *line);
int execute(char **args, shell_data_t *shell_data);
int launch(char **args, shell_data_t *shell_data);

/* Builtin function prototypes */
int shell_cd(char **args, shell_data_t *shell_data);
int shell_exit(char **args, shell_data_t *shell_data);
int shell_env(char **args, shell_data_t *shell_data);
int shell_setenv(char **args, shell_data_t *shell_data);
int shell_unsetenv(char **args, shell_data_t *shell_data);
int shell_alias(char **args, shell_data_t *shell_data);

/* Helper function prototypes */
void initialize_shell_data(shell_data_t *shell_data);
void cleanup_shell_data(shell_data_t *shell_data);
char *get_env_value(const char *name, char **env);
int set_env_value(const char *name, const char *value, char ***env);
int unset_env_value(const char *name, char ***env);
int execute_file(const char *filename, shell_data_t *shell_data);
char *expand_variables(char *input, shell_data_t *shell_data);

#endif
