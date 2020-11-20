#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/Parameter.h>
#include <QatGenericFunctions/Square.h>
#include <QatGenericFunctions/Sqrt.h>
#include <QatGenericFunctions/Exp.h>
#include <QatGenericFunctions/VoigtDistribution.h>
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

Data readData(const std::string& file);
Data refineData(const Data& rawData, const double lowerBound, const double upperBound);
Hist1D binData(const Data& data);
Table tableData(const Data& data);

const std::string dataFileName = "data03.dat";
const bool verbose = true;
const double dataLowerBound = 0;
const double dataUpperBound = 200;
const double nBin = 1000;
const double histogramMin = 0;
const double histogramMax = 200;
const double M_PI4 = M_PI * M_PI * M_PI * M_PI;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QMainWindow window;

    Genfun::Square qPow2;
    Genfun::Sqrt qSqrt;
    Genfun::Exp qExp;

    const Data rawData = readData(dataFileName);
    const Data data = refineData(rawData, dataLowerBound, dataUpperBound);
    Hist1D dataHist = binData(data);
    Table dataTable = tableData(data);
    TableLikelihoodFunctional objFunc(dataTable);
    Genfun::Variable X = dataTable.symbol("E");

//  Prepare View
    const double viewXMin = histogramMin;
    const double viewXMax = histogramMax;
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

// Voigt Fit
    Genfun::Parameter T("T", dataHist.mean()/3, dataHist.mean()/10, dataHist.mean());
    Genfun::GENFUNCTION PlankPDF = (15/M_PI4)/(T*T*T*T) * (X*X*X) / (qExp(X/T) - 1);
    Genfun::GENFUNCTION Plank = dataHist.sum() * dataHist.binWidth() * PlankPDF;

    MinuitMinimizer plankMinimizer(verbose);
    plankMinimizer.addParameter(&T);
    plankMinimizer.addStatistic(&objFunc, &Plank);
    plankMinimizer.minimize();

    PlotFunction1D plankPlot(Plank);
    {
        PlotFunction1D::Properties prop;
        prop.pen.setColor(Qt::red);
        prop.pen.setWidth(2);
        plankPlot.setProperties(prop);
    }
    view->add(&plankPlot);
    legend.add(&plankPlot, "Fit");



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
        if (true)
        {
            data.push_back(dataIn);
        }
    }
    
    std::pair<Data::const_iterator,Data::const_iterator> MIN_MAX = std::minmax_element(data.begin(), data.end());

    std::cout << "Raw Data Min: " << *(MIN_MAX.first) << std::endl;
    std::cout << "Raw Data Max: " << *(MIN_MAX.second) << std::endl;
    std::cout << std::endl;

    return data;
}

Data refineData(const Data& rawData, const double lowerBound, const double upperBound)
{
    std::cout << "Data Selection Range: [" << lowerBound << ", " << upperBound << "]" << std::endl << std::endl;

    Data data;
    for (size_t i = 0; i < rawData.size(); i++)
    {
        if (lowerBound <= rawData[i] && rawData[i] <= upperBound)
        {
            data.push_back(rawData[i]);
        }
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
    
    //Hist1D dataHist(nBin, binMin, binMax);
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