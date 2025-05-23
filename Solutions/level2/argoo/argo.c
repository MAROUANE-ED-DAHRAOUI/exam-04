/*Assignment name: argo
Expected files: argo.c
Allowed functions: getc, ungetc, printf, malloc, calloc, realloc, free, isdigit, fscanf, write
-----------------
Write a function argo that will parse a json file in the structure declared in argo.h:

int	argo(json *dst, FILE *stream);

	dst	- is the pointer to the AST that you will create
	stream	- is the file to parse (man FILE)

Your function will return 1 for success and -1 for failure.
If an unexpected token is found you will print the following message in stdout: "Unexpected token '%c'\n"
or if the token is EOF: "Unexpected end of input\n"
Handle escaping in string only for backslashes and quotation marks.
You don't have to handle spaces in the input (so they should be considered as invalid tokens).

In case of doubt how to parse json, read rfc8259. But you won't need it as the format is simple. Tested with the main, the output should be exactly the same as the input (except for errors).
There are some functions in argo.c that might help you.

Examples that should work:

echo -n input | ./argo /dev/stdin | cat -e		output
------------------------------------------		--------------------------------------------
'1'							1$
'"bonjour"'						"bonjour"$
'"escape!\""'						"escape!\""$
'{"tomatoes":42,"potatoes":234}'		    	{"tomatoes":42,"potatoes":234}$
'{"recursion":{"recursion":{"recursion":"recursion"}}}'	{"recursion":{"recursion":{"recursion":"recursion"}}}$
'"unfinished string'					Unexpected end of input$
'"unfinished string 2\"'				Unexpected end of input$
'{"no value?":}'					Unexpected token '}'$
'{:12}'							Unexpected token ':'$*/
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>

typedef struct	json {
	enum {
		MAP,
		INTEGER,
		STRING
	} type;
	union {
		struct {
			struct pair	*data;
			size_t		size;
		} map;
		int	integer;
		char	*string;
	};
}	json;

typedef struct	pair {
	char	*key;
	json	value;
}	pair;

void	free_json(json j);
int	argo(json *dst, FILE *stream);

int	peek(FILE *stream)
{
	int	c = getc(stream);
	ungetc(c, stream);
	return c;
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("unexpected token '%c'\n", peek(stream));
	else
		printf("unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return 1;
	}
	return 0;
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return 1;
	unexpected(stream);
	return 0;
}

int	parse_int(json *dst, FILE *stream)
{
	int n = 0;

	fscanf(stream, "%d", &n);
	dst->type = INTEGER;
	dst->integer = n;
	return (1);
}

char *get_str(FILE *stream)
{
	char *res = calloc(4096, sizeof(char));
	int   i = 0;
	char  c = getc(stream);

	while (1)
	{
		c = getc(stream);
		if (c == '"')
			break ;
		if (c == EOF)
		{
			unexpected(stream);
			return NULL;
		}
		if (c == '\\')
				c = getc(stream);
		res[i++] = c;
	}
	return (res);
}

int parse_map(json *dst, FILE *stream)
{
	dst->type = MAP;
	dst->map.size = 0;
	dst->map.data = NULL;
	char c = getc(stream);

	if (peek(stream) == '}')
		return 1;

	while (1)
	{
		c = peek(stream);
		if (c != '"')
		{
			unexpected(stream);
			return -1;
		}
		dst->map.data = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
		pair *current = &dst->map.data[dst->map.size];
		current->key = get_str(stream);
		if (current->key == NULL)
			return -1;
		dst->map.size++;
		if (expect(stream, ':') == 0)
			return -1;
		if (argo(&current->value, stream) == -1)
			return -1;
		c = peek(stream);
		if (c == '}')
		{
			accept(stream ,c);
			break ;
		}
		if (c == ',')
			accept(stream, ',');
		else
		{
			unexpected(stream);
			return -1;
		}
	}
	return 1;
}

int parser(json *dst, FILE *stream)
{
	int c = peek(stream);

	if (c == EOF)
	{
		unexpected(stream);
		return -1;
	}
	if (isdigit(c))
		return (parse_int(dst, stream));
	else if (c == '"')
	{
		dst->type = STRING;
		dst->string = get_str(stream);
		if (dst->string == NULL)
			return (-1);
		return (1);
	}
	else if (c == '{')
		return (parse_map(dst, stream));
	else
	{
		unexpected(stream);
		return -1;
	}
	return (1);
}

//free_json() added in the main
int argo(json *dst, FILE *stream)
{
	if (parser(dst, stream) == -1)
		return -1;
	return 1;
}

void	free_json(json j)
{
	switch (j.type)
	{
		case MAP:
			for (size_t i = 0; i < j.map.size; i++)
			{
				free(j.map.data[i].key);
				free_json(j.map.data[i].value);
			}
			free(j.map.data);
			break ;
		case STRING:
			free(j.string);
			break ;
		default:
			break ;
	}
}

void	serialize(json j)
{
	switch (j.type)
	{
		case INTEGER:
			printf("%d", j.integer);
			break ;
		case STRING:
			putchar('"');
			for (int i = 0; j.string[i]; i++)
			{
				if (j.string[i] == '\\' || j.string[i] == '"')
					putchar('\\');
				putchar(j.string[i]);
			}
			putchar('"');
			break ;
		case MAP:
			putchar('{');
			for (size_t i = 0; i < j.map.size; i++)
			{
				if (i != 0)
					putchar(',');
				serialize((json){.type = STRING, .string = j.map.data[i].key});
				putchar(':');
				serialize(j.map.data[i].value);
			}
			putchar('}');
			break ;
	}
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	char *filename = argv[1];
	FILE *stream = fopen(filename, "r");
	json	file;
	if (argo (&file, stream) != 1)
	{
		free_json(file);
		return 1;
	}
	serialize(file);
    free_json(file);
	printf("\n");
}