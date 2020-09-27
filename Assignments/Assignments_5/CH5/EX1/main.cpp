#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <Qt>
#include <QFont>
#include <QatPlotWidgets/PlotView.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotProfile.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotting/PlotKey.h>
#include <QatGenericFunctions/FixedConstant.h>
#include <QatGenericFunctions/Tanh.h>
#include <QatGenericFunctions/Sqrt.h>
#include <QatGenericFunctions/SimpleIntegrator.h>
#include <QatGenericFunctions/QuadratureRule.h>
#include <QatGenericFunctions/CubicSplinePolynomial.h>
#include <QatGenericFunctions/Exp.h>
#include <QatGenericFunctions/Log.h>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>


void showHelp()
{
    std::cout << "Usage: ex1 N n" << std::endl;
    std::cout << "N: max bin number = 2^N, 1 <= N::int <= 32" << std::endl;
    std::cout << "n=1: tanh(x)" << std::endl;
    std::cout << "n=2: sqrt(cosh(x))" << std::endl;
    exit(0);
}

int main(int argc, char **argv)
{
    int option;
    int POW_2_MAX;

    if (argc != 3)
    {
        showHelp();
    }

    if (!(std::istringstream(argv[1]) >> POW_2_MAX) || POW_2_MAX < 0 || POW_2_MAX > 32)
    {
        showHelp();
    }
    
    if (!(std::istringstream(argv[2]) >> option) || (option != 1 && option != 2))
    {
        showHelp();
    }
 
    
    Genfun::AbsFunction *integrand_ptr;
    double analyticalResult;
    std::string title;
    if (option == 1)
    {
        integrand_ptr = (Genfun::Tanh()).clone();
        analyticalResult = std::log(std::cosh(1.0));
        title = "tanh(x)";
    }else
    {
        integrand_ptr = (Genfun::Sqrt()(1.0 / Genfun::Tanh())).clone();
        analyticalResult = M_PI / 2 + std::atanh(std::sqrt(std::tanh(1.0)))
                                    - std::atan(std::sqrt(1.0/std::tanh(1.0)));
        title = "sqrt(coth(x))";
    }
    Genfun::GENFUNCTION integrand = *integrand_ptr;
    integrand_ptr = nullptr;
    
    
    Genfun::CubicSplinePolynomial rectangleInterpolation;
    Genfun::CubicSplinePolynomial trapezoidInterpolation;
    Genfun::CubicSplinePolynomial simpsonInterpolation;
    PlotProfile rectangleProfile;
    PlotProfile trapezoidProfile;
    PlotProfile simpsonProfile;

    const double xLeft = 0;
    const double xRight = 1;
    const size_t N_MAX = 1 << POW_2_MAX;
    double yMin = 1;
    double yMax = 0;
    for (size_t N = N_MAX; N >= 2; N = N >> 1)
    {
        double intervalWidth = (xRight - xLeft) / N;

        Genfun::SimpleIntegrator rectangleIntegrator(xLeft, xRight, Genfun::MidpointRule(), N);
        double rectangleResult = rectangleIntegrator(integrand);

        double trapezoidResult;
        if (option == 1)
        {
            Genfun::SimpleIntegrator trapezoidIntegrator(xLeft, xRight, Genfun::TrapezoidRule(), N);
            trapezoidResult = trapezoidIntegrator(integrand);
        }else if (option == 2)
        {
            Genfun::SimpleIntegrator trapezoidIntegrator(xLeft, xRight, Genfun::OpenTrapezoidRule(), N);
            trapezoidResult = trapezoidIntegrator(integrand);
        }

        double simpsonResult;
        if (option == 1)
        {
            Genfun::SimpleIntegrator simpsonIntegrator(xLeft, xRight, Genfun::SimpsonsRule(), N);
            simpsonResult = simpsonIntegrator(integrand);
        }else if (option == 2)
        {
            Genfun::SimpleIntegrator compositeIntegrator(xLeft, xLeft+intervalWidth, Genfun::MidpointRule(), 1);
            Genfun::SimpleIntegrator simpsonIntegrator(xLeft+intervalWidth, xRight, Genfun::SimpsonsRule(), N-1);
            simpsonResult = compositeIntegrator(integrand) + simpsonIntegrator(integrand);
        }
        
        double rectangleDiff = std::abs(rectangleResult - analyticalResult);
        double trapezoidDiff = std::abs(trapezoidResult - analyticalResult);
        double simpsonDiff = std::abs(simpsonResult - analyticalResult);

        yMin = std::min(yMin, rectangleDiff);
        yMin = std::min(yMin, trapezoidDiff);
        yMin = std::min(yMin, simpsonDiff);

        yMax = std::isfinite(rectangleDiff) ? std::max(yMax, rectangleDiff) : yMax;
        yMax = std::isfinite(trapezoidDiff) ? std::max(yMax, trapezoidDiff) : yMax;
        yMax = std::isfinite(simpsonDiff) ? std::max(yMax, simpsonDiff) : yMax;

        rectangleInterpolation.addPoint(std::log(intervalWidth), std::log(rectangleDiff));
        trapezoidInterpolation.addPoint(std::log(intervalWidth), std::log(trapezoidDiff));
        simpsonInterpolation.addPoint(std::log(intervalWidth), std::log(simpsonDiff));

        rectangleProfile.addPoint(intervalWidth, rectangleDiff);
        trapezoidProfile.addPoint(intervalWidth, trapezoidDiff);
        simpsonProfile.addPoint(intervalWidth, simpsonDiff);
    }
    yMin *= 0.5;
    yMax *= 3;
    const double xMin =  0.5 * 1.0 / N_MAX;
    const double xMax =  2 * 1.0 / 2.0;



    PlotFunction1D rectanglePlot(Genfun::Exp()(rectangleInterpolation(Genfun::Log())));
    PlotFunction1D trapezoidPlot(Genfun::Exp()(trapezoidInterpolation(Genfun::Log())));
    PlotFunction1D simpsonPlot(Genfun::Exp()(simpsonInterpolation(Genfun::Log())));

    {
        PlotProfile::Properties prop;
        prop.symbolSize = 20;
        prop.pen.setWidth(3);

        prop.symbolStyle = PlotProfile::Properties::CIRCLE;
        rectangleProfile.setProperties(prop);

        prop.symbolStyle = PlotProfile::Properties::SQUARE;
        trapezoidProfile.setProperties(prop);

        prop.symbolStyle = PlotProfile::Properties::TRIANGLE_L;
        simpsonProfile.setProperties(prop);
    }

    {
        PlotFunction1D::Properties prop;
        prop.pen.setWidth(3);

        prop.pen.setColor(Qt::black);
        prop.pen.setStyle(Qt::SolidLine);
        rectanglePlot.setProperties(prop);

        prop.pen.setColor(Qt::red);
        prop.pen.setStyle(Qt::DashLine);
        trapezoidPlot.setProperties(prop);

        prop.pen.setColor(Qt::blue);
        prop.pen.setStyle(Qt::DotLine);
        simpsonPlot.setProperties(prop);
    }

    PlotKey legend(xMin*1.5, yMax);
    legend.setFont(QFont("Sans Serif", 20));
    legend.add(&rectanglePlot, "Rectangle Rule");
    legend.add(&trapezoidPlot, "Trapezoid Rule");
    legend.add(&simpsonPlot, "Simpson's Rule");
    
    QApplication app(argc, argv);

    PlotView *view_ptr = new PlotView(PRectF(xMin, xMax, yMin, yMax));
    view_ptr->setLogY(true);
    view_ptr->setLogX(true);
    view_ptr->setFixedWidth(1200);
	view_ptr->setFixedHeight(800);

    PlotStream titleStream(view_ptr->titleTextEdit());
	titleStream << PlotStream::Clear()
				<< PlotStream::Center()
				<< PlotStream::Family("Sans Serif")
				<< PlotStream::Size(24)
				<< title
				<< PlotStream::EndP();

	PlotStream xLabelStream(view_ptr->xLabelTextEdit());
	xLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "h"
				 << PlotStream::EndP();

	PlotStream yLabelStream(view_ptr->yLabelTextEdit());
	yLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "error"
				 << PlotStream::EndP();

    view_ptr->add(&rectanglePlot);
    view_ptr->add(&trapezoidPlot);
    view_ptr->add(&simpsonPlot);

    view_ptr->add(&rectangleProfile);
    view_ptr->add(&trapezoidProfile);
    view_ptr->add(&simpsonProfile);  

    view_ptr->add(&legend);

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