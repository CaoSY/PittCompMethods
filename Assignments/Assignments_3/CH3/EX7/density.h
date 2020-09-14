#ifndef __DENSITY_H_
#define __DENSITY_H_ 1

#include <eigen3/Eigen/Dense>
#include <complex>
#include <vector>

namespace Density
{
    typedef std::complex<double> Complex;

    class WaveFunction
    {
        private:
            Complex A, B, C, D, F, G, H, M, P;

            Eigen::MatrixXcd constructMatrix(Complex ka, Complex v);
            Eigen::VectorXcd constructVector(Complex ka);
            std::vector<Complex> solveCoefficients(double ka, double v);

        public:
            WaveFunction(double ka, double v);

            double reflectionCoefficient() const;
            double transmissionCoefficient() const;
    };

    
    Eigen::VectorXcd solveWaveFunction(double ka, double v);
} // namespace Density

#endif