#include <plot.h>
#include <QatPlotWidgets/PlotView.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotStream.h>
#include <QatGenericFunctions/AbsFunction.h>
#include <QString>
#include <QTextStream>
#include <iostream>

namespace Plot {

    const QString AppName() { return "plot"; }

    const QString AppVersion() { return "1.0"; }

    const QString AppDescription()
    {
        QString description_buffer;
        QTextStream description(&description_buffer);

        description << "This program plots six functions and their derivatives.\n";
        description << "\t1. sin(x)\n";
        description << "\t2. sin(5x)\n";
        description << "\t3. sin(x^2\n";
        description << "\t4. e^(-x)sin(x)\n";
        description << "\t5. Associated Legendre Polynomial with n = 3 and m = 2\n";
        description << "\t6. Gaussian Distribution\n";

        return description.readAll();
    }

    void showCMDArgumentParseErrorText(int exitCode)
    {
        std::cout << "One and only one number between 1 and 6 is needed." << std::endl;
        exit(exitCode);
    }

    double gaussianDistribution(double x) { return std::exp(-x*x); }

    // class FunctionFigure

    FunctionFigure::FunctionFigure(const Genfun::AbsFunction *f, const std::string& fname, double xl, double xr, double yl, double yt):fun(f),xMin(xl),xMax(xr),yMin(yl),yMax(yt),title(fname){}

    FunctionFigure::FunctionFigure(const FunctionFigure& obj):fun(obj.fun->clone()),xMin(obj.xMin),xMax(obj.xMax),yMin(obj.yMin),yMax(obj.yMax),title(obj.title){}

    FunctionFigure::~FunctionFigure() { delete (this->fun); }

    const std::string& FunctionFigure::figureTitle() const
    {
        return this->title;
    }

    PlotView *FunctionFigure::plot(bool plotPrime)
    {
        PlotView *view_ptr = new PlotView(PRectF(this->xMin,this->xMax,this->yMin,this->yMax));
        
        PlotFunction1D *funFigure_ptr = new PlotFunction1D(*(this->fun));
        
        {
            PlotFunction1D::Properties prop;
            prop.pen.setWidth(3);
            funFigure_ptr->setProperties(prop);
        }
        view_ptr->add(funFigure_ptr);

        if (plotPrime)
        {
            PlotFunction1D *funPrimeFigure_ptr = new PlotFunction1D(this->fun->prime());
            {
                PlotFunction1D::Properties prop;
                prop.pen.setWidth(3);
                prop.pen.setStyle(Qt::DashLine);
                funPrimeFigure_ptr->setProperties(prop);
            }
            view_ptr->add(funPrimeFigure_ptr);
        }
        
        PlotStream titleStream(view_ptr->titleTextEdit());
        titleStream << PlotStream::Clear()
                    << PlotStream::Center()
                    << PlotStream::Family("Sans Serif")
                    << PlotStream::Size(16)
                    << this->title
                    << PlotStream::EndP();

        PlotStream xLabelStream(view_ptr->xLabelTextEdit());
        xLabelStream << PlotStream::Clear()
                    << PlotStream::Center()
                    << PlotStream::Family("Sans Serif")
                    << PlotStream::Size(16)
                    << "x"
                    << PlotStream::EndP();
        
        PlotStream yLabelStream(view_ptr->yLabelTextEdit());
        yLabelStream << PlotStream::Clear()
                    << PlotStream::Center()
                    << PlotStream::Family("Sans Serif")
                    << PlotStream::Size(16)
                    << "y"
                    << PlotStream::EndP();
        
        return view_ptr;

    }


}   // namespace Plot