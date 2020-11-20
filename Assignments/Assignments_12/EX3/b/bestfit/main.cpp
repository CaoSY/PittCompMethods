#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/Parameter.h>
#include <QatGenericFunctions/Square.h>
#include <QatGenericFunctions/Sqrt.h>
#include <QatGenericFunctions/Exp.h>
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

PlotView *createPlotView(const std::string& title, const std::string& xLabel, const std::string yLabel, const PRectF& viewRange);

typedef std::vector<double> Data;
typedef std::vector<double> XList;
typedef std::vector<double> YList;

Data readData(const std::string& file);
Hist1D binData(const Data& data);
Table tableData(const Data& data);

const std::string dataFileName = "../data01.dat";
const std::string grainResultFile = "../grain/grain-result.dat";
const double nBin = 1000;
const bool verbose = true;
const int SUCCESS = 3;
const size_t nTrial = 100;
const double wScale = 0.05;
const double nEffTrial = 1 * nTrial;

double mu1, mu2, gamma1, gamma2, f;
double mu1Sig, mu2Sig, gamma1Sig, gamma2Sig, fSig;
const std::string fileExt = "BestFit.png";

void initializeParameter()
{
    std::ifstream iFile(grainResultFile);

    iFile >> mu1 >> mu1Sig;
    iFile >> mu2 >> mu2Sig;
    iFile >> gamma1 >> gamma1Sig;
    iFile >> gamma2 >> gamma2Sig;
    iFile >> f >> fSig;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MultipleViewWindow window;

    initializeParameter();

    Genfun::Square qPow2;
    Genfun::Sqrt qSqrt;
    Genfun::Exp qExp;

    const Data data = readData(dataFileName);
    Hist1D dataHist = binData(data);
    Table dataTable = tableData(data);
    TableLikelihoodFunctional objFunc(dataTable);
    Genfun::Variable X = dataTable.symbol("E");

//  Prepare View
    const double viewXMin = dataHist.binLowerEdge(0) - 10;
    const double viewXMax = dataHist.binUpperEdge(nBin-1) + 10;
    const double viewYMin = 1e-2;
    const double viewYMax = dataHist.maxContents() * 3;
    PRectF viewRange(viewXMin, viewXMax, viewYMin, viewYMax);
    PlotView *view = createPlotView("Spectrum", "Energy / eV", "", viewRange);
    view->setLogY(true);
    window.add(view, "Fit");

    PlotKey legend(viewXMin, viewYMax);
    legend.setFont(QFont("Times", 25, QFont::Bold));
    view->add(&legend);

// Plot Data
    PlotHist1D dataHistPlot(dataHist);
    {
        PlotHist1D::Properties prop;
        prop.plotStyle = PlotHist1D::Properties::SYMBOLS;
        prop.symbolSize = 10;
        prop.pen.setWidth(1.9);
        dataHistPlot.setProperties(prop);
    }
    view->add(&dataHistPlot);
    legend.add(&dataHistPlot, "Data");


//  Lorentz Fit
    Genfun::Parameter pMu1("Mu1", mu1, dataHist.min(), dataHist.max());
    Genfun::Parameter pMu2("Mu2", mu2, dataHist.min(), dataHist.max());
    Genfun::Parameter pGamma1("Gamma1", gamma1, 0, 10*std::sqrt(dataHist.variance()));
    Genfun::Parameter pGamma2("Gamma2", gamma2, 0, 10*std::sqrt(dataHist.variance()));
    Genfun::Parameter pF("fraction", f, 0, 1);

    Genfun::GENFUNCTION LorentzPDF1 = 1 / (M_PI * pGamma1 * (1 + qPow2((X - pMu1)/pGamma1)));
    Genfun::GENFUNCTION LorentzPDF2 = 1 / (M_PI * pGamma2 * (1 + qPow2((X - pMu2)/pGamma2)));
    Genfun::GENFUNCTION Lorentz = dataHist.sum() * dataHist.binWidth() * (pF * LorentzPDF1 + (1-pF) * LorentzPDF2);

    MinuitMinimizer lorentzMinimizer(verbose);
    lorentzMinimizer.addParameter(&pMu1);
    lorentzMinimizer.addParameter(&pMu2);
    lorentzMinimizer.addParameter(&pGamma1);
    lorentzMinimizer.addParameter(&pGamma2);
    lorentzMinimizer.addParameter(&pF);
    lorentzMinimizer.addStatistic(&objFunc, &Lorentz);
    lorentzMinimizer.minimize();

    
    PlotFunction1D LorentzPlot(Lorentz);
    {
        PlotFunction1D::Properties prop;
        prop.pen.setWidth(2);
        prop.pen.setColor(Qt::red);
        LorentzPlot.setProperties(prop);
    }
    view->add(&LorentzPlot);
    legend.add(&LorentzPlot, "Lorentz");
    view->save("Lorentz" + fileExt);

/*
	QToolBar *toolBar = window.addToolBar("Tools");
	
    QAction *quitAction = toolBar->addAction("Quit (q)");
    quitAction->setShortcut(QKeySequence("q"));
	QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));
    
    QAction *saveAction = toolBar->addAction("Save as (s)");
    saveAction->setShortcut(QKeySequence("s"));
    QObject::connect(saveAction, SIGNAL(triggered()), view, SLOT(save()));
*/
	window.show();
	app.exec();

    return 0;
}


//////////////////////////////////////////////////////////////////////////////

Data readData(const std::string& file)
{
    std::ifstream dataFile(file);
    double dataIn;
    Data data;

    while (dataFile >> dataIn)
    {
        data.push_back(dataIn);
    }
    
    return data;
}

Hist1D binData(const Data& data)
{
    std::pair<Data::const_iterator,Data::const_iterator> MIN_MAX_Iter = std::minmax_element(data.begin(), data.end());
    double dataMin = *(MIN_MAX_Iter.first);
    double dataMax = *(MIN_MAX_Iter.second);

    std::cout << "Data Num: " << data.size() << std::endl;
    std::cout << "Data Min: " << dataMin << std::endl;
    std::cout << "Data Max: " << dataMax << std::endl;
    
    Hist1D dataHist(nBin, 90, 110);
    //Hist1D dataHist(100, 90, 110);
    for (size_t i = 0; i < data.size(); i++)
    {
        dataHist.accumulate(data[i]);
    }
    
    std::cout << "Data Mean: " << dataHist.mean() << std::endl;
    std::cout << "Data Variance: " << dataHist.variance() << std::endl;
    std::cout << "Data Sigma: " << std::sqrt(dataHist.variance()) << std::endl;
    std::cout << std::endl;

    return dataHist;
}

Table tableData(const Data& data)
{
    Table dataTable("Data Table");

    for (size_t i = 0; i < data.size(); i++)
    {
        dataTable.add("E", data[i]);
        dataTable.capture();
    }
    
    return dataTable;
}


////////////////////////////////////////////////////////////////////////////////

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