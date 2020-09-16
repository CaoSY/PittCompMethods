#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QColor>
#include <QatGenericFunctions/F1D.h>
#include <QatGenericFunctions/F2D.h>
#include <QatPlotting/PlotWave1D.h>
#include <QatPlotWidgets/PlotView.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotStream.h>
#include <wavefunction.h>
#include <iostream>
#include <sstream>

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        std::cout << "Usage: ./ex14 k v" << std::endl;
        exit(0);
    }

    const size_t N = 12;
    double k, v;
    std::istringstream kStream(argv[1]);
    std::istringstream vStream(argv[2]);
    assert(kStream >> k && k > 0 && vStream >> v);
    WaveFunction::initialize(k, v, N);
    

    double VIEW_X_MAX = 4 + std::max(6 / k, double(4));
    double VIEW_X_MIN = - VIEW_X_MAX; 
    const double VIEW_Y_MIN = std::min(-2.1, v*1.1);
    const double VIEW_Y_MAX = std::max(WaveFunction::estimateMaxDensity(), v) * 1.1;

    QApplication app(argc, argv);
    QMainWindow window;

    PlotView *view_ptr = new PlotView(PRectF(VIEW_X_MIN, VIEW_X_MAX, VIEW_Y_MIN, VIEW_Y_MAX));
    PlotFunction1D *barrier_ptr = new PlotFunction1D(Genfun::F1D(WaveFunction::barrier));
    {
        PlotFunction1D::Properties prop;
        prop.pen.setWidth(3);
        prop.pen.setColor(QColor(255,0,0));
        barrier_ptr->setProperties(prop);
    }
    view_ptr->add(barrier_ptr);
    PlotWave1D *real_ptr = new PlotWave1D(Genfun::F2D(WaveFunction::realPsiT));
    {
        PlotWave1D::Properties prop;
        prop.pen.setWidth(3);
        prop.pen.setStyle(Qt::DashLine);
        real_ptr->setProperties(prop);
    }
    view_ptr->add(real_ptr);
    PlotWave1D *imag_ptr = new PlotWave1D(Genfun::F2D(WaveFunction::imagPsiT));
    {
        PlotWave1D::Properties prop;
        prop.pen.setWidth(3);
        prop.pen.setStyle(Qt::DotLine);
        imag_ptr->setProperties(prop);
    }
    view_ptr->add(imag_ptr);
    PlotWave1D *density_ptr = new PlotWave1D(Genfun::F2D(WaveFunction::densityT));
    {
        PlotWave1D::Properties prop;
        prop.pen.setWidth(3);
        prop.pen.setStyle(Qt::SolidLine);
        density_ptr->setProperties(prop);
    }
    view_ptr->add(density_ptr);

    PlotStream titleStream(view_ptr->titleTextEdit());
    titleStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Sans Serif")
                << PlotStream::Size(16)
                << "k=" << k << " v=" << v << ",    Density (solid),    Re(psi) (dash),    Im(psi) (dot),    Barrier (red)"
                << PlotStream::EndP();

    PlotStream xLabelStream(view_ptr->xLabelTextEdit());
    xLabelStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Sans Serif")
                << PlotStream::Size(16)
                << "x"
                << PlotStream::EndP();

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