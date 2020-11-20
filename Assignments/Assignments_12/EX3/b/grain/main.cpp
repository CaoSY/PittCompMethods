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
const std::string coarseResultFile = "../coarse/coarse-result.dat";
const std::string grainResultFile = "grain-result.dat";
const double nBin = 400;
const bool verbose = false;
const int SUCCESS = 3;
const size_t nTrial = 100;
const double wScale = 0.05;
const double nEffTrial = 1 * nTrial;
const double Precision = 32;

// Grain Search
const std::string searchType = "Grain";
const std::string fileExt = searchType + ".png";
const double pNBins = 100;

double fMin = 0.66;
double fMax = 0.70;
double mu1Min = 99;
double mu1Max = 106;
double mu2Min = 104;
double mu2Max = 105;
double gamma1Min = 0.18;
double gamma1Max = 0.28;
double gamma2Min = 0.35;
double gamma2Max = 0.41;

void initializeParameter()
{
    std::ifstream iFile(coarseResultFile);
    double mu1, mu2, gamma1, gamma2, f;
    double mu1Sig, mu2Sig, gamma1Sig, gamma2Sig, fSig;

    iFile >> mu1 >> mu1Sig;
    iFile >> mu2 >> mu2Sig;
    iFile >> gamma1 >> gamma1Sig;
    iFile >> gamma2 >> gamma2Sig;
    iFile >> f >> fSig;

    fMin = f - fSig; fMax = f + fSig;
    mu1Min = mu1 - mu1Sig; mu1Max = mu1 + mu1Sig;
    mu2Min = mu2 - mu2Sig; mu2Max = mu2 + mu2Sig;
    gamma1Min = gamma1 - gamma1Sig; gamma1Max = gamma1 + gamma1Sig;
    gamma2Min = gamma2 - gamma2Sig; gamma2Max = gamma2 + gamma2Sig;
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
    Genfun::Parameter pMu1("Mu1", 80, dataHist.min(), dataHist.max());
    Genfun::Parameter pMu2("Mu2", 120, dataHist.min(), dataHist.max());
    Genfun::Parameter pGamma1("Gamma1", 0.5, 0, 10*std::sqrt(dataHist.variance()));
    Genfun::Parameter pGamma2("Gamma2", 0.5, 0, 10*std::sqrt(dataHist.variance()));
    Genfun::Parameter pF("fraction", 0.5, 0, 1);

    Genfun::GENFUNCTION LorentzPDF1 = 1 / (M_PI * pGamma1 * (1 + qPow2((X - pMu1)/pGamma1)));
    Genfun::GENFUNCTION LorentzPDF2 = 1 / (M_PI * pGamma2 * (1 + qPow2((X - pMu2)/pGamma2)));
    Genfun::GENFUNCTION Lorentz = dataHist.sum() * (pF * LorentzPDF1 + (1-pF) * LorentzPDF2);

    std::cout << searchType + " Search" << std::endl;

    std::random_device rndDev;
    std::mt19937_64 rndEng(rndDev());
    std::uniform_real_distribution<double> mu1Dist(mu1Min, mu1Max);
    std::uniform_real_distribution<double> mu2Dist(mu2Min, mu2Max);
    std::uniform_real_distribution<double> gamma1Dist(gamma1Min, gamma1Max);
    std::uniform_real_distribution<double> gamma2Dist(gamma2Min, gamma2Max);
    std::uniform_real_distribution<double> fDist(fMin, fMax);
    Hist1D mu1Hist("Mu1", pNBins, mu1Min, mu1Max);
    Hist1D mu2Hist("Mu2", pNBins, mu2Min, mu2Max);
    Hist1D gamma1Hist("Gamma1", pNBins, gamma1Min, gamma1Max);
    Hist1D gamma2Hist("Gamma2", pNBins, gamma2Min, gamma2Max);
    Hist1D fHist("Fraction", pNBins, fMin, fMax);
    double count = nTrial;
    while (count > 0)
    {
        double mu10 = mu1Dist(rndEng);
        double mu20 = mu2Dist(rndEng);
        double gamma10 = gamma1Dist(rndEng);
        double gamma20 = gamma2Dist(rndEng);
        double f0 = fDist(rndEng);

        pMu1.setValue(mu10);
        pMu2.setValue(mu20);
        pGamma1.setValue(gamma10);
        pGamma2.setValue(gamma20);
        pF.setValue(f0);

        MinuitMinimizer lorentzMinimizer(verbose);
        lorentzMinimizer.addParameter(&pMu1);
        lorentzMinimizer.addParameter(&pMu2);
        lorentzMinimizer.addParameter(&pGamma1);
        lorentzMinimizer.addParameter(&pGamma2);
        lorentzMinimizer.addParameter(&pF);
        lorentzMinimizer.addStatistic(&objFunc, &Lorentz);
        lorentzMinimizer.minimize();
        
        if (lorentzMinimizer.getStatus() == SUCCESS)
        {
            --count;

            Eigen::MatrixXd errMat = lorentzMinimizer.getErrorMatrix();
            double weight = wScale / std::sqrt(errMat.trace());
  
            if (pMu1.getValue() <= pMu2.getValue())
            {
                mu1Hist.accumulate(pMu1.getValue(), weight);
                mu2Hist.accumulate(pMu2.getValue(), weight);
                gamma1Hist.accumulate(pGamma1.getValue(), weight);
                gamma2Hist.accumulate(pGamma2.getValue(), weight);
                fHist.accumulate(pF.getValue(), weight);
            }else
            {
                mu1Hist.accumulate(pMu2.getValue(),weight);
                mu2Hist.accumulate(pMu1.getValue(), weight);
                gamma1Hist.accumulate(pGamma2.getValue(), weight);
                gamma2Hist.accumulate(pGamma1.getValue(), weight);
                fHist.accumulate(1 - pF.getValue(), weight);
            }
        }
    }
    
    std::cout << "Mu1 :" << mu1Hist.mean() << " +/- " << std::sqrt(mu1Hist.variance()) << std::endl;
    std::cout << "Mu2 :" << mu2Hist.mean() << " +/- " << std::sqrt(mu2Hist.variance()) << std::endl;
    std::cout << "Gamma1 :" << gamma1Hist.mean() << " +/- " << std::sqrt(gamma1Hist.variance()) << std::endl;
    std::cout << "Gamma2 :" << gamma2Hist.mean() << " +/- " << std::sqrt(gamma2Hist.variance()) << std::endl;
    std::cout << "Fraction :" << fHist.mean() << " +/- " << std::sqrt(fHist.variance()) << std::endl;

    std::ofstream oFile(grainResultFile);
    oFile << std::setprecision(Precision) << mu1Hist.mean() << "\t";
    oFile << std::setprecision(Precision) << std::sqrt(mu1Hist.variance()) << std::endl;
    oFile << std::setprecision(Precision) << mu2Hist.mean() << "\t";
    oFile << std::setprecision(Precision) << std::sqrt(mu2Hist.variance()) << std::endl;
    oFile << std::setprecision(Precision) << gamma1Hist.mean() << "\t";
    oFile << std::setprecision(Precision) << std::sqrt(gamma1Hist.variance()) << std::endl;
    oFile << std::setprecision(Precision) << gamma2Hist.mean() << "\t";
    oFile << std::setprecision(Precision) << std::sqrt(gamma2Hist.variance()) << std::endl;
    oFile << std::setprecision(Precision) << fHist.mean() << "\t";
    oFile << std::setprecision(Precision) << std::sqrt(fHist.variance()) << std::endl;
    oFile.close();

    pMu1.setValue(mu1Hist.mean());
    pMu2.setValue(mu2Hist.mean());
    pGamma1.setValue(gamma1Hist.mean());
    pGamma2.setValue(gamma2Hist.mean());
    pF.setValue(fHist.mean());

    
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

    PlotView *mu1View = createPlotView("Mu1", "", "", PRectF(mu1Min, mu2Max, 0, nEffTrial));
    PlotHist1D mu1Plot(mu1Hist);
    mu1View->add(&mu1Plot);
    window.add(mu1View, "Mu1");
    mu1View->save("Mu1" + fileExt);

    PlotView *mu2View = createPlotView("Mu2", "", "", PRectF(mu2Min, mu2Max, 0, nEffTrial));
    PlotHist1D mu2Plot(mu2Hist);
    mu2View->add(&mu2Plot);
    window.add(mu2View, "Mu2");
    mu2View->save("Mu2" + fileExt);

    PlotView *gamma1View = createPlotView("Gamma1", "", "", PRectF(gamma1Min, gamma2Max, 0, nEffTrial));
    PlotHist1D gamma1Plot(gamma1Hist);
    gamma1View->add(&gamma1Plot);
    window.add(gamma1View, "Gamma1");
    gamma1View->save("Gamma1" + fileExt);
    
    PlotView *gamma2View = createPlotView("Gamma2", "", "", PRectF(gamma2Min, gamma2Max, 0, nEffTrial));
    PlotHist1D gamma2Plot(gamma2Hist);
    gamma2View->add(&gamma2Plot);
    window.add(gamma2View, "Gamma2");
    gamma2View->save("Gamma2" + fileExt);

    PlotView *fView = createPlotView("Fraction", "", "", PRectF(fMin, fMax, 0, nEffTrial));
    PlotHist1D fPlot(fHist);
    fView->add(&fPlot);
    window.add(fView, "Fraction");
    fView->save("Fraction" + fileExt);

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