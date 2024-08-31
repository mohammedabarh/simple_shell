#include "shell.h"

void print_error(char *command, char *message) {
    fprintf(stderr, "./hsh: 1: %s: %s\n", command, message);
}
