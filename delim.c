#include "shell.h"

int token_len(char *str, char *delim);
int count_tokens(char *str, char *delim);
char **_strtok(char *line, char *delim);

/**
 * token_len - Locates the delimiter index marking the end
 *             of the first token contained within a string.
 * @str: The string to be searched.
 * @delim: The delimiter character.
 *
 * Return: The delimiter index marking the end of
 *         the intitial token pointed to be str.
 */
int token_len(char *str, char *delim)
{
	int space = 0, len = 0;

	while (*(str + space) && *(str + space) != *delim)
	{
		len++;
		space++;
	}

	return (len);
}


/**
 * count_tokens - Counts the number of delimited
 *                words contained within a string.
 * @str: The string to be searched.
 * @delim: The delimiter character.
 *
 * Return: The number of words contained within str.
 */
int count_tokens(char *str, char *delim)
{
	int space, tokens = 0, len = 0;

	for (space = 0; *(str + space); space++)
		len++;

	for (space = 0; space < len; space++)
	{
		if (*(str + space) != *delim)
		{
			tokens++;
			space += token_len(str + space, delim);
		}
	}

	return (tokens);
}

/**
 * _strtok - Tokenizes a string.
 * @line: The string.
 * @delim: The delimiter character to tokenize the string by.
 *
 * Return: A pointer to an array containing the tokenized words.
 */
char **_strtok(char *line, char *delim)
{
	char **str;
	int space = 0, tokens, s, letters, u;

	tokens = count_tokens(line, delim);
	if (tokens == 0)
		return (NULL);

	str = malloc(sizeof(char *) * (tokens + 2));
	if (str == NULL)
		return (NULL);

	for (s = 0; s < tokens; s++)
	{
		while (line[space] == *delim)
			space++;
		letters = token_len(line + space, delim);

		str[s] = malloc(sizeof(char) * (letters + 1));
		if (str[s] == NULL)
		{
			for (space -= 1; space >= 0; space--)
				free(str[space]);
			free(str);
			return (NULL);
		}

		for (u = 0; u < letters; u++)
		{
			str[s][u] = line[space];
			space++;
		}

		str[s][u] = '\0';
	}

	str[s] = NULL;
	str[s + 1] = NULL;

	return (str);
}
