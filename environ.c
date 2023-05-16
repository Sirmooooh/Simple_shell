#include "shell.h"
char **_copyenv(void);
void free_env(void);
char **_getenv(const char *var);
/**
 * _copyenv - Creates a copy of the environment.
 *
 * Return: If an error occurs - NULL.
 *         O/w - a double pointer to the new copy.
 */
char **_copyenv(void)
{
char **new_env;
size_t size;
int space;
for (size = 0; environ[size]; size++)
;
new_env = malloc(sizeof(char *) * (size + 1));
if (new_env == NULL)
return (NULL);
for (space = 0; environ[space]; space++)
{
new_env[space] = malloc(_strlen(environ[space]) + 1);
if (!new_env[space])
{
for (space--; space >= 0; space--)
free(new_env[space]);
free(new_env);
return (NULL);
}
_strcpy(new_env[space], environ[space]);
}
new_env[space] = NULL;
return (new_env);
}
/**
 * free_env - Frees the the environment copy.
 */
void free_env(void)
{
int space;
for (space = 0; environ[space]; space++)
free(environ[space]);
free(environ);
}
/**
 * _getenv - Gets an environmental variable from the PATH.
 * @var: The name of the environmental variable to get.
 *
 * Return: If the environmental variable does not exist - NULL.
 *         Otherwise - a pointer to the environmental variable.
 */
char **_getenv(const char *var)
{
int space, len;
len = _strlen(var);
for (space = 0; environ[space]; space++)
{
if (_strncmp(var, environ[space], len) == 0)
return (&environ[space]);
}
return (NULL);
}
