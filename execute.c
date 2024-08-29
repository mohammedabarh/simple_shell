#include "shell.h"

int execute(char **args, shell_data_t *shell_data)
{
    if (args[0] == NULL)
        return 1;

    /* Check for aliases */
    alias_t *alias = shell_data->aliases;
    while (alias)
    {
        if (strcmp(alias->name, args[0]) == 0)
        {
            free(args[0]);
            args[0] = strdup(alias->value);
            break;
        }
        alias = alias->next;
    }

    /* Check for built-in commands */
    if (strcmp(args[0], "cd") == 0)
        return shell_cd(args, shell_data);
    if (strcmp(args[0], "exit") == 0)
        return shell_exit(args, shell_data);
    if (strcmp(args[0], "env") == 0)
        return shell_env(args, shell_data);
    if (strcmp(args[0], "setenv") == 0)
        return shell_setenv(args, shell_data);
    if (strcmp(args[0], "unsetenv") == 0)
        return shell_unsetenv(args, shell_data);
    if (strcmp(args[0], "alias") == 0)
        return shell_alias(args, shell_data);

    return launch(args, shell_data);
}

int launch(char **args, shell_data_t *shell_data)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        /* Child process */
        if (execvp(args[0], args) == -1)
        {
            perror("shell");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        /* Error forking */
        perror("shell");
    }
    else
    {
        /* Parent process */
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    shell_data->last_status = WEXITSTATUS(status);
    return 1;
}
