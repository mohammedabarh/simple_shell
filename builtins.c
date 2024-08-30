#include "shell.h"

int shell_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "shell: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("shell");
        }
    }
    return 1;
}

int shell_help(char **args)
{
    (void)args;
    printf("Simple Shell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("Built-in commands:\n");
    printf("  cd\n  help\n  exit\n  env\n  setenv\n  unsetenv\n  alias\n");
    return 1;
}

int shell_exit(char **args)
{
    (void)args;
    return 0;
}

int shell_env(char **args)
{
    (void)args;
    char **env = environ;
    while (*env) {
        printf("%s\n", *env);
        env++;
    }
    return 1;
}

int shell_setenv(char **args)
{
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
        return 1;
    }
    if (setenv(args[1], args[2], 1) != 0) {
        perror("shell");
    }
    return 1;
}

int shell_unsetenv(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
        return 1;
    }
    if (unsetenv(args[1]) != 0) {
        perror("shell");
    }
    return 1;
}
