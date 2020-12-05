#include <QatPlotWidgets/PlotView.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotProfile.h>
#include <Eigen/Dense>
#include <string>

#ifndef __VISUAL_H__
#define __VISUAL_H__ 1

PlotView *createPlotView(const std::string& title, const std::string& xLabel, const std::string yLabel, const PRectF& viewRange);
PlotFunction1D* plotBar(double x, double y, double halfWidth);
PlotFunction1D* plotCurve(Eigen::VectorXd f, Eigen::VectorXd r);

#endif // __VISUAL_H__