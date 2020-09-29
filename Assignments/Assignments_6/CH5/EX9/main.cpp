#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <QatGenericFunctions/RombergIntegrator.h>
#include <QatGenericFunctions/RootFinder.h>
#include <QatGenericFunctions/F1D.h>
#include <QatGenericFunctions/FixedConstant.h>
#include <QatPlotting/RealArg.h>
#include <stdexcept>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QFont>
#include <QatPlotWidgets/PlotView.h>
#include <QatPlotWidgets/CustomRangeDivider.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotting/PlotKey.h>

const double restMass = 1370 * 2;
const double E0 = 28.4218519833473994252300447491;
const double Beta = 3.51768081444135951932841186128;
const double Gamma = 30.1959438525934070224310663778;
const double x1Eq = 0.434238646090311696003693054554;
const double ROOT_EPS = 1e-12;
const double INTEGRATION_EPS = 1e-11;
const double ENERGY_EPS = 1e-10;
const double E_MIN = 15.6179682146698349069120127724; // Minimal Possible Energy when l = 1


double l = 0;	// Angular Momentum Number
double n = 0;	// Energy Level
double epsilon = 0;		// Reduced Energy
double eta = 3/4;	// Zero Point Energy

double integrandCoreNumeric(double xi)
{
	return epsilon + Beta/xi - Gamma*xi - l*(l+1)/(xi*xi);
}

double integrandNumeric(double xi)
{
	return std::sqrt(integrandCoreNumeric(xi));
}

double xiMinimum()
{
	size_t maxCalls = 100;
	Genfun::NewtonRaphson rootNR(Genfun::F1D(integrandCoreNumeric), ROOT_EPS, maxCalls);
	
	double xiMin = rootNR.root(0.2 * x1Eq);
	
	if (rootNR.nCalls() >= maxCalls)
	{
		throw std::runtime_error("Can't find xi_min");
	}

	return xiMin;	
}

double xiMaximum()
{
	size_t maxCalls = 100;
	Genfun::NewtonRaphson rootNR(Genfun::F1D(integrandCoreNumeric), ROOT_EPS, maxCalls);
	
	double xiMax = rootNR.root(5 * x1Eq);
	
	if (rootNR.nCalls() >= maxCalls)
	{
		throw std::runtime_error("Can't find xi_max");
	}

	return xiMax;
}

double quantizationRule(double ReducedEnergy)
{
	epsilon = ReducedEnergy;

	double xiMin, xiMax;
	if (l == 0)
	{
		xiMin = 0;
		xiMax = xiMaximum();
		eta = 3.0/4.0;
	}else if (l == 1)
	{
		xiMin = xiMinimum();
		xiMax = xiMaximum();
		eta = 1.0/2.0;
	}else
	{
		throw std::range_error("Not supported l");
	}
	
	size_t maxIter = 20;
	Genfun::RombergIntegrator integrator(xiMin, xiMax, Genfun::RombergIntegrator::OPEN, Genfun::RombergIntegrator::ABSOLUTE);
	integrator.setEpsilon(INTEGRATION_EPS);
	integrator.setMaxIter(maxIter);

	double result = integrator(Genfun::F1D(integrandNumeric));
	
	return result - (n + eta) * M_PI;
}

double findEnergy(double energyLevel,double angularMomentumNumber)
{
	n = energyLevel;
	l = angularMomentumNumber;

	size_t maxCalls = 100;

	double energy;
	if (l == 1)
	{
		Genfun::NewtonRaphson root(Genfun::F1D(quantizationRule), ENERGY_EPS, maxCalls);
		energy = root.root(10 * E_MIN);
		if (root.nCalls() >= maxCalls)
		{
			throw std::runtime_error("Can't find Energy");
		}
	}else if (l == 0)
	{
		Genfun::Bisection root(Genfun::F1D(quantizationRule),ENERGY_EPS, maxCalls);
		root.lowerBound() = E_MIN / 50;
		root.upperBound() = E_MIN * 10000;
		energy = root.root(10 * E_MIN);
		if (root.nCalls() >= maxCalls)
		{
			throw std::runtime_error("Can't find Energy");
		}
	}
	
	return energy;
}

