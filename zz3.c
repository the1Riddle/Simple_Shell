#include "shell.h"

/**
 * cmd_execution - executs commands specified
 * @cmd: a character pointer of the commands
 *
 * Description: this func executes a command specified user in cmd,
 * The function replaces the current process with new ones specified
 * and arguments.the parent waits for child process to finish
 * but when an error is encountered it exits with 1.
 *
 * Return: -1 when forking fails and the status of chiled process
 */
int cmd_execution(char **cmd)
{
	pid_t pid;
	int status = 0;

	pid = fork();
	if (pid < 0)
	{
		perror("fork error");
		return (-1);
	}
	else if (pid == 0)
	{
		/* This part will handle the child process */
		execvp(cmd[0], cmd);
		perror("Failed to execute");
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
	return (status);
}

/**
 * parse_cmd - perses commands for external exe
 * @cmd: commands
 *
 * Return: args
 */
char **parse_cmd(char *cmd)
{
	char **args = NULL;
	char *token = strtok(cmd, " \t\n");
	int arg_count = 0;

	while (token != NULL)
	{
		args = realloc(args, (arg_count + 1) * sizeof(char *));
		if (args == NULL)
		{
			perror("Memory allocation error");
			exit(EXIT_FAILURE);
		}
		args[arg_count] = strdup(token);
		if (args[arg_count] == NULL)
		{
			perror("Memory allocation error");
			exit(EXIT_FAILURE);
		}
		arg_count++;
		token = strtok(NULL, " \t\n");
	}

	args = realloc(args, (arg_count + 1) * sizeof(char *));
	if (args == NULL)
	{
		perror("Memory allocation error");
		exit(EXIT_FAILURE);
	}
	args[arg_count] = NULL;

	return (args);
}

/**
 * shell_cd - change the current working directory
 * @path: the path to change to
 *
 * Return: usually 0 for success.
 */
int shell_cd(const char *path)
{
	char *cwd, buffer[1024];
	int chdir_ret;
	const char *pwd, *oldpwd, *dir;

	if (path == NULL)
	{/* If no argument is provided, change to the HOME directory */
		dir = _getenv("HOME");
		if (!dir)
			chdir_ret = chdir(getenv("PWD") ? _getenv("PWD") : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (*path == '-')
	{/* checks if the argument is "-" then change to the OLDPWD */
		oldpwd = _getenv("OLDPWD");
		if (!oldpwd)
		{/* If OLDPWD is not set then it print PWD and ret 1 */
			cwd = getcwd(buffer, sizeof(buffer));
			if (cwd)
			{
				write(STDOUT_FILENO, cwd, _strlen(cwd));
				write(STDOUT_FILENO, "\n", 1);
			}
			return (1);
		}
		write(STDOUT_FILENO, oldpwd, _strlen(oldpwd));
		write(STDOUT_FILENO, "\n", 1);
		chdir_ret = chdir(oldpwd);
	}
	else/* changes to the specified directory */
		chdir_ret = chdir(path);
	if (chdir_ret == -1)
	{/* If changing the directory failed, print an error message */
		write(STDERR_FILENO, "cd failed, try again ",
				_strlen("cd failed, try again "));
		write(STDERR_FILENO, path, _strlen(path));
		write(STDERR_FILENO, "\n", 1);
	}
	else
	{/* Update the OLDPWD and PWD environment variables */
		pwd = getcwd(buffer, sizeof(buffer));
		if (pwd)
		{
			setenv("OLDPWD", getenv("PWD"), 1);
			setenv("PWD", pwd, 1);
		}
	}
	return (0);
}
