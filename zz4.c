#include "shell.h"

/**
 * _getline - read line of text from the user's input and use it in a program
 * @stream: the file to read from
 *
 * Return: -1 on failure or when it's an invalid arg.
 */
char *_getline(char **cmd, FILE *stream)
{
	static char buffer[BUFFER_SIZE];
	static int index;
	static int bytesRead;
	static int currentBuff = BUFFER_SIZE;
	char c;
	char *newCmd;
	int Lsize = 0;

	if (*cmd == NULL)
	{
		*cmd = malloc(currentBuff * sizeof(char));
		if (*cmd == NULL)
		{
			perror("Memory allocation error");
			exit(EXIT_FAILURE);
		}
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
			(*cmd)[Lsize] = '\0';
			break;
		}
		if (Lsize + 1 == currentBuff)
		{
			currentBuff *= 2;
			newCmd = malloc(currentBuff * sizeof(char));
			if (newCmd == NULL)
			{
				perror("Memory allocation error");
				exit(EXIT_FAILURE);
			}
			_memcpy(newCmd, *cmd, Lsize);
			free(*cmd);
			*cmd = newCmd;
		}
		(*cmd)[Lsize] = c;
		Lsize++;
	}
	return (*cmd);
}
/**
 * execute_external_command - the entry point
 * @cmd: commands to be executed
 * @argv: arguments tu be executed with
 *
 * Return: operation status
 */

int execute_external_command(char *cmd, char **argv)
{
	pid_t pid;
	int status = 0, i = 0;

	pid = fork(); /* create a child process */

	if (pid == 0) /* if pid == 0, this is the child process */
	{
		/* This is the child process */
		execve(cmd, argv, NULL); /* execute the command */
		write(2, "Failed to execute.\n", _strlen("Failed to execute.\n"));
		while (argv[i] != NULL)
		{
			free(argv[i]);
			i++;
		}
		free(argv);
		exit(EXIT_FAILURE); /* terminate the child process */
	}
	else if (pid < 0) /* if pid < 0, the fork failed */
	{
		/* The fork failed */
		write(2, "Fork error.\n", _strlen("Fork error.\n"));
		while (argv[i] != NULL)
		{
			free(argv[i]);
			i++;
		}
		free(argv);
		return (-1);
	}
	else /* if pid > 0, this is the parent process */
	{
		/* This is the parent process */
		waitpid(pid, &status, 0); /* wait for the child process to finish */
	}
	return (status);
}

/**
 * main - the entry point
 *
 * Description: The function reads inputs from the user.
 * The function uses a boolean which is combined by the isatty func in the...
 * while loop to check if the stdout is not in the terminal to make the...
 * prompt$ apear only ones.
 * the func also checks whether their is no input by the user where it will...
 * continue to display the prompt again.
 * it also checks the input by the user and when it finds a space at the end...
 * then it removes it to '\0'.
 *
 * Return: 0 for (success).
 */
int main(void)
{
	char **args, *cmd = NULL;
	bool F = false;
	size_t length;
	int status, e_Status;

	while (1 && !F)
	{
		if (isatty(STDIN_FILENO) == 0)
			F = true;
		if (!F)
			write(1, "prompt$ ", _strlen("prompt$ "));
		if (_getline(&cmd, stdin) == NULL)
		{
			write(1, "\n", 1);
			break;
		}

		length = _strlen(cmd);
		if (length > 0 && cmd[length - 1] == '\n')
		{
			cmd[length - 1] = '\0';
			length--;
		}
		if (length == 0)
			continue;/* Internal Commands */
		if (_strcmp(cmd, "clear") == 0)
		{
			clear_scrn();
			continue;
		}
		else if (_strcmp(cmd, "env") == 0)
		{
			printEnv();
			continue;
		}
		else if (_strcmp(cmd, "cd") == 0)
		{
			if (_strlen(cmd) > 3)
			{
				status = shell_cd(cmd + 3);
				if (status != 0)
					write(2, "cd failed", _strlen("cd failed"));
			}
			else
			{
				status = shell_cd(NULL);
				if (status != 0)
					write(2, "cd failed", _strlen("cd failed"));
			}
		}
		else if (_strncmp(cmd, "exit", 4) == 0)
		{
			e_Status = exit_shell(cmd + 5);
			if (e_Status >= -1)
			{
				free(cmd);
				exit(e_Status);
			}
		}
		else
		{
			args = parse_cmd(cmd);
			if (args)
			{
				status = cmd_execution(args);
				free_cmd(args);
				if (status < 0)
					write(STDERR_FILENO, "Failed to execute.\n", _strlen("Failed to execute.\n"));
			}
		}
	}

	free(cmd);
	return (0);
}
