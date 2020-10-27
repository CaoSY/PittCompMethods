#include <iostream>
#include <random>
#include <cmath>
#include <QatDataAnalysis/Hist1D.h>
#include <QatGenericFunctions/F1D.h>
#include <QatPlotting/PlotHist1D.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/RealArg.h>
#include <QatPlotting/PlotStream.h>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QFont>
#include <QatPlotWidgets/PlotView.h>

const double tau = 3.0;
const double sigma = 2.0;
const double sigmaTau = sigma * tau;
const double sigmaTau2 = sigmaTau * sigmaTau;
const size_t sampleNum = 1e6;
const size_t binNum = 50;
const double binMin = -5;
const double binMax = 6;
const double binWidth = (binMax - binMin) / binNum;
const double pdfMax = 0.1968556435586524;
const double uMin = -10;
const double uMax = 11;

double pdf(double y)
{
	return 0.5 * tau * std::exp(sigmaTau2/2.0 - tau*y) * (1 + std::erf((y - sigma * sigmaTau)/(std::sqrt(2)*sigma)));
}

double scalePDF(double y)
{
	return (sampleNum * binWidth) * pdf(y);
}

int main(int argc, char **argv)
{
	PlotFunction1D pdfPlot((Genfun::F1D(scalePDF)));
	{
		PlotFunction1D::Properties prop;
		prop.pen.setWidth(4);
		pdfPlot.setProperties(prop);
	}

	std::random_device rndDev;
	std::mt19937_64 expEngine(rndDev());
	std::mt19937_64 gaussianEngine(rndDev());
	std::exponential_distribution<double> expDist(tau);
	std::normal_distribution<double> gaussianDist(0.0, sigma);

	Hist1D convHist("Convolution Histogram", binNum, binMin, binMax);
	for (size_t i = 0; i < sampleNum; i++)
	{
		double x1 = expDist(expEngine);
		double x2 = gaussianDist(gaussianEngine);
		convHist.accumulate(x1 + x2);
	}
	
	PlotHist1D histPlot(convHist);
	{
		PlotHist1D::Properties prop;
		prop.pen.setColor(Qt::red);
		prop.pen.setWidth(2.5);
		histPlot.setProperties(prop);
	}

	std::mt19937_64 xUEngine(rndDev());
	std::mt19937_64 yUEngine(rndDev());
	std::uniform_real_distribution<double> xUDist(uMin, uMax);
	std::uniform_real_distribution<double> yUDist(0,pdfMax);
	Hist1D rejectHist("Rejection Histogram", binNum, binMin, binMax);
	double count = 0, total = 0;
	while (count < sampleNum)
	{
		double x = xUDist(xUEngine);
		double y = yUDist(yUEngine);
		if (y <= pdf(x))
		{
			rejectHist.accumulate(x);
			++count;
		}
		++total;
	}
	std::cout << "rejection rate: " << 1 - (count / total) << std::endl;
	
	PlotHist1D rejectPlot(rejectHist);
	{
		PlotHist1D::Properties prop;
		prop.pen.setColor(Qt::blue);
		prop.pen.setWidth(2.5);
		rejectPlot.setProperties(prop);
	}


	QApplication app(argc, argv);
	QMainWindow window;

	PlotView * view_ptr = new PlotView(histPlot.rectHint());
    view_ptr->setFixedWidth(1200);
	view_ptr->setFixedHeight(800);
	window.setCentralWidget(view_ptr);

	view_ptr->add(&pdfPlot);
	view_ptr->add(&histPlot);
	view_ptr->add(&rejectPlot);

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