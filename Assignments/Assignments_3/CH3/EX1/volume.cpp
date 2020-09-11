#include <iostream>
#include <eigen3/Eigen/Dense>
#include <complex>
#include <vector>
typedef std::complex<double> Complex;

int main(int argc, char **argv)
{
    double raw_data[3][3] = {{3, 0, 0}, {0.5, 2.0, 0}, {0.3, 0.2, 1.5}};

    std::vector<Eigen::Vector3d> Vecs;

    for (size_t i = 0; i < 3; i++)
    {
        Vecs.push_back(Eigen::Vector3d(raw_data[i][0],raw_data[i][1], raw_data[i][2]));
        std::cout << "Vector " << i << ": " << Vecs.back().transpose() << std::endl;
    }
    
    std::cout << "Volume computed from triple product: ";
    std::cout << std::abs(Vecs[0].dot(Vecs[1].cross(Vecs[2]))) << std::endl;

    std::cout << std::endl;

    Eigen::Matrix3d Mat;

    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            Mat(i,j) = raw_data[i][j];
        }
    }
    std::cout << "Matrix: " << std::endl;
    std::cout << Mat << std::endl;

    std::cout << "Volume computed from determinant: ";
    std::cout << std::abs(Mat.determinant()) << std::endl;    

    return 0;
}