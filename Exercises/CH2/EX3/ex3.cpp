/*
    a + b i -> (a, b, 0)
    c + d i -> (c, d, 0)

                            | x y z |
    (a, b, 0) x (c, d, 0) = | a b 0 | = (0, 0, a*d - b*c)
                            | c d 0 |

    |(a, b, 0) x (c, d, 0)| = |a*d - b*c| = | Im(conj(a + b i) * (c + d i)) |
*/

#include <iostream>
#include <complex>
using namespace std;

typedef complex<double> Complex;

double Area(const Complex& a, const Complex& b)
{
    return abs(0.5 * (conj(a) * b).imag());
}

int main(int argc, char **argv)
{
    Complex A(3, 7), B(3, 2), C(10, 2);

    Complex AB = B - A, AC = C - A;

    cout << "A = " << A << endl;
    cout << "B = " << B << endl;
    cout << "C = " << C << endl;
    cout << "Area of ABC: " << Area(AB, AC) << endl;
    cout << endl;

    Complex t(4, 5);
    Complex Ap = A + t, Bp = B + t, Cp = C + t;
    Complex ApBp = Bp - Ap, ApCp = Cp - Ap;

    cout << "A' = " << Ap << endl;
    cout << "B' = " << Bp << endl;
    cout << "C' = " << Cp << endl;
    cout << "Area of A'B'C': " << Area(ApBp, ApCp) << endl;
    cout << endl;

    cout << "Rotation: pi/2" << endl;
    Complex r = exp(Complex(0, M_PI/2));
    Complex App = A * r, Bpp = B * r, Cpp = C * r;
    Complex AppBpp = Bpp - App, AppCpp = Cpp - App;

    cout << "A'' = " << App << endl;
    cout << "B'' - " << Bpp << endl;
    cout << "C'' = " << Cpp << endl;
    cout << "Area of A''B''C'': " << Area(AppBpp, AppCpp) << endl;

    return 0;
}