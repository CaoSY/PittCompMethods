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

const double alpha = 3.0/2.0;
const double beta = 1.0;
const double gammaAlpha = std::sqrt(M_PI) / 2.0;
const size_t sampleNum = 1e7;
const size_t binNum = 100;
const double binMin = 0.0;
const double binMax = 6.0;
const double binWidth = (binMax-binMin) / binNum;
const double intervalScale = sampleNum * binWidth;

double gammaPDF(double x)
{
	const double factor = 1 / (gammaAlpha * std::pow(beta, alpha));
	return intervalScale * factor * std::pow(x, alpha-1) * std::exp(-x/beta);
}


int main(int argc, char **argv)
{
	PlotFunction1D gammaPlot(Genfun::F1D(gammaPDF), RealArg::Gt(0));
	{
		PlotFunction1D::Properties prop;
		prop.pen.setWidth(4);
		gammaPlot.setProperties(prop);
	}

	std::random_device rndDev;
	std::mt19937_64 rndEngine(rndDev());
	std::gamma_distribution<double> gammaDist(alpha, beta);

	Hist1D gammaHist("Gamma Distribution Histogram", binNum, binMin, binMax);
	for (size_t i = 0; i < sampleNum; i++)
	{
		gammaHist.accumulate(gammaDist(rndEngine));
	}
	
	PlotHist1D histPlot(gammaHist);
	{
		PlotHist1D::Properties prop;
		prop.pen.setColor(Qt::red);
		prop.pen.setWidth(2.5);
		histPlot.setProperties(prop);
	}



	QApplication app(argc, argv);
	QMainWindow window;

	PlotView * view_ptr = new PlotView(histPlot.rectHint());
    view_ptr->setFixedWidth(1200);
	view_ptr->setFixedHeight(800);
	window.setCentralWidget(view_ptr);

	view_ptr->add(&gammaPlot);
	view_ptr->add(&histPlot);

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
				 << "v^2"
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