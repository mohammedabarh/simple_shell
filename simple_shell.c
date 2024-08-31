#include "shell.h"

void sigint_handler(int sig) {
    (void)sig;
    write(STDOUT_FILENO, "\n$ ", 3);
}

int main(void) {
    char *line;
    command_t *commands;
    int status = 1;

    signal(SIGINT, sigint_handler);
    load_history();
    init_aliases();
    load_aliases();

    while (status) {
        printf("$ ");
        line = read_line();
        if (line == NULL) {  // Handle EOF (Ctrl+D)
            printf("\n");
            break;
        }
        remove_comments(line);
        line = replace_variables(line);
        add_to_history(line);
        commands = split_commands(line);
        status = execute(commands);

        free(line);
        free(commands->commands);
        free(commands->operators);
        free(commands);
    }

    save_history();
    save_aliases();
    return EXIT_SUCCESS;
}

