#include "shell.h"

/**
 * _realloc - reallocates memory block with malloc, free
 * @ptr: pointer to memory previously allocated
 * @old_size: size in bytes of allocated space for the ptr
 * @new_size: size in bytes for new memory block
 *
 * Return: If new_size == old_size - ptr, If new_size == 0 ptr is
 * not NULL - NULL Otherwise a pointer to reallocated memory block
 */

void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)

{
	void *mem;
	char *ptr_copy, *filler;
	unsigned int index;

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

	filler = mem;

	for (index = 0; index < old_size && index < new_size; index++)
		filler[index] = *ptr_copy++;

	free(ptr);
	return (mem);
}

/**
 * new_lineptrptr - reassign lineptr variable for _getline
 * @new_lineptr: buffer to store the input string
 * @n: size of lineptr
 * @buffer:string to assign lineptr
 * @b: size of buffer
 */

void  new_lineptrptr(char **lineptr, size_t *n, char *buffer, size_t b)

{
	if (*lineptr == NULL)
	{
		if (b > 120)
			*n = b;
		else
			*n = 120;
		*lineptr = buffer;
	}
	else if (*n < b)
	{
		if (b > 120)
			*n = b;
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
 * _getline - read input from stream
 * @new_lineptr: buffer to store the input
 * @n: size of lineptr
 * @stream: stream to read from
 *
 * Return: number of bytes read
 */

ssize_t _getline(char **lineptr, size_t *n, FILE *stream)

{
	static ssize_t input;
	ssize_t ret;
	char c = 'x', *buffer;
	int r;

	if (input == 0)
		fflush(stream);
	else
		return (-1);
	input = 0;

	buffer = malloc(sizeof(char) * 120);
	if (!buffer)
		return (-1);

	while (c != '\n')
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r == -1 || (r == 0 && input == 0))
		{
			free(buffer);
			return (-1);
		}
		if (r == 0 && input != 0)
		{
			input++;
			break;
		}
		if (input >= 120)
			buffer = _realloc(buffer, input, input + 1);

		buffer[input] = c;
		input++;
	}
	buffer[input] = '\0';

	 new_lineptrptr(lineptr, n, buffer, input);

	ret = input;
	if (r != 0)
		input = 0;
	return (ret);
}
