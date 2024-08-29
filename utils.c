#include "shell.h"

char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t characters;

    characters = getline(&line, &bufsize, stdin);

    if (characters == -1)
    {
        if (feof(stdin))
        {
            free(line);
            return NULL;
        }
        else
        {
            perror("read_line");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}

char **split_line(char *line, char *delim)
{
    int bufsize = BUFFER_SIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, delim);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += BUFFER_SIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, delim);
    }
    tokens[position] = NULL;
    return tokens;
}

char **split_logical_ops(char *line)
{
    int bufsize = BUFFER_SIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;
    char *rest = line;

    if (!tokens)
    {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    while ((token = strtok_r(rest, "&|", &rest)))
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += BUFFER_SIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        if (rest[0] == '&' && rest[1] == '&')
        {
            tokens[position++] = "&&";
            rest += 2;
        }
        else if (rest[0] == '|' && rest[1] == '|')
        {
            tokens[position++] = "||";
            rest += 2;
        }
    }
    tokens[position] = NULL;
    return tokens;
}

void initialize_shell_data(shell_data_t *shell_data)
{
    extern char **environ;
    int env_count = 0;

    while (environ[env_count] != NULL)
        env_count++;

    shell_data->env = malloc((env_count + 1) * sizeof(char *));
    for (int i = 0; i < env_count; i++)
        shell_data->env[i] = strdup(environ[i]);
    shell_data->env[env_count] = NULL;

    shell_data->aliases = NULL;
    shell_data->last_status = 0;
}

void cleanup_shell_data(shell_data_t *shell_data)
{
    for (int i = 0; shell_data->env[i]; i++)
        free(shell_data->env[i]);
    free(shell_data->env);

    alias_t *current = shell_data->aliases;
    while (current)
    {
        alias_t *next = current->next;
        free(current->name);
        free(current->value);
        free(current);
        current = next;
    }
}

char *get_env_value(const char *name, char **env)
{
    size_t namelen = strlen(name);
    for (int i = 0; env[i]; i++)
    {
        if (strncmp(env[i], name, namelen) == 0 && env[i][namelen] == '=')
        {
            return &env[i][namelen + 1];
        }
    }
    return NULL;
}

int set_env_value(const char *name, const char *value, char ***env)
{
    size_t namelen = strlen(name);
    int i;
    char *new_var;

    for (i = 0; (*env)[i]; i++)
    {
        if (strncmp((*env)[i], name, namelen) == 0 && (*env)[i][namelen] == '=')
        {
            free((*env)[i]);
            break;
        }
    }

    new_var = malloc(namelen + strlen(value) + 2);
    if (!new_var)
        return -1;

    sprintf(new_var, "%s=%s", name, value);

    if ((*env)[i])
    {
        (*env)[i] = new_var;
    }
    else
    {
        char **new_env = realloc(*env, (i + 2) * sizeof(char *));
        if (!new_env)
        {
            free(new_var);
            return -1;
        }
        *env = new_env;
        (*env)[i] = new_var;
        (*env)[i + 1] = NULL;
    }

    return 0;
}

int unset_env_value(const char *name, char ***env)
{
    size_t namelen = strlen(name);
    int i, j;

    for (i = 0; (*env)[i]; i++)
    {
        if (strncmp((*env)[i], name, namelen) == 0 && (*env)[i][namelen] == '=')
        {
            free((*env)[i]);
            for (j = i; (*env)[j]; j++)
            {
                (*env)[j] = (*env)[j + 1];
            }
            return 0;
        }
    }

    return -1;
}

int execute_file(const char *filename, shell_data_t *shell_data)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1)
    {
        line[strcspn(line, "\n")] = '\0';  // Remove newline
        char **args = split_line(line, TOKEN_DELIM);
        execute(args, shell_data);
        free(args);
    }

    free(line);
    fclose(file);
    return EXIT_SUCCESS;
}

char *expand_variables(char *input, shell_data_t *shell_data)
{
    char *result = malloc(strlen(input) * 2);  // Allocate more space for expansion
    char *src = input;
    char *dest = result;

    while (*src)
    {
        if (*src == '$')
        {
            src++;
            if (*src == '?')
            {
                dest += sprintf(dest, "%d", shell_data->last_status);
                src++;
            }
            else if (*src == '$')
            {
                dest += sprintf(dest, "%d", getpid());
                src++;
            }
            else
            {
                char var_name[256] = {0};
                int i = 0;
                while (isalnum(*src) || *src == '_')
                {
                    var_name[i++] = *src++;
                }
                char *value = get_env_value(var_name, shell_data->env);
                if (value)
                {
                    strcpy(dest, value);
                    dest += strlen(value);
                }
            }
        }
        else
        {
            *dest++ = *src++;
        }
    }
    *dest = '\0';
    return result;
}
