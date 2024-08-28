#include "shell.h"

/**
 * main - Serves as the entry point for the application
 *
 * Returns: 0 if the program completes successfully
 */
int main(void)
{
	/* Recursive function to maintain an interactive prompt */
    while (1)
    {
        readCmd();
    }

    return (0);
}

