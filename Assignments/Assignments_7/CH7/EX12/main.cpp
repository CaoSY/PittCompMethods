#include <iostream>
#include <sstream>
#include <random>
#include <cmath>
#include <QatDataAnalysis/Hist1D.h>
#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/Sqrt.h>
#include <QatGenericFunctions/Exp.h>
#include <QatGenericFunctions/Tan.h>
#include <QatGenericFunctions/Square.h>
#include <QatPlotting/PlotHist1D.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/RealArg.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotWidgets/PlotView.h>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QFont>

const size_t nSample = 1e7;
const size_t nBin = 100;
const double YMIN = 1;
const double YMAX = 10;
const double M2 = 1*1;
const double MG = M_PI_4;

int main(int argc, char **argv)
{
	assert(argc == 2);

	int type;
	assert(std::istringstream(argv[1]) >> type);

	assert(type == 1 || type == 2);


	Genfun::Variable X;
	Genfun::GENFUNCTION f1 = YMIN * Genfun::Exp()(std::log(YMAX/YMIN) * X);
	Genfun::GENFUNCTION f2 = Genfun::Sqrt()(M2 + MG * Genfun::Tan()(MG * X));
	Genfun::GENFUNCTION rho1 = 1.0/std::log(YMAX/YMIN) * 1.0/X;
	Genfun::GENFUNCTION rho2 = 2 * X / (MG * MG + Genfun::Square()(M2 - X*X));
	
	Genfun::GENFUNCTION f = type == 1 ? f1 : f2;
	Genfun::GENFUNCTION rho = type == 1 ? rho1 : rho2;

	double ymin = f(0);
	double ymax = f(1);
	double binWidth = (ymax - ymin) / nBin;

	PlotFunction1D pdfPlot(nSample * binWidth * rho);
	{
		PlotFunction1D::Properties prop;
		prop.pen.setWidth(4);
		pdfPlot.setProperties(prop);
	}

	std::random_device rndDev;
	std::mt19937_64 rndEngine(rndDev());
	std::uniform_real_distribution<double> uDist(0,1);
	Hist1D tHist(nBin, ymin, ymax);
	for (size_t i = 0; i < nSample; i++)
	{
		double x = uDist(rndEngine);
		double y = f(x);
		tHist.accumulate(y);
	}
	
	
	PlotHist1D tHistPlot(tHist);
	{
		PlotHist1D::Properties prop;
		prop.pen.setColor(Qt::red);
		prop.pen.setWidth(3);
		tHistPlot.setProperties(prop);
	}


	QApplication app(argc, argv);
	QMainWindow window;

	PlotView * view_ptr = new PlotView(tHistPlot.rectHint());
    view_ptr->setFixedWidth(1200);
	view_ptr->setFixedHeight(800);
	window.setCentralWidget(view_ptr);

	view_ptr->add(&pdfPlot);
	view_ptr->add(&tHistPlot);

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