#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/Sin.h>
#include <QatGenericFunctions/Cos.h>
#include <QatGenericFunctions/Exp.h>
#include <QatGenericFunctions/Square.h>
#include <QatGenericFunctions/RKIntegrator.h>
#include <QatGenericFunctions/SimpleRKStepper.h>
#include <QatGenericFunctions/ButcherTableau.h>
#include <QatGenericFunctions/AdaptiveRKStepper.h>
#include <QatGenericFunctions/StepDoublingRKStepper.h>
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
#include <vector>
#include <memory>
#include <utility>

const double m1 = 1;
const double m2 = 2;
const double l1 = 2;
const double l2 = 1;
const double g = 1;
const double P0 = 0;
const double Theta1I0 = M_PI / 12;
const double Theta2I0 = M_PI / 12;
const double MIN_I = -M_PI/12 * 1.5;
const double MAX_I = M_PI/12 * 1.5;
const double tStart_I = 0;
const double tEnd_I = 105;
const double Theta1II0 = M_PI;
const double Theta2II0 = M_PI;
const double MIN_II = -1.5 * M_PI;
const double MAX_II = 1.5 * M_PI;
const double tStart_II = 0;
const double tEnd_II_1 = 100;
const double tEnd_II_2 = 105;

PlotView *createPlotView(const std::string& title, const std::string& xLabel, const std::string yLabel, const PRectF& viewRange);



int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MultipleViewWindow window;

    Genfun::Sin qSin;
    Genfun::Cos qCos;
    Genfun::Square qPow2;

    Genfun::Variable Theta1(0,4);
    Genfun::Variable Theta2(1,4);
    Genfun::Variable P1(2,4);
    Genfun::Variable P2(3,4);
    Genfun::GENFUNCTION T = (m2*l2*l2*P1*P1 + (m1+m2)*l1*l1*P2*P2 - 2*m2*l1*l2*P1*P2*qCos(Theta1-Theta2))
                            / (2*m2*l1*l1*l2*l2 * (m1 + m2*qPow2(qSin(Theta1 - Theta2))));
    Genfun::GENFUNCTION V = -(m1+m2)*g*l1*qCos(Theta1) - m2*g*l2*qCos(Theta2);
    Genfun::GENFUNCTION H = T + V;

    Genfun::GENFUNCTION DTheta1Dt = H.partial(P1);
    Genfun::GENFUNCTION DTheta2Dt = H.partial(P2);
    Genfun::GENFUNCTION DP1Dt = -H.partial(Theta1);
    Genfun::GENFUNCTION DP2dt = -H.partial(Theta2);

    Genfun::RKIntegrator integratorI;
    integratorI.addDiffEquation(&DTheta1Dt, "Theta1", Theta1I0);
    integratorI.addDiffEquation(&DTheta2Dt, "Theta2", Theta2I0);
    integratorI.addDiffEquation(&DP1Dt, "P1", P0);
    integratorI.addDiffEquation(&DP2dt, "P2", P0);
    Genfun::GENFUNCTION theta1I = *integratorI.getFunction(Theta1);
    Genfun::GENFUNCTION theta2I = *integratorI.getFunction(Theta2);

    Genfun::RKIntegrator integratorII;
    integratorII.addDiffEquation(&DTheta1Dt, "Theta1", Theta1II0);
    integratorII.addDiffEquation(&DTheta2Dt, "Theta2", Theta2II0);
    integratorII.addDiffEquation(&DP1Dt, "P1", P0);
    integratorII.addDiffEquation(&DP2dt, "P2", P0);
    Genfun::GENFUNCTION theta1II = *integratorII.getFunction(Theta1);
    Genfun::GENFUNCTION theta2II = *integratorII.getFunction(Theta2);

    PlotOrbit orbitI(theta1I, theta2I, tStart_I, tEnd_I);
    std::string orbitI_Title = "Orbit I (t=" + std::to_string(tEnd_I)+")";
    PlotView *orbitViewI = createPlotView(orbitI_Title, "theta1", "theta2", PRectF(MIN_I, MAX_I, MIN_I, MAX_I));
    orbitViewI->add(&orbitI);
    window.add(orbitViewI, "Orbit I");

    PlotOrbit::Properties prop;
    prop.pen.setWidth(5);
    prop.pen.setColor(Qt::red);

    PlotOrbit orbitII_1(theta1II, theta2II, tStart_II, tEnd_II_1);
    orbitII_1.setProperties(prop);
    std::string orbitII_Title_1 = "Orbit II (t=" + std::to_string(tEnd_II_1) + ")";
    PlotView *orbitViewII_1 = createPlotView(orbitII_Title_1, "theta1", "theta2", PRectF(MIN_II, MAX_II, MIN_II, MAX_II));
    orbitViewII_1->add(&orbitII_1);
    window.add(orbitViewII_1, "Orbit II 1");

    PlotOrbit orbitII_2(theta1II, theta2II, tStart_II, tEnd_II_2);
    orbitII_2.setProperties(prop);
    std::string orbitII_Title_2 = "Orbit II (t=" + std::to_string(tEnd_II_2) + ")";
    PlotView *orbitViewII_2 = createPlotView(orbitII_Title_2, "theta1", "theta2", PRectF(MIN_II, MAX_II, MIN_II, MAX_II));
    orbitViewII_2->add(&orbitII_2);
    window.add(orbitViewII_2, "Orbit II 2");

    orbitViewI->save(orbitI_Title + ".png");
    orbitViewII_1->save(orbitII_Title_1 + ".png");
    orbitViewII_2->save(orbitII_Title_2 + ".png");

/*
	QToolBar *toolBar = window.addToolBar("Tools");
	
    QAction *quitAction = toolBar->addAction("Quit (q)");
    quitAction->setShortcut(QKeySequence("q"));
	QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));
    
    QAction *saveAction = toolBar->addAction("Save as (s)");
    saveAction->setShortcut(QKeySequence("s"));
    QObject::connect(saveAction, SIGNAL(triggered()), XhView, SLOT(save()));
*/
	window.show();
	app.exec();


    return 0;
}


PlotView *createPlotView(const std::string& title, const std::string& xLabel, const std::string yLabel, const PRectF& viewRange)
{
    PlotView *view_ptr = new PlotView(viewRange);
    view_ptr->setFixedWidth(800);
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