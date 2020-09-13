#include <eigen3/Eigen/Dense>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QatGenericFunctions/F1D.h>
#include <QatPlotWidgets/PlotView.h>
#include <QatPlotting/PlotFunction1D.h>
#include <QatPlotting/PlotStream.h>
#include <complex>
#include <limits>
#include <cmath>
#include <sstream>

typedef std::complex<double> Complex;
const Complex I(0,1);

Eigen::Matrix4cd constructMatrix(const Complex k, const Complex v, const Complex a)
{
    const Complex n = sqrt(1.0 - v);

    Eigen::Matrix4cd mat = Eigen::Matrix4cd::Zero();
    
    mat(0,0) = std::exp(I*k*a);
    mat(0,1) = -std::exp(-I*n*k*a);
    mat(0,2) = -std::exp(I*n*k*a);
    mat(1,1) = std::exp(I*n*k*a);
    mat(1,2) = std::exp(-I*n*k*a);
    mat(1,3) = -std::exp(I*k*a);
    mat(2,0) = -I*k*std::exp(I*k*a);
    mat(2,1) = -I*n*k*std::exp(-I*n*k*a);
    mat(2,2) = I*n*k*std::exp(I*n*k*a);
    mat(3,1) = I*n*k*std::exp(I*n*k*a);
    mat(3,2) = -I*n*k*std::exp(-I*n*k*a);
    mat(3,3) = -I*k*std::exp(I*k*a);

    return mat;
}

Eigen::Vector4cd constructVector(const Complex k, const Complex a)
{
    Eigen::Vector4cd vec = Eigen::Vector4cd::Zero();

    vec(0) = -std::exp(-I*k*a);
    vec(2) = -I*k*std::exp(-I*k*a);

    return vec;
}

class Density
{
    private:
        static Complex A, B, C, D, F, ik, ink;
        static double halfWidth, barrierHeight;
    
    public:

        static void intialize(Complex k, Complex v, Complex a, Complex b, Complex c, Complex d, Complex f)
        {
            Complex n = std::sqrt(1.0 - v);

            A = Complex(1,0);
            B = b * A;
            C = c * A;
            D = d * A;
            F = f * A;
            ik = I * k;
            ink = I*n*k;
            halfWidth = std::abs(a.real());
            barrierHeight = v.real();
        }
        
        static double density(double x)
        {
            if (x <= -halfWidth)    // region I
            {
                return std::norm(A*std::exp(ik*x) + B*std::exp(-ik*x));
            }else if (x >= halfWidth)   // region III
            {
                return std::norm(F*std::exp(ik*x));
            }else   // region II
            {
                return std::norm(C*std::exp(ink*x) + D*std::exp(-ink*x));
            }
        }

        static double barrier(double x)
        {
            if (-halfWidth < x && x < halfWidth)
            {
                return barrierHeight;
            }else
            {
                return 0;
            }
        }

        static double getBarrierHeight()
        {
            return barrierHeight;
        }

        static double getMaxDensity() { return std::norm(std::abs(A) + std::abs(B)); }

        static double getReflectionCoeffcient() { return std::norm(B); };

        static double getTransmissionCoeffcient() { return std::norm(F); };
};

Complex Density::A, Density::B, Density::C, Density::D, Density::F, Density::ik, Density::ink;
double Density::halfWidth, Density::barrierHeight;

void showHelp()
{
    std::cout << "./density k v a" << std::endl;
    std::cout << "Plot the probability density for a 1D rectangular barrier." << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << "    k        wave vector of incident wave (k > 0)" << std::endl;
    std::cout << "    v        ratio between barrier height and energy" << std::endl;
    std::cout << "    a        halfwidth of the barrier (a > 0)" << std::endl;

    exit(0);
}

void parseArgument(int argc, char **argv, double &k, double &v, double &a)
{
    if (argc != 4)
    {
        showHelp();
    }
    
    std::istringstream kStream(argv[1]);
    std::istringstream vStream(argv[2]);
    std::istringstream aStream(argv[3]);

    if (!(kStream>>k && k>0 && vStream>>v && aStream>>a && a>0))
    {
        showHelp();
    }
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    // Parse Argument
    double kArg, vArg, aArg;
    parseArgument(argc, argv, kArg, vArg, aArg);

    Complex k(kArg,0), v(vArg,0), a(aArg,0);

    Eigen::Matrix4cd mat = constructMatrix(k, v, a);
    Eigen::Vector4cd cVec = constructVector(k, a);
    Eigen::Vector4cd xVec = mat.fullPivLu().solve(cVec);

    Complex b(xVec[0]), c(xVec[1]), d(xVec[2]), f(xVec[3]);

    Density::intialize(k,v,a,b,c,d,f);
    std::cout << "Reflection: " << Density::getReflectionCoeffcient() << std:: endl;
    std::cout << "Transmission: " << Density::getTransmissionCoeffcient() << std::endl;
    std::cout << "Total: " << Density::getReflectionCoeffcient() + Density::getTransmissionCoeffcient() << std::endl;

    Genfun::GENFUNCTION densityFunc = Genfun::F1D(Density::density);
    Genfun::GENFUNCTION barrierFunc = Genfun::F1D(Density::barrier);

    double xMax = 2 * M_PI / kArg + aArg;
    double xMin = -xMax;
    double yMax = Density::getMaxDensity() * 1.2;
    double yMin = std::min(-0.2, Density::getBarrierHeight()*1.2);
    
    PlotView *view_ptr = new PlotView(PRectF(xMin, xMax, yMin, yMax));

    PlotFunction1D *densityFunc_ptr = new PlotFunction1D(densityFunc);
    {
        PlotFunction1D::Properties prop;
        prop.pen.setWidth(3);
        densityFunc_ptr->setProperties(prop);
    }
    view_ptr->add(densityFunc_ptr);
        
    PlotFunction1D *barrierFunc_ptr = new PlotFunction1D(barrierFunc);
    {
        PlotFunction1D::Properties prop;
        prop.pen.setWidth(3);
        prop.pen.setStyle(Qt::DashLine);
        barrierFunc_ptr->setProperties(prop);
    }
    view_ptr->add(barrierFunc_ptr);
    

    PlotStream titleStream(view_ptr->titleTextEdit());
    titleStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Sans Serif")
                << PlotStream::Size(16)
                << "k=" << kArg << " a=" << aArg << " v=" << vArg
                << PlotStream::EndP();

    PlotStream xLabelStream(view_ptr->xLabelTextEdit());
    xLabelStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Sans Serif")
                << PlotStream::Size(16)
                << "x"
                << PlotStream::EndP();
    
    
    QMainWindow window;
    window.setCentralWidget(view_ptr);

    QToolBar *toolBar = window.addToolBar("Tools");
    QAction  *quitAction = toolBar->addAction("Quit(q)");
    quitAction->setShortcut(QKeySequence("q"));
    QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));

    QAction *printAction = toolBar->addAction("Print to PDF (p)");
    printAction->setShortcut(QKeySequence("p"));
    QObject::connect(printAction, SIGNAL(triggered()), view_ptr, SLOT(print()));

    QAction *saveAction = toolBar->addAction("Save as SVG (s)");
    saveAction->setShortcut(QKeySequence("s"));
    QObject::connect(saveAction, SIGNAL(triggered()), view_ptr, SLOT(save()));

    window.show();
    app.exec();
    return 0;
}