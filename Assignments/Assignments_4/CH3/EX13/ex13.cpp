#include <eigen3/Eigen/Dense>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QColor>
#include <Qt>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotWidgets/PlotView.h>
#include <QatGenericFunctions/F1D.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <complex>
#include <algorithm>
#include <numeric>

typedef std::complex<double> Complex;

double plotAve(double t);
double plotM0(double t);
double plotM1(double t);
double plotM2(double t);
double plotM3(double t);
double plotM4(double t);

const double m = 1;
const double k = 1;
const double t0 = std::sqrt(m/k);
const double omega0 = 1 / t0;

const std::vector<double> kVec = {k, 2*k, 2*k, k};
const std::vector<double> mVec = {m, 3*m, 2*m, m, 2*m};
const double totalMass = std::accumulate(mVec.begin(),mVec.end(), 0);
const std::vector<double (*)(double)> plotFuncVec = {plotM0,plotM1,plotM2,plotM3,plotM4};
Eigen::VectorXcd omega;
Eigen::VectorXcd eta;
Eigen::MatrixXcd aMat;

PlotFunction1D *plotTrace(double (*fun_ptr)(double), QColor color)
{
    PlotFunction1D *M_ptr = new PlotFunction1D(Genfun::F1D(fun_ptr));
    PlotFunction1D::Properties prop;
    prop.pen.setWidth(3);
    prop.pen.setColor(color);
    M_ptr->setProperties(prop);
    return M_ptr;
}

int main(int argc, char **argv)
{

    Eigen::MatrixXcd M1_2 = Eigen::MatrixXcd::Zero(5,5);
    Eigen::MatrixXcd M_1_2 = Eigen::MatrixXcd::Zero(5,5);
    for (int i = 0; i < M_1_2.rows(); i++)
    {
        M1_2(i,i) = std::sqrt(mVec[i]);
        M_1_2(i,i) = 1.0/std::sqrt(mVec[i]);
    }

    Eigen::MatrixXcd K = Eigen::MatrixXcd::Zero(5,5); 

    K(0,0) = kVec[0];
    K(0,1) = -kVec[0]; 
    for (int i = 1; i < K.rows()-1; i++)
    {
        K(i,i-1) = -kVec[i-1];
        K(i,i) = kVec[i-1] + kVec[i];
        K(i,i+1) = -kVec[i];
    }
    K(K.rows()-1,K.cols()-2) = -kVec.back();
    K(K.rows()-1,K.cols()-1) = kVec.back();

    Eigen::MatrixXcd Omega2 = M_1_2 * K * M_1_2;

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXcd> eigenSolver(Omega2);

    Eigen::VectorXcd omega2 = eigenSolver.eigenvalues();
    omega = omega2.cwiseSqrt() / omega0;
    Eigen::MatrixXcd bMat = eigenSolver.eigenvectors();

    Eigen::VectorXcd x0(5);
    x0 << -2, 3, 0, -3, 2;
    Eigen::VectorXd v0 = Eigen::VectorXd::Zero(5);

    eta = bMat.transpose()*M1_2*x0 + 
        Complex(0,1)*(bMat.transpose()*M1_2*v0).cwiseQuotient(omega);

    aMat = M_1_2 * bMat;

    QApplication app(argc, argv);
    QMainWindow window;

    PlotView *view_ptr = new PlotView(PRectF(0, 20, -5, 5));
    view_ptr->add(plotTrace(plotAve,Qt::black));
    view_ptr->add(plotTrace(plotM0,Qt::cyan));
    view_ptr->add(plotTrace(plotM1,Qt::green));
    view_ptr->add(plotTrace(plotM2,Qt::blue));
    view_ptr->add(plotTrace(plotM3,Qt::magenta));
    view_ptr->add(plotTrace(plotM4,Qt::red));
    window.setCentralWidget(view_ptr);

    PlotStream titleStream(view_ptr->titleTextEdit());
    titleStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Sans Serif")
                << PlotStream::Size(16)
                << "Position"
                << PlotStream::EndP();

    PlotStream xLabelStream(view_ptr->xLabelTextEdit());
    xLabelStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Sans Serif")
                << PlotStream::Size(16)
                << "t/t0"
                << PlotStream::EndP();

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

double plotAve(double t)
{
    double pos = 0;
    for (size_t i = 0; i < plotFuncVec.size(); ++i)
    {
        pos += mVec[i] * plotFuncVec[i](t);
    }
    return pos/totalMass;
}

double plotM0(double t)
{
    const int ind = 0;
    Complex I(0,1);
    Eigen::VectorXcd tEvo = -I*t*omega;
    tEvo = tEvo.array().exp();
    return (aMat.row(ind) * eta.cwiseProduct(tEvo))(0,0).real();
}

double plotM1(double t)
{
    const int ind = 1;
    Complex I(0,1);
    Eigen::VectorXcd tEvo = -I*t*omega;
    tEvo = tEvo.array().exp();
    return (aMat.row(ind) * eta.cwiseProduct(tEvo))(0,0).real();
}

double plotM2(double t)
{
    const int ind = 2;
    Complex I(0,1);
    Eigen::VectorXcd tEvo = -I*t*omega;
    tEvo = tEvo.array().exp();
    return (aMat.row(ind) * eta.cwiseProduct(tEvo))(0,0).real();
}

double plotM3(double t)
{
    const int ind = 3;
    Complex I(0,1);
    Eigen::VectorXcd tEvo = -I*t*omega;
    tEvo = tEvo.array().exp();
    return (aMat.row(ind) * eta.cwiseProduct(tEvo))(0,0).real();
}

double plotM4(double t)
{
    const int ind = 4;
    Complex I(0,1);
    Eigen::VectorXcd tEvo = -I*t*omega;
    tEvo = tEvo.array().exp();
    return (aMat.row(ind) * eta.cwiseProduct(tEvo))(0,0).real();
}