#include <eigen3/Eigen/Dense>
#include <QImage>
#include <QColor>
#include <complex>
#include <limits>
#include <cmath>
#include <sstream>
#include <iostream>
#include <density.h>

typedef std::complex<double> Complex;
const Complex I(0,1);

QColor indexColor(double val)
{
    int h = std::round(240 * val);
    return QColor::fromHsv(h, 255, 255);
}

const double kaMin = 1e-7;
const double kaMax = 3;
const double vMin = -2.5;
const double vMax = 2.5;
const int imageWidth = std::round(vMax - vMin) * 200;
const int imageHeight = std::round(kaMax- kaMin) * 200;

void posToKVA(int i, int j, double &ka, double &v)
{
    ka = kaMin + (kaMax - kaMin) / (imageHeight - 1) * (imageHeight - j - 1);
    v = vMin + (vMax - vMin) / (imageWidth - 1) * i;
    if (v==0.5 || v==1)
    {
        v += 1e-7;
    }
}

int vToI(double v)
{
    return std::round((v - vMin)/(vMax - vMin) * (imageWidth - 1));
}

QImage generateColormap()
{
    QImage colormap(400, 20, QImage::Format_RGB32);

    for (size_t i = 0; i < 400; i++)
    {
        double val = i / 399.0;
        for (size_t j = 0; j < 20; j++)
        {
            colormap.setPixelColor(i, j, indexColor(val));
        }
        
    }
    
    return colormap;
}

int main()
{   
    QImage reflectionImage(imageWidth,imageHeight,QImage::Format_RGB32);
    QImage transmissionImage(imageWidth, imageHeight, QImage::Format_RGB32);
    QImage colormap = generateColormap();

    double ka, v;
    for (int i = 0; i < imageWidth; i++)
    {
        for (int j = 0; j < imageHeight; j++)
        {
            posToKVA(i, j, ka, v);

            Density::WaveFunction wave(ka, v);

            double reflectionCoef = wave.reflectionCoefficient();
            double transmissionCoef = wave.transmissionCoefficient();

            reflectionImage.setPixelColor(i,j,indexColor(reflectionCoef));
            transmissionImage.setPixelColor(i,j,indexColor(transmissionCoef));
        }
        
    }

    int zeroI = vToI(0);
    for (size_t j = 0; j < imageHeight; j++)
    {
        if ((j/10)%2 == 0)
        {
            reflectionImage.setPixelColor(zeroI,j,qRgb(0,0,0));
            transmissionImage.setPixelColor(zeroI,j,qRgb(0,0,0));
        }
    }
    

    reflectionImage.save("reflection.bmp");
    transmissionImage.save("transmission.bmp");
    colormap.save("colormap.bmp");


    return 0;
}