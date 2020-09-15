#ifndef __WAVEFUNCTION_h_
#define __WAVEFUNCTION_H_

#include <eigen3/Eigen/Sparse>
#include <complex>

namespace WaveFunction
{
    // a = 1;
    typedef std::complex<double> Complex;
    
    void initialize(double ka, double v, int N);

    double gaussianPotential(double x);
    double barrier(double x);
    Complex psi(double x);
    double density(double x);
    double reflection();
    double transmission();
} // namespace WaveFunction


#endif