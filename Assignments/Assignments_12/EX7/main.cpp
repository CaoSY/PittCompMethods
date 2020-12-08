#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/Parameter.h>
#include <QatGenericFunctions/Square.h>
#include <QatGenericFunctions/Sqrt.h>
#include <QatGenericFunctions/Exp.h>
#include <QatGenericFunctions/Sin.h>
#include <QatGenericFunctions/Cos.h>
#include <QatGenericFunctions/VoigtDistribution.h>
#include <QatGenericFunctions/RKIntegrator.h>
#include <QatDataAnalysis/Hist1D.h>
#include <QatDataAnalysis/Table.h>
#include <QatDataModeling/TableLikelihoodFunctional.h>
#include <QatDataModeling/MinuitMinimizer.h>
#include <QatPlotWidgets/PlotView.h>
#include <QatPlotWidgets/MultipleViewWindow.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotting/PlotHist1D.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotProfile.h>
#include <QatPlotting/PlotKey.h>
#include <QatPlotting/PlotOrbit.h>
#include <Eigen/Dense>
#include <QApplication>
#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QFont>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>

PlotView *createPlotView(const std::string& title, const std::string& xLabel, const std::string yLabel, const PRectF& viewRange,  const double Width=1200, const double Height=800);

typedef std::vector<double> Data;
typedef std::unique_ptr<Genfun::AbsFunction> FuncPtr;
typedef std::weak_ptr<Genfun::Parameter> ParamPtr;

class PlanetOrbit
{
private:
    Genfun::RKIntegrator integrator;

public:
    Genfun::Parameter* GM;
    Genfun::Parameter* e;
    Genfun::Parameter* a;
    Genfun::Parameter* theta0;
    Genfun::Parameter* inclination;
    Genfun::Parameter* orientation;
    FuncPtr X;
    FuncPtr Y;

    PlanetOrbit()
    {
        Genfun::Variable theta;
        Genfun::Sin qSin;
        Genfun::Cos qCos;
        Genfun::Sqrt qSqrt;

        GM = integrator.createControlParameter("GM", 0.0002959, 0);
        e = integrator.createControlParameter("eccentricity", 0, 0, 1);
        a = integrator.createControlParameter("semimajor axis", 1);
        theta0 = integrator.createControlParameter("Theta0", 0, -2*M_PI, 2*M_PI);
        orientation = new Genfun::Parameter("Orientation", 0, -2*M_PI, 2*M_PI);
        inclination = new Genfun::Parameter("Inclination", 0, 0, M_PI);

        Genfun::GENFUNCTION r = (*a) * (1 - (*e)*(*e)) / (1 + (*e) * qCos(theta - (*theta0)));
        Genfun::GENFUNCTION DTheta = qSqrt((*GM) * (*a) * (1 - (*e)*(*e))) / (r*r);

        integrator.addDiffEquation(&DTheta);

        Genfun::GENFUNCTION Theta = *integrator.getFunction(theta);
        Genfun::GENFUNCTION Radius = (*a) * (1 - (*e) * (*e)) / (1 + (*e) * qCos(Theta - (*theta0)));

        X = FuncPtr((Radius * qCos(Theta - (*theta0) + (*orientation))).clone());
        Y = FuncPtr((Radius * qSin(Theta - (*theta0) + (*orientation))).clone());
    }

    ~PlanetOrbit()
    {
        delete orientation;
    }
};

const double MAX_ANGLE = 3 * M_PI;

double Time2Radian(const double hour, const double minute, const double second)
{
    const double Hour2Radian = 2 * M_PI / 24;
    const double Minute2Radian = Hour2Radian / 60;
    const double Second2Radian = Minute2Radian/ 60;
    double angle = Hour2Radian*std::abs(hour) + Minute2Radian*std::abs(minute) + Second2Radian*std::abs(second);
    return std::signbit(hour) ? -angle : angle;
}

double Degree2Radian(const double degree, const double minute, const double second)
{
    const double Degree2Radian = 2 * M_PI / 360;
    const double Minute2Radian = Degree2Radian / 60;
    const double Second2Radian = Minute2Radian / 60;
    double angle = Degree2Radian*std::abs(degree) + Minute2Radian*std::abs(minute) + Second2Radian*std::abs(second);
    return std::signbit(degree) ? -angle : angle;
}

void readData(const std::string& file, Data& TIME, Data& RA, Data& DEC)
{
    std::ifstream inFile(file);
    std::string day, time;

    for (size_t i = 1; inFile >> day >> time; i++)
    {
        TIME.push_back(i);

        double hour, minute, second;
        inFile >> hour >> minute >> second;
        RA.push_back(Time2Radian(hour, minute, second));
        inFile >> hour >> minute >> second;
        DEC.push_back(Degree2Radian(hour, minute, second));
    }  
}

Data refineData(const Data& rawData, const double lowerBound, const double upperBound);
Hist1D binData(const Data& data);
Table tableData(const Data& data);

const std::string dataFileName = "Mars01.dat";
const bool verbose = true;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MultipleViewWindow window;

    QToolBar *toolBar = window.addToolBar("Tools");
    QAction *quitAction = toolBar->addAction("Quit (q)");
    quitAction->setShortcut(QKeySequence("q"));
	QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));

    PlanetOrbit Earth;
    Earth.e->setValue(0.8);
    Earth.theta0->setValue(M_PI/3);
    Earth.orientation->setValue(-M_PI/3);
    PlotOrbit earthOrbit(*(Earth.X), *(Earth.Y), 0, 360);
    PlotView *earthView = createPlotView("Earth", "X", "Y", PRectF(-2, 2, -2, 2), 800, 800);
    earthView->add(&earthOrbit);
    window.add(earthView, "Earth");

    Data TIME, RA, DEC;
    readData(dataFileName, TIME, RA, DEC);

    std::cout << "RA @ 0: " << RA[0] << std::endl;
    std::cout << "DEC @ 0: " << DEC[0] << std::endl;

    PlotView *RAView = createPlotView("Right Ascension", "Days", "", PRectF(-10, 760, -0.5, 2*M_PI + 0.5));
    window.add(RAView, "RA");

    PlotProfile RAProfile;
    for (size_t i = 0; i < TIME.size(); i++)
    {
        RAProfile.addPoint(TIME[i],RA[i]);
    }
    {
        PlotProfile::Properties prop;
        prop.pen.setWidth(1);
        RAProfile.setProperties(prop);
    }
    RAView->add(&RAProfile);


    PlotView *DECView = createPlotView("Declination", "Days", "", PRectF(-10, 760, -1, 1));
    window.add(DECView, "DEC");

    PlotProfile DECProfile;
    for (size_t i = 0; i < TIME.size(); i++)
    {
        DECProfile.addPoint(TIME[i], DEC[i]);
    }
    {
        PlotProfile::Properties prop;
        prop.pen.setWidth(1);
        DECProfile.setProperties(prop);
    }
    DECView->add(&DECProfile);
    

	window.show();
	app.exec();

    return 0;
}


PlotView *createPlotView(const std::string& title, const std::string& xLabel, const std::string yLabel, const PRectF& viewRange, const double Width, const double Height)
{
    PlotView *view_ptr = new PlotView(viewRange);
    view_ptr->setFixedWidth(Width);
    view_ptr->setFixedHeight(Height);

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
                << xLabel
                << PlotStream::EndP();

    PlotStream yLabelStream(view_ptr->yLabelTextEdit());
    yLabelStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Sans Serif")
                << PlotStream::Size(24)
                << yLabel
                << PlotStream::EndP();
    
    return view_ptr;
}