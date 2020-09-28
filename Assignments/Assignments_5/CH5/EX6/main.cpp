#include <cmath>
#include <iostream>
#include <iomanip>
#include <array>
#include <QatGenericFunctions/GaussIntegrator.h>
#include <QatGenericFunctions/GaussQuadratureRule.h>
#include <QatGenericFunctions/HermitePolynomial.h>

const size_t N = 6;
typedef std::array<std::array<double,N>,N> Matrix;

double delta(size_t i, size_t j)
{
	return i == j;
}

void printMatrix(const Matrix& mat)
{
	for (size_t i = 0; i < mat.size(); i++)
	{
		for (size_t j = 0; j < mat[i].size(); j++)
		{
			std::cout << std::setw(14) << std::setprecision(4) << mat[i][j];
		}
		std::cout << std::endl;
	}
}

int main()
{
	
	for (size_t num = 1; num < 7; num++)
	{
		std::cout << "Number of Sample Point: " << num << std::endl;
		Matrix mat;
		for (size_t i = 0; i < mat.size(); i++)
		{
			for (size_t j = 0; j < mat[i].size(); j++)
			{
				Genfun::GENFUNCTION integrand = 
					Genfun::HermitePolynomial(i, Genfun::TWIDDLE) * 
					Genfun::HermitePolynomial(j, Genfun::TWIDDLE);
				
				Genfun::GaussIntegrator hermiteIntegrator(Genfun::GaussHermiteRule(num), Genfun::GaussIntegrator::INTEGRATE_WX_DX);
					
				mat[i][j] = hermiteIntegrator(integrand);
			}
		}
		printMatrix(mat);
		std::cout << std::endl;
	}
	
	return 0;
}