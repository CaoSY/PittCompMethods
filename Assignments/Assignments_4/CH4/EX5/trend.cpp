#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QPen>
#include <QatPlotWidgets/PlotView.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotting/PlotProfile.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatGenericFunctions/Variable.h>
#include <QatGenericFunctions/InterpolatingFunction.h>
#include <iostream>
#include <vector>
#include <utility>

class Tag
{
    public:
        double year;
        double price;
        Tag(double y, double p):year(y),price(p) {}
};

std::vector<Tag> parsePrice()
{
    double year, price;
    std::vector<Tag> tags;
    while (std::cin >> year >> price)
    {
        tags.push_back(Tag(year, price));
    }
    return tags;
}

PlotProfile *showPoints(const std::vector<Tag>& tags)
{
	PlotProfile *profile_ptr = new PlotProfile();
	
	for (size_t i = 0; i < tags.size(); i++)
	{
		profile_ptr->addPoint(tags[i].year, tags[i].price);
	}
	
	{
		PlotProfile::Properties prop;
		prop.symbolSize = 10;
		prop.pen.setWidth(5);
		profile_ptr->setProperties(prop);
	}

	return profile_ptr;
}

bool sortByYear(const Tag& a, const Tag& b)
{
    return a.year < b.year;
}

std::pair<double,double> linearFit(const std::vector<Tag>& tags)
{
    double xSum = 0, ySum = 0, xySum = 0, xxSum = 0;
    for (size_t i = 0; i < tags.size(); i++)
    {
        xSum += tags[i].year;
        ySum += tags[i].price;
        xySum += tags[i].year * tags[i].price;
        xxSum += tags[i].year * tags[i].year;
    }
    
    double xBar = xSum / tags.size();
    double yBar = ySum / tags.size();
    double xyBar = xySum / tags.size();
    double xxBar = xxSum / tags.size();

    double k = (xyBar - xBar*yBar) / (xxBar - xBar*xBar);
    double b = yBar - k * xBar;

    return std::pair<double,double>(k,b);
}

PlotFunction1D *showFitLine(Genfun::GENFUNCTION fitFunc, Qt::PenStyle penStyle)
{	
	PlotFunction1D *price_ptr = new PlotFunction1D(fitFunc);
	{
		PlotFunction1D::Properties prop;
		prop.pen.setWidth(3);
        prop.pen.setStyle(penStyle);
		price_ptr->setProperties(prop);
	}
	return price_ptr;
}


int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QMainWindow window;
	QToolBar *toolBar = window.addToolBar("Tools");
	QAction *nextAction = toolBar->addAction("Next");

	nextAction->setShortcut(QKeySequence("n"));
	QObject::connect(nextAction, SIGNAL(triggered()), &app, SLOT(quit()));

    PlotView view(PRectF(2005, 2022, 0, 10000));
    view.setFixedWidth(1000);
	view.setFixedHeight(700);

	window.setCentralWidget(&view);

    ///////////////////////////////////////////////////////////////

    std::vector<Tag> tags = parsePrice();    
    std::sort(tags.begin(), tags.end(), sortByYear);

	Genfun::InterpolatingFunction polynomialFitFucn(Genfun::POLYNOMIAL);
    Genfun::InterpolatingFunction cubicSplineFitFunc(Genfun::CUBIC_SPLINE);
	for (size_t i = 0; i < tags.size(); i++)
	{
        polynomialFitFucn.addPoint(tags[i].year, tags[i].price);
        cubicSplineFitFunc.addPoint(tags[i].year, tags[i].price);
	}

    std::pair<double,double> coefs = linearFit(tags);
    Genfun::Variable X;
    Genfun::GENFUNCTION linearFitFunc = coefs.first * X + coefs.second;

    const int columnWidth = 20;
    std::cout << std::setw(columnWidth) << std::left << "Year";
    std::cout << std::setw(columnWidth) << std::left << "2020";
    std::cout << std::setw(columnWidth) << std::left << "2021";
    std::cout << std::setw(columnWidth) << std::left << "Profit" << std::endl;

    std::cout << std::setw(columnWidth) << std::left << "Polynomical";
    std::cout << std::setw(columnWidth) << std::left << polynomialFitFucn(2020);
    std::cout << std::setw(columnWidth) << std::left << polynomialFitFucn(2021);
    std::cout << std::setw(columnWidth) << std::left << polynomialFitFucn(2021) - polynomialFitFucn(2020) << std::endl;
    
    std::cout << std::setw(columnWidth) << std::left << "Cubic Spline";
    std::cout << std::setw(columnWidth) << std::left << cubicSplineFitFunc(2020);
    std::cout << std::setw(columnWidth) << std::left << cubicSplineFitFunc(2021);
    std::cout << std::setw(columnWidth) << std::left << cubicSplineFitFunc(2021) - cubicSplineFitFunc(2020) << std::endl;

    std::cout << std::setw(columnWidth) << std::left << "Linear Regression";
    std::cout << std::setw(columnWidth) << std::left << linearFitFunc(2020);
    std::cout << std::setw(columnWidth) << std::left << linearFitFunc(2021);
    std::cout << std::setw(columnWidth) << std::left << linearFitFunc(2021) - linearFitFunc(2020) << std::endl;


    view.add(showFitLine(polynomialFitFucn, Qt::SolidLine));
    view.add(showFitLine(cubicSplineFitFunc, Qt::DashLine));
    view.add(showFitLine(linearFitFunc, Qt::DotLine));
    view.add(showPoints(tags));
    

	PlotStream titleStream(view.titleTextEdit());
	titleStream << PlotStream::Clear()
				<< PlotStream::Center()
				<< PlotStream::Family("Sans Serif")
				<< PlotStream::Size(16)
				<< "Price Trend"
				<< PlotStream::EndP();

	PlotStream xLabelStream(view.xLabelTextEdit());
	xLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(16)
				 << "Year"
				 << PlotStream::EndP();

	PlotStream yLabelStream(view.yLabelTextEdit());
	yLabelStream << PlotStream::Clear()
				 << PlotStream::Center()
				 << PlotStream::Family("Sans Serif")
				 << PlotStream::Size(16)
				 << "Price / $"
				 << PlotStream::EndP();

    QAction *saveAction = toolBar->addAction("Save as (s)");
    saveAction->setShortcut(QKeySequence("s"));
    QObject::connect(saveAction, SIGNAL(triggered()), &view, SLOT(save()));

	view.show();
	window.show();
	app.exec();
	return 1;
}
