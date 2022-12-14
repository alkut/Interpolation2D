#include "Common.h"
#include <stdlib.h>

int LowerBound(double *arr, double x, int n)
{
    int l = 0, r = n - 1;
    int middle;
    while (r - l > 1)
    {
        middle = (l + r) >> 1;
        if (arr[middle] < x)
        {
            l = middle;
        }
        else if (arr[middle] > x)
        {
            r = middle;
        }
        else
            return middle;
    }
    return l;
}

double EvaluatePolynomial(double **arr, double dx, double dy)
{
    double ans = 0.0;
    double powsX[4], powsY[4];
    powsX[0] = powsY[0] = 1.0;
    for (int i = 1; i < 4; ++i)
    {
        powsY[i] = powsY[i-1] * dy;
        powsX[i] = powsX[i-1] * dx;
    }
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            ans += arr[i][j] * powsX[i] * powsY[j];
    return ans;
}

array4 create(int nx, int ny)
{
    array4 res;
    res.nx = nx;
    res.ny = ny;
    res.array = (double ****)malloc(nx * sizeof (double ***));
    res.memory = (double *)malloc(16 * nx * ny * sizeof (double ));
    for (int i = 0; i < nx; ++i)
    {
        res.array[i] = (double ***)malloc(ny * sizeof (double **));
        for (int j = 0; j < ny; ++j)
        {
            res.array[i][j] = (double **)malloc(4 * sizeof(double *));
            for (int k = 0; k < 4; ++k)
            {
                res.array[i][j][k] = res.memory + i * ny * 16 + j * 16 + k * 4;
                for (int l = 0; l < 4; ++l)
                    res.array[i][j][k][l] = 0.0;
            }
        }
    }
    return res;
}

void FindGamma(array4 gamma, double **A, double **TMP, array4 f, double *X, double *Y)
{
    for (int i = 0; i < gamma.nx - 1; ++i)
    {
        for (int j = 0; j < gamma.ny - 1; ++j)
        {
            Calculate(gamma.array[i][j], A, TMP, f.array[i][j], X[i + 1] - X[i], Y[j + 1] - Y[j]);
        }
    }
}

void Calculate(double **gamma, double **A, double **TMP, double ** f, double dx, double dy)
{
    FindA(A, dx);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            TMP[i][j] = 0.0;
            for (int k = 0; k < 4; ++k)
                TMP[i][j] += A[i][k] * f[k][j];
        }
    FindA(A, dy);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) 
            for (int k = 0; k < 4; ++k)
                gamma[i][j] += TMP[i][k] * A[j][k];
}

void FindA(double **A, double h)
{
    double tmp = 1.0 / h;
    double tmp2 = tmp * tmp;
    double tmp3 = tmp2 * tmp;
    A[2][0] = - 3.0 * tmp2;
    A[2][1] = - 2.0 * tmp;
    A[2][2] = 3.0 * tmp2;
    A[2][3] = - 1.0 * tmp;

    A[3][0] = 2.0 * tmp3;
    A[3][1] = 1.0 * tmp2;
    A[3][2] = -2.0 * tmp3;
    A[3][3] = 1.0 * tmp2;
}

void destroy(array4 arr)
{
    free(arr.memory);
    for (int i = 0; i < arr.nx; ++i)
    {
        for (int j = 0; j < arr.ny; ++j)
            free(arr.array[i][j]);
        free(arr.array[i]);
    }
    free(arr.array);
}

matrix3 create3(int n)
{
    matrix3 res;
    res.n = n;
    res.main = (double *) malloc(n * sizeof (double ));
    res.down = (double *) malloc(n * sizeof (double ));
    res.up = (double *) malloc(n * sizeof (double ));
    for (int i = 0; i < n; ++i)
    {
        res.main[i] = 0.0;
        res.down[i] = 0.0;
        res.up[i] = 0.0;
    }
    return res;
}

