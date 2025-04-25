/*Subject: vbc
Files to turn: *.c, *.h
Functions allowed: malloc, calloc, realloc, free, printf, isdigit, write

Program: print the result of a math expression given as argument
- Handle + * and parenthesis
- Don't handle whitespaces
- All the values in the expression will be between 0 and 9 included
- In case of unexpected symbol, print "Unexpected token '%c'\n" and exit code 1.
    If the symbol is in the end of the input: "Unexpected end of input\n".
- In case of syscall failure -exit code 1

Examples:
-------------
./vbc '1'
1

./vbc '2+3'
5

./vbc '3*4+5'
17

./vbc '(3+4)*5'
35

....*/

#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node    *parse_expr(char **s);
node    *parse_term(char **s);
node    *parse_factor(char **s);

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
    exit(1);
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
    return (0);
}

node    *parse_expr(char **s) 
{
    node *left = parse_term(s);
    if(!left)
        return NULL;
    while(accept(s, '+'))
    {
        node *right = parse_term(s);
        if(!right)
        {
            destroy_tree(left);
            return NULL;
        }
        node n = {ADD, 0, left, right};
        left = new_node(n);
        if(!left)
        {
            destroy_tree(right);
            return NULL;
        }
    }
    return left;
}

node    *parse_term(char **s) 
{
    node *left = parse_factor(s);
    if(!left)
        return NULL;
    while(accept(s, '*'))
    {
        node *right = parse_factor(s);
        if(!right)
        {
            destroy_tree(left);
            return NULL;
        }

        node n = {MULTI, 0, left, right};
        left = new_node(n);
        if(!left)
        {
            destroy_tree(right);
            return NULL;
        }
    }
    return left;
}

node    *parse_factor(char **s)
{
    if(isdigit(**s))
    {
        node n = {VAL, **s - '0', NULL, NULL};
        (*s)++;
        return (new_node(n));
    }

    if(accept(s, '('))
    {
        node *n = parse_expr(s);
        if(!expect(s, ')'))
        {
            destroy_tree(n);
            return NULL;
        }
        return (n);
    }
    unexpected(**s);
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    char *s = argv[1];
    node *tree = parse_expr(&s);
    if (!tree || *s) {
        if (*s)
            unexpected(*s);
        else
            unexpected(0);
        return (1);
    }
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}
