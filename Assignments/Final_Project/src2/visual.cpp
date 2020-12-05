#include <visual.h>
#include <QatPlotting/PlotStream.h>
#include <QatPlotting/RealArg.h>
#include <QatGenericFunctions/FixedConstant.h>
#include <QatGenericFunctions/InterpolatingFunction.h>

PlotView *createPlotView(const std::string& title, const std::string& xLabel, const std::string yLabel, const PRectF& viewRange)
{
    PlotView *view_ptr = new PlotView(viewRange);
    view_ptr->setFixedWidth(1200);
    view_ptr->setFixedHeight(800);

    PlotStream titleStream(view_ptr->titleTextEdit());
    titleStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Times New Roman")
                << PlotStream::Size(24)
                << title
                << PlotStream::EndP();

    PlotStream xLabelStream(view_ptr->xLabelTextEdit());
    xLabelStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Times New Roman")
                << PlotStream::Size(24)
                << xLabel
                << PlotStream::EndP();

    PlotStream yLabelStream(view_ptr->yLabelTextEdit());
    yLabelStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Times New Roman")
                << PlotStream::Size(24)
                << yLabel
                << PlotStream::EndP();
    
    return view_ptr;
}

PlotFunction1D* plotBar(double x, double y, double halfWidth)
{
    double xLeft = x - halfWidth;
    double xRight = x + halfWidth;

    return new PlotFunction1D(Genfun::FixedConstant(y), RealArg::Gt(xLeft) && RealArg::Lt(xRight));
}

PlotFunction1D* plotCurve(Eigen::VectorXd f, Eigen::VectorXd r)
{
    Genfun::InterpolatingFunction fInter(Genfun::CUBIC_SPLINE);
    for (int i = 0; i < r.size(); i++)
    {
        fInter.addPoint(r(i), f(i));
    }

    return new PlotFunction1D(fInter);
    
/*
    PlotProfile *psiProfile = new PlotProfile();
    for (int i = 0; i < r.size(); i++)
    {
        psiProfile->addPoint(r(i), psi(i));
    }
    return psiProfile;*/
}