#include <iostream>
#include <vector>
#include <algorithm>
#include <QatGenericFunctions/GaussIntegrator.h>
#include <QatGenericFunctions/GaussQuadratureRule.h>
#include <QatGenericFunctions/HermitePolynomial.h>
#include <QatGenericFunctions/InterpolatingFunction.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotProfile.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotWidgets/PlotView.h>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>

struct Dispersion
{
	double waveLength;
	double indexOfRefraction;
};

bool compareWaveLength(const Dispersion& a, const Dispersion& b)
{
	return a.waveLength < b.waveLength;
}

bool compareIndexOfRefraction(const Dispersion& a, const Dispersion& b)
{
	return a.indexOfRefraction < b.indexOfRefraction;
}

std::vector<Dispersion> parseTable()
{
	Dispersion input;
	std::vector<Dispersion> table;

	while (std::cin >> input.waveLength >> input.indexOfRefraction)
	{
		table.push_back(input);
	}
	
	std::sort(table.begin(), table.end(), compareWaveLength);

	return table;
}


int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	std::vector<Dispersion> table = parseTable();

	Genfun::InterpolatingFunction interpolation(Genfun::CUBIC_SPLINE);
	PlotProfile profile;

	for (size_t i = 0; i < table.size(); i++)
	{
		interpolation.addPoint(table[i].waveLength, table[i].indexOfRefraction);
		profile.addPoint(table[i].waveLength, table[i].indexOfRefraction);
	}
	
	PlotFunction1D interpolationLine(interpolation);

	{
		PlotFunction1D::Properties prop;
		prop.pen.setWidth(3);
		interpolationLine.setProperties(prop);
	}

	{
		PlotProfile::Properties prop;
		prop.symbolSize = 10;
		prop.pen.setWidth(3);
		profile.setProperties(prop);
	}

	double xMin = 0.9 * std::min_element(table.begin(), table.end(), compareWaveLength)->waveLength;
	double xMax = 1.1 * std::max_element(table.begin(), table.end(), compareWaveLength)->waveLength;
	double yMin = 0.9 * std::min_element(table.begin(), table.end(), compareIndexOfRefraction)->indexOfRefraction;
	double yMax = 1.1 * std::max_element(table.begin(), table.end(), compareIndexOfRefraction)->indexOfRefraction;

	PlotView *view_ptr = new PlotView(PRectF(xMin, xMax, yMin, yMax));
	view_ptr->setFixedWidth(1200);
	view_ptr->setFixedHeight(800);

	view_ptr->add(&interpolationLine);
	view_ptr->add(&profile);

	PlotStream titleStream(view_ptr->titleTextEdit());
	titleStream << PlotStream::Clear()
				<< PlotStream::Center()
				<< PlotStream::Family("Sans Serif")
				<< PlotStream::Size(24)
				<< "Index of Refraction"
				<< PlotStream::EndP();

	PlotStream xLabelStream(view_ptr->xLabelTextEdit());
	xLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "Wave Length / nm"
				 << PlotStream::EndP();

	PlotStream yLabelStream(view_ptr->yLabelTextEdit());
	yLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(24)
				 << "Index of Refraction"
				 << PlotStream::EndP();

	QMainWindow window;
	window.setCentralWidget(view_ptr);

	QToolBar *toolBar = window.addToolBar("Tools");
	
    QAction *quitAction = toolBar->addAction("Quit (q)");
    quitAction->setShortcut(QKeySequence("q"));
	QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));
    
    QAction *saveAction = toolBar->addAction("Save as (s)");
    saveAction->setShortcut(QKeySequence("s"));
    QObject::connect(saveAction, SIGNAL(triggered()), view_ptr, SLOT(save()));

	window.show();
	app.exec();
	return 0;
}