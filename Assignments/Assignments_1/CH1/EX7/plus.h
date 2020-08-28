# ifndef _PLUS_H_
# define _PLUS_H

struct PlusOperands
{
    double a;
    double b;
};


PlusOperands *parse(int argc, char **argv);

double add(const PlusOperands& args);

void print(const PlusOperands& args);

# endif