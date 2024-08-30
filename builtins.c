#include "shell.h"

int shell_cd(char **args)
{
    char *dir = args[1];
    char cwd[PATH_MAX];
    char *home = getenv("HOME");

    if (dir == NULL)
    {
        dir = home;
    }
    else if (strcmp(dir, "-") == 0)
    {
        dir = getenv("OLDPWD");
    }

    if (dir == NULL)
    {
        fprintf(stderr, "cd: No such file or directory\n");
        return 1;
    }

    if (chdir(dir) != 0)
    {
        perror("cd");
        return 1;
    }

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        setenv("PWD", cwd, 1);
    }

    return 1;
}

int shell_help(char **args)
{
    (void)args;
    printf("Simple Shell Help:\n");
    printf("cd [dir] - Change the current directory to dir or to HOME if dir is not provided.\n");
    printf("help - Display information about built-in commands.\n");
    printf("exit - Exit the shell.\n");
    printf("env - Print the current environment.\n");
    printf("setenv [var] [value] - Set an environment variable.\n");
    printf("unsetenv [var] - Unset an environment variable.\n");
    return 1;
}

int shell_exit(char **args)
{
    (void)args;
    return 0;
}

int shell_env(char **args)
{
    char **environ_ptr;
    (void)args;
    
    environ_ptr = environ;
    while (*environ_ptr)
    {
        printf("%s\n", *environ_ptr);
        environ_ptr++;
    }
    return 1;
}

int shell_setenv(char **args)
{
    if (args[1] == NULL || args[2] == NULL)
    {
        fprintf(stderr, "Usage: setenv VAR VALUE\n");
        return 1;
    }

    if (setenv(args[1], args[2], 1) != 0)
    {
        perror("setenv");
    }

    return 1;
}

int shell_unsetenv(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "Usage: unsetenv VAR\n");
        return 1;
    }

    if (unsetenv(args[1]) != 0)
    {
        perror("unsetenv");
    }

    return 1;
}
