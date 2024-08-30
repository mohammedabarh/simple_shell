#include "shell.h"

int status = 0;

int main(int argc, char **argv)
{
    char *input;
    char **args;
    int exec_status = 1;

    if (argc == 2) {
        process_file(argv[1]);
        return 0;
    }

    while (exec_status) {
        printf("$ ");
        input = read_line();

        char *comment = strchr(input, '#');
        if (comment) {
            *comment = '\0';
        }

        char **commands = split_line(input, ";");
        for (int i = 0; commands[i] != NULL; i++) {
            char **logical_ops = split_logical_ops(commands[i]);
            int prev_status = 1;

            for (int j = 0; logical_ops[j] != NULL; j++) {
                if (strcmp(logical_ops[j], "&&") == 0) {
                    if (prev_status == 0) break;
                    continue;
                } else if (strcmp(logical_ops[j], "||") == 0) {
                    if (prev_status != 0) break;
                    continue;
                }

                args = split_line(logical_ops[j], TOKEN_DELIM);
                expand_variables(args);
                prev_status = execute(args);
                free(args);
            }

            free(logical_ops);
        }
        free(commands);
        free(input);
    }

    return 0;
}
