/*Expected files : *.c *.h
Allowed functions: atoi, fprintf, malloc, calloc, realloc, free, stdout, stderr, write
Write a program that will take as argument an integer n followed by a set s of integer. 
your program should display all the subsets of s whose sum of elements is n.
The order of the lines is not important, but the order of the element in a subset is.
 You must not have any duplicates (eg: '1 2' and '2 1').
in case of a malloc error your program will exit with the code 1.
We will not test with invalid sets (for example '1 1 2')
For example this should work:
$>./powerset 3 1 0 2 4 5 3 | cat -e
3$
0 3$
1 2$
1 0 2$
$>./powerset 12 5 2 1 8 4 3 7 11 | cat -e
8 4$
1 11$
1 4 7$
1 8 3$
2 3 7$
5 7$
5 4 3$
5 2 1 4$
$>./powerset 0 1 -1 | cat -e
1 -1$
$> ./powerset 7 3 8 2 | cat -e 
*/

#include <stdlib.h>
#include <stdio.h>

void find_subsets(int *arr, int len, int target, int *subset, int subset_size, int sum, int pos) 
{
    if (sum == target && subset_size > 0) 
    {
        for (int i = 0; i < subset_size; i++)
        {
            if (i > 0) 
                printf(" ");
            printf("%d", subset[i]);
        }
        printf("\n");
    }
    
    for (int i = pos; i < len; i++) 
    {
        if(i > pos && arr[i] == arr[i - 1])
            continue;
        subset[subset_size] = arr[i];
        find_subsets(arr, len, target, subset, subset_size + 1, sum + arr[i], i + 1);
    }
}

int main(int ac, char **av) 
{
    if (ac < 3)
        return 1;
    
    int target = atoi(av[1]);
    int len = ac - 2;
    int *arr = malloc(len * sizeof(int));
    if (!arr)
        return 1;
    
    for (int i = 0; i < len; i++) 
    {
        arr[i] = atoi(av[i + 2]);
    }
    int *subset = malloc(len * sizeof(int));
    if (!subset) 
    {
        free(arr);
        return 1;
    }
    find_subsets(arr, len, target, subset, 0, 0, 0);
    free(arr);
    free(subset);
    return 0;
}