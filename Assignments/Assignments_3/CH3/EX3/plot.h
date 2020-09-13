#ifndef __PLOT_H_
#define __PLOT_H_ 1
#include <QatPlotWidgets/PlotView.h>
#include <QatGenericFunctions/AbsFunction.h>
#include <QString>
#include <string>

namespace Plot {
    const QString AppName();
    const QString AppVersion();
    const QString AppDescription();
    
    void showCMDArgumentParseErrorText(int exitCode = 0);
    
    double gaussianDistribution(double x);

    class FunctionFigure
    {
        private:
            const Genfun::AbsFunction *fun;
            double xMin;
            double xMax;
            double yMin;
            double yMax;
            std::string title;

        public:
            FunctionFigure(const Genfun::AbsFunction *f, const std::string& fname, double xl = -5, double xr = 5, double yl = -2.5, double yt = 2.5);
            FunctionFigure(const FunctionFigure& obj);
            ~FunctionFigure();
            PlotView *plot(bool plotPrime = false);
            const std::string& figureTitle() const;
    };
} // namespace Plot

#endif