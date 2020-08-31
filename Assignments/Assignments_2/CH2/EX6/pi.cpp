#include <iostream>
#include <iomanip>
#include <complex>
#include <cmath>
using namespace std;

int main(int argc, char **argv)
{
    size_t side_num = 6;
    complex<double> x0(1,0), x1(1.0/2.0, sqrt(3.0)/2.0);

    const int column_width = 34;
    const int side_num_width =16;
    const int number_precision = 28;

    cout << setw(side_num_width) << left << "side number";
    cout << setw(column_width) << left << "pi";
    cout << setw(column_width) << left << "relative error";
    cout << endl;

    for (size_t i = 0; i < 30; i++)
    {
        double pi = (side_num / 2) * x1.imag();

        cout << setw(side_num_width) << setprecision(number_precision) << left << side_num;
        cout << setw(column_width) << setprecision(number_precision) << left << pi;
        cout << setw(column_width) << setprecision(number_precision) << left << (M_PI - pi) / M_PI;
        cout << endl;

        x1 = (x1 + x0) / 2.0;
        x1 = x1 / abs(x1);
        side_num *= 2;
    }
    

    return 0;
}