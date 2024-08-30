#include "shell.h"

char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

char **split_line(char *line, char *delim)
{
    int bufsize = BUFFER_SIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "Allocation error\n");
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
                fprintf(stderr, "Allocation error\n");
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
            perror("shell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("shell");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return WEXITSTATUS(status);
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
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    while ((token = strstr(rest, "&&")) != NULL || (token = strstr(rest, "||")) != NULL) {
        *token = '\0';
        tokens[position++] = rest;
        tokens[position++] = (token[1] == '&') ? "&&" : "||";
        rest = token + 2;

        if (position >= bufsize) {
            bufsize += BUFFER_SIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    tokens[position++] = rest;
    tokens[position] = NULL;
    return tokens;
}
