#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <Qt>
#include <QColor>
#include <QFont>
#include <QatPlotting/PlotStream.h>
#include <QatPlotting/PlotProfile.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotKey.h>
#include <QatPlotting/RealArg.h>
#include <QatPlotWidgets/PlotView.h>
#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/FixedConstant.h>
#include <QatGenericFunctions/Cos.h>
#include <QatGenericFunctions/Sqrt.h>
#include <QatGenericFunctions/SimpleIntegrator.h>
#include <QatGenericFunctions/RombergIntegrator.h>
#include <QatGenericFunctions/GaussIntegrator.h>
#include <QatGenericFunctions/GaussQuadratureRule.h>
#include <QatGenericFunctions/QuadratureRule.h>
#include <cmath>
#include <iostream>
#include <vector>

PlotFunction1D plotBar(double x, double y, double halfWidth)
{
    double xLeft = x - halfWidth;
    double xRight = x + halfWidth;
    PlotFunction1D bar(Genfun::FixedConstant(y), RealArg::Gt(xLeft) && RealArg::Lt(xRight));

    return bar;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    const double xMin = 0;
    const double xMax = M_PI;
    const double yMin = 0.9;
    double yMax = 0;

    std::vector<double> highPrecisionValue = {1.004300572166438,1.017408790466781,1.039973339336188,1.073181999488621,1.118959037463299,1.180340594425067,1.262211619759275,1.372880489784419,1.527947581547658,1.762203727208658,2.185443629799353};

    std::vector<PlotFunction1D> barSimpleVector;
    std::vector<PlotFunction1D> barRombergVector;
    std::vector<PlotFunction1D> barChebyshevVector;
    std::vector<PlotFunction1D> barHighPrecisionVector;

    double MAX_N = 12;
    const size_t numberOfInterval = (1<<10);
    double relativeErrorSimple = 0;
    double relativeErrorRomberg = 0;
    double relativeErrorChebyshev = 0;
    for (double i = 1; i < MAX_N; i++)
    {
        double maxAngle = (i / MAX_N) * M_PI;
        
        Genfun::GENFUNCTION integrand =
            (1.0/(M_PI * std::sqrt(2))) / Genfun::Sqrt()(Genfun::Cos() - Genfun::Cos()(maxAngle));
        
        Genfun::Variable Theta;
        Genfun::GENFUNCTION integrandNormalize =
            (maxAngle/(M_PI * std::sqrt(2))) / Genfun::Sqrt()(Genfun::Cos()(maxAngle * Theta) - Genfun::Cos()(maxAngle));

        Genfun::SimpleIntegrator simpleIntegrator(-maxAngle, maxAngle, Genfun::OpenTrapezoidRule(), numberOfInterval);
        Genfun::RombergIntegrator rombergIntegrator(-maxAngle, maxAngle,Genfun::RombergIntegrator::OPEN);
        Genfun::GaussIntegrator chebyshevIntegrator(Genfun::GaussTchebyshevRule(numberOfInterval,Genfun::TchebyshevPolynomial::FirstKind), Genfun::GaussIntegrator::INTEGRATE_DX);

        double TSimple = simpleIntegrator(integrand);
        double TRomberg = rombergIntegrator(integrand);
        double TChebyshev = chebyshevIntegrator(integrandNormalize);
        double THighPrecision = highPrecisionValue[i-1];

        relativeErrorSimple += std::abs(THighPrecision - TSimple) / THighPrecision;
        relativeErrorRomberg += std::abs(THighPrecision - TRomberg) / THighPrecision;
        relativeErrorChebyshev += std::abs(THighPrecision - TChebyshev) / THighPrecision;

        yMax = std::max(yMax, TSimple);
        yMax = std::max(yMax, TRomberg);
        yMax = std::max(yMax, TChebyshev);
        yMax = std::max(yMax, THighPrecision);

        barSimpleVector.push_back(plotBar(maxAngle, TSimple, 0.1));
        barRombergVector.push_back(plotBar(maxAngle, TRomberg, 0.1));
        barChebyshevVector.push_back(plotBar(maxAngle, TChebyshev, 0.1));
        barHighPrecisionVector.push_back(plotBar(maxAngle, THighPrecision, 0.2));
    }
    yMax *= 1.1;
    relativeErrorSimple /= (MAX_N - 1);
    relativeErrorRomberg /= (MAX_N - 1);
    relativeErrorChebyshev /= (MAX_N - 1);

    std::cout << "Simple relative error: " << relativeErrorSimple << std::endl;
    std::cout << "Romberg relative error: " << relativeErrorRomberg << std::endl;
    std::cout << "Chebyshev relative error: " << relativeErrorChebyshev << std::endl;

    PlotFunction1D::Properties propSimple;
    PlotFunction1D::Properties propRomberg;
    PlotFunction1D::Properties propChebyshev;
    PlotFunction1D::Properties propHighPrecision;

    const double lineWidth = 3;
    propSimple.pen.setWidth(lineWidth);
    propRomberg.pen.setWidth(lineWidth);
    propChebyshev.pen.setWidth(lineWidth);
    propHighPrecision.pen.setWidth(lineWidth * 0.8);

    propSimple.pen.setColor(Qt::blue);
    propRomberg.pen.setColor(Qt::green);
    propChebyshev.pen.setColor(Qt::red);
    propHighPrecision.pen.setColor(Qt::black);
    propHighPrecision.pen.setStyle(Qt::DotLine);

    PlotKey legend(xMin*1.5, yMax);
    legend.setFont(QFont("Sans Serif", 20));
    legend.add(&barSimpleVector[0], "Midpoint Rule");
    legend.add(&barRombergVector[0], "Romberg Method");
    legend.add(&barChebyshevVector[0], "Gaussian Chebyshev");
    legend.add(&barHighPrecisionVector[0], "Value Precise up tp 16 Digits");

    PlotView *view_ptr = new PlotView(PRectF(xMin, xMax, yMin, yMax));
    view_ptr->setFixedWidth(1200);
	view_ptr->setFixedHeight(800);

    for (size_t i = 0; i < barSimpleVector.size(); i++)
    {
        view_ptr->add(&(barHighPrecisionVector[i]));
        barHighPrecisionVector[i].setProperties(propHighPrecision);

        view_ptr->add(&(barSimpleVector[i]));
        barSimpleVector[i].setProperties(propSimple);

        view_ptr->add(&(barRombergVector[i]));
        barRombergVector[i].setProperties(propRomberg);

        view_ptr->add(&(barChebyshevVector[i]));
        barChebyshevVector[i].setProperties(propChebyshev);
    }
    
    view_ptr->add(&legend);

    PlotStream titleStream(view_ptr->titleTextEdit());
	titleStream << PlotStream::Clear()
				<< PlotStream::Center()
				<< PlotStream::Family("Sans Serif")
				<< PlotStream::Size(24)
				<< "Period"
				<< PlotStream::EndP();

	PlotStream xLabelStream(view_ptr->xLabelTextEdit());
	xLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "Max Angle"
				 << PlotStream::EndP();

	PlotStream yLabelStream(view_ptr->yLabelTextEdit());
	yLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "Period"
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