#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARG 125

int readCmd(void);
int tokenize(char *cmd, char *Argcmd[]);
char *rmvchara(char *str, int len);




#endif
