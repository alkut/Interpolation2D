#include "CubicSpline.h"
#include <stdlib.h>

void CubicSplineInterpolation(double (*Function)(double , double ), double *nest_x, double *nest_y, array4 gamma)
{
    double **A, **TMP;
    A = (double **)malloc(4 * sizeof (double *));
    TMP = (double **)malloc(4 * sizeof (double *));
    double *rawA = (double *) malloc(16 * sizeof (double ));
    double *rawTMP = (double *) malloc(16 * sizeof (double ));
    for (int i = 0; i < 4; ++i)
    {
        A[i] = rawA + 4 * i;
        TMP[i] = rawTMP + 4 * i;
    }
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 4; ++j)
            A[i][j] = (double )(i == j);
    array4 F = FindF_(Function, nest_x, nest_y, gamma);
    for (int i = 0; i < gamma.nx - 1; ++i)
    {
        for (int j = 0; j < gamma.ny - 1; ++j)
        {
            Calculate(gamma.array[i][j], A, TMP, F.array[i][j], nest_x[i+1] - nest_x[i], nest_y[j+1] - nest_y[j]);
        }
    }
    free(rawA);
    free(rawTMP);
    free(A);
    free(TMP);
    destroy(F);
}

array4 FindF_(double (*Function)(double , double ), double *nest_x, double *nest_y, array4 gamma)
{
    array4 res = create(gamma.nx, gamma.ny);
    matrix3 Ax = FindA_(nest_x, gamma.nx);
    matrix3 Cx = FindC(nest_x, gamma.nx);
    matrix3 Ay = FindA_(nest_y, gamma.ny);
    matrix3 Cy = FindC(nest_y, gamma.ny);
    double **F = FindF__(Function, nest_x, nest_y, gamma.nx, gamma.ny);
    double **Fx = multiply_left(Cx, F, gamma.nx, gamma.ny);
    multiply_left_inv(Ax, Fx, gamma.ny);
    transpose3(Ay);
    transpose3(Cy);
    double **tmpFy = multiply_right(Cy, F, gamma.nx, gamma.ny);
    double **Fy = multiply_right_inv(Ay, tmpFy, gamma.nx);
    dealloc(tmpFy, gamma.nx);
    double **Fxy = multiply_left(Cx, Fy, gamma.nx, gamma.ny);
    multiply_left_inv(Ax, Fxy, gamma.ny);

    for (int i = 0; i < gamma.nx - 1; ++i)
        for (int j = 0; j < gamma.ny - 1; ++j)
        {
            double **t = gamma.array[i][j];

            t[0][0] = F[i][j];
            t[0][1] = Fy[i][j];
            t[0][2] = F[i][j+1];
            t[0][3] = Fy[i][j+1];

            t[1][0] = Fx[i][j];
            t[1][1] = Fxy[i][j];
            t[1][2] = Fx[i][j+1];
            t[1][3] = Fxy[i][j+1];

            t[2][0] = F[i+1][j];
            t[2][1] = Fy[i+1][j];
            t[2][2] = F[i+1][j+1];
            t[2][3] = Fy[i+1][j+1];

            t[3][0] = Fx[i+1][j];
            t[3][1] = Fxy[i+1][j];
            t[3][2] = Fx[i+1][j+1];
            t[3][3] = Fxy[i+1][j+1];
        }
    destroy3(Ax);
    destroy3(Ay);
    destroy3(Cx);
    destroy3(Cy);
    dealloc(F, gamma.nx);
    dealloc(Fx, gamma.nx);
    dealloc(Fy, gamma.nx);
    dealloc(Fxy, gamma.nx);
    return res;
}