void transpose3(matrix3 m)
{
    double buf;
    for (int i = 0; i < m.n - 1; ++i)
    {
        buf = m.up[i];
        m.up[i] = m.down[i+1];
        m.down[i+1] = buf;
    }
}

double **transpose(double **m, int nx, int ny)
{
    double **res = alloc(ny, nx);
    for (int i = 0; i < nx; ++i)
        for (int j = i + 1; j < ny; ++j)
        {
            res[j][i] = m[i][j];
        }
    return res;
}

double **multiply_left(matrix3 m, double **arr, int nx, int ny)
{
    double **res = alloc(nx, ny);
    for (int i = 0; i < nx; ++i)
        for (int j = 0; j < ny; ++j)
        {
            res[i][j] = m.main[i] * arr[i][j];
            if (i != 0)
                res[i][j] += m.down[i] * arr[i-1][j];
            if (i != nx - 1)
                res[i][j] += m.up[i] * arr[i+1][j];
        }

    return res;
}

double **multiply_right(matrix3 m, double **arr, int nx, int ny)
{
    double **arrT, **tmp, **res;
    transpose3(m);
    arrT = transpose(arr, nx, ny);
    tmp = multiply_left(m, arrT, ny, nx);
    res = transpose(tmp, ny, nx);
    transpose3(m);
    dealloc(tmp, ny);
    dealloc(arrT, ny);
    return res;
}

void multiply_left_inv(matrix3 m, double **OutMatrix, int ny)
{
    for (int i = 0; i < m.n; ++i)
    {
        double x = m.main[i];
        m.main[i] /= x;
        m.up[i] /= x;
        for (int j = 0; j < ny; ++j)
            OutMatrix[i][j] /= x;
        if (i != m.n - 1) {
            double y = m.down[i + 1];
            m.down[i + 1] -= y;
            m.main[i + 1] -= y * m.up[i];
            for (int j = 0; j < ny; ++j)
                OutMatrix[i + 1][j] -= y * OutMatrix[i][j];
        }
    }
    for (int i = m.n - 1; i > 0; --i)
    {
        double x = m.up[i-1];
        m.up[i-1] -= x;
        for (int j = 0; j < ny; ++j)
            OutMatrix[i-1][j] -= x * OutMatrix[i][j];
    }
}

double **multiply_right_inv(matrix3 m, double **OutMatrix, int nx)
{
    double **OutT, **res;
    transpose3(m);
    OutT = transpose(OutMatrix, nx, m.n);
    multiply_left_inv(m, OutT, nx);
    res = transpose(OutT, m.n, nx);
    transpose3(m);
    dealloc(OutT, m.n);
    return res;
}

void gauss(matrix3 m, double **OutMatrix)
{
    for (int i = 0; i < m.n; ++i)
    {
        double x = m.main[i];
        m.main[i] /= x;
        m.up[i] /= x;
        for (int j = 0; j < m.n; ++j)
            OutMatrix[i][j] /= x;
        if (i != m.n - 1) {
            double y = m.down[i + 1];
            m.down[i + 1] -= y;
            m.main[i + 1] -= y * m.up[i];
            for (int j = 0; j < m.n; ++j)
                OutMatrix[i + 1][j] -= y * OutMatrix[i][j];
        }
    }
    for (int i = m.n - 1; i > 0; --i)
    {
        double x = m.up[i-1];
        m.up[i-1] -= x;
        for (int j = 0; j < m.n; ++j)
            OutMatrix[i-1][j] -= x * OutMatrix[i][j];
    }
}

void destroy3(matrix3 m)
{
    free(m.main);
    free(m.down);
    free(m.up);
}

double ** alloc(int n, int m)
{
    double** res = (double **) malloc(n * sizeof (double *));
    for (int i = 0; i < n; ++i)
    {
        res[i] = (double*) malloc(m * sizeof (double ));
        for (int j = 0; j < m; ++j)
            res[i][j] = 0.0;
    }
    return res;
}

void dealloc(double** ptr, int n)
{
    for (int i = 0; i < n; ++i)
        free(ptr[i]);
    free(ptr);
}
