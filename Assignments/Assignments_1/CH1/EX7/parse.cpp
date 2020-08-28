#include "./plus.h"
#include <sstream>

PlusOperands *parse(int argc, char **argv)
{
    PlusOperands *argnum_ptr = nullptr;

    if (argc == 3)
    {
        std::stringstream arg_stream;
        arg_stream << argv[1] << " " << argv[2];

        argnum_ptr = new PlusOperands();
        if (!(arg_stream >> argnum_ptr->a >> argnum_ptr->b))
        {
            delete argnum_ptr;
            argnum_ptr = nullptr;
        }
    }
    
    return argnum_ptr;
}