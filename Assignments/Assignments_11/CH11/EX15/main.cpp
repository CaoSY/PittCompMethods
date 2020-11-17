#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/RKIntegrator.h>
#include <QApplication>
#include <QMainWindow>
#include <QatPlotWidgets/PlotView.h>
#include <QatPlotWidgets/MultipleViewWindow.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QAction>
#include <QToolBar>
#include <cmath>
#include <iostream>
#include <sstream>

const double beta = 0.2;
const double Gamma = 0.1;
const double b = 1e-4;
const double d = 1e-4;
const double alpha = 0.1;
const double v = 1e-3;
const double I0 = 1e-6;
const double N0 = 1;
const double S0 = N0 - I0;
const double E0 = 0;
const double R0 = 0;
const double ratioMax = 1;
const double ratioMin = 1e-8;
const double tStart = 0;
const double tEndDefault = 6000;

PlotView *createPlotView(const std::string& title, const std::string& xLabel, const std::string yLabel, const PRectF& viewRange);



int main(int argc, char **argv)
{
    double tEnd;
    bool argErr = true;
    if (argc == 1)
    {
        tEnd = tEndDefault;
        argErr = false;
    }else if (argc == 2)
    {
        std::stringstream argSStream(argv[1]);
        if (argSStream >> tEnd && tEnd > 0)
        {
            argErr = false;
        }
    }

    if (argErr)
    {
        std::cout << "seir [tEnd]" << std::endl;
        std::cout << "tEnd is a positive number (default: " << tEndDefault << ")" << std::endl;
        exit(0);
    }
    
    

    QApplication app(argc, argv);
    MultipleViewWindow window;

    Genfun::Variable S(0,4), E(1,4), I(2,4), R(3,4);
    Genfun::GENFUNCTION N = S + E + I + R;
    Genfun::GENFUNCTION lambda = beta * I / N;

    Genfun::GENFUNCTION DSDt = b*N + v*R - (d+lambda)*S;
    Genfun::GENFUNCTION DEDt = lambda*S - (d+alpha)*E;
    Genfun::GENFUNCTION DIDt = alpha*E - (d+Gamma)*I;
    Genfun::GENFUNCTION DRDt = Gamma*I - (d+v)*R;

    Genfun::RKIntegrator integrator;
    integrator.addDiffEquation(&DSDt, "S", S0);
    integrator.addDiffEquation(&DEDt, "E", E0);
    integrator.addDiffEquation(&DIDt, "I", I0);
    integrator.addDiffEquation(&DRDt, "R", R0);

    Genfun::GENFUNCTION sInte = *integrator.getFunction(S);
    Genfun::GENFUNCTION eInte = *integrator.getFunction(E);
    Genfun::GENFUNCTION iInte = *integrator.getFunction(I);
    Genfun::GENFUNCTION rInte = *integrator.getFunction(R);
    Genfun::GENFUNCTION nInte = sInte + eInte + iInte + rInte;
    Genfun::GENFUNCTION ratio = iInte / nInte;

    PlotFunction1D ratioPlot(ratio);
    PlotFunction1D::Properties prop;
    prop.pen.setWidth(3);
    ratioPlot.setProperties(prop);

    PlotView *ratioView = createPlotView("Infected Poputation Ratio", "Days", "", PRectF(tStart,tEnd,ratioMin,ratioMax));
    ratioView->setLogY(true);
    ratioView->add(&ratioPlot);
    window.add(ratioView, "Ratio");

    std::cout << "Ratio at " << tEnd << " days: " << ratio(tEnd) << std::endl;

    ratioView->save("ratio.png");

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