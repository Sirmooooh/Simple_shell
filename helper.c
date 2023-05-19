#include "shell.h"

char *get_args(char *line, int *exe_spe);
int call_args(char **args, char **speed, int *exe_spe);
int run_args(char **args, char **speed, int *exe_spe);
int handle_args(int *exe_spe);
int check_args(char **args);

/**
 * get_args - Gets a command from standard input.
 * @line: A buffer to store the command.
 * @exe_spe: The return value of the last executed command.
 *
 * Return: If an error occurs - NULL.
 *         Otherwise - a pointer to the stored command.
 */
char *get_args(char *line, int *exe_spe)
{
	size_t n = 0;
	ssize_t read;
	char *prompt = "$ ";

	if (line)
		free(line);

	read = _getline(&line, &n, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		hist++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (get_args(line, exe_spe));
	}

	line[read - 1] = '\0';
	var_replacement(&line, exe_spe);
	handle_line(&line, read);

	return (line);
}

/**
 * call_args - Partitions operators from commands and calls them.
 * @args: An array of arguments.
 * @speed: A double pointer to the beginning of args.
 * @exe_spe: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int call_args(char **args, char **speed, int *exe_spe)
{
	int spe, space;

	if (!args[0])
		return (*exe_spe);
	for (space = 0; args[space]; space++)
	{
		if (_strncmp(args[space], "||", 2) == 0)
		{
			free(args[space]);
			args[space] = NULL;
			args = replace_aliases(args);
			spe = run_args(args, speed, exe_spe);
			if (*exe_spe != 0)
			{
				args = &args[++space];
				space = 0;
			}
			else
			{
				for (space++; args[space]; space++)
					free(args[space]);
				return (spe);
			}
		}
		else if (_strncmp(args[space], "&&", 2) == 0)
		{
			free(args[space]);
			args[space] = NULL;
			args = replace_aliases(args);
			spe = run_args(args, speed, exe_spe);
			if (*exe_spe == 0)
			{
				args = &args[++space];
				space = 0;
			}
			else
			{
				for (space++; args[space]; space++)
					free(args[space]);
				return (spe);
			}
		}
	}
	args = replace_aliases(args);
	spe = run_args(args, speed, exe_spe);
	return (spe);
}

/**
 * run_args - Calls the execution of a command.
 * @args: An array of arguments.
 * @speed: A double pointer to the beginning of args.
 * @exe_spe: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int run_args(char **args, char **speed, int *exe_spe)
{
	int spe, i;
	int (*builtin)(char **args, char **speed);

	builtin = get_builtin(args[0]);

	if (builtin)
	{
		spe = builtin(args + 1, speed);
		if (spe != EXIT)
			*exe_spe = spe;
	}
	else
	{
		*exe_spe = execute(args, speed);
		spe = *exe_spe;
	}

	hist++;

	for (i = 0; args[i]; i++)
		free(args[i]);

	return (spe);
}

/**
 * handle_args - Gets, calls, and runs the execution of a command.
 * @exe_spe: The return value of the parent process' last executed command.
 *
 * Return: If an end-of-file is read - END_OF_FILE (-2).
 *         If the input cannot be tokenized - -1.
 *         O/w - The exit value of the last executed command.
 */
int handle_args(int *exe_spe)
{
	int spe = 0, space;
	char **args, *line = NULL, **speed;

	line = get_args(line, exe_spe);
	if (line == NULL)
		return (END_OF_FILE);

	args = _strtok(line, " ");
	free(line);
	if (args == NULL)
		return (spe);
	if (check_args(args) != 0)
	{
		*exe_spe = 2;
		free_args(args, args);
		return (*exe_spe);
	}
	speed = args;

	for (space = 0; args[space]; space++)
	{
		if (_strncmp(args[space], ";", 1) == 0)
		{
			free(args[space]);
			args[space] = NULL;
			spe = call_args(args, speed, exe_spe);
			args = &args[++space];
			space = 0;
		}
	}
	if (args)
		spe = call_args(args, speed, exe_spe);

	free(speed);
	return (spe);
}

/**
 * check_args - Checks if there are any leading ';', ';;', '&&', or '||'.
 * @args: 2D pointer to tokenized commands and arguments.
 *
 * Return: If a ';', '&&', or '||' is placed at an invalid position - 2.
 *         Otherwise - 0.
 */
int check_args(char **args)
{
	size_t i;
	char *c, *ex;

	for (i = 0; args[i]; i++)
	{
		c = args[i];
		if (c[0] == ';' || c[0] == '&' || c[0] == '|')
		{
			if (i == 0 || c[1] == ';')
				return (create_error(&args[i], 2));
			ex = args[i + 1];
			if (ex && (ex[0] == ';' || ex[0] == '&' || ex[0] == '|'))
				return (create_error(&args[i + 1], 2));
		}
	}
	return (0);
}
