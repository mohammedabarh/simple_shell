#include "shell.h"

void sigint_handler(int sig) {
    (void)sig;
    write(STDOUT_FILENO, "\n$ ", 3);
}

int main(void) {
    char *line;
    char **args;
    int status = 1;

    signal(SIGINT, sigint_handler);

    while (status) {
        printf("$ ");
        line = read_line();
        if (line == NULL) {  
            printf("\n");
            break;
        }
        args = split_line(line);
        status = execute(args);

        free(line);
        free(args);
    }

    return EXIT_SUCCESS;
}
