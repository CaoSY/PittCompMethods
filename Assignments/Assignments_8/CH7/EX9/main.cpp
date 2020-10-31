#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/Exp.h>
#include <QatPlotting/PlotProfile.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotWidgets/PlotView.h>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QFont>

const double n = 1;
const double lambda = 1;
const double tau = 1;
const double nBatch = 1000;

std::random_device rndDev;
std::mt19937_64 tEng(rndDev());
std::mt19937_64 zEng(rndDev());
std::mt19937_64 uEng(rndDev());
std::mt19937_64 rEng(rndDev());
std::exponential_distribution<double> rDist(1);
std::exponential_distribution<double> tDist(1);
std::uniform_real_distribution<double> uDist(-1,1);	// sample cos(theta)

double F(double l, double t)
{
	std::uniform_real_distribution<double> zDist(0, l);

	double nSample_d = nBatch * n * l;
	size_t nSample = std::ceil(nSample_d);
	size_t count = 0;

	for (size_t i = 0; i < nSample; i++)
	{
		if (t > tDist(tEng))
		{
			double u = uDist(uEng);
			if (u > 0)
			{
				double z = zDist(zEng);
				double r = z / u;
				if (r < rDist(rEng))
				{
					++count;
				}
			}
		}
	}
	return (count / nBatch) * (nSample_d / nSample);
}

double leastSquareFit(const std::vector<double> x, const std::vector<double> y)
{
	double xBar = 0;
	double yBar = 0;
	double xyBar = 0;
	double x2Bar = 0;
	for (size_t i = 0; i < x.size(); i++)
	{
		xBar += x[i];
		yBar += y[i];
		xyBar += x[i] * y[i];
		x2Bar += x[i] * x[i];
	}
	xBar /= x.size();
	yBar /= x.size();
	xyBar /= x.size();
	x2Bar /= x.size();

	double k = (xyBar - xBar*yBar) / (x2Bar - xBar*xBar);
	double b = yBar - k * xBar;
	return b;
}

int main(int argc, char **argv)
{
	Genfun::Variable T;
	Genfun::GENFUNCTION Ft = n * lambda / 4 * (1 - Genfun::Exp()(-T));

	double t0 = 2;
	std::vector<double> hVec, FtVec;
	for (size_t i = 4; i <= 6; i++)
	{
		std::cout << i << std::endl;
		double L = std::pow(10,i);
		double f = F(L, t0);
		hVec.push_back(1/L);
		FtVec.push_back(f);	
	}

	PlotProfile profile;
	double fMin = 1e7;
	double fMax = 0;
	double xMin = 10;
	double xMax = 0;
	for (size_t i = 0; i < hVec.size(); i++)
	{
		fMax = fMax < FtVec[i] ? FtVec[i] : fMax;
		fMin = fMin > FtVec[i] ? FtVec[i] : fMin;
		xMax = xMax < hVec[i] ? hVec[i] : xMax;
		xMin = xMin > hVec[i] ? hVec[i] : xMin;
		profile.addPoint(hVec[i], FtVec[i]);
		std::cout << FtVec[i] << std::endl;
	}
	fMax *= 1.1;
	fMin *= 0.9;
	xMin *= 0.5;
	xMax *= 2;
	PlotProfile::Properties prop;
	prop.pen.setColor(Qt::red);
	prop.pen.setWidth(5);
	profile.setProperties(prop);
	
	std::cout << "fit: " << leastSquareFit(hVec, FtVec) << std::endl;


	QApplication app(argc, argv);
	QMainWindow window;

	PlotView * view_ptr = new PlotView(PRectF(xMin, xMax, fMin, fMax));
    view_ptr->setFixedWidth(1200);
	view_ptr->setFixedHeight(800);
	window.setCentralWidget(view_ptr);

	view_ptr->add(&profile);
	view_ptr->setLogX(true);

    PlotStream titleStream(view_ptr->titleTextEdit());
	titleStream << PlotStream::Clear()
				<< PlotStream::Center()
				<< PlotStream::Family("Sans Serif")
				<< PlotStream::Size(24)
				<< "Number"
				<< PlotStream::EndP();

	PlotStream xLabelStream(view_ptr->xLabelTextEdit());
	xLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "y"
				 << PlotStream::EndP();

	PlotStream yLabelStream(view_ptr->yLabelTextEdit());
	yLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << ""
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