#include "shell.h"

char *read_line(void) {
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t characters;

    characters = getline(&line, &bufsize, stdin);

    if (characters == -1) {
        if (feof(stdin)) {
            free(line);
            return NULL;  // We received an EOF
        } else  {
            perror("read_line");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}

char **split_line(char *line) {
    int bufsize = BUFFER_SIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token, *saveptr;

    if (!tokens) {
        fprintf(stderr, "split_line: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok_r(line, TOKEN_DELIM, &saveptr);
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

        token = strtok_r(NULL, TOKEN_DELIM, &saveptr);
    }
    tokens[position] = NULL;
    return tokens;
}

int execute(char **args) {
    if (args[0] == NULL) {
        return 1;
    }

    if (is_builtin(args[0])) {
        return execute_builtin(args);
    }

    return launch(args);
}

int launch(char **args) {
    pid_t pid;
    int status;
    char *command_path;

    pid = fork();
    if (pid == 0) {
        command_path = get_location(args[0]);
        if (command_path == NULL) {
            print_error(args[0], "not found");
            exit(127);
        }
        if (execve(command_path, args, environ) == -1) {
            print_error(args[0], strerror(errno));
            free(command_path);
            exit(126);
        }
    } else if (pid < 0) {
        perror("launch");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
