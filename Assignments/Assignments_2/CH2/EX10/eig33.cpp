/*
    reference:
    https://en.wikipedia.org/wiki/Eigenvalue_algorithm#3%C3%973_matrices
    https://en.wikipedia.org/wiki/Cubic_equation#General_cubic_formula
*/

#include <iostream>
#include <iomanip>
#include <complex>
#include <array>
#include <limits>
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>
using namespace std;

#define RANK 3
// #define COMPLEX_EPS numeric_limits<double>::epsilon()
#define COMPLEX_EPS 1e-5

typedef complex<double> Complex;
typedef array<Complex,RANK> Vector;
typedef array<Vector,RANK> Matrix;

struct EigenValue
{
    Complex val;
    int order;

    EigenValue(const Complex& v, const int o): val(v), order(o) {}
};

Matrix parse_input()
{
    Matrix mat;

    for (size_t i = 0; i < RANK; i++)
    {
        for (size_t j = 0; j < RANK; j++)
        {
            cin >> mat[i][j];
        }
    }

    return mat;
}

void print_matrix(Matrix mat)
{
    for (size_t i = 0; i < mat.size(); i++)
    {
        for (size_t j = 0; j < mat[i].size(); j++)
        {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
}

void print_eigenvectors(const vector<Vector>& vecs)
{
    const size_t column_width = 30;
    if (vecs.size() > 0)
    {
        for (size_t i = 0; i < vecs[0].size(); i++)
        {
            for (size_t j = 0; j < vecs.size(); j++)
            {
                cout << setw(column_width) << left << vecs[j][i] << " ";
            }
            cout << endl;
        }
        
    }
}

void print_Vector(const Vector& a)
{
    for (size_t i = 0; i < a.size(); i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;
}

// Complex operation

bool complexless(const Complex& a, const Complex& b)
{
    if (a.real() < b.real())
        return true;
    
    if (a.real() > b.real())
        return false;
    
    if (a.imag() < b.imag())
        return true;
    
    return false;
}

Complex square(const Complex& a)
{
    return a * a;
}

Complex cubic(const Complex& a)
{
    return a * a * a;
}

Complex cbrt(const Complex& a)
{
    return pow(a, 1.0/3.0);
}

// Vector operation

Vector random_vector()
{
    // size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    size_t seed = 0;
    mt19937_64 generator(seed);
    uniform_real_distribution<double> uniform_dist(-1.0, 1.0);

    Vector a;

    for (size_t i = 0; i < a.size(); i++)
    {
        a[i] = Complex(uniform_dist(generator), uniform_dist(generator));
    }
    
    return a;
}

Complex dot(Vector a, Vector b)
{
    Complex sum;

    for (size_t i = 0; i < a.size(); i++)
    {
        sum += conj(a[i]) * b[i];
    }
    
    return sum;
}

double length(Vector a)
{
    return sqrt(dot(a, a)).real();
}

Vector operator - (const Vector& a, const Vector& b)
{
    Vector c;

    for (size_t i = 0; i < c.size(); i++)
    {
        c[i] = a[i] - b[i];
    }
    
    return c;
}

Vector operator * (const Complex& a, const Vector& b)
{
    Vector c;

    for (size_t i = 0; i < c.size(); i++)
    {
        c[i] = a * b[i];
    }
    
    return c;
}

Vector normalize(const Vector& a)
{
    if (length(a) < COMPLEX_EPS)    // avoid normalizing zero vector
        return a;
    
    return 1 / length(a) * a;
}

// Matrix operation

Matrix identity()
{
    Matrix a;

    for (size_t i = 0; i < a.size(); i++)
    {
        a[i][i] = Complex(1.0, 0);
    }
    
    return a;
}

Matrix random_matrix()
{
    size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    mt19937_64 generator(seed);
    uniform_real_distribution<double> uniform_dist(-1.0, 1.0);

    Matrix a;

    for (size_t i = 0; i < a.size(); i++)
    {
        for (size_t j = 0; j < a[i].size(); j++)
        {
            a[i][j] = Complex(uniform_dist(generator), uniform_dist(generator));
        }
    }
    
    return a;
}

Matrix operator + (const Matrix& a, const Matrix& b)
{
    Matrix c;

    for (size_t i = 0; i < a.size(); i++)
    {
        for (size_t j = 0; j < a[i].size(); j++)
        {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
    
    return c;
}

Matrix operator - (const Matrix& a, const Matrix& b)
{
    Matrix c;

    for (size_t i = 0; i < c.size(); i++)
    {
        for (size_t j = 0; j < c[i].size(); j++)
        {
            c[i][j] = a[i][j] - b[i][j];
        }
    }
    
    return c;
}

Matrix operator * (const Complex a, const Matrix& b)
{
    Matrix c;

    for (size_t i = 0; i < b.size(); i++)
    {
        for (size_t j = 0; j < b[i].size(); j++)
        {
            c[i][j] = a * b[i][j];
        }
    }
    
    return c;
}

Matrix multiply(const Matrix& a, const Matrix& b)
{
    Matrix c;

    for (size_t i = 0; i < c.size(); i++)
    {
        for (size_t j = 0; j < c[i].size(); j++)
        {
            for (size_t k = 0; k < a[i].size(); k++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    
    return c;
}

Complex trace(const Matrix& a)
{
    Complex tr(0.0, 0.0);

    for (size_t i = 0; i < a.size(); i++)
    {
        tr += a[i][i];
    }
    
    return tr;
}

Complex det(const Matrix& a)
{
    return a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1])
            - a[1][0] * (a[0][1] * a[2][2] - a[2][1] * a[0][2])
            + a[2][0] * (a[0][1] * a[1][2] - a[1][1] * a[0][2]);
}

// Linear Algebra operation

array<Complex,3> solve_cubic(Complex a, Complex b, Complex c, Complex d)
{
    Complex delta0 = square(b) - 3.0 * a * c;
    Complex delta1 = 2.0 * cubic(b) - 9.0 * a * b * c + 27.0 * square(a) * b;
    
    Complex C = cbrt((delta1 + sqrt(square(delta1) - 4.0 * cubic(delta0)))/2.0);
    if (abs(C) < COMPLEX_EPS)
    {
        C = cbrt((delta1 - sqrt(square(delta1) - 4.0 * cubic(delta0)))/2.0);
    }

    Complex Xi(-1.0/2.0,sqrt(3.0)/2.0);

    array<Complex, 3> vals;
    Complex Xi_k(1,0);
    for (size_t i = 0; i < vals.size(); i++)
    {
        vals[i] = -1.0 / (3.0 * a) * (b + Xi_k * C + delta0 / (Xi_k * C));
        Xi_k *= Xi;
    }

    return vals;
}

vector<EigenValue> eigenvalue(const Matrix& mat)
{
    Complex a(1,0);
    Complex b = -trace(mat);
    Complex c = -0.5 * (trace(multiply(mat,mat)) - square(trace(mat)));
    Complex d = -det(mat);

    array<Complex,3> sols = solve_cubic(a, b, c, d);

    sort(sols.begin(), sols.end(),complexless);

    vector<EigenValue> eig_vals;
    eig_vals.push_back(EigenValue(sols[0],1));

    for (size_t i = 1; i < sols.size(); i++)
    {
        if (abs(eig_vals.back().val - sols[i]) < COMPLEX_EPS)
        {
            eig_vals.back().order += 1;
        }else
        {
            eig_vals.push_back(EigenValue(sols[i],1));
        }
    }
    
    return eig_vals;
}

Vector gram_schmidt(const Vector& a, const Vector& b)
{
    if (length(b) < COMPLEX_EPS)    // avoid projecting on a zero vector
        return a;

    return a - dot(b,a) / dot(b,b) * b;
}

vector<Vector> orthonormalize(vector<Vector> vec_list)
{
    for (size_t i = 0; i < vec_list.size(); i++)
    {
        for (size_t j = 0; j < i; j++)
        {
            vec_list[i] = gram_schmidt(vec_list[i], vec_list[j]);
        }
        
        if (length(vec_list[i]) > COMPLEX_EPS)
        {
            vec_list[i] = normalize(vec_list[i]);
        }
    }
    
    return vec_list;
}

Vector orthogonalize(Vector a, const vector<Vector>& basis)
{   
    for (size_t i = 0; i < basis.size(); i++)
    {
        a = gram_schmidt(a, basis[i]);
    }
    
    return a;
}

Vector vector_conj(const Vector& a)
{
    Vector c;

    for (size_t i = 0; i < a.size(); i++)
    {
        c[i] = conj(a[i]);
    }
    
    return c;
}

Vector correct_angle(const Vector& a)
{
    double angle = 0;
    size_t count = 0;

    for (size_t i = 0; i < a.size(); i++)
    {
        if (abs(a[i]) > COMPLEX_EPS)
        {
            angle += arg(a[i]);
            ++ count;
        }
    }
    angle /= count;
    Complex correction(cos(-angle),sin(-angle));

    Vector c;

    for (size_t i = 0; i < a.size(); i++)
    {
        c[i] = a[i] * correction;
    }

    return c;
}


vector<Vector> eigenvector_of_lambda(Matrix mat, EigenValue eig_val)
{
    Matrix A = mat - eig_val.val * identity();
    
    vector<Vector> row_vecs;
    row_vecs.insert(row_vecs.end(), A.begin(), A.end());
    for (size_t i = 0; i < mat.size(); i++)
    {
        row_vecs[i] = vector_conj(row_vecs[i]);
    }
    
    
    row_vecs = orthonormalize(row_vecs);

    vector<Vector> eigvecs;

    for (size_t i = 0; i < eig_val.order; i++)
    {
        Vector vec = random_vector();
        vec = orthogonalize(vec, row_vecs);
        eigvecs.push_back(vec);
    }
    
    eigvecs = orthonormalize(eigvecs);

    for (size_t i = 0; i < eigvecs.size(); i++)
    {
        eigvecs[i] = correct_angle(eigvecs[i]);
    }

    return eigvecs;
}


vector<Vector> eigenvector(Matrix mat, vector<EigenValue> eig_vals)
{
    vector<Vector> eig_vecs;

    for (size_t i = 0; i < eig_vals.size(); i++)
    {
        vector<Vector> eigvec_lamda = eigenvector_of_lambda(mat, eig_vals[i]);
        eig_vecs.insert(eig_vecs.end(), eigvec_lamda.begin(), eigvec_lamda.end());
    }

    return eig_vecs;
}


int main(int argc, char **argv)
{
    Matrix mat = parse_input();

    cout << "Matrix:" << endl;
    print_matrix(mat);
    cout << endl;

    vector<EigenValue> eig_vals = eigenvalue(mat);

    cout << setw(30) << left << "Eigenvalues:" << "Order" << endl;
    for (size_t i = 0; i < eig_vals.size(); i++)
    {
        cout << setw(30) << left << eig_vals[i].val << eig_vals[i].order << endl;
    }

    cout << endl;

    vector<Vector> eig_vecs = eigenvector(mat, eig_vals);

    cout << "Eigenvectors:" << endl;
    print_eigenvectors(eig_vecs);

    return 0;
}