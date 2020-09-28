#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <Qt>
#include <QColor>
#include <QFont>
#include <QatPlotting/PlotStream.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotWidgets/PlotView.h>
#include <QatGenericFunctions/F1D.h>
#include <cmath>
#include <iostream>
#include <vector>


double numberOfBoundState(double v)
{
    double N = std::floor(4*std::sqrt(M_PI * v) - 0.5);
    return N > 0 ? N : 0;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    const double xMin = 0;
    const double xMax = 5;
    const double yMin = 0;
    double yMax = 1.1 * numberOfBoundState(xMax);


    PlotView *view_ptr = new PlotView(PRectF(xMin, xMax, yMin, yMax));
    view_ptr->setFixedWidth(1200);
	view_ptr->setFixedHeight(800);

	PlotFunction1D NPlot((Genfun::F1D(numberOfBoundState)));
    {
		PlotFunction1D::Properties prop;
		prop.pen.setWidth(3);
		NPlot.setProperties(prop);
	}
	view_ptr->add(&NPlot);

    
    PlotStream titleStream(view_ptr->titleTextEdit());
	titleStream << PlotStream::Clear()
				<< PlotStream::Center()
				<< PlotStream::Family("Sans Serif")
				<< PlotStream::Size(24)
				<< "Number of Bound States"
				<< PlotStream::EndP();

	PlotStream xLabelStream(view_ptr->xLabelTextEdit());
	xLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "Reduced Potential"
				 << PlotStream::EndP();

	PlotStream yLabelStream(view_ptr->yLabelTextEdit());
	yLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "N"
				 << PlotStream::EndP();


	QMainWindow window;
    window.setCentralWidget(view_ptr);

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