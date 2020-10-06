#ifndef _Mueller_h_
#define _Mueller_h_

#include <eigen3/Eigen/Dense>
#include <complex>

class StokesVector: public 	Eigen::Vector4d
{
	public:
		using Eigen::Vector4d::Vector4d;
		enum Type {Horizontal,Vertical,Diagonal,Antidiagonal,RightHand,LeftHand,Unpolarized};

		StokesVector(const double S0, const double S1, const double S2, const double S3);
		StokesVector(const Type type, const double density=1, const double polarization=1);
};

class MuellerMatrix: public Eigen::Matrix4d
{
	public:
		using Eigen::Matrix4d::Matrix4d;
		enum Type {Identity,Horizontal,Vertical,Diagonal,Antidiagonal,FastHorizontal,FastVertical, Mirror};

		MuellerMatrix(const Type type);
		MuellerMatrix(const double transmission);	// Attenuating filter
		MuellerMatrix(const double theta, const double delta);	// General linear retarder
};

#endif
