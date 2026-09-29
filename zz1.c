#include "shell.h"

#define BUFFER_SIZE 1024

/**
 * _getline - read line of text from the user's input and use it in a program
 * @stream: the file to read from
 *
 * Return: -1 on failure or when it's an invalid arg.
 */
char *_getline(FILE *stream)
{
	static char buffer[BUFFER_SIZE];
	static int index, bytesRead;
	char c, *newCmd;
	int Lsize = 0, currentBuff = BUFFER_SIZE;

	char *cmd = malloc(currentBuff * sizeof(char));
	if (cmd == NULL)
	{
		perror("Memory allocation error");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		if (index == bytesRead)
		{
			bytesRead = read(fileno(stream), buffer, BUFFER_SIZE);
			if (bytesRead == -1)
			{
				perror("Error from read");
				exit(EXIT_FAILURE);
			}
			if (bytesRead == 0)
				break;
			index = 0;
		}
		c = buffer[index++];
		if (c == '\n')
		{
			cmd[Lsize] = '\0';
			break;
		}
		if (Lsize + 1 == currentBuff)
		{
			currentBuff *= 2;
			newCmd = realloc(cmd, currentBuff * sizeof(char));
			if (newCmd == NULL)
			{
				free(cmd);
				perror("Memory allocation error");
				exit(EXIT_FAILURE);
			}
			cmd = newCmd;
		}
		cmd[Lsize] = c;
		Lsize++;
	}
	return (cmd);
}
