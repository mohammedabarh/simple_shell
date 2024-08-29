#include "shell.h"

/**
 * shell_cd - Changes the current directory
 * @args: Arguments (args[1] is the directory)
 *
 * Return: 1 on success, 0 otherwise
 */
int shell_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "Expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("Error");
        }
    }
    return 1;
}

/**
 * shell_help - Prints help information
 * @args: Arguments (not used)
 *
 * Return: Always returns 1
 */
int shell_help(char **args)
{
    int i;
    char *builtin_str[] = {
        "cd",
        "help",
        "exit",
        "env"
    };
    (void)args;

    printf("Simple Shell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < 4; i++)
    {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

/**
 * shell_exit - Exits the shell
 * @args: Arguments (not used)
 *
 * Return: Always returns 0
 */
int shell_exit(char **args)
{
    int status = 0;

    if (args[1] != NULL)
    {
        status = atoi(args[1]);
    }
    exit(status);
}

/**
 * shell_env - Prints the current environment
 * @args: Arguments (not used)
 *
 * Return: Always returns 1
 */
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
