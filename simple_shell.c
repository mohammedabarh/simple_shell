#include "shell.h"

/**
 * main - Entry point for the simple shell
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
    char *input = NULL;
    size_t input_size = 0;
    ssize_t read_size;
    char **commands;
    char **args;
    int status = 1;
    int i, exec_status;

    while (status)
    {
        if (isatty(STDIN_FILENO))
            printf("($) ");

        read_size = getline(&input, &input_size, stdin);

        if (read_size == -1)
        {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        /* Remove newline character */
        input[strcspn(input, "\n")] = '\0';

        commands = split_line(input, ";");
        for (i = 0; commands[i] != NULL; i++)
        {
            char **logical_ops = split_logical_ops(commands[i]);
            int j;
            int prev_status = 1;

            for (j = 0; logical_ops[j] != NULL; j++)
            {
                if (strcmp(logical_ops[j], "&&") == 0)
                {
                    if (prev_status == 0)
                        break;
                    continue;
                }
                else if (strcmp(logical_ops[j], "||") == 0)
                {
                    if (prev_status != 0)
                        break;
                    continue;
                }

                args = split_line(logical_ops[j], TOKEN_DELIM);
                exec_status = execute(args);
                free(args);

                prev_status = exec_status;
            }

            free(logical_ops);

            if (exec_status == 0)
                break;
        }
        free(commands);
    }

    free(input);
    return 0;
}

