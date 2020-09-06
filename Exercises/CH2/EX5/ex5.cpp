/*
    shift -> rotate -> shift back
*/

#include <iostream>
#include <complex>
using namespace std;

typedef complex<double> Complex;

double degree_to_radian(double deg)
{
    return deg / 180 * M_PI;
}

Complex shift_rotate_shiftback(const Complex& a, const Complex& O, double rad)
{
    return (a - O) * exp(Complex(0, rad)) + O;
}

int main(int argc, char **argv)
{
    Complex A(3, 7), B(3, 2), C(10, 2), O(4, 5);
    double angle = degree_to_radian(755);

    cout << "A: " << A << endl;
    cout << "B: " << B << endl;
    cout << "C: " << C << endl;
    cout << endl;

    cout << "A': " << shift_rotate_shiftback(A, O, angle) << endl;
    cout << "B': " << shift_rotate_shiftback(B, O, angle) << endl;
    cout << "C': " << shift_rotate_shiftback(C, O, angle) << endl;

    return 0;
}