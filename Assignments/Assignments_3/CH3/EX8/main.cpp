#include <eigen3/Eigen/Dense>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QatGenericFunctions/F1D.h>
#include <QatPlotWidgets/PlotView.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotStream.h>
#include <complex>
#include <limits>
#include <cmath>
#include <sstream>
#include <iostream>
#include "./density.h"

typedef std::complex<double> Complex;
const Complex I(0,1);



int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    const double v = 1.5;

    if (argc != 2)
    {
        std::cout << "Usage: ./ex8 ka (ka > 0)" << std::endl;
        exit(0);
    }

    std::istringstream kaArg(argv[1]);
    double ka;
    if (!(kaArg >> ka))
    {
        std::cout << "Usage: ./ex8 ka (ka > 0)" << std::endl;
        exit(0);
    }

    Density::WaveFunction::initialize(ka,v);
    
    Genfun::GENFUNCTION densityFunc = Genfun::F1D(Density::WaveFunction::density);
    Genfun::GENFUNCTION barrierFunc = Genfun::F1D(Density::WaveFunction::barrier);


    double xMax = std::max(6.0, 2 + 6 / ka);
    double xMin = -xMax;
    double yMax = 4.2;
    double yMin = -0.2;
    
    PlotView *view_ptr = new PlotView(PRectF(xMin, xMax, yMin, yMax));

    PlotFunction1D *densityFunc_ptr = new PlotFunction1D(densityFunc);
    {
        PlotFunction1D::Properties prop;
        prop.pen.setWidth(3);
        densityFunc_ptr->setProperties(prop);
    }
    view_ptr->add(densityFunc_ptr);
        
    PlotFunction1D *barrierFunc_ptr = new PlotFunction1D(barrierFunc);
    {
        PlotFunction1D::Properties prop;
        prop.pen.setWidth(3);
        prop.pen.setStyle(Qt::DashLine);
        barrierFunc_ptr->setProperties(prop);
    }
    view_ptr->add(barrierFunc_ptr);
    

    PlotStream titleStream(view_ptr->titleTextEdit());
    titleStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Sans Serif")
                << PlotStream::Size(16)
                << "ka=" << ka << " v=" << v
                << PlotStream::EndP();

    PlotStream xLabelStream(view_ptr->xLabelTextEdit());
    xLabelStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Sans Serif")
                << PlotStream::Size(16)
                << "x"
                << PlotStream::EndP();
    
    
    QMainWindow window;
    window.setCentralWidget(view_ptr);

    QToolBar *toolBar = window.addToolBar("Tools");
    QAction  *quitAction = toolBar->addAction("Quit(q)");
    quitAction->setShortcut(QKeySequence("q"));
    QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));

    QAction *printAction = toolBar->addAction("Print to PDF (p)");
    printAction->setShortcut(QKeySequence("p"));
    QObject::connect(printAction, SIGNAL(triggered()), view_ptr, SLOT(print()));

    QAction *saveAction = toolBar->addAction("Save as SVG (s)");
    saveAction->setShortcut(QKeySequence("s"));
    QObject::connect(saveAction, SIGNAL(triggered()), view_ptr, SLOT(save()));

    window.show();
    app.exec();

    return 0;
}