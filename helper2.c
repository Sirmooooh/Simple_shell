#include "shell.h"

void handle_line(char **line, ssize_t read);
ssize_t get_new_len(char *line);
void logical_ops(char *line, ssize_t *new_len);

/**
 * handle_line - Partitions a line read from standard input as needed.
 * @line: A pointer to a line read from standard input.
 * @read: The length of line.
 *
 * Description: Spaces are inserted to separate ";", "||", and "&&".
 *              Replaces "#" with '\0'.
 */

void handle_line(char **line, ssize_t read)
{

char *o_line, *n_line;
char previous, current, next;
size_t q, r;
ssize_t new_len;

new_len = get_new_len(*line);
if (new_len == read - 1)
return;
n_line = malloc(new_len + 1);
if (n_line == NULL)
return;
r = 0;
o_line = *line;
for (q = 0; o_line[q]; q++)
{
current = o_line[q];
next = o_line[q + 1];
if (q != 0)
{
previous = o_line[q - 1];
if (current == ';')
{
if (next == ';' && previous != ' ' && previous != ';')
{
n_line[r++] = ' ';
n_line[r++] = ';';
continue;
}
else if (previous == ';' && next != ' ')
{
n_line[r++] = ';';
n_line[r++] = ' ';
continue;
}
if (previous != ' ')
n_line[r++] = ' ';
n_line[r++] = ';';
if (next != ' ')
n_line[r++] = ' ';
continue;
}
else if (current == '&')
{
if (next == '&' && previous != ' ')
n_line[r++] = ' ';
else if (previous == '&' && next != ' ')
{
n_line[r++] = '&';
n_line[r++] = ' ';
continue;
}
}
else if (current == '|')
{
if (next == '|' && previous != ' ')
n_line[r++]  = ' ';
else if (previous == '|' && next != ' ')
{
n_line[r++] = '|';
n_line[r++] = ' ';
continue;
}
}
}
else if (current == ';')
{
if (q != 0 && o_line[q - 1] != ' ')
n_line[r++] = ' ';
n_line[r++] = ';';
if (next != ' ' && next != ';')
n_line[r++] = ' ';
continue;
}
n_line[r++] = o_line[q];
}
n_line[r] = '\0';

free(*line);
*line = n_line;
}

/**
 * get_new_len - Gets the new length of a line partitioned
 *               by ";", "||", "&&&", or "#".
 * @line: The line to check.
 *
 * Return: The new length of the line.
 *
 * Description: Cuts short lines containing '#' comments with '\0'.
 */

ssize_t get_new_len(char *line)
{
size_t i;
ssize_t new_len = 0;
char current, next;

for (i = 0; line[i]; i++)
{
current = line[i];
next = line[i + 1];
if (current == '#')
{
if (i == 0 || line[i - 1] == ' ')
{
line[i] = '\0';
break;
}
}
else if (i != 0)
{
if (current == ';')
{
if (next == ';' && line[i - 1] != ' ' && line[i - 1] != ';')
{
new_len += 2;
continue;
}
else if (line[i - 1] == ';' && next != ' ')
{
new_len += 2;
continue;
}
if (line[i - 1] != ' ')
new_len++;
if (next != ' ')
new_len++;
}
else
logical_ops(&line[i], &new_len);
}
else if (current == ';')
{
if (i != 0 && line[i - 1] != ' ')
new_len++;
if (next != ' ' && next != ';')
new_len++;
}
new_len++;
}
return (new_len);
}


/**
 * logical_ops - Checks a line for logical operators "||" or "&&".
 * @line: A pointer to the character to check in the line.
 * @new_len: Pointer to new_len in get_new_len function.
 */
void logical_ops(char *line, ssize_t *new_len)
{
char previous, current, next;

previous = *(line - 1);
current = *line;
next = *(line + 1);

if (current == '&')
{
if (next == '&' && previous != ' ')
(*new_len)++;
else if (previous == '&' && next != ' ')
(*new_len)++;
}
else if (current == '|')
{
if (next == '|' && previous != ' ')
(*new_len)++;
else if (previous == '|' && next != ' ')
(*new_len)++;
}
}
