#include "shell.h"

int shell_alias(char **args, char __attribute__((__unused__)) **speed);
void set_alias(char *var_name, char *val);
void print_alias(alias_t *alias);

/**
 * shell_alias - Builtin command that either prints all aliases, specific
 * aliases, or sets an alias.
 * @args: An array of arguments.
 * @speed: A double pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int shell_alias(char **args, char __attribute__((__unused__)) **speed)
{
	alias_t *tmp = aliases;
	int i, mooh = 0;
	char *val;

	if (args[0] == NULL)
	{
		while (tmp)
		{
			print_alias(tmp);
			tmp = tmp->next;
		}
		return (mooh);
	}
	for (i = 0; args[i]; i++)
	{
		tmp = aliases;
		val = _strchr(args[i], '=');
		if (val == NULL)
		{
			while (tmp)
			{
				if (_strcmp(args[i], tmp->name) == 0)
				{
					print_alias(tmp);
					break;
				}
				tmp = tmp->next;
			}
			if (tmp == NULL)
				mooh = create_error(args + i, 1);
		}
		else
			set_alias(args[i], val);
	}
	return (mooh);
}

/**
 * set_alias - Will either set an existing alias 'name' with a new val,
 * 'val' or creates a new alias with 'name' and 'value'.
 * @var_name: Name of the alias.
 * @val: Value of the alias.
 */
void set_alias(char *var_name, char *val)
{
	alias_t *tmp = aliases;
	int len, j, k;
	char *new_val;

	*val = '\0';
	val++;
	len = _strlen(val) - _strspn(val, "'\"");
	new_val = malloc(sizeof(char) * (len + 1));
	if (new_val == NULL)
		return;
	for (j = 0, k = 0; val[j]; j++)
	{
		if (val[j] != '\'' && val[j] != '"')
			new_val[k++] = val[j];
	}
	new_val[k] = '\0';
	while (tmp)
	{
		if (_strcmp(var_name, tmp->name) == 0)
		{
			free(tmp->val);
			tmp->val = new_val;
			break;
		}
		tmp = tmp->next;
	}
	if (tmp == NULL)
		add_alias_end(&aliases, var_name, new_val);
}

/**
 * print_alias - Prints the alias in the format name='val'.
 * @alias: Pointer to an alias.
 */
void print_alias(alias_t *alias)
{
	char *alias_str;
	int len = _strlen(alias->name) + _strlen(alias->val) + 4;

	alias_str = malloc(sizeof(char) * (len + 1));
	if (alias_str == NULL)
		return;
	_strcpy(alias_str, alias->name);
	_strcat(alias_str, "='");
	_strcat(alias_str, alias->val);
	_strcat(alias_str, "'\n");

	write(STDOUT_FILENO, alias_str, len);
	free(alias_str);
}

/**
 * replace_aliases - Goes through the arguments and replace any matching alias
 * with their value.
 * @args: 2D pointer to the arguments.
 *
 * Return: 2D pointer to the arguments.
 */
char **replace_aliases(char **args)
{
	alias_t *tmp;
	int j;
	char *new_val;

	if (_strcmp(args[0], "alias") == 0)
		return (args);
	for (j = 0; args[j]; j++)
	{
		tmp = aliases;
		while (tmp)
		{
			if (_strcmp(args[j], tmp->name) == 0)
			{
				new_val = malloc(sizeof(char) * (_strlen(tmp->val) + 1));
				if (new_val == NULL)
				{
					free_args(args, args);
					return (NULL);
				}
				_strcpy(new_val, tmp->val);
				free(args[j]);
				args[j] = new_val;
				j--;
				break;
			}
			tmp = tmp->next;
		}
	}

	return (args);
}

