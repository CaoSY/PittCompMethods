#include <iostream>
#include <iomanip>
#include <complex>
using namespace std;

typedef complex<double> Complex;

int main(int argc, char **argv)
{
    Complex i_unit(0,1);
    
    const int double_width = 28;
    const int number_precision = 24;

    cout << setw(double_width) << left << "x";
    cout << setw(double_width) << left << "abs(sin(ix) - i sinh(x)";
    cout << setw(double_width) << left << "abs(cos(ix) - cosh(x)";
    cout << endl;
    for (double x = -3; x < 3.1; x += 0.5)
    {
        cout << setw(double_width) << setprecision(number_precision) << x;
        cout << setw(double_width) << setprecision(number_precision)
             << abs(sin(i_unit * x) - i_unit * sinh(x));
        cout << setw(double_width) << setprecision(number_precision)
             << abs(cos(i_unit * x) - cosh(x));
        cout << endl;
    }

    return 0;
}