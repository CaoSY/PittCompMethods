/*
    D = A + s * AC (0 <= s <= 1)
    BD = A + s * AC - B = BA + s * AC;

    BD . AC = 0
    => BA . AC + s * AC . AC = 0
    => s = - (BA . AC) / (AC . AC)

    a + b i -> (a, b)
    c + d i -> (c, d)
    (a, b) . (c, d) = a * c + b * d = Re(conj(a + b i) * (c + d i))

*/

#include <iostream>
#include <complex>
using namespace std;

typedef complex<double> Complex;

double dot(const Complex& a, const Complex& b)
{
    return (conj(a) * b).real();
}

double distance(const Complex& a, const Complex& b)
{
    return abs(b - a);
}

Complex ClosetApproach(const Complex& A, const Complex& B, const Complex& C)
{
    Complex BA = A - B, AC = C - A;
    double s = -dot(BA, AC) / dot(AC, AC);

    return A + s * AC;
}

int main(int argc, char **argv)
{
    Complex A(3, 7), B(3, 2), C(10, 2);

    Complex AB = B - A, AC = C - A;
    Complex D = ClosetApproach(A, B, C);

    cout << "A = " << A << endl;
    cout << "B = " << B << endl;
    cout << "C = " << C << endl;
    cout << "D = " << D << endl;
    cout << "Distance BD: " << distance(B, D) << endl;
    cout << endl;

    Complex t(4, 5);
    Complex Ap = A + t, Bp = B + t, Cp = C + t;
    Complex Dp = ClosetApproach(Ap, Bp, Cp);

    cout << "A' = " << Ap << endl;
    cout << "B' = " << Bp << endl;
    cout << "C' = " << Cp << endl;
    cout << "D' = " << Dp << endl;
    cout << "Distance B'D': " << distance(Bp, Dp) << endl;
    cout << endl;

    cout << "Rotation: pi/2" << endl;
    Complex r = exp(Complex(0, M_PI/2));
    Complex App = A * r, Bpp = B * r, Cpp = C * r;
    Complex Dpp = ClosetApproach(App, Bpp, Cpp);

    cout << "A'' = " << App << endl;
    cout << "B'' = " << Bpp << endl;
    cout << "C'' = " << Cpp << endl;
    cout << "D'' = " << Dpp << endl;
    cout << "Distance B''D'': " << distance(Bpp, Dpp) << endl;

    return 0;
}