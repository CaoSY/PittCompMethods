#include <wavefunction.h>
#include <eigen3/Eigen/SparseLU>
#include <vector>
#include <cassert>
#include <iostream>

namespace WaveFunction
{
    // Type definition
    class PieceWiseFunction
    {
        // psi(x) = A * exp(ika*x) + B * exp(-ika*x)
        private:
            Complex ik, A, B;
        
        public:
            PieceWiseFunction(Complex _ik, Complex _A = Complex(1,0), Complex _B = Complex(1,0))
                :ik(_ik), A(_A), B(_B) {}
            
            void setLeftCoef(Complex _A) { this->A = _A; }
            void setRightCoef(Complex _B) { this->B = _B; }
            Complex getLeftCoef() const { return this->A; }
            Complex getRightCoef() const { return this->B; }
            Complex getIK() const { return this->ik; }
            Complex leftVal(double x) const
            {
                return A * std::exp(ik * x);
            }
            Complex rightVal(double x) const
            {
                return B * std::exp(-ik * x);
            }
            Complex leftPrime(double x) const
            {
                return A * ik * std::exp(ik * x);
            }
            Complex rightPrime(double x) const
            {
                return B * (-ik) * std::exp(-ik * x);
            }
            Complex val(double x) const
            {
                return leftVal(x) + rightVal(x);
            }
            Complex density(double x) const
            {
                return std::norm(val(x));
            }
    };


    // private funcitons declaration
    double gaussianPotential(double x);
    void initializePosOfBoundaries();        // initialize positions of boundaries
    void initializeBarrierVec();     // initialize barrier vector
    void initializeWaveVec(double ka);       // initialize piecewiese wave function vector
    Eigen::SparseMatrix<Complex> constructMatrix(); // construct matrix of boundary conditions
    Eigen::SparseVector<Complex> constructVector(); // construct vector of boudnary conditions
    void setupWaveVec(Eigen::SparseVector<Complex> &coeff); // assign correct coefficients to waveVec


    // global variables declaration
    const double xMin = -4;
    const double xMax = 4;
    double V0;
    double binWidth;
    size_t numOfBins;   // number of bins from -4 to 4
    size_t numOfUnkonws;    // number of unkonw coefficents
    size_t numOfBoundaries; // number of boundaries
    std::vector<double> posOfBoundaries;        // positions of boundaries
    std::vector<double> barrierVec;             // piecewise barrier
    std::vector<PieceWiseFunction> waveVec;     // piecewise wave function
    
    
    // function implementation
    double gaussianPotential(double x)
    {   
        if (WaveFunction::xMin <= x && x <= WaveFunction::xMax)
        {
            return WaveFunction::V0 * std::exp(-x*x/2);
        }else
        {
            return 0;
        }
    }

    void initializePosOfBoundaries()
    {
        for (size_t i = 0; i < WaveFunction::numOfBins; i++)
        {
            posOfBoundaries.push_back(xMin + double(i) / double(numOfBins) * (xMax - xMin));
        }
        posOfBoundaries.push_back(xMax);
    }

    void initializeBarrierVec()
    {
        // barrier < xMin is 0;
        barrierVec.push_back(0);

        // sample guassian profile using mid-points of each bin.
        for (size_t i = 0; i < WaveFunction::numOfBins; i++)
        {
            double samplePos = xMin + (i + 0.5) / double(numOfBins) * (xMax - xMin);
            barrierVec.push_back(gaussianPotential(samplePos));
        }
        
        // barrier > xMax is 0
        barrierVec.push_back(0);
    }

    void initializeWaveVec(double k)
    {
        Complex ik0 = Complex(0,1) * k;

        for (size_t i = 0; i < barrierVec.size(); i++)
        {
            Complex n = std::sqrt(Complex(1.0 - barrierVec[i],0));
            Complex ik = ik0 * n;
            waveVec.push_back(PieceWiseFunction(ik));
        }
        
        // wave function at x < xMin is exp(ika*x) + B*exp(-ika*x)
        waveVec.front().setLeftCoef(1.0);
        
        // wave function at x > xMax is A*exp(ika*x)
        waveVec.back().setRightCoef(0.0);
    }

