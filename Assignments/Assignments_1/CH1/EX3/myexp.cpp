#include <iostream>
#include <iomanip>
#include <cmath>
#include <cfloat>
using namespace std;

/*
    implementation of exp() using taylor series up to the x^15.
*/
double myExp(double exponent)
{
    double sum = 1, term = 1, order = 1;

    for (double order = 1; abs(term) > DBL_EPSILON; ++order)
    {
        term *= exponent / order;
        sum += term;
    }

    return sum;
}

int main(int argc, char **argv)
{
    cout << setprecision(16);

    for (int i = -10; i <=10; ++i)
    {
        cout << i << "\t" << exp(i) << "\t" << myExp(i) << "\t" << exp(i) - myExp(i) << endl;
    }

    return 0;
}