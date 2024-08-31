#include "shell.h"

int is_builtin(char *command) {
    char *builtins[] = {"cd", "exit", "env", "setenv", "unsetenv", "alias", "history", NULL};
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
    } else if (strcmp(args[0], "setenv") == 0) {
        return shell_setenv(args);
    } else if (strcmp(args[0], "unsetenv") == 0) {
        return shell_unsetenv(args);
    } else if (strcmp(args[0], "alias") == 0) {
        return shell_alias(args);
    } else if (strcmp(args[0], "history") == 0) {
        return shell_history(args);
    }
    return 1;
}

int shell_cd(char **args) {
    char *home_dir;
    char current_dir[PATH_MAX];

    if (args[1] == NULL) {
        home_dir = getenv("HOME");
        if (home_dir == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
        if (chdir(home_dir) != 0) {
            perror("cd");
            return 1;
        }
    } else if (strcmp(args[1], "-") == 0) {
        char *prev_dir = getenv("OLDPWD");
        if (prev_dir == NULL) {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return 1;
        }
        if (chdir(prev_dir) != 0) {
            perror("cd");
            return 1;
        }
        printf("%s\n", prev_dir);
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd");
            return 1;
        }
    }

    if (getcwd(current_dir, sizeof(current_dir)) != NULL) {
        setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", current_dir, 1);
    } else {
        perror("getcwd");
        return 1;
    }

    return 0;
}

int shell_exit(char **args) {
    int status = 0;
    if (args[1] != NULL) {
        status = atoi(args[1]);
    }
    exit(status);
}

int shell_env(char **args) {
    (void)args;
    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
    return 0;
}

int shell_setenv(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
        return 1;
    }
    if (setenv(args[1], args[2], 1) != 0) {
        perror("setenv");
        return 1;
    }
    return 0;
}

int shell_unsetenv(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
        return 1;
    }
    if (unsetenv(args[1]) != 0) {
        perror("unsetenv");
        return 1;
    }
    return 0;
}

int shell_alias(char **args) {
    if (args[1] == NULL) {
        // Print all aliases
        for (int i = 0; i < ALIAS_SIZE; i++) {
            if (aliases[i].name != NULL) {
                printf("%s='%s'\n", aliases[i].name, aliases[i].value);
            }
        }
    } else if (args[2] == NULL) {
        // Print specific alias
        char *value = get_alias(args[1]);
        if (value != NULL) {
            printf("%s='%s'\n", args[1], value);
        } else {
            fprintf(stderr, "Alias not found: %s\n", args[1]);
            return 1;
        }
    } else {
        // Set alias
        char *name = args[1];
        char *value = args[2];
        for (int i = 3; args[i] != NULL; i++) {
            value = strcat(value, " ");
            value = strcat(value, args[i]);
        }
        add_alias(name, value);
    }
    return 0;
}

int shell_history(char **args) {
    (void)args;
    for (int i = 0; i < HISTORY_SIZE; i++) {
        char *entry = get_history_entry(i);
        if (entry != NULL) {
            printf("%5d  %s\n", i + 1, entry);
        }
    }
    return 0;
}
