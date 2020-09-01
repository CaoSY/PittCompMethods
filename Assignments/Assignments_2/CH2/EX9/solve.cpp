#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <complex>
using namespace std;

typedef complex<double> Complex;

vector<Complex> parse_input(const int arg_num)
{
    vector<Complex> args;

    for (size_t i = 0; i < arg_num; i++)
    {
        Complex tmp;

        if (cin >> tmp)
        {
            args.push_back(tmp);
        }else
        {
            break;
        }
        
    }
    
    return args;
}

vector<Complex> solve(Complex a, Complex b, Complex c)
{
    vector<Complex> result;

    Complex sqrt_delta = sqrt(pow(b, 2.0) - 4.0 * a * c);
    
    result.push_back((-b + sqrt_delta) / (2.0 * a));
    result.push_back((-b - sqrt_delta) / (2.0 * a));

    return result;
}

void print_result(vector<Complex> result)
{
    ostringstream x1_buffer;
    ostringstream x2_buffer;

    const int number_precision = 16;
    const int solution_width = 50;

    x1_buffer << "x1 -> " << setw(solution_width) << setprecision(number_precision) << left << result[0];
    x1_buffer << "x2 -> " << setw(solution_width) << setprecision(number_precision) << left << result[1];

    cout << x1_buffer.str() << "\t" << x2_buffer.str() << endl;
}

int main(int argc, char **argv)
{
    while (true)
    {
        const int arg_num = 3;
        vector<Complex> args = parse_input(arg_num);

        if (args.size() == arg_num)
        {
            vector<Complex> result = solve(args[0], args[1], args[2]);

            print_result(result);
        }else
        {
            break;
        }
    }
    
    return 0;
}