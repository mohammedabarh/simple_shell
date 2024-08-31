#include "shell.h"

char *replace_variables(char *str) {
    char *result = malloc(strlen(str) * 2);
    char *dst = result;
    char var_name[256];
    char *var_value;
    int in_var = 0;
    int var_len = 0;

    while (*str) {
        if (*str == '$' && (*(str + 1) == '{' || isalpha(*(str + 1)))) {
            in_var = 1;
            var_len = 0;
            str++;
            if (*str == '{') str++;
            continue;
        }
        if (in_var) {
            if (isalnum(*str) || *str == '_') {
                var_name[var_len++] = *str;
            } else {
                in_var = 0;
                var_name[var_len] = '\0';
                var_value = getenv(var_name);
                if (var_value) {
                    strcpy(dst, var_value);
                    dst += strlen(var_value);
                }
                if (*str == '}') str++;
                continue;
            }
        } else {
            *dst++ = *str;
        }
        str++;
    }
    if (in_var) {
        var_name[var_len] = '\0';
        var_value = getenv(var_name);
        if (var_value) {
            strcpy(dst, var_value);
            dst += strlen(var_value);
        }
    }
    *dst = '\0';
    return result;
}

void remove_comments(char *str) {
    char *comment_start = strchr(str, '#');
    if (comment_start != NULL) {
        *comment_start = '\0';
    }
}
