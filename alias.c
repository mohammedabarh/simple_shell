#include "shell.h"

Alias aliases[MAX_ALIASES];
int alias_count = 0;

void add_alias(char *name, char *value) {
    int i;
    for (i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].name, name) == 0) {
            free(aliases[i].value);
            aliases[i].value = strdup(value);
            return;
        }
    }

    if (alias_count < MAX_ALIASES) {
        aliases[alias_count].name = strdup(name);
        aliases[alias_count].value = strdup(value);
        alias_count++;
    }
}

char *get_alias(char *name) {
    int i;
    for (i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].name, name) == 0) {
            return aliases[i].value;
        }
    }
    return NULL;
}

void print_aliases() {
    int i;
    for (i = 0; i < alias_count; i++) {
        printf("%s='%s'\n", aliases[i].name, aliases[i].value);
    }
}

int shell_alias(char **args) {
    int i;

    if (args[1] == NULL) {
        print_aliases();
        return 1;
    }

    for (i = 1; args[i] != NULL; i++) {
        char *equals = strchr(args[i], '=');
        if (equals) {
            *equals = '\0';
            add_alias(args[i], equals + 1);
        } else {
            char *value = get_alias(args[i]);
            if (value) {
                printf("%s='%s'\n", args[i], value);
            }
        }
    }

    return 1;
}
