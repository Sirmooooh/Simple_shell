#include "shell.h"

char *fill_path_dir(char *path);
list_t *get_path_dir(char *path);

/**
 * get_location - Locates a command in the PATH.
 * @command: The command to locate.
 *
 * Return: If an error occurs or the command cannot be located - NULL.
 *         Otherwise - the full pathname of the command.
 */

char *get_location(char *command)
{
	char **path, *temp;
	list_t *mooh, *head;
	struct stat st;

	path = _getenv("PATH");
	if (!path || !(*path))
		return (NULL);

	mooh = get_path_dir(*path + 5);
	head = mooh;

	while (mooh)
	{
		temp = malloc(_strlen(mooh->dir) + _strlen(command) + 2);
		if (temp == NULL)
			return (NULL);

		_strcpy(temp, mooh->dir);
		_strcat(temp, "/");
		_strcat(temp, command);

		if (stat(temp, &st) == 0)
		{
			free_list(head);
			return (temp);
		}

		mooh = mooh->next;
		free(temp);
	}

	free_list(head);

	return (NULL);
}

/**
 * fill_path_dir - Copies path but also replaces leading/sandwiched/trailing
 *                 colons (:) with current working directory.
 * @path: The colon-separated list of directories.
 *
 * Return: A copy of path with any leading/sandwiched/trailing colons replaced
 *         with the current working directory.
 */

char *fill_path_dir(char *path)
{
	int j, length = 0;
	char *path_copy, *pwd;

	pwd = *(_getenv("PWD")) + 4;
	for (j = 0; path[j]; j++)
	{
		if (path[j] == ':')
		{
			if (path[j + 1] == ':' || j == 0 || path[j + 1] == '\0')
				length += _strlen(pwd) + 1;
			else
				length++;
		}
		else
			length++;
	}
	path_copy = malloc(sizeof(char) * (length + 1));

	if (path_copy == NULL)
		return (NULL);
	path_copy[0] = '\0';
	for (j = 0; path[j]; j++)
	{
		if (path[j] == ':')
		{
			if (j == 0)
			{
				_strcat(path_copy, pwd);
				_strcat(path_copy, ":");
			}
			else
				_strcat(path_copy, ":");
		}
		else
		{
			_strncat(path_copy, &path[j], 1);
		}
	}
	return (path_copy);
}

/**
 * get_path_dir - Tokenizes a colon-separated list of
 *                directories into a list_s linked list.
 * @path: The colon-separated list of directories.
 *
 * Return: A pointer to the initialized linked list.
 */

list_t *get_path_dir(char *path)
{
	int space;
	char **mooh, *path_copy;
	list_t *head = NULL;

	path_copy = fill_path_dir(path);

	if (path_copy == NULL)
		return (NULL);
	mooh = _strtok(path_copy, ":");
	free(path_copy);

	if (mooh == NULL)
		return (NULL);

	for (space = 0; mooh[space]; space++)
	{
		if (add_node_end(&head, mooh[space]) == NULL)
		{
			free_list(head);
			free(mooh);
			return (NULL);
		}
	}
	free(mooh);

	return (head);
}
