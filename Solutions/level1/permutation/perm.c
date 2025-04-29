/*permutations

expected files: *.c *.h
function allowed: puts, malloc, calloc, realloc, free, write

write a program that will print all the permutations of a string given as an argument
solutions must be given in an alphabetical order

`
./permutations a
a

./permutations ab
ab	
ba

./permutations abc
abc
acb
bac
bca
cab
cba
*/

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

void	ft_putstr(char *s)
{
	while (*s)
		write(1, s++, 1);
	write(1, "\n", 1);
}

int	ft_strlen(char *s)
{
	int	i = 0;

	while (s[i])
		i++;
	return (i);
}

void	bubble_sort(char *set, int len)
{
	int	i = -1;
	int	j;
	char	tmp;
	bool	sorted = true;

	while (++i < len)
	{
		j = -1;
		sorted = true;
		while (++j < len - 1)
		{
			if (set[j] > set[j + 1])
			{
				tmp = set[j];
				set[j] = set[j + 1];
				set[j + 1] = tmp;
				sorted = false;
			}
		}
		if (sorted)
				break ;
	}
}

void	process(char *set, int len, int idx, char *tmp, bool *was_included)
{
	int	i = -1;

	if (idx == len)
	{
		ft_putstr(tmp);
		return ;
	}
	
	while (++i < len)
	{
		if (was_included[i])
			continue ;
		tmp[idx] = set[i];
		was_included[i] = true;
		idx++;
		process(set, len, idx, tmp, was_included);
		idx--;
		was_included[i] = false;
	}
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (1);

	int	    len = ft_strlen(argv[1]);
	char	set[len + 1];
	char	tmp[len + 1];
	bool	was_included[len];

	int	    i = -1; 
	while (++i < len)
	{
		set[i] = argv[1][i];
		tmp[i] = 48;
		was_included[i] = false;
	}
	set[len] = '\0';
	tmp[len] = '\0';
	bubble_sort(set, len);
	process(set, len, 0, tmp, was_included);
    return 0;
}
