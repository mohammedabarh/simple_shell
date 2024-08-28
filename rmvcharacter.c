#include "shell.h"

/**
 * strip_newline - removes the trailing newline character from the user input string
 * @str: pointer to the user input string
 * @len: length of the user input string
 * 
 * Return: the input string with the newline character removed from the end
 */

char *rmvchara(char *str, int len)
{
	if (str > 0 && str[len - 1] == '\n')
		str[len - 1] = '\0';

	return (str);
}


