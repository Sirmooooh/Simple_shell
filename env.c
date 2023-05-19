#include "shell.h"

int shell_env(char **args, char __attribute__((__unused__)) **speed);
int shell_setenv(char **args, char __attribute__((__unused__)) **speed);
int shell_unsetenv(char **args, char __attribute__((__unused__)) **speed);

/**
 * shell_env - Prints the current environment.
 * @args: An array of arguments passed to the shell.
 * @speed: A double pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 *
 *
 * Description: Prints one variable per line in the
 *              format 'variable'='value'.
 */

int shell_env(char **args, char __attribute__((__unused__)) **speed)
{
	int space;
	char c = '\n';

	if (environ == NULL)
		return (-1);

	for (space = 0; environ[space]; space++)
	{
		write(STDOUT_FILENO, environ[space], _strlen(environ[space]));
		write(STDOUT_FILENO, &c, 1);
	}

	(void)args;
	return (0);
}

/**
 * shell_setenv - Changes or adds an environmental variable to the PATH.
 * @args: An array of arguments passed to the shell.
 * @speed: A double pointer to the beginning of args.
 * Description: args[1] is the name of the new or existing PATH variable.
 *              args[2] is the value to set the new or changed variable to.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */

int shell_setenv(char **args, char __attribute__((__unused__)) **speed)
{
	char **env_var = NULL, **new_env, *new_val;
	size_t size;
	int space;

	if (!args[0] || !args[1])
		return (create_error(args, -1));

	new_val = malloc(_strlen(args[0]) + 1 + _strlen(args[1]) + 1);
	if (new_val == NULL)
		return (create_error(args, -1));
	_strcpy(new_val, args[0]);
	_strcat(new_val, "=");
	_strcat(new_val, args[1]);

	env_var = _getenv(args[0]);
	if (env_var)
	{
		free(*env_var);
		*env_var = new_val;
		return (0);
	}
	for (size = 0; environ[size]; size++)
		;

	new_env = malloc(sizeof(char *) * (size + 2));
	if (new_env == NULL)
	{
		free(new_val);
		return (create_error(args, -1));
	}

	for (space = 0; environ[space]; space++)
		new_env[space] = environ[space];

	free(environ);
	environ = new_env;
	environ[space] = new_val;
	environ[space + 1] = NULL;

	return (0);
}

/**
 * shell_unsetenv - Deletes an environmental variable from the PATH.
 * @args: An array of arguments passed to the shell.
 * @speed: A double pointer to the beginning of args.
 * Description: args[1] is the PATH variable to remove.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int shell_unsetenv(char **args, char __attribute__((__unused__)) **speed)
{
	char **env_var, **new_env;
	size_t size;
	int space, space2;

	if (!args[0])
		return (create_error(args, -1));
	env_var = _getenv(args[0]);
	if (env_var == NULL)
		return (0);

	for (size = 0; environ[size]; size++)
		;

	new_env = malloc(sizeof(char *) * size);
	if (new_env == NULL)
		return (create_error(args, -1));

	for (space = 0, space2 = 0; environ[space]; space++)
	{
		if (*env_var == environ[space])
		{
			free(*env_var);
			continue;
		}
		new_env[space2] = environ[space];
		space2++;
	}
	free(environ);
	environ = new_env;
	environ[size - 1] = NULL;

	return (0);
}
