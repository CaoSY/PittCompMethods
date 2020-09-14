#include <density.h>
#include <complex>

namespace Density
{
    WaveFunction::WaveFunction(double ka, double v)
    {
        Complex kaC(ka,0), vC(v,0);

        Eigen::MatrixXcd mat = constructMatrix(kaC, vC);
        Eigen::VectorXcd vec = constructVector(kaC);

        Eigen::VectorXcd coef = mat.fullPivLu().solve(vec);

        this->A = Complex(1,0);
        this->B = coef[0];
        this->C = coef[1];
        this->D = coef[2];
        this->F = coef[3];
        this->G = coef[4];
        this->H = coef[5];
        this->M = coef[6];
        this->P = coef[7];
    }

    Eigen::MatrixXcd WaveFunction::constructMatrix(Complex ka, Complex v)
    {
        Complex I(0,1);

        Complex n1 = std::sqrt(1.0 - v);
        Complex n2 = std::sqrt(1.0 - 2.0 * v);

        Complex ika = I * ka;
        Complex ik1a = I * n1 * ka;
        Complex ik2a = I * n2 * ka;

        Eigen::MatrixXcd mat = Eigen::MatrixXcd::Zero(8, 8);

        // b
        mat(0,0) = std::exp(2.0 * ika);
        mat(4,0) = -ika * std::exp(2.0 * ika);

        // c
        mat(0,1) = -std::exp(-2.0 * ik1a);
        mat(1,1) = std::exp(-ik1a);
        mat(4,1) = -ik1a * std::exp(-2.0 * ik1a);
        mat(5,1) = ik1a * std::exp(-ik1a);
        
        // d
        mat(0,2) = -std::exp(2.0 * ik1a);
        mat(1,2) = std::exp(ik1a);
        mat(4,2) = ik1a * std::exp(2.0 * ik1a);
        mat(5,2) = -ik1a * std::exp(ik1a);

        // f
        mat(1,3) = -std::exp(-ik2a);
        mat(2,3) = std::exp(ik2a);
        mat(5,3) = -ik2a * std::exp(-ik2a);
        mat(6,3) = ik2a * std::exp(ik2a);

        // g
        mat(1,4) = -std::exp(ik2a);
        mat(2,4) = std::exp(-ik2a);
        mat(5,4) = ik2a * std::exp(ik2a);
        mat(6,4) = -ik2a * std::exp(-ik2a);

        // h
        mat(2,5) = -std::exp(ik1a);
        mat(3,5) = std::exp(2.0 * ik1a);
        mat(6,5) = -ik1a * std::exp(ik1a);
        mat(7,5) = ik1a * std::exp(2.0 * ik1a);

        // m
        mat(2,6) = -std::exp(-ik1a);
        mat(3,6) = std::exp(-2.0 * ik1a);
        mat(6,6) = ik1a * std::exp(-ik1a);
        mat(7,6) = -ik1a * std::exp(-2.0 * ik1a);

        // p
        mat(3,7) = -std::exp(2.0 * ika);
        mat(7,7) = -ika * std::exp(2.0 *ika);

        return mat;
    }

    Eigen::VectorXcd WaveFunction::constructVector(Complex ka)
    {
        Complex I(0,1);
        Complex ika = I * ka;

        Eigen::VectorXcd vec = Eigen::VectorXcd::Zero(8);

        vec(0) = -std::exp(-2.0 * ika);
        vec(4) = -ika * std::exp(-2.0 * ika);

        return vec;
    }

    double WaveFunction::reflectionCoefficient() const
    {
        return std::norm(this->B);
    }

    double WaveFunction::transmissionCoefficient() const
    {
        return std::norm(this->P);
    }

} // namespace Density