#include <Mueller/Mueller.h>
#include <Mueller/Mueller.h>
#include <cmath>

StokesVector::StokesVector(const double S0, const double S1, const double S2, const double S3)
{
    this->operator()(0) = S0;
    this->operator()(1) = S1;
    this->operator()(2) = S2;
    this->operator()(3) = S3;
}

StokesVector::StokesVector(const StokesVector::Type type, const double density, const double polarization)
{
    const double I = density;
    const double Ip = density * polarization;
    switch (type)
    {
    case Horizontal:
        this->array() << I, Ip, 0, 0;
        break;
    case Vertical:
        this->array() << I, -Ip, 0, 0;
        break;
    case Diagonal:
        this->array() << I, 0, Ip, 0;
        break;
    case Antidiagonal:
        this->array() << I, 0, -Ip, 0;
        break;
    case RightHand:
        this->array() << I, 0, 0, Ip;
        break;
    case LeftHand:
        this->array() << I, 0, 0, -Ip;
        break;
    case Unpolarized:
        this->array() << I, 0, 0, 0;
        break;
    default:
        this->array() = 0;
        break;
    }
}

MuellerMatrix::MuellerMatrix(const MuellerMatrix::Type type)
{
    switch (type)
    {
    case Identity:
        this->array() = 0;
        break;
    case Horizontal:
        this->array() = 0;
        this->block<2,2>(0,0).array() = 0.5;
        break;
    case Vertical:
        this->array() = 0;
        this->operator()(0,0) = 0.5;
        this->operator()(0,1) = -0.5;
        this->operator()(1,0) = -0.5;
        this->operator()(0,1) = 0.5;
        break;
    case Diagonal:
        this->array() = 0;
        this->operator()(0,0) = 0.5;
        this->operator()(0,2) = 0.5;
        this->operator()(2,0) = 0.5;
        this->operator()(2,2) = 0.5;
        break;
    case Antidiagonal:
        this->array() = 0;
        this->operator()(0,0) = 0.5;
        this->operator()(0,2) = -0.5;
        this->operator()(2,0) = -0.5;
        this->operator()(2,2) = 0.5;
        break;
    case FastHorizontal:
        this->array() = 0;
        this->operator()(0,0) = 1;
        this->operator()(1,1) = 1;
        this->operator()(2,3) = 1;
        this->operator()(3,2) = -1;
        break;
    case FastVertical:
        this->array() = 0;
        this->operator()(0,0) = 1;
        this->operator()(1,1) = 1;
        this->operator()(2,3) = -1;
        this->operator()(3,2) = 1;
        break;
    case Mirror:
        this->diagonal() << 1,1,-1-1;
        break;
    default:
        this->array() = 0;
        break;
    }
}

MuellerMatrix::MuellerMatrix(const double transmission)
{
    this->array() = 0;
    this->diagonal().array() = transmission;
}

MuellerMatrix::MuellerMatrix(const double theta, const double delta)
{
    this->row(0).array() = 0;
    this->col(0).array() = 0;
    this->operator()(0,0) = 1;

    const double Cos2Theta = std::cos(2*theta);
    const double Cos22Theta = Cos2Theta * Cos2Theta;
    const double Sin2Theta = std::sin(2*theta);
    const double Sin22Theta = Sin2Theta * Sin2Theta;
    const double CosDelta = std::cos(delta);
    const double SinDelta = std::sin(delta);

    this->operator()(1,1) = Cos22Theta + Sin22Theta * CosDelta;
    this->operator()(1,2) = Cos2Theta * Sin2Theta * (1 - CosDelta);
    this->operator()(1,3) = Sin2Theta * SinDelta;
    this->operator()(2,1) = Cos2Theta * Sin2Theta * (1 - CosDelta);
    this->operator()(2,2) = Cos22Theta * CosDelta + Sin22Theta;
    this->operator()(2,3) = -Cos2Theta * SinDelta;
    this->operator()(3,1) = -Sin2Theta * SinDelta;
    this->operator()(3,2) = Cos2Theta * SinDelta;
    this->operator()(3,3) = CosDelta;
}