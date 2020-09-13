#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QStringList>
#include <QatPlotWidgets/PlotView.h>
#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/Sin.h>
#include <QatGenericFunctions/Exp.h>
#include <QatGenericFunctions/AssociatedLegendre.h>
#include <QatGenericFunctions/F1D.h>
#include <vector>
#include <iostream>
#include <plot.h>


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QApplication::setApplicationName(Plot::AppName());
    QApplication::setApplicationVersion(Plot::AppVersion());

    QCommandLineParser cmdlineParser;
    cmdlineParser.setApplicationDescription(Plot::AppDescription());
    cmdlineParser.addHelpOption();
    cmdlineParser.addVersionOption();
    cmdlineParser.addPositionalArgument("functionNumber", "Serial number of the function to be plotted.");

    QCommandLineOption primeOption({"p", "prime"}, "flag to plot derivative of the chosen function.");
    cmdlineParser.addOption(primeOption);

    cmdlineParser.process(app);

    // Sanity Check
    const QStringList& positionalArgs = cmdlineParser.positionalArguments();
    if (positionalArgs.size() != 1)
    {
        Plot::showCMDArgumentParseErrorText();
    }

    bool parseOK;
    int funNum = positionalArgs.constFirst().toInt(&parseOK);
    if (!parseOK || !(1 <= funNum && funNum <=6))
    {
        Plot::showCMDArgumentParseErrorText();
    }
    
    std::vector<Plot::FunctionFigure> funcVec;
    Genfun::Variable X;
    funcVec.push_back(Plot::FunctionFigure(Genfun::Sin()(X).clone(), "Sin(x)", -5, 5, -1.2, 1.2));
    funcVec.push_back(Plot::FunctionFigure(Genfun::Sin()(5 * X).clone(), "Sin(5x)", -3, 3, -6, 6));
    funcVec.push_back(Plot::FunctionFigure(Genfun::Sin()(X * X).clone(), "Sin(x^2)", -3, 3, -6, 6));
    funcVec.push_back(Plot::FunctionFigure((Genfun::Exp()(-X) * Genfun::Sin()(X)).clone(), "e^(-x)*Sin(x)", -2, 10, -6, 6));
    funcVec.push_back(Plot::FunctionFigure(Genfun::AssociatedLegendre(3,2)(X).clone(), "P_3^2 (x)", -0.99, 0.99, -20, 20));
    funcVec.push_back(Plot::FunctionFigure(Genfun::F1D(Plot::gaussianDistribution).clone(), "e^(-x^2)", -3, 3, -1.2, 1.2));
    
    PlotView *fiugreView_ptr = funcVec[funNum - 1].plot(cmdlineParser.isSet(primeOption));

    QMainWindow window;
    window.setCentralWidget(funcVec[funNum - 1].plot(cmdlineParser.isSet(primeOption)));

    QToolBar *toolBar = window.addToolBar("Tools");
    QAction  *quitAction = toolBar->addAction("Quit(q)");
    quitAction->setShortcut(QKeySequence("q"));
    QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));

    QAction *printAction = toolBar->addAction("Print to PDF (p)");
    printAction->setShortcut(QKeySequence("p"));
    QObject::connect(printAction, SIGNAL(triggered()), fiugreView_ptr, SLOT(print()));

    QAction *saveAction = toolBar->addAction("Save as SVG (s)");
    saveAction->setShortcut(QKeySequence("s"));
    QObject::connect(saveAction, SIGNAL(triggered()), fiugreView_ptr, SLOT(save()));


    window.show();
    app.exec();

    return 0;
}