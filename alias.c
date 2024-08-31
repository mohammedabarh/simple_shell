#include "shell.h"

#define ALIAS_FILE ".simple_shell_aliases"

static alias_t aliases[ALIAS_SIZE];

void init_aliases(void) {
    for (int i = 0; i < ALIAS_SIZE; i++) {
        aliases[i].name = NULL;
        aliases[i].value = NULL;
    }
}

void add_alias(char *name, char *value) {
    for (int i = 0; i < ALIAS_SIZE; i++) {
        if (aliases[i].name == NULL || strcmp(aliases[i].name, name) == 0) {
            aliases[i].name = strdup(name);
            aliases[i].value = strdup(value);
            return;
        }
    }
    fprintf(stderr, "Alias limit reached\n");
}

char *get_alias(char *name) {
    for (int i = 0; i < ALIAS_SIZE; i++) {
        if (aliases[i].name != NULL && strcmp(aliases[i].name, name) == 0) {
            return aliases[i].value;
        }
    }
    return NULL;
}

void save_aliases(void) {
    FILE *fp = fopen(ALIAS_FILE, "w");
    if (fp == NULL) {
        perror("Error saving aliases");
        return;
    }
    for (int i = 0; i < ALIAS_SIZE; i++) {
        if (aliases[i].name != NULL) {
            fprintf(fp, "%s=%s\n", aliases[i].name, aliases[i].value);
        }
    }
    fclose(fp);
}

void load_aliases(void) {
    FILE *fp = fopen(ALIAS_FILE, "r");
    if (fp == NULL) {
        return;  
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1) {
        line[strcspn(line, "\n")] = 0;  
        char *name = strtok(line, "=");
        char *value = strtok(NULL, "");
        if (name && value) {
            add_alias(name, value);
        }
    }
    free(line);
    fclose(fp);
}
