#include "shell.h"

void process_file(char *filename) {
    FILE *file = fopen(filename, "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, file)) != -1) {
        line[strcspn(line, "\n")] = '\0';  // Remove newline
        char *comment = strchr(line, '#');
        if (comment) {
            *comment = '\0';
        }

        char **commands = split_line(line, ";");
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

                char **args = split_line(logical_ops[j], TOKEN_DELIM);
                expand_variables(args);
                prev_status = execute(args);
                free(args);
            }

            free(logical_ops);
        }
        free(commands);
    }

    free(line);
    fclose(file);
}
