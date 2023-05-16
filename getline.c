#include "shell.h"
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
void assign_lineptr(char **lineptr, size_t *n, char *buffer, size_t q);
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);
/**
 * _realloc - Reallocates a memory block using malloc and free.
 * @ptr: A pointer to the memory previously allocated.
 * @old_size: The size of bytes of the allocated space for ptr.
 * @new_size: The size of bytes for the new memory block.
 *
 * Return: If new_size == old_size - ptr.
 *         If new_size == 0 and ptr is not NULL - NULL.
 *         Otherwise - a pointer to the reallocated memory block.
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
void *mem;
char *ptr_copy, *input;
unsigned int space;
if (new_size == old_size)
return (ptr);
if (ptr == NULL)
{
mem = malloc(new_size);
if (mem == NULL)
return (NULL);
return (mem);
}
if (new_size == 0 && ptr != NULL)
{
free(ptr);
return (NULL);
}
ptr_copy = ptr;
mem = malloc(sizeof(*ptr_copy) * new_size);
if (mem == NULL)
{
free(ptr);
return (NULL);
}
input = mem;
for (space = 0; space < old_size && space < new_size; space++)
input[space] = *ptr_copy++;
free(ptr);
return (mem);
}
/**
 * assign_lineptr - Reassigns the lineptr variable for _getline.
 * @lineptr: A buffer to store an input string.
 * @n: The size of lineptr.
 * @buffer: The string to assign to lineptr.
 * @q: The size of buffer.
 */
void assign_lineptr(char **lineptr, size_t *n, char *buffer, size_t q)
{
if (*lineptr == NULL)
{
if (q > 120)
*n = q;
else
*n = 120;
*lineptr = buffer;
}
else if (*n < q)
{
if (q > 120)
*n = q;
else
*n = 120;
*lineptr = buffer;
}
else
{
_strcpy(*lineptr, buffer);
free(buffer);
}
}
/**
 * _getline - Reads input from a stream.
 * @lineptr: A buffer to store the input.
 * @n: The size of lineptr.
 * @stream: The stream to read from.
 *
 * Return: The number of bytes read.
 */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
static ssize_t input;
ssize_t spe;
char d = 'x', *buffer;
int z;
if (input == 0)
fflush(stream);
else
return (-1);
input = 0;
buffer = malloc(sizeof(char) * 120);
if (buffer == NULL)
return (-1);
while (d != '\n')
{
z = read(STDIN_FILENO, &d, 1);
if (z == -1 || (z == 0 && input == 0))
{
free(buffer);
return (-1);
}
if (z == 0 && input != 0)
{
input++;
break;
}
if (input >= 120)
buffer = _realloc(buffer, input, input + 1);
buffer[input] = d;
input++;
}
buffer[input] = '\0';
assign_lineptr(lineptr, n, buffer, input);
spe = input;
if (z != 0)
input = 0;
return (spe);
}
