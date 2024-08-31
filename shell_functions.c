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

command_t *split_commands(char *line) {
    command_t *commands = malloc(sizeof(command_t));
    int bufsize = BUFFER_SIZE, position = 0;
    char *token, *saveptr;
    
    commands->commands = malloc(bufsize * sizeof(char*));
    commands->operators = malloc(bufsize * sizeof(char));

    if (!commands->commands || !commands->operators) {
        fprintf(stderr, "split_commands: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok_r(line, COMMAND_DELIM, &saveptr);
    while (token != NULL) {
        commands->commands[position] = token;
        if (saveptr[0] != '\0') {
            commands->operators[position] = saveptr[0];
            saveptr++;
        } else {
            commands->operators[position] = '\0';
        }
        position++;

        if (position >= bufsize) {
            bufsize += BUFFER_SIZE;
            commands->commands = realloc(commands->commands, bufsize * sizeof(char*));
            commands->operators = realloc(commands->operators, bufsize * sizeof(char));
            if (!commands->commands || !commands->operators) {
                fprintf(stderr, "split_commands: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok_r(NULL, COMMAND_DELIM, &saveptr);
    }
    commands->commands[position] = NULL;
    commands->operators[position] = '\0';
    return commands;
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

int execute(command_t *commands) {
    int i = 0, result = 1, should_run = 1;
    char **args;

    while (commands->commands[i] != NULL && should_run) {
        args = split_line(commands->commands[i]);
        if (args[0] == NULL) {
            free(args);
            i++;
            continue;
        }

        // Handle aliases
        char *alias_value = get_alias(args[0]);
        if (alias_value != NULL) {
            free(args[0]);
            args[0] = strdup(alias_value);
        }

        if (is_builtin(args[0])) {
            result = execute_builtin(args);
        } else {
            if (handle_redirection(args) == 0) {
                if (handle_pipes(args) == 0) {
                    result = launch(args);
                }
            }
        }

        if (commands->operators[i] == '&' && result != 0) {
            should_run = 0;
        } else if (commands->operators[i] == '|' && result == 0) {
            should_run = 0;
        }

        free(args);
        i++;
    }

    return 1;
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

    return WEXITSTATUS(status);
}

char *get_location(char *command) {
    char *path, *path_copy, *path_token, *file_path;
    int command_length, directory_length;
    struct stat buffer;

    path = getenv("PATH");
    if (path) {
        path_copy = strdup(path);
        command_length = strlen(command);
        path_token = strtok(path_copy, ":");
        while (path_token != NULL) {
            directory_length = strlen(path_token);
            file_path = malloc(command_length + directory_length + 2);
            strcpy(file_path, path_token);
            strcat(file_path, "/");
            strcat(file_path, command);
            strcat(file_path, "\0");
            if (stat(file_path, &buffer) == 0) {
                free(path_copy);
                return (file_path);
            } else {
                free(file_path);
                path_token = strtok(NULL, ":");
            }
        }
        free(path_copy);
        if (stat(command, &buffer) == 0) {
            return (strdup(command));
        }
        return (NULL);
    }
    return (NULL);
}

void print_error(char *command, char *message) {
    fprintf(stderr, "./hsh: %s: %s\n", command, message);
}
