#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QFont>
#include <QColor>
#include <QatGenericFunctions/F1D.h>
#include <QatGenericFunctions/CubicSplinePolynomial.h>
#include <QatGenericFunctions/InterpolatingFunction.h>
#include <QatPlotWidgets/PlotView.h>
#include <QatPlotting/Plotable.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotProfile.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotting/PlotKey.h>
#include <wavefunction.h>
#include <iostream>
#include <complex>
#include <string>
#include <sstream>
#include <vector>
#include <limits>

typedef PlotProfile::Properties::SymbolStyle SymbolShape;

const size_t N_MAX = 10;
const double LOG_V0_LIM = 0.9;
const double V0_MIN = 0;
const double V0_MAX = 2;
const double INTERVAL_NUM = 101;
double k = 0;

QColor indexColor(double val)
{
    int h = std::round((val - V0_MIN) * (240/(V0_MAX - V0_MIN)));
    return QColor::fromHsv(h, 255, 255);
}


Plotable *plotTransmission(double V)
{
    Genfun::InterpolatingFunction fitFunc(Genfun::InterpolationType::LINEAR);

    for (size_t i = 0; i <= N_MAX; i++)
    {
        WaveFunction::initialize(k, V, i);
        fitFunc.addPoint(i, WaveFunction::transmission());
        assert(std::abs(WaveFunction::reflection() + WaveFunction::transmission() - 1) <
            std::numeric_limits<float>::epsilon());
    }
    
    PlotFunction1D *plot_ptr = new PlotFunction1D(fitFunc);

    PlotFunction1D::Properties prop;
    prop.pen.setWidth(2);
    prop.pen.setColor(indexColor(V));
    plot_ptr->setProperties(prop);

    return plot_ptr;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    if (argc != 4)
    {
        std::cout << "Usage: ./ex9 k ymin ymax" << std::endl;
        exit(0);
    }

    std::istringstream kStream(argv[1]);
    assert(kStream >> k && k > 0);

    double ymin, ymax;
    std::istringstream yminStream(argv[2]);
    assert(yminStream >> ymin && ymin < 1);
    std::istringstream ymaxStream(argv[3]);
    assert(ymaxStream >> ymax && ymax > 0);

    std::vector<double> vVec = {0, 0.5, 2, 6};

    
    
    PlotView *view_ptr = new PlotView(PRectF(0, 10, ymin, ymax));
    for (size_t i= 0; i <= INTERVAL_NUM; ++i)
    {
        double V = V0_MIN + i * ((V0_MAX - V0_MIN) / INTERVAL_NUM);
        view_ptr->add(plotTransmission(V));
    }
   


    PlotStream titleStream(view_ptr->titleTextEdit());
    titleStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Sans Serif")
                << PlotStream::Size(16)
                << "Transmission (k=" << k << ")"
                << PlotStream::EndP();

    PlotStream xLabelStream(view_ptr->xLabelTextEdit());
    xLabelStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Sans Serif")
                << PlotStream::Size(16)
                << "N"
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