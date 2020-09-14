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
            static Complex A, B, C, D, F, G, H, M, P, V, ika, ik1a, ik2a;

            static Eigen::MatrixXcd constructMatrix();
            static Eigen::VectorXcd constructVector();
            static std::vector<Complex> solveCoefficients(double ka, double v);

        public:
            static void initialize(double ka, double v);

            static double reflectionCoefficient();
            static double transmissionCoefficient();
            static double barrier(double x);
            static double density(double x);
    };
} // namespace Density

#endif