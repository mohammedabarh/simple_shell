#include "shell.h"

/**
 * tokenize - processes and splits user input into tokens
 * @cmd: the command input provided by the user
 * @Argcmd: an array to store the resulting tokens
 * return: the total count of tokens generated
 */
int tokenize(char *cmd, char *Argcmd[])
{
	char *dm = " ", *tkn;
	int count = 0;
	
	tkn = strtok(cmd, dm);
	
	while (tkn)
	{
		printf("%s", tkn);
		Argcmd[count++] = tkn;
		tkn = strtok(NULL, dm);
	}

	Argcmd[count] = NULL;
	
	return (count);
}
	

