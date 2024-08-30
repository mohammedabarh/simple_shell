#include "shell.h"

char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t characters;

    characters = getline(&line, &bufsize, stdin);

    if (characters == -1) {
        if (feof(stdin)) {
            free(line);
            exit(EXIT_SUCCESS);
        } else {
            free(line);
            perror("read_line");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}

char **split_line(char *line, char *delim)
{
    int bufsize = BUFFER_SIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "split_line: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, delim);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += BUFFER_SIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "split_line: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, delim);
    }
    tokens[position] = NULL;
    return tokens;
}

int launch(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("launch");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("launch");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return WEXITSTATUS(status);
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

void expand_variables(char **args) {
    int i;
    for (i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "$?") == 0) {
            free(args[i]);
            args[i] = malloc(16);
            snprintf(args[i], 16, "%d", WEXITSTATUS(status));
        } else if (strcmp(args[i], "$$") == 0) {
            free(args[i]);
            args[i] = malloc(16);
            snprintf(args[i], 16, "%d", getpid());
        } else if (args[i][0] == '$') {
            char *value = getenv(args[i] + 1);
            if (value) {
                free(args[i]);
                args[i] = strdup(value);
            }
        }
    }
}

char **split_logical_ops(char *line)
{
    int bufsize = BUFFER_SIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token, *rest = line;

    if (!tokens) {
        fprintf(stderr, "split_logical_ops: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while ((token = strstr(rest, "&&")) != NULL || (token = strstr(rest, "||")) != NULL) {
        *token = '\0';
        tokens[position++] = rest;
        tokens[position++] = (*token == '&') ? "&&" : "||";
        rest = token + 2;

        if (position >= bufsize) {
            bufsize += BUFFER_SIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "split_logical_ops: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    tokens[position++] = rest;
    tokens[position] = NULL;
    return tokens;
}
