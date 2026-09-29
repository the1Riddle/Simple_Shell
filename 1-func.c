#include "shell.h"
/**
 * clear_scrn - inbuilt to clear screen
 */
void clear_scrn(void)
{
	write(STDOUT_FILENO, "\033[2J\033[H", 7);
}
/**
 * exit_shell - exit inbuild
 * @cmd: the argument passed
 * 
 * Return: 0 for success or -2 on failure
 */
int exit_shell(char *cmd)
{
	int e_Status;
	const char *error_msg;

	if (cmd && cmd[0])/* If there is an exit argument */
	{
		e_Status = atoi(cmd);
		if (e_Status == 0 && cmd[0] != '0')
		{/* if conversion fails */
			error_msg = "exit number provided error: ";
			write(STDERR_FILENO, error_msg, _strlen(error_msg));
			write(STDERR_FILENO, cmd, _strlen(cmd));
			write(2, " :Exiting failed", _strlen(" :Exiting failed"));
			write(STDERR_FILENO, "\n", 1);
			return (-2);/* return -2 for an error */
		}
		return (e_Status);
	}
	else/* if no arg is provided then */
	{
		write(2, "Exiting...\n", strlen("Exiting...\n"));
		return (0);
	}
}

/**
 * _strdup - returns pointer to new mem alloc space which contains copy
 * @strtodup: string to be duplicated
 *
 * Return: a pointer to the new duplicated string
 */
char *_strdup(char *strtodup)
{
	char *copy;

	int len, i;

	if (strtodup == 0)
		return (NULL);

	for (len = 0; strtodup[len]; len++)
		;
	copy = malloc((len + 1) * sizeof(char));

	for (i = 0; i <= len; i++)
		copy[i] = strtodup[i];

	return (copy);
}

/**
 * _strlen - returns the length of a string
 * @s: a pointer to a string
 *
 * Return: length
 */
int _strlen(const char *s)
{
	int lngth = 0;

	if (*s)
	{
		lngth++;
		lngth += _strlen(s + 1);
	}

	return (lngth);
}

/**
 * _strcspn - determine the length of the prefix substring
 * @str1: first string
 * @str2: second string
 *
 * Return: count
 */
size_t _strcspn(const char *str1, const char *str2)
{
	const char *p1, *p2;
	size_t count = 0;

	for (p1 = str1; *p1 != '\0'; p1++)
	{
		for (p2 = str2; *p2 != '\0'; p2++)
		{
			if (*p1 == *p2)
				return (count);
		}
		count++;
	}

	return (count);
}
