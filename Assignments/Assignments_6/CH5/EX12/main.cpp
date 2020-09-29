#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <QatGenericFunctions/RombergIntegrator.h>
#include <QatGenericFunctions/RootFinder.h>
#include <QatGenericFunctions/F1D.h>
#include <QatGenericFunctions/FixedConstant.h>
#include <QatPlotting/RealArg.h>
#include <QatGenericFunctions/IncompleteGamma.h>
#include <QatGenericFunctions/Parameter.h>
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

double incompleteUpperGamma(double x, double s)
{
	Genfun::IncompleteGamma gamma = Genfun::IncompleteGamma(Genfun::IncompleteGamma::UPPER);
	Genfun::Parameter A("a", x);
	gamma.a() = A;
	return gamma(s);
}

double F(double z)
{
	double res = 0.5 * (std::exp(-z) - z * incompleteUpperGamma(0,z));
	return res;
}

int main(int argc, char **argv)
{
	PlotFunction1D fPlot((Genfun::F1D(F)), RealArg::Gt(0));
	{
		PlotFunction1D::Properties prop;
		prop.pen.setWidth(3);
		fPlot.setProperties(prop);
	}

	QApplication app(argc, argv);
	QMainWindow window;

	const double xLeft = 0;
	const double xRight = 5;
	const double yBottom = 0;
	const double yTop = 0.5;

	PlotView *view_ptr = new PlotView(PRectF(xLeft, xRight, yBottom, yTop));
    view_ptr->setFixedWidth(1200);
	view_ptr->setFixedHeight(800);
	window.setCentralWidget(view_ptr);

	view_ptr->add(&fPlot);


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
				 << "z / \\lambda"
				 << PlotStream::EndP();

	PlotStream yLabelStream(view_ptr->yLabelTextEdit());
	yLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "Number"
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