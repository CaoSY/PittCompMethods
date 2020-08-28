/*
    This program takes in a list of numbers from commandline and outputs exp(#) in the same order.
*/


#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    vector<double> num_buffer;

    for (int i = 1; i < argc; ++i)
    {
        double num;
        istringstream input_str(argv[i]);
        
        if (input_str >> num)
        {
            num_buffer.push_back(num);
        }else
        {
            num_buffer.clear();
            cout << "exp only accepts real numbers" << endl;
            break;
        }
    }

    if (num_buffer.size() > 0)
    {
        ostringstream output_str;
        for (int i = 0; i < num_buffer.size(); i++)
        {
            output_str << exp(num_buffer[i]) << " ";
        }
        cout << output_str.str() << endl;
    }

    return 0;
}