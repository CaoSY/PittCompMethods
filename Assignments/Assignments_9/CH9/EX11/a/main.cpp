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
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotWidgets/PlotView.h>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QFont>

typedef std::unique_ptr<Genfun::AbsFunction> GENFUNCTION_Ptr;

Genfun::ASin qatASin;
Genfun::Abs qatAbs;

Genfun::AbsFunction *pdfTheta(double n)
{
	Genfun::Variable b;
	Genfun::GENFUNCTION Theta = 2.0 * qatASin(b) - 4.0 * qatASin(b / n);
	Genfun::GENFUNCTION pdfTheta = 2.0 * b / qatAbs(Theta.prime());
	return pdfTheta.clone();
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
	
	std::vector<GENFUNCTION_Ptr> pdfVec;
	std::vector<PlotFunction1D> lineVec;
	for (double lambda = 200; lambda <= 1000; lambda += 10)
	{
		double n = refractionIndexOfLambda(lambda);
		pdfVec.push_back(GENFUNCTION_Ptr(pdfTheta(n)));
		lineVec.push_back(PlotFunction1D(*pdfVec.back()));
	}
	
	QApplication app(argc, argv);
	QMainWindow window;

	PlotView * view_ptr = new PlotView(PRectF(0, 1, 0, 100));
    view_ptr->setFixedWidth(1200);
	view_ptr->setFixedHeight(800);
	window.setCentralWidget(view_ptr);

	for (size_t i = 0; i < lineVec.size(); i++)
	{
		view_ptr->add(&(lineVec[i]));
	}
	

    PlotStream titleStream(view_ptr->titleTextEdit());
	titleStream << PlotStream::Clear()
				<< PlotStream::Center()
				<< PlotStream::Family("Sans Serif")
				<< PlotStream::Size(24)
				<< "PDF"
				<< PlotStream::EndP();

	PlotStream xLabelStream(view_ptr->xLabelTextEdit());
	xLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "b"
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