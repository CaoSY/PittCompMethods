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

PlotView *createPlotView(const std::string& title, const std::string& xLabel, const std::string yLabel, const PRectF& viewRange);

typedef std::vector<double> Data;
typedef std::vector<double> XList;
typedef std::vector<double> YList;

Data readData(const std::string& file);
Hist1D binData(const Data& data);
Table tableData(const Data& data);

const std::string dataFileName = "data00.dat";
const double nBin = 400;
const bool verbose = true;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QMainWindow window;

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
    window.setCentralWidget(view);

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
    Genfun::Parameter pMuL("Mu", dataHist.mean(), dataHist.min(), dataHist.max());
    Genfun::Parameter pGammaL("Gamma", std::sqrt(dataHist.variance()), 0, 10*std::sqrt(dataHist.variance()));

    Genfun::GENFUNCTION LorentzPDF = 1 / (M_PI * pGammaL * (1 + qPow2((X - pMuL)/pGammaL)));
    Genfun::GENFUNCTION Lorentz = dataHist.sum() * LorentzPDF;

    std::cout << "Lorentz Fit" << std::endl;
    MinuitMinimizer lorentzMinimizer(verbose);
    lorentzMinimizer.addParameter(&pMuL);
    lorentzMinimizer.addParameter(&pGammaL);
    lorentzMinimizer.addStatistic(&objFunc, &Lorentz);
    lorentzMinimizer.minimize();
    std::cout << std::endl;

    PlotFunction1D LorentzPlot(Lorentz);
    {
        PlotFunction1D::Properties prop;
        prop.pen.setWidth(2);
        prop.pen.setColor(Qt::red);
        LorentzPlot.setProperties(prop);
    }
    view->add(&LorentzPlot);
    legend.add(&LorentzPlot, "Lorentz");


//  Gauss Fit
    Genfun::Parameter pMuG("Mu", dataHist.mean(), dataHist.min(), dataHist.max());
    Genfun::Parameter pSigmaG("Sigma", std::sqrt(dataHist.variance()), 0, std::sqrt(dataHist.variance()));

    Genfun::GENFUNCTION GaussPDF = (1/std::sqrt(2*M_PI))/pSigmaG * qExp(-0.5 * qPow2((X-pMuG)/pSigmaG));
    Genfun::GENFUNCTION Gauss = dataHist.sum() * GaussPDF;

    std::cout << "Gauss Fit" << std::endl;
    MinuitMinimizer gaussMinimizer(verbose);
    gaussMinimizer.addParameter(&pMuG);
    gaussMinimizer.addParameter(&pSigmaG);
    gaussMinimizer.addStatistic(&objFunc, &Gauss);
    gaussMinimizer.minimize();
    std::cout << std::endl;

    PlotFunction1D GaussPlot(Gauss);
    {
        PlotFunction1D::Properties prop;
        prop.pen.setWidth(2);
        prop.pen.setColor(Qt::blue);
        GaussPlot.setProperties(prop);
    }
    view->add(&GaussPlot);
    legend.add(&GaussPlot, "Gauss");


	QToolBar *toolBar = window.addToolBar("Tools");
	
    QAction *quitAction = toolBar->addAction("Quit (q)");
    quitAction->setShortcut(QKeySequence("q"));
	QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));
    
    QAction *saveAction = toolBar->addAction("Save as (s)");
    saveAction->setShortcut(QKeySequence("s"));
    QObject::connect(saveAction, SIGNAL(triggered()), view, SLOT(save()));

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
    
    Hist1D dataHist(nBin, std::floor(dataMin-1), std::ceil(dataMax+1));
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