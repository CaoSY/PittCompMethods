#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/Sin.h>
#include <QatGenericFunctions/Cos.h>
#include <QatGenericFunctions/RKIntegrator.h>
#include <QApplication>
#include <QMainWindow>
#include <QatPlotWidgets/PlotView.h>
#include <QatPlotWidgets/MultipleViewWindow.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotting/PlotOrbit.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QAction>
#include <QToolBar>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

const double a = 1;
const double g = 1;
const double m = 1;
const double lambda = m * std::sqrt(a*a*a*g);
const double theta0 = M_PI / 9;
const double Tstart = 0;
const double Tend = 25;

int main(int argc, char **argv)
{
    double Omega;
    bool argError = true;
    if (argc == 1)
    {
        Omega = 0;
        argError = false;
    }else if (argc == 2)
    {
        std::stringstream omegaSStream(argv[1]);
        if(omegaSStream >> Omega)
        {
            argError = false;
        }
    }
    
    if (argError)
    {
        std::cout << "./spherical_pendulum Omega (Omega is a real number)" << std::endl;
        exit(0);
    }
    

    Genfun::Variable Theta(0,4);
    Genfun::Variable Phi(1,4);
    Genfun::Variable P_theta(2,4);
    Genfun::Variable P_phi(3,4);

    Genfun::Sin qSin;
    Genfun::Cos qCos;
    Genfun::GENFUNCTION H = 1/(2*m*a*a)*(P_theta*P_theta + (P_phi*P_phi) / (qSin(Theta)*qSin(Theta))) - m*g*a * qCos(Theta);

    Genfun::GENFUNCTION DThetaDt = H.partial(P_theta);
    Genfun::GENFUNCTION DPhiDt = H.partial(P_phi);
    Genfun::GENFUNCTION DP_thetaDt = -H.partial(Theta);
    Genfun::GENFUNCTION DP_phiDt = -H.partial(Phi);

    Genfun::RKIntegrator integrator;
    integrator.addDiffEquation(&DThetaDt, "Theta", theta0);
    integrator.addDiffEquation(&DPhiDt, "Phi", 0);
    integrator.addDiffEquation(&DP_thetaDt, "P_theta", 0);
    integrator.addDiffEquation(&DP_phiDt, "P_phi", lambda);

    Genfun::GENFUNCTION theta = *integrator.getFunction(Theta);
    Genfun::GENFUNCTION phi = *integrator.getFunction(Phi);

    Genfun::Variable t;
    Genfun::GENFUNCTION x = qSin(theta) * qCos(phi + Omega * t);
    Genfun::GENFUNCTION y = qSin(theta) * qSin(phi + Omega * t);

    PlotOrbit projectedOrbit(x, y, Tstart, Tend);

    QApplication app(argc, argv);
	QMainWindow window;

	PlotView * view_ptr = new PlotView(PRectF(-1, 1, -1, 1));
    view_ptr->setFixedWidth(800);
	view_ptr->setFixedHeight(800);
	window.setCentralWidget(view_ptr);

    view_ptr->add(&projectedOrbit);

    PlotStream titleStream(view_ptr->titleTextEdit());
	titleStream << PlotStream::Clear()
				<< PlotStream::Center()
				<< PlotStream::Family("Sans Serif")
				<< PlotStream::Size(24)
				<< ("Projected Orbit (Omega = " + std::to_string(Omega) + ")")
				<< PlotStream::EndP();

	PlotStream xLabelStream(view_ptr->xLabelTextEdit());
	xLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "x"
				 << PlotStream::EndP();

	PlotStream yLabelStream(view_ptr->yLabelTextEdit());
	yLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "y"
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