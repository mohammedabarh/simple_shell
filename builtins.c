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
        if (dir == NULL)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return 1;
        }
        printf("%s\n", dir);
    }

    if (chdir(dir) != 0)
    {
        perror("cd");
        return 1;
    }

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", cwd, 1);
    }
    else
    {
        perror("getcwd");
    }

    return 1;
}

int shell_help(char **args)
{
    int i;
    char *builtin_str[] = {
        "cd",
        "help",
        "exit",
        "env",
        "setenv",
        "unsetenv"
    };
    (void)args;

    printf("Simple Shell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < 6; i++)
    {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int shell_exit(char **args)
{
    int status = 0;

    if (args[1] != NULL)
    {
        status = atoi(args[1]);
    }
    exit(status);
}

int shell_env(char **args)
{
    int i = 0;
    (void)args;

    while (environ[i])
    {
        printf("%s\n", environ[i]);
        i++;
    }
    return 1;
}

int shell_setenv(char **args)
{
    if (args[1] == NULL || args[2] == NULL)
    {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
        return 1;
    }

    if (setenv(args[1], args[2], 1) != 0)
    {
        fprintf(stderr, "Failed to set environment variable\n");
        return 1;
    }

    return 1;
}

int shell_unsetenv(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
        return 1;
    }

    if (unsetenv(args[1]) != 0)
    {
        fprintf(stderr, "Failed to unset environment variable\n");
        return 1;
    }

    return 1;
}
