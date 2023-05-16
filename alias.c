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
OB
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
