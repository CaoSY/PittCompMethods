#include <iostream>
#include <Mueller/Mueller.h>
#include <cmath>

int main(int argc, char **argv)
{
	StokesVector v0(StokesVector::RightHand, 1, 0.5);
	
	std::cout << "Initial vector: density=1, polarization=0.5, Right-hand circularly polarized" << std::endl;
	std::cout << v0 << std::endl;

	std::cout << std::endl;

	MuellerMatrix p(MuellerMatrix::Horizontal);
	MuellerMatrix a(0.75);
	MuellerMatrix l(M_PI/4, M_PI/2);

	std::cout << "Filter: Horizontal linear polarizer" << std::endl;
	std::cout << p << std::endl;

	std::cout << std::endl;

	std::cout << "Filter: Attenuation (Transmission 0.75)" << std::endl;
	std::cout << a << std::endl;

	std::cout << std::endl;

	std::cout << "Filter: Left hand circular polarizer" << std::endl;
	std::cout << l << std::endl;

	std::cout << std::endl;

	std::cout << "Final vector:" << std::endl;
	std::cout << l * a * p * v0 << std::endl;

	return 0;
}
