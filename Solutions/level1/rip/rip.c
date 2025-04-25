/*rip
===============================================================================
allowed files: *.c, *.h
allowed functions: puts(), write()
===============================================================================
Write a program that will take as argument a string containing only parenthesis.

If the parenthesis are unbalanced (for example '())'), your program shall remove
the minimum number of parenthesis for the expression to be balanced.

You will then print all the solutions. (can be more than one)

The order of the solutions is not important.
===============================================================================

Examples:

> ./a.out '(()' | cat -e
 ()$
( )$
> ./a.out '((()()())())' | cat -e
((()()())())$
> ./a.out '()())()' | cat -e
()() ()$
()( )()$
( ())()$
> ./a.out '(()(()(' | cat -e 
(()  ) $
( )( ) $
( ) () $
 ()( ) $
 () () $

===============================================================================
my notes:
	Q: what about  '))))' ?
	A: four spaces '    ' */

#include <unistd.h>

int calcul_removals(char *str)
{
	  int open;
	  int close;
	  int i;
	
	  open = 0;
	  close = 0;
	  i = 0;
	  while (str[i])
	  {
		if (str[i] == '(')
		  open++;
		else if (str[i] == ')')
		{
		  if(open > 0)
			open--;
		  else
			close++;
		}
		i++;
	  }
	return (open + close);
}
	
int valid_str(char *str)
{
	  int i;
	  int b;
	  
	  i = 0;
	  b = 0;
	  while (str[i])
	  {
		if (str[i] == '(')
		  b++;
		else if (str[i] == ')')
		  b--;
		if (b < 0)
		  return 1;
		i++;
	  }
	  if (b != 0)
		return 1;
	  return 0;
}
	
void  gen_valid_strs(char *str, int i, int len, int removals)
{
	  if (i == len)
	  {
		if (removals == 0 && !valid_str(str)) // add check for balanced str
		{
		  write(1, str, len);
		  write(1, "\n", 1);
		}
		  return ;
	  }
		if (str[i] != '(' && str[i] != ')')
		{
		  gen_valid_strs(str, i + 1, len, removals);
		  return ;
		}
	  if (removals > 0)
	  {
		char  old = str[i];
		str[i] = ' ';
		gen_valid_strs(str, i + 1, len, removals - 1);
		str[i] = old;
	  }
	  gen_valid_strs(str, i + 1, len, removals);
}
	
int main(int ac, char **av)
{
	  if (ac != 2)
		return 1;
	  int   len;
	  len = 0;
	  while (av[1][len])
		len++;
	  int   removals;
	  removals = calcul_removals(av[1]);
	  gen_valid_strs(av[1], 0, len, removals);
	  return 0;
}