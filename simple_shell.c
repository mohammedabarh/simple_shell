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
    char **args;
    int status = 1;

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

        args = split_line(input);
        status = execute(args);

        free(args);
    }

    free(input);
    return (0);
}
