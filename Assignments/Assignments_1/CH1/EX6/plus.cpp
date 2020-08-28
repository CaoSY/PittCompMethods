#include "./plus.h"
#include <iostream>

int main(int argc, char **argv)
{
    PlusOperands *argnum_ptr = parse(argc, argv);

    if (argnum_ptr != nullptr)
    {
        print(*argnum_ptr);
    }else
    {
        std::cout << "usage: plus number1 number2" << std::endl;
    }
    

    return 0;
}