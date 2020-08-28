#include "./plus.h"
#include <iostream>

void print(const PlusOperands& args)
{
    std::cout << args.a << "+" << args.b << "=" << add(args) << std::endl;
}