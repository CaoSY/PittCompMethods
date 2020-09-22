#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/Theta.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Exp.h"
#include <QatGenericFunctions/InterpolatingFunction.h>
#include <QatGenericFunctions/Sin.h>
#include <QatGenericFunctions/Cos.h>
#include <QatPlotting/PlotProfile.h>
#include <QatPlotting/PlotOrbit.h>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QAction>
#include <QColor>
#include <QObject>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <libgen.h>
#include <complex>
#include <vector>

typedef std::complex<double> Complex;

std::vector<Complex> parsePoints()
{
	double x, y;
	std::vector<Complex> points;
	while (std::cin >> x >> y)
	{
		points.push_back(Complex(x,y));
	}
	return points;
}

PlotProfile *showPoints(const std::vector<Complex>& points)
{
	PlotProfile *profile_ptr = new PlotProfile();
	
	for (size_t i = 0; i < points.size(); i++)
	{
		profile_ptr->addPoint(points[i].real(), points[i].imag());
	}
	
	{
		PlotProfile::Properties prop;
		prop.symbolSize = 20;
		prop.pen.setWidth(7);
		prop.pen.setColor(QColor(118,158,9));
		profile_ptr->setProperties(prop);
	}

	return profile_ptr;
}

bool sortByArg(Complex a, Complex b)
{
	return std::arg(a) < std::arg(b);
}

PlotOrbit *showFitLine(std::vector<Complex> points)
{
	std::sort(points.begin(), points.end(), sortByArg);

	Genfun::InterpolatingFunction rho(Genfun::CUBIC_SPLINE);

	for (size_t i = 0; i < points.size(); i++)
	{
		rho.addPoint(std::arg(points[i]), std::abs(points[i]));
	}
	
	Genfun::Variable theta;
	Genfun::GENFUNCTION Xtheta = rho(theta) * Genfun::Cos()(theta);
	Genfun::GENFUNCTION Ytheta = rho(theta) * Genfun::Sin()(theta);

	const double rightEndExtension = 0.1, leftEndExtension = 0.05;
	PlotOrbit *orbit_ptr = new PlotOrbit(Xtheta, Ytheta, std::arg(points.front())-leftEndExtension, std::arg(points.back())+rightEndExtension);
	{
		PlotOrbit::Properties prop;
		prop.pen.setWidth(5);
		prop.pen.setColor(QColor(9,118,158));
		orbit_ptr->setProperties(prop);
	}
	return orbit_ptr;
}

int main(int argc, char **argv)
{

	// Automatically generated:-------------------------:

	std::string usage = std::string("usage: ") + argv[0];
	if (argc != 1)
	{
		std::cout << usage << std::endl;
	}

	QApplication app(argc, argv);

	QMainWindow window;
	QToolBar *toolBar = window.addToolBar("Tools");
	QAction *nextAction = toolBar->addAction("Next");

	nextAction->setShortcut(QKeySequence("n"));

	QObject::connect(nextAction, SIGNAL(triggered()), &app, SLOT(quit()));

	PRectF rect;
	rect.setXmin(0.0);
	rect.setXmax(631);
	rect.setYmin(0.0);
	rect.setYmax(442);

	PlotView view(rect);
	view.setXZero(false);
	view.setYZero(false);
	view.setGrid(false);
	view.setFixedWidth(900);
	view.setFixedHeight(700);
	window.setCentralWidget(&view);

	char *wd = dirname(argv[0]);
	std::string imageName = std::string(wd) + "/../../src/p1.png";
	QPixmap imagePix(imageName.c_str());
	QGraphicsPixmapItem imagePixItem;
	imagePixItem.setPixmap(imagePix);
	//imagePixItem.setFlags(QGraphicsItem::ItemIsMovable);
	view.scene()->addItem(&imagePixItem);
	imagePixItem.setScale(1.0);
	imagePixItem.setPos(130, 130);

	std::vector<Complex> points = parsePoints();
	view.add(showPoints(points));
	view.add(showFitLine(points));

	PlotStream titleStream(view.titleTextEdit());
	titleStream << PlotStream::Clear()
				<< PlotStream::Center()
				<< PlotStream::Family("Sans Serif")
				<< PlotStream::Size(16)
				<< "P1 Bus interpolated position"
				<< PlotStream::EndP();

	PlotStream xLabelStream(view.xLabelTextEdit());
	xLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(16)
				 << "X position, pixel units"
				 << PlotStream::EndP();

	PlotStream yLabelStream(view.yLabelTextEdit());
	yLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(16)
				 << "Y position, pixel units"
				 << PlotStream::EndP();

    QAction *saveAction = toolBar->addAction("Save as (s)");
    saveAction->setShortcut(QKeySequence("s"));
    QObject::connect(saveAction, SIGNAL(triggered()), &view, SLOT(save()));

	view.show();
	window.show();
	app.exec();
	return 1;
}
