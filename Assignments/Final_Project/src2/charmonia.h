#include <cstddef>
#include <Eigen/Dense>
#include <QatGenericFunctions/Parameter.h>
#include <QString>

#ifndef __CHARMONIA_H__
#define __CHARMONIA_H__ 1

class Charmonia
{
private:
    static constexpr double _m_c = 1.48;
    static constexpr double _alpha_s = 0.546;
    static constexpr double _b = 0.143;
    static constexpr double _sigma = 1.095;
    static constexpr double S_c = 0.5;
    static constexpr double S_cbar = 0.5;

public:
    static Genfun::Parameter mcScale;
    static Genfun::Parameter alphasScale;
    static Genfun::Parameter bScale;
    static Genfun::Parameter sigmaScale;

    static double Mc();
    static double Mu();
    static double AlphaS();
    static double B();
    static double Sigma();

private:
    const int L;
    const int S;
    const int J;
    double mass;
    double correction;
    const std::string _JPC;
    Eigen::VectorXd psi;
    Eigen::VectorXd r;
    Eigen::VectorXd V;

    double SS() const;
    double LS() const;
    double T() const;

public:
    struct State
    {
        int N;
        int L;
        int S;
        int J;
        double mass;
        std::string JPC;
        QString name;
    };

    Charmonia(const State& s, const int matrixSize, const double rMax);
    double Mass() const;
    double fsCorrection() const;
    const std::string& JPC() const;
    const Eigen::VectorXd& Psi() const;
    const Eigen::VectorXd& R() const;
    const Eigen::VectorXd& Potential() const;
};

#endif // __CHARMONIA_H__