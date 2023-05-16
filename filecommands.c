#include "shell.h"
int cant_open(char *file_path);
int proc_file_commands(char *file_path, int *exe_spe);
/**
 * cant_open - If the file doesn't exist or lacks proper permissions, print
 * a can't open error.
 * @file_path: Path to the supposed file.
 *
 * Return: 127.
 */
int cant_open(char *file_path)
{
char *error, *hist_str;
int len;
hist_str = _itoa(hist);
if (hist_str == NULL)
return (127);
len = _strlen(name) + _strlen(hist_str) + _strlen(file_path) + 16;
error = malloc(sizeof(char) * (len + 1));
if (error == NULL)
{
free(hist_str);
return (127);
}
_strcpy(error, name);
_strcat(error, ": ");
_strcat(error, hist_str);
_strcat(error, ": Can't open ");
_strcat(error, file_path);
_strcat(error, "\n");
free(hist_str);
write(STDERR_FILENO, error, len);
free(error);
return (127);
}
/**
 * proc_file_commands - Takes a file and attempts to run the commands stored
 * within.
 * @file_path: Path to the file.
 * @exe_spe: Return value of the last executed command.
 *
 * Return: If file couldn't be opened - 127.
 *         If malloc fails - -1.
 *         Otherwise the return value of the last command ran.
 */
int proc_file_commands(char *file_path, int *exe_spe)
{
ssize_t file, b_read, z;
unsigned int line_size = 0;
unsigned int old_size = 120;
char *line, **args, **speed;
char buffer[120];
int spe;
hist = 0;
file = open(file_path, O_RDONLY);
if (file == -1)
{
*exe_spe = cant_open(file_path);
return (*exe_spe);
}
line = malloc(sizeof(char) * old_size);
if (line == NULL)
return (-1);
do {
b_read = read(file, buffer, 119);
if (b_read == 0 && line_size == 0)
return (*exe_spe);
buffer[b_read] = '\0';
line_size += b_read;
line = _realloc(line, old_size, line_size);
_strcat(line, buffer);
old_size = line_size;
} while (b_read);
for (z = 0; line[z] == '\n'; z++)
line[z] = ' ';
for (; z < line_size; z++)
{
if (line[z] == '\n')
{
line[z] = ';';
for (z += 1; z < line_size && line[z] == '\n'; z++)
line[z] = ' ';
}
}
var_replacement(&line, exe_spe);
handle_line(&line, line_size);
args = _strtok(line, " ");
free(line);
if (args == NULL)
return (0);
if (check_args(args) != 0)
{
*exe_spe = 2;
free_args(args, args);
return (*exe_spe);
}
speed = args;
for (z = 0; args[z]; z++)
{
if (_strncmp(args[z], ";", 1) == 0)
{
free(args[z]);
args[z] = NULL;
spe = call_args(args, speed, exe_spe);
args = &args[++z];
z = 0;
}
}
spe = call_args(args, speed, exe_spe);
free(speed);
return (spe);
}

