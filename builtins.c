#include "shell.h"

int shell_cd(char **args, shell_data_t *shell_data)
{
    char *dir = args[1];
    char cwd[PATH_MAX];
    char *home = get_env_value("HOME", shell_data->env);

    if (dir == NULL)
    {
        dir = home;
    }
    else if (strcmp(dir, "-") == 0)
    {
        dir = get_env_value("OLDPWD", shell_data->env);
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
        set_env_value("OLDPWD", get_env_value("PWD", shell_data->env), &shell_data->env);
        set_env_value("PWD", cwd, &shell_data->env);
    }
    else
    {
        perror("getcwd");
    }

    return 1;
}

int shell_exit(char **args, shell_data_t *shell_data)
{
    int status = shell_data->last_status;

    if (args[1] != NULL)
    {
        status = atoi(args[1]);
    }
    cleanup_shell_data(shell_data);
    exit(status);
}

int shell_env(char **args, shell_data_t *shell_data)
{
    (void)args;
    for (int i = 0; shell_data->env[i]; i++)
    {
        printf("%s\n", shell_data->env[i]);
    }
    return 1;
}

int shell_setenv(char **args, shell_data_t *shell_data)
{
    if (args[1] == NULL || args[2] == NULL)
    {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
        return 1;
    }

    if (set_env_value(args[1], args[2], &shell_data->env) != 0)
    {
        fprintf(stderr, "Failed to set environment variable\n");
        return 1;
    }

    return 1;
}

int shell_unsetenv(char **args, shell_data_t *shell_data)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
        return 1;
    }

    if (unset_env_value(args[1], &shell_data->env) != 0)
    {
        fprintf(stderr, "Failed to unset environment variable\n");
        return 1;
    }

    return 1;
}

int shell_alias(char **args, shell_data_t *shell_data)
{
    if (args[1] == NULL)
    {
        /* Print all aliases */
        alias_t *current = shell_data->aliases;
        while (current)
        {
            printf("%s='%s'\n", current->name, current->value);
            current = current->next;
        }
    }
    else if (strchr(args[1], '=') == NULL)
    {
        /* Print specific aliases */
        for (int i = 1; args[i]; i++)
        {
            alias_t *alias = shell_data->aliases;
            while (alias)
            {
                if (strcmp(alias->name, args[i]) == 0)
                {
                    printf("%s='%s'\n", alias->name, alias->value);
                    break;
                }
                alias = alias->next;
            }
            if (!alias)
            {
                fprintf(stderr, "alias: %s not found\n", args[i]);
            }
        }
    }
    else
    {
        /* Set new aliases */
        for (int i = 1; args[i]; i++)
        {
            char *name = strtok(args[i], "=");
            char *value = strtok(NULL, "");
            if (name && value)
            {
                alias_t *new_alias = malloc(sizeof(alias_t));
                new_alias->name = strdup(name);
                new_alias->value = strdup(value);
                new_alias->next = shell_data->aliases;
                shell_data->aliases = new_alias;
            }
        }
    }
    return 1;
}
