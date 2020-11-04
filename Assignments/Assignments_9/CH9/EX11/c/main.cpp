#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <memory>
#include <QatGenericFunctions/InterpolatingFunction.h>
#include <QatDataAnalysis/Hist2D.h>
#include <mpi.h>
#include <QColor>
#include <QImage>

typedef std::unique_ptr<Genfun::AbsFunction> GENFUNCTION_Ptr;

const double bMin = 0;
const double bMax = 1;
const double lambdaMin = 200;
const double lambdaMax = 1000;
const double thetaMin = 0;
const double thetaMax = 0.8;
const size_t nBinTheta = 800;
const size_t nBinLambda = 800;
const size_t nSamplePerProcess = 1e8;
const int MASTER = 0;
const int HIST_TAG = 1;

class RandomSample
{
	private:
		std::mt19937_64 bEng;
		std::mt19937_64 lambdaEng;
		std::uniform_real_distribution<double> bDist;
		std::uniform_real_distribution<double> lambdaDist;
		GENFUNCTION_Ptr nOfLambda_Ptr;
	public:
		struct SampleResult
		{
			double theta;
			double lambda;
		};
		
		RandomSample(std::random_device& rndDev, Genfun::AbsFunction *n_ptr):
		bEng(rndDev()), lambdaEng(rndDev()), bDist(bMin,bMax), lambdaDist(lambdaMin, lambdaMax),
		nOfLambda_Ptr(n_ptr)
		{}

		double Theta(double b, double n) const
		{
			return 4*std::asin(b/n) - 2*std::asin(b);
		}

		SampleResult next()
		{
			SampleResult result;
			double b = std::sqrt(bDist(bEng));
			result.lambda = lambdaDist(lambdaEng);
			result.theta = Theta(b, (*nOfLambda_Ptr)(result.lambda));
			return result;
		}
};

Genfun::AbsFunction *fitDisperion()
{
	std::ifstream inFile("../dispersion.txt");
	double inLambda, inN;
	Genfun::InterpolatingFunction refractionIndexOfLambda(Genfun::CUBIC_SPLINE);
	while (inFile >> inLambda >> inN)
	{
		refractionIndexOfLambda.addPoint(inLambda, inN);
	}
	inFile.close();
	return refractionIndexOfLambda.clone();
}

double (*simu())[nBinTheta]
{
	std::random_device rndDev;
	RandomSample sample(rndDev, fitDisperion());

	Hist2D hist(nBinLambda, lambdaMin, lambdaMax, nBinTheta,thetaMin, thetaMax);
	for (size_t i = 0; i < nSamplePerProcess; i++)
	{
		const RandomSample::SampleResult result = sample.next();
		hist.accumulate(result.lambda, result.theta);
	}

	double (*data_ptr)[nBinTheta] = new double[nBinLambda][nBinTheta];

	for (size_t i = 0; i < nBinLambda; i++)
	{
		for (size_t j = 0; j < nBinTheta; j++)
		{
			data_ptr[i][j] = hist.bin(i,j);
		}
		
	}

	return data_ptr;
}

void appendTo(double (*dest)[nBinTheta], const double (*sour)[nBinTheta])
{
	for (size_t i = 0; i < nBinLambda; i++)
	{
		for (size_t j = 0; j < nBinTheta; j++)
		{
			dest[i][j] += sour[i][j];
		}
	}
}

// 0 <= val <= 1
QColor indexColor(double val)
{
    int h = std::round(240 * val);
    return QColor::fromHsv(h, 255, 255);
}

void saveImage(const double (*data)[nBinTheta])
{
	double maxBinCount = 0;
	for (size_t i = 0; i < nBinLambda; i++)
	{
		for (size_t j = 0; j < nBinTheta; j++)
		{
			maxBinCount = maxBinCount > data[i][j] ? maxBinCount : data[i][j];
		}
	}

	QImage histImage(nBinLambda, nBinTheta, QImage::Format_RGB32);

	for (size_t i = 0; i < nBinLambda; i++)
	{
		for (size_t j = 0; j < nBinTheta; j++)
		{
			histImage.setPixelColor(i,nBinTheta-1-j,indexColor(data[i][j]/maxBinCount));
		}
		
	}
	
	histImage.save("hist.bmp");
}


int main(int argc, char **argv)
{
	MPI_Init(&argc,&argv);
	int taskID, groupSize;
	MPI_Comm_rank(MPI_COMM_WORLD, &taskID);
	MPI_Comm_size(MPI_COMM_WORLD, &groupSize);

	std::cout << "group size: " << groupSize << "  taskID: " << taskID << std::endl;

	double (*histData_ptr)[nBinTheta] = simu();

	if (taskID == MASTER)
	{
		if (groupSize > 1)
		{
			double (*recvBuffer)[nBinTheta] = new double[nBinLambda][nBinTheta];
			MPI_Status status;
			for (int i = 1; i < groupSize; i++)
			{
				MPI_Recv(recvBuffer, nBinLambda*nBinTheta, MPI_DOUBLE, i, HIST_TAG, MPI_COMM_WORLD, &status);
				assert(status.MPI_ERROR == MPI_SUCCESS);
				appendTo(histData_ptr, recvBuffer);
				std::cout << "Master recieves data from worker " << i << std::endl;
			}
			delete recvBuffer;
		}
		
		saveImage(histData_ptr);
		std::cout << "Image saved." << std::endl;
	}else
	{
		MPI_Send(histData_ptr, nBinTheta*nBinLambda, MPI_DOUBLE, MASTER, HIST_TAG, MPI_COMM_WORLD);
		std::cout << "Worker " << taskID << " sends data to Master." << std::endl;
	}

	delete []histData_ptr;

	MPI_Finalize();
	return 0;
}