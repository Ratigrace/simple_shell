#include "shell.h"

/**
 * free_args - free up memory used by args
 * @args: null terminated double pointer containing commands and arguments
 * @front: double pointer to beginning of args
 */

void free_args(char **args, char **front)

{
	size_t i;

	for (i = 0; args[i] || args[i + 1]; i++)
		free(args[i]);

	free(front);
}

/**
 * get_pid - fetch current process ID
 * Description: open stat file, space delimited file containing info
 * on current process, PID is the first word in file, function reads
 * PID into buffer and replaces the space at end of \0 byte
 *
 * Return: current process ID or NULL if failure
 */

char *get_pid(void)
{
	size_t i = 0;
	char *buffer;
	ssize_t file;

	file = open("/proc/self/stat", O_RDONLY);
	if (file == -1)
	{
		perror("Cant read file");
		return (NULL);
	}
	buffer = malloc(120);
	if (!buffer)
	{
		close(file);
		return (NULL);
	}
	read(file, buffer, 120);
	while (buffer[i] != ' ')
		i++;
	buffer[i] = '\0';

	close(file);
	return (buffer);
}

/**
 * get_env_value - fetch value that corresponds to environmental variable
 * @beginning: environmental variable to be searched
 * @len: length of environmental variable to be searched
 *
 * Return: If no variable is found return empty string Otherwise
 * return value of enviromental variable
 * Description: variables stored in format VARIABLE=VALUE
 */

char *get_env_value(char *beginning, int len)

{
	char **var_addr;
	char *replacement = NULL, *temp, *var;

	var = malloc(len + 1);
	if (!var)
		return (NULL);
	var[0] = '\0';
	_strncat(var, beginning, len);

	var_addr = _getenv(var);
	free(var);
	if (var_addr)
	{
		temp = *var_addr;
		while (*temp != '=')
			temp++;
		temp++;
		replacement = malloc(_strlen(temp) + 1);
		if (replacement)
			_strcpy(replacement, temp);
	}

	return (replacement);
}

/**
 *  var_rep - handle variable replacement
 * @line: double pointer containing command & arguments
 * @exe_ret: pointer to return value of last executed command
 *
 * Description: replace $$ with current PID $? with return value
 * of last executed program & eviromental variables,
 * preceded by $ with corresponding value
 */

void  var_rep(char **line, int *exe_ret)

{
	int j, k = 0, len;
	char *replacement = NULL, *old_line = NULL, *new_line;

	old_line = *line;
	for (j = 0; old_line[j]; j++)
	{
		if (old_line[j] == '$' && old_line[j + 1] &&
		    old_line[j + 1] != ' ')
		{
			if (old_line[j + 1] == '$')
			{
				replacement = get_pid();
				k = j + 2;
			}
			else if (old_line[j + 1] == '?')
			{
				replacement = _itoa(*exe_ret);
				k = j + 2;
			}
			else if (old_line[j + 1])
			{
				/* extract variable name to search */
				for (k = j + 1; old_line[k] &&
					     old_line[k] != '$' &&
					     old_line[k] != ' '; k++)
					;
				len = k - (j + 1);
				replacement = get_env_value(&old_line[j + 1], len);
			}
			new_line = malloc(j + _strlen(replacement)
					  + _strlen(&old_line[k]) + 1);
			if (!line)
				return;
			new_line[0] = '\0';
			_strncat(new_line, old_line, j);
			if (replacement)
			{
				_strcat(new_line, replacement);
				free(replacement);
				replacement = NULL;
			}
			_strcat(new_line, &old_line[k]);
			free(old_line);
			*line = new_line;
			old_line = new_line;
			j = -1;
		}
	}
}
