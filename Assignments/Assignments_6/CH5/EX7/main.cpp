#include <iostream>
#include <iomanip>
#include <array>
#include <QatGenericFunctions/HermitePolynomial.h>
#include <QatGenericFunctions/Exp.h>
#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/GaussIntegrator.h>
#include <QatGenericFunctions/GaussQuadratureRule.h>

const double EPS = 1e-12;
const size_t N = 5;
const size_t N_SAMPLE_POINT = 2 * (N - 1) + 2;
typedef std::array<std::array<double, N>, N> Matrix;
typedef Genfun::AbsFunction *(* Operator)(Genfun::GENFUNCTION);
typedef Genfun::GENFUNCTION State;
typedef Genfun::AbsFunction* StatePtr;

void printMatrix(const Matrix& mat)
{
	for (size_t i = 0; i < mat.size(); i++)
	{
		for (size_t j = 0; j < mat[i].size(); j++)
		{
			double val = std::abs(mat[i][j]) < EPS ? 0 : mat[i][j];
			std::cout << std::setw(15) << val;
		}
		std::cout << std::endl;
	}
}

StatePtr phi(size_t i)
{
	Genfun::Variable X;
	Genfun::GENFUNCTION state = Genfun::Exp()(-X*X/2) * 
		Genfun::HermitePolynomial(i, Genfun::TWIDDLE);
	
	return state.clone();
}

StatePtr x(State state)
{
	Genfun::Variable X;
	State newState = X * state;
	
	return newState.clone();
}

StatePtr x2(State state)
{
	State tmpState = *x(state);
	State newState = *x(tmpState);
	return newState.clone();
}

StatePtr D(State state)
{
	return (state.prime()).clone();
}

StatePtr D2(State state)
{
	State tmpState = *D(state);
	State newState = *D(tmpState);
	return newState.clone();
}

StatePtr Hamiltonian(State state)
{
	State D2State = *D2(state);
	State x2State = *x2(state);

	return (-0.5 * D2State + 0.5 * x2State).clone();
}

double operatorExpectation(State bra, Operator Op, State ket)
{
	Genfun::GaussIntegrator integrator(Genfun::GaussHermiteRule(N_SAMPLE_POINT), Genfun::GaussIntegrator::INTEGRATE_DX);

	State transformedState = *Op(ket);
	Genfun::GENFUNCTION integrand = bra * transformedState;
	
	return integrator(integrand);
}

Matrix matrixOfOperator(Operator Op)
{
	Matrix mat;

	for (size_t i = 0; i < mat.size(); i++)
	{
		for (size_t j = 0; j < mat[i].size(); j++)
		{
			State bra = *phi(i);
			State ket = *phi(j);
			mat[i][j] = operatorExpectation(bra, Op, ket);
		}
	}
	
	return mat;
}

int main()
{
	std::cout << "x" << std::endl;
	Matrix XMatrix = matrixOfOperator(x);
	printMatrix(XMatrix);
	std::cout << std::endl;

	std::cout << "x^2" << std::endl;
	Matrix X2Matrix = matrixOfOperator(x2);
	printMatrix(X2Matrix);
	std::cout << std::endl;

	std::cout << "D" << std::endl;
	Matrix DMatrix = matrixOfOperator(D);
	printMatrix(DMatrix);
	std::cout << std::endl;

	std::cout << "D2" << std::endl;
	Matrix D2Matrix = matrixOfOperator(D2);
	printMatrix(D2Matrix);
	std::cout << std::endl;

	std::cout << "Hamiltonian" << std::endl;
	Matrix HMatrix = matrixOfOperator(Hamiltonian);
	printMatrix(HMatrix);

	return 0;
}