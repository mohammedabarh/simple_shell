#include "shell.h"

int main(int argc, char **argv)
{
    char *input;
    char **args;
    int status;
    int interactive = isatty(STDIN_FILENO);

    /* Initialize shell data */
    shell_data_t shell_data;
    initialize_shell_data(&shell_data);

    if (argc == 2)
    {
        return execute_file(argv[1], &shell_data);
    }

    do {
        if (interactive)
            printf("($) ");

        input = read_line();
        if (!input)
            break;

        /* Handle comments */
        char *comment = strchr(input, '#');
        if (comment)
            *comment = '\0';

        /* Handle multiple commands (;) */
        char **commands = split_line(input, ";");
        for (int i = 0; commands[i] != NULL; i++)
        {
            /* Handle logical operators (&& and ||) */
            char **logical_ops = split_logical_ops(commands[i]);
            int prev_status = 1;

            for (int j = 0; logical_ops[j] != NULL; j++)
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
                status = execute(args, &shell_data);
                free(args);

                prev_status = status;
            }

            free(logical_ops);
        }
        free(commands);
        free(input);
    } while (status);

    cleanup_shell_data(&shell_data);
    return EXIT_SUCCESS;
}
