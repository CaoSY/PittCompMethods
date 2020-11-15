#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/Sin.h>
#include <QatGenericFunctions/Cos.h>
#include <QatGenericFunctions/Exp.h>
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
#include <QatPlotting/PlotProfile.h>
#include <QAction>
#include <QToolBar>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <utility>

typedef std::shared_ptr<PlotProfile>  PlotProfile_Ptr;

class ErrorPlots
{
    public:
        PlotProfile_Ptr Xh, Etah, Xtol, Etatol;
        ErrorPlots(PlotProfile_Ptr _Xh, PlotProfile_Ptr _Etah, PlotProfile_Ptr _Xtol, PlotProfile_Ptr _Etatol)
        : Xh(_Xh), Etah(_Etah), Xtol(_Xtol), Etatol(_Etatol){}
};

const double zeta = 1.0 / 2.0;
const double omega = std::sqrt(3) / 2;
const double lambda = 2;
const double phi = M_PI / 6;
const double X0 = 1;
const double Eta0 = 1;
const double tauEnd = 10;
const double hMax = 1;
const double hMin = 1e-4;
const double errXhMax = 10;
const double errXhMin = 1e-16;
const double errEtahMax = 10;
const double errEtahMin = 1e-16;
const double tolMax = 1;
const double tolMin = 1e-8;
const double errXtolMax = 10;
const double errXtolMin = 1e-9;
const double errEtatolMax = 10;
const double errEtatolMin = 1e-9;


Genfun::Sin qSin;
Genfun::Exp qExp;

Genfun::Variable tau;
Genfun::GENFUNCTION xA = 2 * qExp(-tau/lambda) * qSin(phi + omega * tau);
Genfun::GENFUNCTION etaA = 2 * qExp(-tau/lambda) * qSin(phi - omega * tau);

Genfun::Variable X(0,2), Eta(1,2);
Genfun::GENFUNCTION DXDt = Eta;
Genfun::GENFUNCTION DEtaDt = -2 * zeta * Eta - X;

PlotView *createPlotView(const std::string& title, const std::string& xLabel, const std::string yLabel, const PRectF& viewRange);

ErrorPlots plotError(const Genfun::ButcherTableau & tableau, PlotProfile::Properties::SymbolStyle symbolShape)
{
    PlotProfile_Ptr XhPlot(new PlotProfile);
    PlotProfile_Ptr EtahPlot(new PlotProfile);

    for (double h = hMax; h >= hMin; h /= 10)
    {
        Genfun::SimpleRKStepper stepper(tableau, h);
        Genfun::RKIntegrator integrator(&stepper);

        integrator.addDiffEquation(&DXDt, "X", X0);
        integrator.addDiffEquation(&DEtaDt, "Eta", Eta0);

        Genfun::GENFUNCTION xN = *integrator.getFunction(X);
        Genfun::GENFUNCTION etaN = *integrator.getFunction(Eta);

        XhPlot->addPoint(h, std::abs(xA(tauEnd) - xN(tauEnd)));
        EtahPlot->addPoint(h, std::abs(etaA(tauEnd) - etaN(tauEnd)));
    }
    
    PlotProfile_Ptr XtolPlot(new PlotProfile);
    PlotProfile_Ptr EtatolPlot(new PlotProfile);

    for (double tol = tolMax; tol >= tolMin; tol /= 10)
    {
        Genfun::StepDoublingRKStepper SDstepper(tableau);
        Genfun::AdaptiveRKStepper stepper(&SDstepper);
        stepper.tolerance() = tol;
        Genfun::RKIntegrator integrator(&stepper);

        integrator.addDiffEquation(&DXDt, "X", X0);
        integrator.addDiffEquation(&DEtaDt, "Eta", Eta0);

        Genfun::GENFUNCTION xN = *integrator.getFunction(X);
        Genfun::GENFUNCTION etaN = *integrator.getFunction(Eta);

        XtolPlot->addPoint(tol, std::abs(xA(tauEnd) - xN(tauEnd)));
        EtatolPlot->addPoint(tol, std::abs(etaA(tauEnd) - etaN(tauEnd)));
    }



    PlotProfile::Properties prop;
    prop.symbolStyle = symbolShape;
    prop.symbolSize = 15;
    prop.pen.setWidth(5);
    XhPlot->setProperties(prop);
    EtahPlot->setProperties(prop);
    XtolPlot->setProperties(prop);
    EtatolPlot->setProperties(prop);

    return ErrorPlots(XhPlot, EtahPlot, XtolPlot, EtatolPlot);
}




int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MultipleViewWindow window;

    ErrorPlots eulerErrhPlots = plotError(Genfun::EulerTableau(), PlotProfile::Properties::CIRCLE);
    ErrorPlots midpointErrhPlots = plotError(Genfun::MidpointTableau(), PlotProfile::Properties::SQUARE);
    ErrorPlots trapezoidErrhPlots = plotError(Genfun::TrapezoidTableau(), PlotProfile::Properties::TRIANGLE_L);
    ErrorPlots classicalRKErrhPlots = plotError(Genfun::ClassicalRungeKuttaTableau(), PlotProfile::Properties::TRIANGLE_U);
    // Euler,Midpoint, Trapezoid, Classical Runge-Kutta
    

    PlotView *XhView = createPlotView("Error of X v.s. h", "h", "Error", PRectF(hMin/10, hMax*10, errXhMin/10, errXhMax*10));
    XhView->add(eulerErrhPlots.Xh.get());
    XhView->add(midpointErrhPlots.Xh.get());
    XhView->add(trapezoidErrhPlots.Xh.get());
    XhView->add(classicalRKErrhPlots.Xh.get());
    window.add(XhView, "Xh");
    
    PlotView *EtahView = createPlotView("Error of Eta v.s. h", "h", "Error", PRectF(hMin/10, hMax*10, errEtahMin/10, errEtahMax*10));
    EtahView->add(eulerErrhPlots.Etah.get());
    EtahView->add(midpointErrhPlots.Etah.get());
    EtahView->add(trapezoidErrhPlots.Etah.get());
    EtahView->add(classicalRKErrhPlots.Etah.get());
    window.add(EtahView, "Etah");

    PlotView *XtolView = createPlotView("Error of X v.s. tolerance", "tolerance", "Error", PRectF(tolMin/10, tolMax*10, errXtolMin/10, errXtolMax*10));
    XtolView->add(eulerErrhPlots.Xtol.get());
    XtolView->add(midpointErrhPlots.Xtol.get());
    XtolView->add(trapezoidErrhPlots.Xtol.get());
    XtolView->add(classicalRKErrhPlots.Xtol.get());
    window.add(XtolView, "Xtol");

    PlotView *EtatolView = createPlotView("Error of Eta v.s. tolerance", "tolerance", "error", PRectF(tolMin/10, tolMax*10,errEtatolMin/10, errEtatolMax*10));
    EtatolView->add(eulerErrhPlots.Etatol.get());
    EtatolView->add(midpointErrhPlots.Etatol.get());
    EtatolView->add(trapezoidErrhPlots.Etatol.get());
    EtatolView->add(classicalRKErrhPlots.Etatol.get());
    window.add(EtatolView, "Etatol");

    XhView->save("Xh.png");
    EtahView->save("Etah.png");
    XtolView->save("Xtol.png");
    EtatolView->save("Etatol.png");

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
    view_ptr->setFixedWidth(1200);
    view_ptr->setFixedHeight(800);
    view_ptr->setLogX(true);
    view_ptr->setLogY(true);

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