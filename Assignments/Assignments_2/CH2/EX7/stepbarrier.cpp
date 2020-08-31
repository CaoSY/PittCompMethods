#include <iostream>
#include <iomanip>
#include <sstream>
#include <list>
#include <complex>
using namespace std;

typedef complex<double> Complex;

list<double> parse_input(const int argc, char ** const argv)
{
    list<double> args;

    if (argc >= 3)
    {
        for (size_t i = 1; i < argc; i++)
        {
            double tmp;
            istringstream input_buffer(argv[i]);
            
            if (input_buffer >> tmp)
            {
                args.push_back(tmp);
            }else
            {
                cout << "arguments must be real numbers" << endl;
                args.clear();
                break;
            }
        }
    }else
    {
        cout << "help: stepbarrier E V [pos1 pos2 ...]" << endl;
    }
    
    return args;
}

list<Complex> compute(list<double> args)
{
    list<Complex> result;
    if (args.size() >= 2)
    {
        Complex E(args.front(), 0);     // Energy of incident particles
        args.pop_front();
        
        Complex V(args.front(), 0);     // Height of barrier
        args.pop_front();

        Complex k_L = sqrt(E);      // wave vector on the left
        Complex k_R = sqrt(E-V);    // wave vector on the right

        Complex A_r = (k_L - k_R) / (k_L + k_R);    // Amplitude of reflected wave
        Complex A_t = 2.0 * k_L / (k_L + k_R);        // Amplitude of transmitted wave

        Complex R = norm(k_L - k_R) / norm(k_L + k_R);
        Complex T = 4.0 * conj(k_L) * k_R / norm(k_L + k_R);

        result.push_back(R);
        result.push_back(T);

        Complex i_unit(0.0, 1.0);
        for (list<double>::iterator arg_ptr = args.begin(); arg_ptr != args.end(); ++arg_ptr)
        {
            double x = *arg_ptr;

            if (x >= 0)
            {
                Complex psi = A_t * exp(i_unit * k_R * x);
                result.push_back(psi);
            }else
            {
                Complex psi = exp(i_unit * k_L * x) + A_r * exp(-i_unit * k_L * x);
                result.push_back(psi);
            }
        }
    }

    return result;
}

void print_result(list<double> args, list<Complex> result)
{
    if (result.size() >= 2 && result.size() == args.size())
    {
        cout << "Refection:\t" << result.front().real() << endl;
        args.pop_front();
        result.pop_front();

        cout << "Transmission:\t" << result.front().real() << endl;
        args.pop_front();
        result.pop_front();

        cout << endl;

        if (result.size() > 0)
        {   
            const int real_number_width = 20;
            const int complex_number_width = 2 * real_number_width + 10;
            const int number_precision = 16;

            cout << setw(real_number_width) << left << "x";
            cout << setw(complex_number_width) << left << "psi";
            cout << setw(real_number_width) << left << "amplitude";
            cout << endl;

            list<double>::iterator arg_ptr = args.begin();
            list<Complex>::iterator res_ptr = result.begin();

            while (arg_ptr != args.end() && res_ptr != result.end())
            {
                cout << setw(real_number_width) << setprecision(number_precision) << left << *arg_ptr;
                cout << setw(complex_number_width) << setprecision(number_precision) << left << *res_ptr;
                cout << setw(real_number_width) << setprecision(number_precision) << left << abs(*res_ptr);
                cout << endl;

                ++ arg_ptr;
                ++ res_ptr;
            }
        }
    }
}

int main(int argc, char **argv)
{
    list<double> args = parse_input(argc, argv);

    list<Complex> result = compute(args);
    
    print_result(args, result);

    return 0;
}