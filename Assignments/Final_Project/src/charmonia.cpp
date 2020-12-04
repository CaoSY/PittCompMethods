#include <charmonia.h>
#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/Exp.h>
#include <cmath>
#include <cfloat>

Genfun::Parameter Charmonia::mcScale("mcScale", 1, 0.9, 1.1);
Genfun::Parameter Charmonia::alphasScale("alphasScale", 1, 0.9, 1.1);
Genfun::Parameter Charmonia::bScale("bScale", 1, 0.9, 1.1);
Genfun::Parameter Charmonia::sigmaScale("sigmaScale", 1, 0.9, 1.1);

Charmonia::Charmonia(const State& state, const int matrixSize, const double rMax)
:L(state.L), S(state.S), J(state.J), _JPC(state.JPC)
{
    const double delta = rMax / matrixSize;
    const double da = -1/(2*Mu()*delta*delta);
    const double dc = 1/(Mu()*delta*delta);
    const Eigen::ArrayXd subDiagEle = Eigen::ArrayXd::Ones(matrixSize-1) * da;
    const Eigen::ArrayXd X = Eigen::ArrayXd::LinSpaced(matrixSize, 1, matrixSize)*delta - 0.5*delta;
    const Eigen::ArrayXd X2 = X*X;
    const Eigen::ArrayXd X3 = X*X*X;

    const Eigen::ArrayXd Vc = (L*(L+1)/(2*Mu())) / X2;
    const Eigen::ArrayXd Vs = (-4*AlphaS()/3) / X + B()*X;
    const Eigen::ArrayXd Vhyp =
        (32*M_PI*AlphaS()/(9*Mc()*Mc()) * std::pow(Sigma()/std::sqrt(M_PI),3) * SS()) * ((-Sigma()*Sigma())*X2).exp();
    const Eigen::ArrayXd Vfs =
        (LS()/(Mc()*Mc()))*((2*AlphaS())/X3 - (B()/2)/X2) + (4*AlphaS()*T()/(Mc()*Mc()))/X3;
    V = Vc + Vs + Vhyp +Vfs;
    const Eigen::ArrayXd diagEle = dc + Vc + Vs + Vhyp;

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es;
    es.computeFromTridiagonal(diagEle, subDiagEle, Eigen::ComputeEigenvectors);

    int sIndex = state.N - 1;

    mass = (2*Mc() + es.eigenvalues()(sIndex)) * 1000;
    psi = es.eigenvectors().col(sIndex).normalized() / sqrt(delta);
    r = X;

    correction = (psi.array() * Vfs * psi.array()).sum() * delta;
    /*
    for (int i = 0; i < matrixSize; i++)
    {
        if (i != sIndex)
        {
            double Es = es.eigenvalues()(sIndex);
            double Ei = es.eigenvalues()(i);
            Eigen::VectorXd psiI = es.eigenvectors().col(i).normalized() / sqrt(delta);
            double prod = (psiI.array() * Vfs * psi.array()).sum() * delta;
            correction += (prod*prod) / (Es - Ei);
        }
    }
    */
    correction *= 1000;
}

double Charmonia::Mc() { return _m_c * mcScale.getValue(); }
double Charmonia::Mu() { return 0.5 * Mc(); }
double Charmonia::AlphaS() { return _alpha_s * alphasScale.getValue(); }
double Charmonia::B() { return _b * bScale.getValue(); }
double Charmonia::Sigma() { return _sigma * sigmaScale.getValue(); }

const std::string& Charmonia::JPC() const { return _JPC; }
double Charmonia::Mass() const { return mass + correction; }
double Charmonia::fsCorrection() const { return correction; }
const Eigen::VectorXd& Charmonia::Psi() const { return psi; }
const Eigen::VectorXd& Charmonia::R() const { return r; }
const Eigen::VectorXd& Charmonia::Potential() const { return V; }

double Charmonia::SS() const
{
    return 0.5 * (S*(S+1) - S_c*(S_c+1) - S_cbar*(S_cbar+1));
}

double Charmonia::LS() const
{
    return 0.5 * (J*(J+1) - S*(S+1) - L*(L+1));
}

double Charmonia::T() const
{
    if (S == 1)
    {
        if (J == L + 1)
        {
            return -L / (6.0*(2*L + 3.0));
        }else if (J == L)
        {
            return 1.0/6.0;
        }else if (J == L-1)
        {
            return -(L+1.0)/(6.0*(2*L - 1.0));
        }
    }

    return 0;
}