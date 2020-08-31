#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

double gamma_beta(double beta)
{
    return 1 / sqrt(1.0 - pow(beta,2.0));
}

double gamma_epsilon(double epsilon)
{
    return 1 / sqrt((2 - epsilon) * epsilon);
}

int main(int argc, char **argv)
{
    const int number_width = 20;
    const int number_precision = 16;

    double beta = 0.9;
    double epsilon = 0.1;

    cout << setw(number_width) << left << "beta";
    cout << setw(number_width) << left << "gamma from beta";
    cout << setw(number_width) << left << "epsilon";
    cout << setw(number_width) << left << "gamma from epsilon";
    cout << setw(number_width) << left << "relative error";
    cout << endl;

    for(int i = 0; i < 20; ++i)
    {
        cout << setw(number_width) << setprecision(number_precision) << beta;
        cout << setw(number_width) << setprecision(number_precision) << gamma_beta(beta);
        cout << setw(number_width) << setprecision(number_precision) << epsilon;
        cout << setw(number_width) << setprecision(number_precision) << gamma_epsilon(epsilon);
        cout << setw(number_width) << setprecision(number_precision) << (gamma_beta(beta) - gamma_epsilon(epsilon)) / gamma_epsilon(epsilon);
        cout << endl;

        beta = 0.9 + beta / 10;
        epsilon = epsilon / 10;
    }

    return 0;
}