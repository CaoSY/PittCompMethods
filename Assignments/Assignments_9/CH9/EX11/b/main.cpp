#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <vector>
#include <memory>
#include <QatGenericFunctions/InterpolatingFunction.h>
#include <QatGenericFunctions/Parameter.h>
#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/ASin.h>
#include <QatGenericFunctions/Abs.h>
#include <QatDataAnalysis/Hist2D.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotHist2D.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotWidgets/PlotView.h>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QFont>
#include <QImage>

typedef std::unique_ptr<Genfun::AbsFunction> GENFUNCTION_Ptr;


const double bMin = 0;
const double bMax = 1;
const double lambdaMin = 200;
const double lambdaMax = 1000;
const double thetaMin = 0;
const double thetaMax = 0.8;
const double nBinTheta = 800;
const size_t nBinLambda = 800;
const size_t nSample = 4e8;

std::random_device rndDev;
std::mt19937_64 bEng(rndDev());
std::mt19937_64 lambdaEng(rndDev());
std::uniform_real_distribution<double> bDist(bMin, bMax);
std::uniform_real_distribution<double> lambdaDist(lambdaMin, lambdaMax);

Genfun::ASin qatASin;
Genfun::Abs qatAbs;

inline double sampleB()
{
	return std::sqrt(bDist(bEng));
}

inline double sampleLambda()
{
	return lambdaDist(lambdaEng);
}

double Theta(double b, double n)
{
	return 4*std::asin(b/n) - 2*std::asin(b);
}

// 0 <= val <= 1
QColor indexColor(double val)
{
    int h = std::round(240 * val);
    return QColor::fromHsv(h, 255, 255);
}

QImage generateColormap()
{
    QImage colormap(400, 20, QImage::Format_RGB32);

    for (size_t i = 0; i < 400; i++)
    {
        double val = i / 399.0;
        for (size_t j = 0; j < 20; j++)
        {
            colormap.setPixelColor(i, j, indexColor(val));
        }
        
    }
    
    return colormap;
}

int main(int argc, char **argv)
{
	std::ifstream inFile("../dispersion.txt");
	double inLambda, inN;
	Genfun::InterpolatingFunction refractionIndexOfLambda(Genfun::CUBIC_SPLINE);
	while (inFile >> inLambda >> inN)
	{
		refractionIndexOfLambda.addPoint(inLambda, inN);
	}
	inFile.close();
	
	Hist2D hist(nBinLambda, lambdaMin, lambdaMax, nBinTheta,thetaMin, thetaMax);
	for (size_t i = 0; i < nSample; i++)
	{
		double b = sampleB();
		double lambda = sampleLambda();
		double n = refractionIndexOfLambda(lambda);
		double theta = Theta(b, n);
		hist.accumulate(lambda,theta);
	}
	double maxBinCount = hist.maxContents();


	QImage histImage(nBinLambda, nBinTheta, QImage::Format_RGB32);
    QImage colormap = generateColormap();

	for (size_t i = 0; i < nBinLambda; i++)
	{
		for (size_t j = 0; j < nBinTheta; j++)
		{
			double count = hist.bin(i, nBinTheta - 1 - j);
			histImage.setPixelColor(i,j,indexColor(count/maxBinCount));
		}
	}
	
	histImage.save("hist.bmp");
	colormap.save("colormap.bmp");
	return 0;
}