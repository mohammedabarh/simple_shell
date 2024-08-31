#include "shell.h"

int handle_redirection(char **args) {
    int i;
    int in_redirect = 0, out_redirect = 0, append_redirect = 0;
    char *in_file = NULL, *out_file = NULL;

    for (i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {
            in_redirect = 1;
            in_file = args[i + 1];
            args[i] = NULL;
            i++;
        } else if (strcmp(args[i], ">") == 0) {
            out_redirect = 1;
            out_file = args[i + 1];
            args[i] = NULL;
            i++;
        } else if (strcmp(args[i], ">>") == 0) {
            append_redirect = 1;
            out_file = args[i + 1];
            args[i] = NULL;
            i++;
        }
    }

    if (in_redirect) {
        int fd = open(in_file, O_RDONLY);
        if (fd == -1) {
            perror("Input redirection");
            return 1;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (out_redirect || append_redirect) {
        int flags = O_WRONLY | O_CREAT;
        if (append_redirect) {
            flags |= O_APPEND;
        } else {
            flags |= O_TRUNC;
        }
        int fd = open(out_file, flags, 0644);
        if (fd == -1) {
            perror("Output redirection");
            return 1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    return 0;
}

int handle_pipes(char **args) {
    int i, pipe_count = 0;
    int pipe_positions[10];
    int pipe_fds[2];
    pid_t pid;

    
    for (i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_positions[pipe_count++] = i;
            args[i] = NULL;
        }
    }

    if (pipe_count == 0) {
        return 0;  
    }

    int start = 0;
    for (i = 0; i <= pipe_count; i++) {
        if (pipe(pipe_fds) == -1) {
            perror("pipe");
            return 1;
        }

        pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {  
            if (i > 0) {
                dup2(pipe_fds[0], STDIN_FILENO);
            }
            if (i < pipe_count) {
                dup2(pipe_fds[1], STDOUT_FILENO);
            }
            close(pipe_fds[0]);
            close(pipe_fds[1]);

            char *command_path = get_location(args[start]);
            if (command_path == NULL) {
                fprintf(stderr, "%s: command not found\n", args[start]);
                exit(127);
            }
            execve(command_path, &args[start], environ);
            perror("execve");
            exit(1);
        }

        
        close(pipe_fds[1]);
        if (i > 0) {
            close(pipe_fds[0]);
        }
        start = pipe_positions[i] + 1;
    }

    
    for (i = 0; i <= pipe_count; i++) {
        wait(NULL);
    }

    return 0;
}
