#include "shell.h"

/**
 * exe - Executes the specified user command
 * @Argcmd: An array of strings representing the command and its arguments
 *
 * Returns:
 *   0 if the command executes successfully
 *   Non-zero value if an error occurs
 */

int exe(char *Argcmd[])
{
	pid_t pid;
	int execut;
	int sts;

	pid = fork();

	if (pid == -1)
	{
		perror("failed to create process");
	}
	else if (pid == 0)
	{
		execut = execve(Argcmd[0], Argcmd, NULL);
		if (execut == -1)
		{
			perror("error execut cmd");
		}
	}
	else
		wait(&sts);

	return (0);
}
