#include "shell.h"
int (*get_builtin(char *command))(char **args, char **speed);
int shell_exit(char **args, char **speed);
int shell_cd(char **args, char __attribute__((__unused__)) **speed);
int shell_help(char **args, char __attribute__((__unused__)) **speed);
/**
 * get_builtin - Matches a command with a relating
 *               shell builtin function.
 * @command: The command to match.
 *
 * Return: A function pointer to the related builtin.
 */
int (*get_builtin(char *command))(char **args, char **speed)
{
builtin_t funcs[] = {
{ "exit", shell_exit },
{ "env", shell_env },
{ "setenv", shell_setenv },
{ "unsetenv", shell_unsetenv },
{ "cd", shell_cd },
{ "alias", shell_alias },
{ "help", shell_help },
{ NULL, NULL }
};
int c;
for (c = 0; funcs[c].name; c++)
{
if (_strcmp(funcs[c].name, command) == 0)
break;
}
return (funcs[c].f);
}
/**
 * shell_exit - Causes normal process termination
 *                for the shell.
 * @args: An array of arguments containing the exit value.
 * @speed: A double pointer to the beginning of arguments.
 *
 * Return: If there are no arguments - -3.
 *         If the given exit value is invalid - 2.
 *         O/w - exits with the given current value.
 *
 * Description: Upon returning -3, the program exits back into main function.
 */
int shell_exit(char **args, char **speed)
{
int d, len_int = 10;
unsigned int num = 0, max = 1 << (sizeof(int) * 8 - 1);
if (args[0])
{
if (args[0][0] == '+')
{
d = 1;
len_int++;
}
for (; args[0][d]; d++)
{
if (d <= len_int && args[0][d] >= '0' && args[0][d] <= '9')
num = (num * 10) + (args[0][d] - '0');
else
return (create_error(--args, 2));
}
}
else
{
return (-3);
}
if (num > max - 1)
return (create_error(--args, 2));
args -= 1;
free_args(args, speed);
free_env();
free_alias_list(aliases);
exit(num);
}
/**
 * shell_cd - Changes the current directory of the shell process.
 * @args: An array of arguments.
 * @speed: A double pointer to the beginning of arguments.
 *
 * Return: If the given string is not a directory - 2.
 *         If an error occurs - -1.
 *         Otherwise - 0.
 */
int shell_cd(char **args, char __attribute__((__unused__)) **speed)
{
char **dir_inf, *new_line = "\n";
char *oldpwd = NULL, *pwd = NULL;
struct stat dir;
oldpwd = getcwd(oldpwd, 0);
if (oldpwd == NULL)
return (-1);
if (args[0])
{
if (*(args[0]) == '-' || _strcmp(args[0], "--") == 0)
{
if ((args[0][1] == '-' && args[0][2] == '\0') ||
args[0][1] == '\0')
{
if (_getenv("OLDPWD") != NULL)
(chdir(*_getenv("OLDPWD") + 7));
}
else
{
free(oldpwd);
return (create_error(args, 2));
}
}
else
{
if (stat(args[0], &dir) == 0 && S_ISDIR(dir.st_mode)
&& ((dir.st_mode & S_IXUSR) != 0))
chdir(args[0]);
else
{
free(oldpwd);
return (create_error(args, 2));
}
}
}
else
{
if (_getenv("HOME") != NULL)
chdir(*(_getenv("HOME")) + 5);
}
pwd = getcwd(pwd, 0);
if (pwd == NULL)
return (-1);
dir_inf = malloc(sizeof(char *) * 2);
if (dir_inf == NULL)
return (-1);
dir_inf[0] = "OLDPWD";
dir_inf[1] = oldpwd;
if (shell_setenv(dir_inf, dir_inf) == -1)
return (-1);
dir_inf[0] = "PWD";
dir_inf[1] = pwd;
if (shell_setenv(dir_inf, dir_inf) == -1)
return (-1);
if (args[0] && args[0][0] == '-' && args[0][1] != '-')
{
write(STDOUT_FILENO, pwd, _strlen(pwd));
write(STDOUT_FILENO, new_line, 1);
}
free(oldpwd);
free(pwd);
free(dir_inf);
return (0);
}
/**
 * shell_help - Displays info about shell builtin commands.
 * @args: An array of arguments.
 * @speed: A pointer to the beginning of arguments.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int shellby_help(char **args, char __attribute__((__unused__)) **speed)
{
if (args[0] == NULL)
help_all();
else if (_strcmp(args[0], "alias") == 0)
help_alias();
else if (_strcmp(args[0], "cd") == 0)
help_cd();
else if (_strcmp(args[0], "exit") == 0)
help_exit();
else if (_strcmp(args[0], "env") == 0)
help_env();
else if (_strcmp(args[0], "setenv") == 0)
help_setenv();
else if (_strcmp(args[0], "unsetenv") == 0)
help_unsetenv();
else if (_strcmp(args[0], "help") == 0)
help_help();
else
write(STDERR_FILENO, name, _strlen(name));
return (0);
}
