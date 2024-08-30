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

int shell_help(char **args __attribute__((unused)))
{
    printf("Simple Shell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("Built-in commands:\n");
    printf("  cd\n  help\n  exit\n  env\n  setenv\n  unsetenv\n  alias\n");
    return 1;
}

int shell_exit(char **args __attribute__((unused)))
{
    return 0;
}

int shell_env(char **args __attribute__((unused)))
{
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

int execute(char **args)
{
    int i;
    char *builtin_str[] = {
        "cd",
        "help",
        "exit",
        "env",
        "setenv",
        "unsetenv",
        "alias"
    };
    int (*builtin_func[]) (char **) = {
        &shell_cd,
        &shell_help,
        &shell_exit,
        &shell_env,
        &shell_setenv,
        &shell_unsetenv,
        &shell_alias
    };

    if (args[0] == NULL)
        return 1;

    for (i = 0; i < 7; i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
            return (*builtin_func[i])(args);
    }

    return launch(args);
}
