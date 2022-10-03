#include "Common.h"
#include <stdlib.h>
#include "HermitInterpolation.h"
#include "interpolation2.h"

void HermitInterpolation(double (*function)(double , double ), double (*derivative_x)(double , double ),
                         double (*derivative_y)(double , double ), double (*derivative_xy)(double , double ),
                         double *nest_x, double *nest_y, array4 gamma)
{

    double **A, **TMP;
    double *rawA, *rawTMP;
    array4 F;
    A = (double **)malloc(4 * sizeof (double *));
    TMP = (double **)malloc(4 * sizeof (double *));
    rawA = (double *) malloc(16 * sizeof (double ));
    rawTMP = (double *) malloc(16 * sizeof (double ));
    for (int i = 0; i < 4; ++i)
    {
        A[i] = rawA + 4 * i;
        TMP[i] = rawTMP + 4 * i;
    }
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 4; ++j)
            A[i][j] = (double )(i == j);
    F = FindF(function, derivative_x, derivative_y, derivative_xy, nest_x, nest_y, gamma);
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

array4 FindF(double (*function)(double , double ), double (*derivative_x)(double , double ),
           double (*derivative_y)(double , double ), double (*derivative_xy)(double , double ),
           double *nest_x, double *nest_y, array4 gamma)
{
    array4 res;
    res = create(gamma.nx, gamma.ny);
    for (int i = 0; i < gamma.nx - 1; ++i)
    {
        for (int j = 0; j < gamma.ny - 1; ++j)
        {
            double **t = res.array[i][j];
            t[0][0] = function(nest_x[i], nest_y[j]);
            t[0][1] = derivative_y(nest_x[i], nest_y[j]);
            t[0][2] = function(nest_x[i], nest_y[j+1]);
            t[0][3] = derivative_y(nest_x[i], nest_y[j+1]);

            t[1][0] = derivative_x(nest_x[i], nest_y[j]);
            t[1][1] = derivative_xy(nest_x[i], nest_y[j]);
            t[1][2] = derivative_x(nest_x[i], nest_y[j+1]);
            t[1][3] = derivative_xy(nest_x[i], nest_y[j+1]);

            t[2][0] = function(nest_x[i+1], nest_y[j]);
            t[2][1] = derivative_y(nest_x[i+1], nest_y[j]);
            t[2][2] = function(nest_x[i+1], nest_y[j+1]);
            t[2][3] = derivative_y(nest_x[i+1], nest_y[j+1]);

            t[3][0] = derivative_x(nest_x[i+1], nest_y[j]);
            t[3][1] = derivative_xy(nest_x[i+1], nest_y[j]);
            t[3][2] = derivative_x(nest_x[i+1], nest_y[j+1]);
            t[3][3] = derivative_xy(nest_x[i+1], nest_y[j+1]);
        }
    }
    return res;
}
