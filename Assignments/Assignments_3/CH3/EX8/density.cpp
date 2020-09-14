#include "./density.h"
#include <complex>

namespace Density
{
    Complex WaveFunction::A;
    Complex WaveFunction::B;
    Complex WaveFunction::C;
    Complex WaveFunction::D;
    Complex WaveFunction::F;
    Complex WaveFunction::G;
    Complex WaveFunction::H;
    Complex WaveFunction::M;
    Complex WaveFunction::P;
    Complex WaveFunction::V;
    Complex WaveFunction::ika;
    Complex WaveFunction::ik1a;
    Complex WaveFunction::ik2a;

    void WaveFunction::initialize(double ka, double v)
    {
        WaveFunction::V = Complex(v,0);

        Complex I(0,1);
        Complex n1 = std::sqrt(1.0 - V);
        Complex n2 = std::sqrt(1.0 - 2.0 * V);

        WaveFunction::ika = I * ka;
        WaveFunction::ik1a = I * n1 * ka;
        WaveFunction::ik2a = I * n2 * ka;

        Eigen::MatrixXcd mat = WaveFunction::constructMatrix();
        Eigen::VectorXcd vec = WaveFunction::constructVector();

        Eigen::VectorXcd coef = mat.fullPivLu().solve(vec);

        WaveFunction::A = Complex(1,0);
        WaveFunction::B = coef[0];
        WaveFunction::C = coef[1];
        WaveFunction::D = coef[2];
        WaveFunction::F = coef[3];
        WaveFunction::G = coef[4];
        WaveFunction::H = coef[5];
        WaveFunction::M = coef[6];
        WaveFunction::P = coef[7];
    }

    Eigen::MatrixXcd WaveFunction::constructMatrix()
    {
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

    Eigen::VectorXcd WaveFunction::constructVector()
    {
        Eigen::VectorXcd vec = Eigen::VectorXcd::Zero(8);

        vec(0) = -std::exp(-2.0 * ika);
        vec(4) = -ika * std::exp(-2.0 * ika);

        return vec;
    }

    double WaveFunction::reflectionCoefficient()
    {
        return std::norm(WaveFunction::B);
    }

    double WaveFunction::transmissionCoefficient()
    {
        return std::norm(WaveFunction::P);
    }

    double WaveFunction::density(double x)
    {
        if (x <= -2)
        {
            return std::norm(A*std::exp(ika*x) + B*std::exp(-ika*x));
        }else if (x <= -1)
        {
            return std::norm(C*std::exp(ik1a*x) + D*std::exp(-ik1a*x));
        }else if (x <= 1)
        {
            return std::norm(F*std::exp(ik2a*x) + G*std::exp(-ik2a*x));
        }else if (x <= 2)
        {
            return std::norm(H*std::exp(ik1a*x) + M*std::exp(-ik1a*x));
        }else
        {
            return std::norm(P*std::exp(ika*x));
        }
    }

    double WaveFunction::barrier(double x)
    {
        if (x <= -2)
        {
            return 0;
        }else if (x <= -1)
        {
            return V.real();
        }else if (x <= 1)
        {
            return 2 * V.real();
        }else if (x <= 2)
        {
            return V.real();
        }else
        {
            return 0;
        }
    }
} // namespace Density