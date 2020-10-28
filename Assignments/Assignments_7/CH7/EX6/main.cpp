#include <iostream>
#include <random>
#include <cmath>
#include <QatDataAnalysis/Hist1D.h>
#include <QatGenericFunctions/F1D.h>
#include <QatGenericFunctions/Square.h>
#include <QatGenericFunctions/AssociatedLegendre.h>
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


const size_t nSample = 1e7;
const size_t nBin = 100;
const double binMin = -1;
const double binMax = 1;
const double binWidth = (binMax - binMin) / nBin;
const double amp1 = std::sqrt(3.0/8.0);
const double amp2 = std::sqrt(7.0/6.0);
const double amp3 = std::sqrt(11.0/24.0);
const double amp4 = std::sqrt(15.0/6.0);


int main(int argc, char **argv)
{
	Genfun::GENFUNCTION qSquare = Genfun::Square();
	Genfun::GENFUNCTION qP1 = Genfun::AssociatedLegendre(1,0);
	Genfun::GENFUNCTION qP3 = Genfun::AssociatedLegendre(3,0);
	Genfun::GENFUNCTION qP5 = Genfun::AssociatedLegendre(5,0);
	Genfun::GENFUNCTION qP7 = Genfun::AssociatedLegendre(7,0);
	Genfun::GENFUNCTION pdf = qSquare(amp1 * qP1 - amp2 * qP3 + amp3 * qP5 - amp4 * qP7);
	double pdfMax = pdf(1);


	PlotFunction1D pdfPlot(nSample * binWidth * pdf);
	{
		PlotFunction1D::Properties prop;
		prop.pen.setWidth(4);
		pdfPlot.setProperties(prop);
	}

	std::random_device rndDev;
	std::mt19937_64 xUEngine(rndDev());
	std::mt19937_64 yUEngine(rndDev());
	std::uniform_real_distribution<double> xUDist(binMin, binMax);
	std::uniform_real_distribution<double> yUDist(0,pdfMax);
	Hist1D rejectHist("Rejection Histogram", nBin, binMin, binMax);
	double count = 0, total = 0;
	while (count < nSample)
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
		prop.pen.setColor(Qt::red);
		prop.pen.setWidth(3);
		rejectPlot.setProperties(prop);
	}


	QApplication app(argc, argv);
	QMainWindow window;

	PlotView * view_ptr = new PlotView(PRectF(-1,1,0,nSample*pdfMax*binWidth));
    view_ptr->setFixedWidth(1200);
	view_ptr->setFixedHeight(800);
	window.setCentralWidget(view_ptr);

	view_ptr->add(&pdfPlot);
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
				 << "Cos Theta"
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