    Eigen::SparseMatrix<Complex> constructMatrix()
    {
        // the row order is different from the matrix on the textbook so that
        // the matrix is a band diagonal matrix, which will improve the perfomance
        // of sparse matrices.

        Eigen::SparseMatrix<Complex> mat(numOfUnkonws,numOfUnkonws);

        double pos = posOfBoundaries.front();
        // at the fisrt boundary, value is continuous
        mat.insert(0,0) = waveVec.front().rightVal(pos);
        // at the first boundary, derivative is continuous
        mat.insert(1,0) = waveVec.front().rightPrime(pos);

        for (size_t i = 0; i < numOfBins; i++)
        {
            double pos1 = posOfBoundaries[i];
            double pos2 = posOfBoundaries[i+1];
            size_t psiNum = i + 1;  // index of wave fucntion in waveVec

            // wave function at bin i is A*exp(ika*x) + B*exp(-ika*x)

            size_t ACol = 1 + 2*i;  // column position of coefficient A
            size_t BCol = 2 + 2*i;  // column position of coefficient B
            size_t lRow = 2*i;        // row position of val condition on the left side of bin i
            size_t lpRow = 2*i + 1;   // row position of derivative condition on the left side of bin i
            size_t rRow = 2*i + 2;    // row position of val condition on the right side of bin i
            size_t rpRow = 2*i + 3;   // row position of derivative condition on the right side of bin i

            // Coefficient A
            // at boundary i, value is continuous
            mat.insert(lRow,ACol) = -waveVec[psiNum].leftVal(pos1);
            // at boundary i, derivative is continuous
            mat.insert(lpRow,ACol) = -waveVec[psiNum].leftPrime(pos1);
            // at boundary i+1, value is continuous
            mat.insert(rRow,ACol) = waveVec[psiNum].leftVal(pos2);
            // at boundary i+1, derivative is continuous
            mat.insert(rpRow,ACol) = waveVec[psiNum].leftPrime(pos2);

            // Coefficient B
            // at boundary i, value is continuous
            mat.insert(lRow,BCol) = -waveVec[psiNum].rightVal(pos1);
            // at boundary i, derivative is continuous
            mat.insert(lpRow,BCol) = -waveVec[psiNum].rightPrime(pos1);
            // at boundary i+1, value is continuous
            mat.insert(rRow,BCol) = waveVec[psiNum].rightVal(pos2);
            // at boundary i+1, derivative is continuous
            mat.insert(rpRow,BCol) = waveVec[psiNum].rightPrime(pos2);
        }
        
        pos = posOfBoundaries.back();
        // at the last boundary, value is continuous
        mat.insert(numOfUnkonws-2,numOfUnkonws-1) = waveVec.back().leftVal(pos);
        // at the last boundary, derivative is continuous
        mat.insert(numOfUnkonws-1,numOfUnkonws-1) = waveVec.back().leftPrime(pos);

        mat.makeCompressed();
        return mat;
    }

    Eigen::SparseVector<Complex> constructVector()
    {
        Eigen::SparseVector<Complex> cVec(numOfUnkonws);

        // the leftmost wave function is exp(ika*x) + B*exp(-ika*x);
        double pos = posOfBoundaries.front();
        cVec.insert(0) = -waveVec.front().leftVal(pos);
        cVec.insert(1) = -waveVec.front().leftPrime(pos);

        return cVec;
    }

    void setupWaveVec(Eigen::SparseVector<Complex> &coeff)
    {
        Eigen::VectorXcd coeffD(coeff);

        waveVec.front().setRightCoef(coeffD(0));

        for (size_t i = 0; i < numOfBins; i++)
        {
            size_t psiNum = i + 1;
            size_t posLeft = 2*i + 1;
            size_t posRight = 2*i + 2;
            waveVec[psiNum].setLeftCoef(coeffD(posLeft));
            waveVec[psiNum].setRightCoef(coeffD(posRight));
        }
        
        waveVec.back().setLeftCoef(coeffD(numOfUnkonws - 1));
    }

    void initialize(double _k, double _v, int _N)
    {
        // intialize global variables
        WaveFunction::V0 = _v;
        WaveFunction::numOfBins = (1 << _N);
        WaveFunction::binWidth = (xMax - xMin) / numOfBins;
        WaveFunction::numOfBoundaries = 1 + numOfBins;
        WaveFunction::numOfUnkonws = 2 * numOfBoundaries;
        
        // clear all vectors
        posOfBoundaries.clear();
        barrierVec.clear();
        waveVec.clear();

        // initialization must be called in order
        initializePosOfBoundaries();
        initializeBarrierVec();
        initializeWaveVec(_k);

        Eigen::SparseMatrix<Complex> mat = constructMatrix();
        Eigen::SparseVector<Complex> cVec= constructVector();
        Eigen::SparseLU<Eigen::SparseMatrix<Complex>> solver;
        solver.compute(mat);
        if (solver.info() != Eigen::Success)
        {
            std::cout << Eigen::MatrixXcd(mat) << std::endl;
        }

        assert(solver.info() == Eigen::Success);
        Eigen::SparseVector<Complex> coeff = solver.solve(cVec);
        assert(solver.info() == Eigen::Success);

        setupWaveVec(coeff);
    }
    
    double barrier(double x)
    {
        if (xMin < x && x < xMax)
        {
            size_t binIndex = 1 + std::floor((x - xMin) / binWidth);
            return barrierVec[binIndex];
        }
        
        return 0;
    }

    Complex psi(double x)
    {
        if (x <= xMin)
        {
            return waveVec.front().val(x);
        }

        if (x >= xMax)
        {
            return waveVec.back().val(x);
        }

        size_t regionIndex = 1 + std::floor((x - xMin) / binWidth);
        return waveVec[regionIndex].val(x);
    }

    double density(double x)
    {
        return std::norm(psi(x));
    }

    double reflection()
    {
        return std::norm(waveVec.front().getRightCoef());
    }

    double transmission()
    {
        return std::norm(waveVec.back().getLeftCoef());
    }
} // namespace WaveFunction
