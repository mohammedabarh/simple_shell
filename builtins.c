#include "shell.h"

int is_builtin(char *command) {
    char *builtins[] = {"cd", "exit", "env", NULL};
    for (int i = 0; builtins[i]; i++) {
        if (strcmp(command, builtins[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int execute_builtin(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        return shell_cd(args);
    } else if (strcmp(args[0], "exit") == 0) {
        return shell_exit(args);
    } else if (strcmp(args[0], "env") == 0) {
        return shell_env(args);
    }
    return 1;
}

int shell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "shell: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("shell");
        }
    }
    return 1;
}

int shell_exit(char **args) {
    if (args[1] != NULL) {
        exit(atoi(args[1]));
    }
    exit(0);
}

int shell_env(char **args) {
    (void)args;
    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
    return 1;
}
