#include <iostream>
#include <sstream>
using namespace std;

int main(int argc, char **argv)
{
    if (argc == 3)
    {
        stringstream arg_stream;
        arg_stream << argv[1] << " " << argv[2];

        double a, b;
        if (arg_stream >> a >> b)
        {
            cout << a << "+" << b << "=" << a+b <<endl;
        }else
        {
            cout << "Arguments must be real numbers." << endl;
        }
        
    }else
    {
        cout << "usage: plus number1 number2" <<endl;
    }
    
    return 0;
}