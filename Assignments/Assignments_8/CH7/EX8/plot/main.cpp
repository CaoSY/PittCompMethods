#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <QatPlotting/PlotHist1D.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/RealArg.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotting/PlotProfile.h>
#include <QatPlotWidgets/PlotView.h>
#include <QatPlotWidgets/MultipleViewWindow.h>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QFont>

std::vector<double> readData(const std::string& fileName)
{
	std::ifstream inFile(fileName);
	double inNum;
	std::vector<double> inVec;
	while (inFile >> inNum)
	{
		inVec.push_back(inNum);
	}
	return inVec;
}

PlotView *plotData(const std::vector<double>& xVec, const std::vector<double>& yVec, const std::string& name)
{
	PlotProfile *profile = new PlotProfile();
	for (size_t i = 0; i < xVec.size(); i++)
	{
		profile->addPoint(xVec[i], yVec[i]);
	}
	PlotProfile::Properties prop;
	prop.pen.setWidth(3);
	profile->setProperties(prop);
	double xMin = -0.5 + *std::min_element(xVec.begin(), xVec.end());
	double xMax = 0.5 + *std::max_element(xVec.begin(), xVec.end());
	double yMin = *std::min_element(yVec.begin(), yVec.end());
	double yMax = *std::max_element(yVec.begin(), yVec.end());
	yMax = yMax>0 ? yMax*1.2 : yMax*0.8;
	yMin = yMin>0 ? yMin*0.8 : yMin*1.2; 

	PlotView *view_ptr = new PlotView(PRectF(xMin, xMax, yMin, yMax));
	view_ptr->add(profile);

	PlotStream titleStream(view_ptr->titleTextEdit());
	titleStream << PlotStream::Clear()
				<< PlotStream::Center()
				<< PlotStream::Family("Sans Serif")
				<< PlotStream::Size(24)
				<< name
				<< PlotStream::EndP();

	PlotStream xLabelStream(view_ptr->xLabelTextEdit());
	xLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "d"
				 << PlotStream::EndP();

	PlotStream yLabelStream(view_ptr->yLabelTextEdit());
	yLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << name
				 << PlotStream::EndP();

	return view_ptr;
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	MultipleViewWindow window;

	std::fstream fileD("../D.txt", std::ios::in);
	std::fstream fileNs("../Ns.txt", std::ios::in);
	std::fstream fileNa("../Na.txt", std::ios::in);
	std::fstream fileEs("../Es.txt", std::ios::in);
	std::fstream fileEa("../Ea.txt", std::ios::in);
	
	std::vector<double> dVec = readData("../D.txt");
	std::vector<double> NsVec = readData("../Ns.txt");
	std::vector<double> NaVec = readData("../Na.txt");
	std::vector<double> EsVec = readData("../Es.txt");
	std::vector<double> EaVec = readData("../Ea.txt");

	
	PlotView *NsViewPtr = plotData(dVec, NsVec, "Ns");
	PlotView *NaViewPtr = plotData(dVec, NaVec, "Na");
	PlotView *EsViewPtr = plotData(dVec, EsVec, "Es");
	PlotView *EaViewPtr = plotData(dVec, EaVec, "Ea");
	
	window.add(NsViewPtr, "Ns");
	window.add(NaViewPtr, "Na");
	window.add(EsViewPtr, "Es");
	window.add(EaViewPtr, "Ea");

	NsViewPtr->save("../Ns.png");
	NaViewPtr->save("../Na.png");
	EsViewPtr->save("../Es.png");
	EaViewPtr->save("../Ea.png");

	window.show();
	app.exec();

	return 0;
}