PlotFunction1D plotBar(double x, double y, double halfWidth)
{
    double xLeft = x - halfWidth;
    double xRight = x + halfWidth;
    PlotFunction1D bar(Genfun::FixedConstant(y), RealArg::Gt(xLeft) && RealArg::Lt(xRight));

    return bar;
}

/*
double l = 0;	// Angular Momentum Number
double n = 0;	// Energy Level
double epsilon = 0;		// Reduced Energy
double eta = 3/4;	// Zero Point Energy
*/
int main(int argc, char **argv)
{
	std::array<std::array<double, 2>, 3> energyTable;

	for (size_t energyLevel = 0; energyLevel < energyTable.size(); energyLevel++)
	{
		for (size_t AzimuthalNumber = 0; AzimuthalNumber < energyTable[energyLevel].size(); AzimuthalNumber++)
		{
			energyTable[energyLevel][AzimuthalNumber] = restMass + E0 * findEnergy(energyLevel, AzimuthalNumber);
			std::cout << "n=" << energyLevel;
			std::cout << " l=" << AzimuthalNumber;
			std::cout << " " << energyTable[energyLevel][AzimuthalNumber] << std::endl;
		}
		
	}
	
	QApplication app(argc, argv);
	QMainWindow window;

	double xLeft = -0.5;
	double xRight = 2.5;
	double yBottom = restMass * 0.9;
	double yTop = 0;

	std::vector<PlotFunction1D> barPlots;
	for (size_t i = 0; i < energyTable.size(); i++)
	{
		for (size_t j = 0; j < energyTable[j].size(); j++)
		{
			barPlots.push_back(plotBar(i, energyTable[i][j], 0.3));
			yTop = std::max(yTop, energyTable[i][j]);
		}
	}
	yTop = 1.2 * yTop;

	PlotView *view_ptr = new PlotView(PRectF(xLeft, xRight, yBottom, yTop));
    view_ptr->setFixedWidth(1200);
	view_ptr->setFixedHeight(800);
	window.setCentralWidget(view_ptr);

	for (size_t i = 0; i < barPlots.size(); i++)
	{
		view_ptr->add(&barPlots[i]);
		PlotFunction1D::Properties prop;
		prop.pen.setWidth(3);
		if (i % 2)
		{
			prop.pen.setStyle(Qt::DashLine);
		}
		barPlots[i].setProperties(prop);
	}
	
	PlotFunction1D restMassPlot = plotBar(1.5, restMass, 5);
	{
		PlotFunction1D::Properties prop;
		prop.pen.setWidth(3);
		prop.pen.setStyle(Qt::DotLine);
		restMassPlot.setProperties(prop);
	}
	view_ptr->add(&restMassPlot);

	PlotKey legend(xLeft+0.2, yTop);
    legend.setFont(QFont("Sans Serif", 20));
	legend.add(&barPlots[0], "L = 0");
	legend.add(&barPlots[1], "L = 1");
	legend.add(&restMassPlot, "Rest Mass");
	view_ptr->add(&legend);
	
	CustomRangeDivider xRange;
	xRange.add(0, "0");
	xRange.add(1, "1");
	xRange.add(2, "2");
	view_ptr->setXRangeDivider(&xRange);


    PlotStream titleStream(view_ptr->titleTextEdit());
	titleStream << PlotStream::Clear()
				<< PlotStream::Center()
				<< PlotStream::Family("Sans Serif")
				<< PlotStream::Size(24)
				<< "Energy"
				<< PlotStream::EndP();

	PlotStream xLabelStream(view_ptr->xLabelTextEdit());
	xLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "n"
				 << PlotStream::EndP();

	PlotStream yLabelStream(view_ptr->yLabelTextEdit());
	yLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "Energy / MeV"
				 << PlotStream::EndP();


	QToolBar *toolBar = window.addToolBar("Tools");
	
    QAction *quitAction = toolBar->addAction("Quit (q)");
    quitAction->setShortcut(QKeySequence("q"));
	QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));
    
    QAction *saveAction = toolBar->addAction("Save as (s)");
    saveAction->setShortcut(QKeySequence("s"));
    QObject::connect(saveAction, SIGNAL(triggered()), view_ptr, SLOT(save()));

	window.show();
	app.exec();

	
	return 0;
}