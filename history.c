#include "shell.h"

#define HISTORY_FILE ".simple_shell_history"

static char *history[HISTORY_SIZE];
static int history_count = 0;

void add_to_history(char *command) {
    if (history_count < HISTORY_SIZE) {
        history[history_count++] = strdup(command);
    } else {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) {
            history[i-1] = history[i];
        }
        history[HISTORY_SIZE-1] = strdup(command);
    }
}

char *get_history_entry(int index) {
    if (index >= 0 && index < history_count) {
        return history[index];
    }
    return NULL;
}

void save_history(void) {
    FILE *fp = fopen(HISTORY_FILE, "w");
    if (fp == NULL) {
        perror("Error saving history");
        return;
    }
    for (int i = 0; i < history_count; i++) {
        fprintf(fp, "%s\n", history[i]);
    }
    fclose(fp);
}

void load_history(void) {
    FILE *fp = fopen(HISTORY_FILE, "r");
    if (fp == NULL) {
        return;  
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1) {
        line[strcspn(line, "\n")] = 0;  
        add_to_history(line);
    }
    free(line);
    fclose(fp);
}
