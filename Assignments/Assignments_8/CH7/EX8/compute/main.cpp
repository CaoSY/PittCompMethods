#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <cmath>
#include <vector>
#include <algorithm>
#include <QatDataAnalysis/Hist1D.h>
#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/Sqrt.h>
#include <QatGenericFunctions/Exp.h>
#include <QatGenericFunctions/Tan.h>
#include <QatGenericFunctions/Square.h>
#include <QatGenericFunctions/Sin.h>
#include <QatGenericFunctions/Cos.h>
#include <QatGenericFunctions/Abs.h>
#include <QatGenericFunctions/Argument.h>


const size_t nSample = 1e4;
const size_t nBatch = 1e3;
const double PI_1_SQRT = 1/std::sqrt(M_PI);
const double tau = 2;
Genfun::GENFUNCTION qSquare = Genfun::Square();
Genfun::GENFUNCTION qSqrt = Genfun::Sqrt();
Genfun::GENFUNCTION qExp = Genfun::Exp();
Genfun::GENFUNCTION qatAbs = Genfun::Abs();
Genfun::Variable X(0,3),Y(1,3),Z(2,3);

class RandomSample
{
	private:
		double d;
		std::mt19937_64 uEng, vEng, rEng, dEng;
		std::uniform_real_distribution<double> uDist;
		std::uniform_real_distribution<double> vDist;
		std::uniform_int_distribution<int> dDist;
		std::exponential_distribution<double> rDist;
		Genfun::AbsFunction *sampleFunc;
	public:
		RandomSample(double d, std::random_device& rndDev):
		d(d),
		uEng(rndDev()), vEng(rndDev()), rEng(rndDev()), dEng(rndDev()),
		uDist(0, 1), vDist(-1, 1), dDist(0, 1), rDist(tau)
		{
			Genfun::GENFUNCTION L1 = qSqrt(qSquare(X) + qSquare(Y) + qSquare(Z-d/2));
			Genfun::GENFUNCTION L2 = qSqrt(qSquare(X) + qSquare(Y) + qSquare(Z+d/2));

			sampleFunc = (1 / (4 * M_PI) * (tau * qExp(-tau*L1) + tau * qExp(-tau*L2))).clone();
		}

		Genfun::GENFUNCTION sampleFunction()
		{
			return *sampleFunc;
		}

		Genfun::Argument next()
		{
			double r = rDist(rEng);
			double zShift = d * (dDist(dEng) - 0.5);
			double theta = 2 * M_PI * uDist(uEng);
			double v = vDist(vEng);

			double x = r * std::sqrt(1 - v*v) * std::cos(theta);
			double y = r * std::sqrt(1 - v*v) * std::sin(theta);
			double z = r * v + zShift;

			return Genfun::Argument({x,y,z});
		}

		~RandomSample()
		{
			delete sampleFunc;
		}
};


double MCIntegral(Genfun::GENFUNCTION f, RandomSample& sample)
{
	Genfun::GENFUNCTION h = f / sample.sampleFunction();
	double batchSum = 0;

	for (size_t i = 0; i < nBatch; i++)
	{
		double sum = 0;
		for (size_t j = 0; j < nSample; j++)
		{
			double hVal = h(sample.next());
			if (std::isfinite(hVal))
			{
				sum += hVal;
			}else
			{
				j--;
			}
		}
		batchSum += sum / nSample;
	}

	return batchSum / nBatch;
}


int main()
{
	const double dMin = 0;
	const double dMax = 10;
	const size_t dN = 10;
	std::random_device rndDev;
	std::vector<double> dVec, NsVec, NaVec, EsVec, EaVec;
	for (double i = 1; i <= dN; i++)
	{
		double d = dMin + double(i) * (dMax - dMin) / dN;
		Genfun::GENFUNCTION L1 = qSqrt(qSquare(X) + qSquare(Y) + qSquare(Z-d/2));
		Genfun::GENFUNCTION L2 = qSqrt(qSquare(X) + qSquare(Y) + qSquare(Z+d/2));
		Genfun::GENFUNCTION phi1 = PI_1_SQRT * qExp(-L1);
		Genfun::GENFUNCTION phi2 = PI_1_SQRT * qExp(-L2);
		Genfun::GENFUNCTION psiS = phi1 + phi2;
		Genfun::GENFUNCTION psiA = phi1 - phi2;

		Genfun::GENFUNCTION V = -1/L1 - 1/L2;

		Genfun::GENFUNCTION pdfS = qSquare(psiS);
		Genfun::GENFUNCTION pdfA = qSquare(psiA);

		Genfun::GENFUNCTION HS = psiS * (-0.5*psiS - phi2/L1 - phi1/L2);
		Genfun::GENFUNCTION HA = psiA * (-0.5*psiA - phi1/L2 + phi2/L1);


		RandomSample sample(d, rndDev);

		dVec.push_back(d);

		double Ns2 = 1 / MCIntegral(pdfS, sample);
		NsVec.push_back(std::sqrt(Ns2));

		double Es = Ns2 * MCIntegral(HS, sample);
		EsVec.push_back(Es);

		double Na2 = 1 / MCIntegral(pdfA, sample);
		NaVec.push_back(std::sqrt(Na2));

		double Ea = Na2 * MCIntegral(HA, sample);
		EaVec.push_back(Ea);

		std::cout << d << std::endl;
	}

	std::fstream fileD("../D.txt", std::ios::out);
	std::fstream fileNs("../Ns.txt", std::ios::out);
	std::fstream fileNa("../Na.txt", std::ios::out);
	std::fstream fileEs("../Es.txt", std::ios::out);
	std::fstream fileEa("../Ea.txt", std::ios::out);
	
	for (size_t i = 0; i < dVec.size(); i++)
	{
		fileD << std::setprecision(20) << dVec[i] << std::endl;
		fileNs << std::setprecision(20) << NsVec[i] << std::endl;
		fileNa << std::setprecision(20) << NaVec[i] << std::endl;
		fileEs << std::setprecision(20) << EsVec[i] << std::endl;
		fileEa << std::setprecision(20) << EaVec[i] << std::endl;
	}
	
	fileD.close();
	fileNs.close();
	fileNa.close();
	fileEs.close();
	fileEa.close();

	std::cout << '\7';
	return 0;
}