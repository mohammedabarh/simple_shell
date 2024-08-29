#include "shell.h"

/**
 * readCmd - Reads user input from standard input
 *
 * Returns:
 *   The number of characters read
 */
int readCmd(void)
{
    size_t n = 0;
    char *buff = NULL, *cmd, *Argcmd[MAX_ARG];
    int size;
    int argc;

    size = getline(&buff, &n, stdin);
    if (size == -1)
    {
	    if (feof(stdin))
	    {
		    free(buff);
		    exit(0);
	    }
	    else
	    {
		    perror("Failed to read user input");
		    free(buff);
	    }
    }

    cmd = rmvchara(buff, size);
    argc = tokenize(cmd, Argcmd);

    exe(Argcmd);
 

    free(buff);
    return (size);
}

