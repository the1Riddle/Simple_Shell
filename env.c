#include "shell.h"
/**
* _getenv - gets the the environment variable value.
* @name: the name of environment var.
*
* Return: NULL if the variable is not found or a pointer to the value
* of the environment var.
*/
char *_getenv(const char *name)
{
	int nameL = _strlen(name);
	int x;

	for (x = 0; environ[x] != NULL; x++)
	{
/* if the value matchs and has = */
		if (_strncmp(environ[x], name, nameL) == 0 && environ[x][nameL] == '=')
		{
			return (&(environ[x][nameL + 1]));
		}
	}
	return (NULL);
}
/**
* printEnv - prints the environment vars.
*
* Return: nothing becouse its a void func
*/
void printEnv(void)
{
	char **env;
	size_t len;

/* iterates through env vars. */
	for (env = environ; *env != NULL; env++)
	{
		len = _strlen(*env);
		write(STDOUT_FILENO, *env, len);
		write(STDOUT_FILENO, "\n", 1);
	}
}
/**
* _setenv - chenges or adds environment variable
* @name: the env var name
* @value: the asigned value to the env var
* @overwrite: used to indicate whether to over write or not
*
* Return: 0 for success and -1 on failure
*/
int _setenv(const char *name, const char *value, int overwrite)
{
	int nameL = _strlen(name);
	int valueL = _strlen(value);
	int x;
	char *newEnt;

	if (name == NULL || value == NULL || _strchr(name, '=') != NULL)
	{
		return (-1);/* this means an error ocured during input */
	}
	for (x = 0; environ[x] != NULL; x++)
	{/*check if the env var exists */
		if (_strncmp(environ[x], name, nameL) == 0 && environ[x][nameL] == '=')
		{
			if (overwrite)/* if it exists then will overwrite it */
			{
				newEnt = malloc(nameL + valueL + 2);
				if (newEnt == NULL)
				{
					return (-1);
				}
				_strcat(newEnt, name);
				_strcat(newEnt, "=");
				_strcat(newEnt, value);
				free(environ[x]);
				environ[x] = newEnt;
			}
			return (0);
		}
	}
	newEnt = malloc(nameL + valueL + 2);
	if (newEnt == NULL)
	{/* and if the env var is not found then a new one is created */
		return (-1);
	}
	_strcat(newEnt, name);
	_strcat(newEnt, "=");
	_strcat(newEnt, value);
	environ[x] = newEnt;
	environ[x + 1] = NULL;
	return (0);
}
/**
* _unsetenv - unsets the environment
* @var: environment var
*
* Return: usually 0 for success
*/
int _unsetenv(char *var)
{
	if (unsetenv(var) != 0)
	{
		perror("unsetenv error");
		return (-1);
	}

	return (0);
}
