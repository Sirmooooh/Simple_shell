#include "shell.h"

void free_args(char **args, char **speed);
char *get_pid(void);
char *get_env_value(char *beginning, int len);
void var_replacement(char **args, int *exe_spe);

/**
 * free_args - Frees up memory taken by args.
 * @args: A null-terminated double pointer containing commands/arguments.
 * @speed: A double pointer to the beginning of args.
 */
void free_args(char **args, char **speed)
{
	size_t m;

	for (m = 0; args[m] || args[m + 1]; m++)
		free(args[m]);

	free(speed);
}

/**
 * get_pid - Gets the current process ID.
 * Description: Opens the stat file, a space-delimited file containing
 *              information about the current process. The PID is the
 *              first word in the file. The function reads the PID into
 *              a buffer and replace the space at the end with a \0 byte.
 *
 * Return: The current process ID or NULL on failure.
 */
char *get_pid(void)
{
	size_t m = 0;
	char *buffer;
	ssize_t file;

	file = open("/proc/self/stat", O_RDONLY);
	if (file == -1)
	{
		perror("Cant read file");
		return (NULL);
	}
	buffer = malloc(120);
	if (buffer == NULL)
	{
		close(file);
		return (NULL);
	}
	read(file, buffer, 120);
	while (buffer[m] != ' ')
		m++;
	buffer[m] = '\0';

	close(file);
	return (buffer);
}

/**
 * get_env_value - Gets the value corresponding to an environmental variable.
 * @beginning: The environmental variable to search for.
 * @len: The length of the environmental variable to search for.
 *
 * Return: If the variable is not found - an empty string.
 *         Otherwise - the value of the environmental variable.
 *
 * Description: Variables are stored in the format VARIABLE=VALUE.
 */
char *get_env_value(char *beginning, int len)
{
	char **var_add;
	char *replacement = NULL, *tmp, *var;

	var = malloc(len + 1);
	if (var == NULL)
		return (NULL);
	var[0] = '\0';
	_strncat(var, beginning, len);

	var_add = _getenv(var);
	free(var);
	if (var_add)
	{
		tmp = *var_add;
		while (*tmp != '=')
			tmp++;
		tmp++;
		replacement = malloc(_strlen(tmp) + 1);
		if (replacement)
			_strcpy(replacement, tmp);
	}

	return (replacement);
}

/**
 * var_replacement - Handles variable replacement.
 * @line: A double pointer containing the command and arguments.
 * @exe_spe: A pointer to the return value of the last executed command.
 *
 * Description: Replaces $$ with the current PID, $? with the return value
 *              of the last executed program, and envrionmental variables
 *              preceded by $ with their corresponding value.
 */
void var_replacement(char **line, int *exe_spe)
{
	int i, j = 0, len;
	char *replacement = NULL, *o_line = NULL, *n_line;

	o_line = *line;
	for (i = 0; o_line[i]; i++)
	{
		if (o_line[i] == '$' && o_line[i + 1] &&
				o_line[i + 1] != ' ')
		{
			if (o_line[i + 1] == '$')
			{
				replacement = get_pid();
				j = i + 2;
			}
			else if (o_line[i + 1] == '?')
			{
				replacement = _itoa(*exe_spe);
				j = i + 2;
			}
			else if (o_line[i + 1])
			{
				for (j = i + 1; o_line[j] &&
						o_line[j] != '$' &&
						o_line[j] != ' '; j++)
					;
				len = j - (i + 1);
				replacement = get_env_value(&o_line[j + 1], len);
			}
			n_line = malloc(j + _strlen(replacement)
					+ _strlen(&o_line[j]) + 1);
			if (line == NULL)
				return;
			n_line[0] = '\0';
			_strncat(n_line, o_line, i);
			if (replacement)
			{
				_strcat(n_line, replacement);
				free(replacement);
				replacement = NULL;
			}
			_strcat(n_line, &o_line[j]);
			free(o_line);
			*line = n_line;
			o_line = n_line;
			i = -1;
		}
	}
}
