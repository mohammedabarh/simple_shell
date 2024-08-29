#include "shell.h"

char **split_line(char *line, char *delim)
{
    int bufsize = BUFFER_SIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, delim);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += BUFFER_SIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, delim);
    }
    tokens[position] = NULL;
    return tokens;
}

char **split_logical_ops(char *line)
{
    int bufsize = BUFFER_SIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;
    char *rest = line;

    if (!tokens)
    {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    while ((token = strtok_r(rest, "&|", &rest)))
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += BUFFER_SIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        if (rest[0] == '&')
        {
            tokens[position++] = "&&";
            rest++;
        }
        else if (rest[0] == '|')
        {
            tokens[position++] = "||";
            rest++;
        }
    }
    tokens[position] = NULL;
    return tokens;
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
        "unsetenv"
    };
    int (*builtin_func[]) (char **) = {
        &shell_cd,
        &shell_help,
        &shell_exit,
        &shell_env,
        &shell_setenv,
        &shell_unsetenv
    };

    if (args[0] == NULL)
        return 1;

    for (i = 0; i < 6; i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
            return (*builtin_func[i])(args);
    }

    return launch(args);
}

int launch(char **args)
{
    pid_t pid;
    int status;
    char *command_path;

    pid = fork();
    if (pid == 0)
    {
        command_path = get_location(args[0]);
        if (command_path == NULL)
        {
            fprintf(stderr, "%s: command not found\n", args[0]);
            exit(EXIT_FAILURE);
        }

        if (execve(command_path, args, environ) == -1)
        {
            perror("Error:");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("Error:");
    }
    else
    {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

char *get_location(char *command)
{
    char *path, *path_copy, *path_token, *file_path;
    int command_length, directory_length;
    struct stat buffer;

    path = getenv("PATH");
    if (path)
    {
        path_copy = strdup(path);
        command_length = strlen(command);
        path_token = strtok(path_copy, ":");
        while (path_token != NULL)
        {
            directory_length = strlen(path_token);
            file_path = malloc(command_length + directory_length + 2);
            strcpy(file_path, path_token);
            strcat(file_path, "/");
            strcat(file_path, command);
            strcat(file_path, "\0");

            if (stat(file_path, &buffer) == 0)
            {
                free(path_copy);
                return (file_path);
            }
            else
            {
                free(file_path);
                path_token = strtok(NULL, ":");
            }
        }
        free(path_copy);
        if (stat(command, &buffer) == 0)
        {
            return (command);
        }
        return (NULL);
    }
    return (NULL);
}