double **FindG(double *nest_x, int nx)
{
    double **res = alloc(nx, nx);

    res[0][0] = - 3.0 / (nest_x[1] - nest_x[0]);
    res[0][1] = 3.0 / (nest_x[1] - nest_x[0]);

    for (int i = 1; i < nx - 1; ++i)
    {
        res[i][i - 1] = -3.0 * (nest_x[i + 1] - nest_x[i]) / (nest_x[i] - nest_x[i - 1]);
        res[i][i] = 3.0 * (nest_x[i + 1] - nest_x[i]) / (nest_x[i] - nest_x[i - 1]) 
                -3.0 * (nest_x[i] - nest_x[i - 1]) / (nest_x[i + 1] - nest_x[i]);
        res[i][i + 1] = 3.0 * (nest_x[i] - nest_x[i - 1]) / (nest_x[i + 1] - nest_x[i]);
    }

    res[nx - 1][nx - 2] = - 3.0 / (nest_x[nx - 1] - nest_x[nx - 2]);
    res[nx - 1][nx - 1] = 3.0 / (nest_x[nx - 1] - nest_x[nx - 2]);


    matrix3 matrix = create3(nx);

    matrix.main[0] = 2.0;
    for (int i = 1; i < nx - 1; ++i)
        matrix.main[i] = 2.0 * (nest_x[i+1] - nest_x[i-1]);
    matrix.main[nx-1] = 1.0;

    matrix.up[0] = 1.0;
    for (int i = 1; i < nx - 1; ++i)
        matrix.up[i] = (nest_x[i] - nest_x[i-1]);
    matrix.up[nx-1] = 0.0;

    matrix.down[0] = 0.0;
    for (int i = 1; i < nx - 1; ++i)
        matrix.down[i] = (nest_x[i+1] - nest_x[i]);
    matrix.down[nx-1] = 2.0;

    gauss(matrix, res);
    destroy3(matrix);

    return res;
}

double **FindF__(double (*Function)(double , double ), double *nest_x, double *nest_y, int nx, int ny);double **FindF__(double (*Function)(double , double ), double *nest_x, double *nest_y, int nx, int ny)
{
    double **res = alloc(nx, ny);

    for (int i = 0; i < nx; ++i)
        for (int j = 0; j < ny; ++j)
            res[i][j] = Function(nest_x[i], nest_y[j]);
    return res;
}

matrix3 FindA_(double *nest_x, int nx)
{
    matrix3 matrix = create3(nx);
    matrix.main[0] = 2.0;
    for (int i = 1; i < nx - 1; ++i)
        matrix.main[i] = 2.0 * (nest_x[i+1] - nest_x[i-1]);
    matrix.main[nx-1] = 1.0;

    matrix.up[0] = 1.0;
    for (int i = 1; i < nx - 1; ++i)
        matrix.up[i] = (nest_x[i] - nest_x[i-1]);
    matrix.up[nx-1] = 0.0;

    matrix.down[0] = 0.0;
    for (int i = 1; i < nx - 1; ++i)
        matrix.down[i] = (nest_x[i+1] - nest_x[i]);
    matrix.down[nx-1] = 2.0;
    return matrix;
}

matrix3 FindC(double *nest_x, int nx)
{
    matrix3 res = create3(nx);
    res.main[0] = - 3.0 / (nest_x[1] - nest_x[0]);
    res.up[0] = 3.0 / (nest_x[1] - nest_x[0]);

    for (int i = 1; i < nx - 1; ++i)
    {
        res.down[i] = -3.0 * (nest_x[i + 1] - nest_x[i]) / (nest_x[i] - nest_x[i - 1]);
        res.main[i] = 3.0 * (nest_x[i + 1] - nest_x[i]) / (nest_x[i] - nest_x[i - 1])
                    -3.0 * (nest_x[i] - nest_x[i - 1]) / (nest_x[i + 1] - nest_x[i]);
        res.up[i] = 3.0 * (nest_x[i] - nest_x[i - 1]) / (nest_x[i + 1] - nest_x[i]);
    }

    res.down[nx - 1] = - 3.0 / (nest_x[nx - 1] - nest_x[nx - 2]);
    res.main[nx - 1] = 3.0 / (nest_x[nx - 1] - nest_x[nx - 2]);

    return res;
}

