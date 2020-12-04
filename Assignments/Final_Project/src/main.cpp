#include <charmonia.h>
#include <visual.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <sstream>
#include <QApplication>
#include <QToolBar>
#include <QAction>
#include <QTextEdit>
#include <QFont>
#include <QatPlotWidgets/MultipleViewWindow.h>
#include <QatPlotWidgets/MultipleViewWidget.h>
#include <QatPlotWidgets/CustomRangeDivider.h>
#include <QatPlotting/PlotProfile.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotting/PlotText.h>

typedef std::unique_ptr<PlotFunction1D> PF1D_Ptr;
typedef std::unique_ptr<PlotProfile> PP_Ptr;
typedef std::unique_ptr<MultipleViewWidget> MVW_Ptr;
typedef std::unique_ptr<PlotText> PT_Ptr;

bool JPCOrder(const Charmonia::State& a, const Charmonia::State& b)
{
    return a.JPC < b.JPC;
}


int main(int argc, char **argv)
{
    const double rMax = 15;
    const int matrixSize = 500;
    const double halfWidth = 0.25;

    std::vector<Charmonia::State> states = {
        {1, 0, 0, 0, 2983.4,  "0-+", "ηc(1S)"},
        {1, 0, 1, 1, 3096.9,  "1--", "J/ψ(1S)"},
        {2, 0, 0, 0, 3639.2,  "0-+", "ηc(2S)"},
        {2, 0, 1, 1, 3686.10, "1--", "ψ(2S)"},
        {1, 2, 1, 1, 3770,    "1--", "ψ(3770)"},
        {3, 0, 1, 1, 4040,    "1--", "ψ(4040)"},
        {2, 2, 1, 1, 4160,    "1--", "ψ(4160)"},
        {1, 1, 1, 0, 3414.75, "0++", "χc0(1P)"},
        {1, 1, 1, 1, 3510.66, "1++", "χc1(1P)"},
        {1, 1, 1, 2, 3556.20, "2++", "χc2(1P)"},
        {1, 1, 0, 1, 3525.38, "1+-", "hc(1P)"},
        {2, 1, 1, 0, 3860,    "0++", "χc0(2P)"},
        {2, 1, 1, 1, 3871.69, "1++", "χc1(2P)"},
        {2, 1, 1, 0, 3927.20, "2++", "χc2(2P)"},
        {1, 2, 1, 2, 3822.2,  "2--", "ψ(3823)"},
        {4, 0, 1, 1, 4421,    "1--", "ψ(4415)"},
    };

    std::vector<Charmonia> charmStates;
    for (size_t i = 0; i < states.size(); i++)
    {
        charmStates.push_back(Charmonia(states[i], matrixSize, rMax));
    }

    for (size_t i = 0; i < charmStates.size(); i++)
    {
        std::cout << "(N,L,S,J)=(" << states[i].N << ","
                << states[i].L << ","
                << states[i].S << ","
                << states[i].J << ",) "
                << "mass=" << std::setprecision(5) << std::setw(7) << charmStates[i].Mass() << "MeV, "
                << "fs=" << std::setprecision(5) << std::setw(7) << charmStates[i].fsCorrection() << "MeV, "
                << "err=" << std::setprecision(5) << std::setw(7) << std::abs(charmStates[i].Mass() - states[i].mass) << "MeV"
                << std::endl;
    }

    QApplication app(argc, argv);
    MultipleViewWindow window;

    QToolBar *toolBar=window.addToolBar("Tools");
    QAction  *quitAction=toolBar->addAction("Quit");
    quitAction->setShortcut(QKeySequence("q"));
    QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));

    std::vector<std::string> rowLabels = {"0-+", "1--", "1+-", "0++", "1++", "2++", "2--"};
    CustomRangeDivider xDivider;
    QTextEdit edit;
    PlotStream xStream(&edit);
    xStream << PlotStream::Size(24) << PlotStream::Family("Times New Roman")
            << "J" 
            << PlotStream::Super() << "PC" << PlotStream::Normal() 
            << "=" << PlotStream::EndP();

    xDivider.add(-0.5, edit.document()->clone());

    for (size_t i = 0; i < rowLabels.size(); i++)
    {
        xStream << PlotStream::Clear()
            << rowLabels[i].substr(0,1)
            << PlotStream::Super() << rowLabels[i].substr(1,2) << PlotStream::Normal()
            << PlotStream::EndP();
        xDivider.add(i, edit.document()->clone());
    }

    std::vector<PF1D_Ptr> experLevels;
    std::vector<PT_Ptr> stateTexts;
    for (size_t i = 0; i < states.size(); i++)
    {
        size_t rowNum = std::find(rowLabels.begin(), rowLabels.end(), states[i].JPC) - rowLabels.begin();
        if (rowNum < rowLabels.size())
        {
            experLevels.push_back(PF1D_Ptr(plotBar(rowNum, states[i].mass, halfWidth)));
            {
                PlotFunction1D::Properties prop;
                prop.pen.setStyle(Qt::DashLine);
                prop.pen.setWidth(2);
                experLevels.back()->setProperties(prop);
            }

            stateTexts.push_back(PT_Ptr(new PlotText(rowNum+halfWidth, states[i].mass+50, states[i].name)));
            stateTexts.back()->setFont(QFont("Times New Roman", 14));
        }
    }
    PlotView *levelView = createPlotView("Levels", "", "Mass/GeV", PRectF(-0.5, rowLabels.size(), 2800, 4500));
    for (size_t i = 0; i < states.size(); i++)
    {
        levelView->add(experLevels[i].get());
        levelView->add(stateTexts[i].get());
    }
    levelView->setXRangeDivider(&xDivider);
    window.add(levelView, "Levels");
    
    PlotProfile compLevels;
    for (size_t i = 0; i < charmStates.size(); i++)
    {
        size_t rowNum = std::find(rowLabels.begin(), rowLabels.end(), charmStates[i].JPC()) - rowLabels.begin(); 
        if (rowNum < rowLabels.size())
        {
            compLevels.addPoint(rowNum, charmStates[i].Mass());
        }
    }
    {
        PlotProfile::Properties prop;
        prop.pen.setColor(Qt::red);
        prop.pen.setWidth(8);
        prop.symbolSize = 8;
        compLevels.setProperties(prop);
    }
    levelView->add(&compLevels);


    std::vector<MVW_Ptr> curveList;
    for (size_t i = 0; i < charmStates.size(); i++)
    {
        curveList.push_back(MVW_Ptr(new MultipleViewWidget()));

        PlotFunction1D *psiCurve = plotCurve(charmStates[i].Psi(), charmStates[i].R());
        PlotFunction1D *vCurve = plotCurve(charmStates[i].Potential(), charmStates[i].R());
        PlotFunction1D::Properties prop;
        prop.pen.setWidth(2);
        psiCurve->setProperties(prop);
        vCurve->setProperties(prop);
        
        std::ostringstream label;
        label << "N=" << states[i].N << "/L=" << states[i].L << "/S=" << states[i].S << "/J=" << states[i].J;

        PlotView *psiView = createPlotView(label.str(), "r/GeV^-1", "psi", PRectF(0, rMax, -1, 1));
        psiView->add(psiCurve);

        PlotView *vView = createPlotView(label.str(), "r/GeV^-1", "V", PRectF(0, rMax, -10, 10));
        vView->add(vCurve);

        curveList.back()->add(psiView, "Psi");
        curveList.back()->add(vView, "Potential");

        window.add(curveList.back().get(), label.str());
    }

    levelView->save("levels.png");

    window.show();
    app.exec();
    return 0;
}