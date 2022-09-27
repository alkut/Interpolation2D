#include <stdlib.h>
#include <stdio.h>
#include "Common.h"
#include "HermitInterpolation.h"
#include "TestFunctions.h"
#include "interpolation2.h"
#include "CubicSpline.h"

const double (*Functions[8])(double, double) = {f0, f1, f2, f3, f4, f5, f6, f7};
const double (*DerivativesX[8])(double, double) = {f0x, f1x, f2x, f3x, f4x, f5x, f6x, f7x};
const double (*DerivativesY[8])(double, double) = {f0y, f1y, f2y, f3y, f4y, f5y, f6y, f7y};
const double (*DerivativesXY[8])(double, double) = {f0xy, f1xy, f2xy, f3xy, f4xy, f5xy, f6xy, f7xy};

struct interpolation2_ctx_inner {
	int	method;
	int	n;
	double	x_a, x_b,
		y_a, y_b;
	double	*f_a;
    double *X, *Y;
    array4 gamma;
	double	(*f)(double, double);
};

interpolation2_ctx interpolation2_create(int method, int n_x, int n_y, int k,
					 double x_a, double x_b,
					 double y_a, double y_b)
{
	interpolation2_ctx res_ptr;

    if ((k < 0 || k > 7) || (x_a >= x_b) ||
    (y_a >= y_b) || (method!=1 && method!=2) || (n_x < 5) || (n_y < 5))
    {
        return NULL;
    }

	res_ptr = malloc(sizeof(*res_ptr));

	res_ptr->method = method;
	res_ptr->n = n_x + n_y;
	res_ptr->x_a = x_a;
	res_ptr->x_b = x_b;
    res_ptr->y_a = y_a;
    res_ptr->y_b = y_b;
    res_ptr->gamma = create(n_x, n_y);

    res_ptr->X = (double *) malloc(n_x * sizeof (double ));
    for (int i = 0; i<n_x; i++)
    {
        res_ptr->X[i] = x_a + i*(x_b-x_a)/(n_x-1);
    }

    res_ptr->Y = (double *) malloc(n_y * sizeof (double ));
    for (int i = 0; i<n_y; i++)
    {
        res_ptr->Y[i] = y_a + i*(y_b-y_a)/(n_y-1);
    }

	switch (method) {
	case INTERPOLATION2_F_X:
        HermitInterpolation(Functions[k], DerivativesX[k], DerivativesY[k], DerivativesXY[k], res_ptr->X, res_ptr->Y, res_ptr->gamma);
		break;
	default:
        CubicSplineInterpolation(Functions[k], res_ptr->X, res_ptr->Y, res_ptr->gamma);
	}

	return res_ptr;
}

double interpolation2_calculate(interpolation2_ctx ctx, double x, double y)
{
    if (x < ctx->x_a || x > ctx->x_b || y < ctx->y_a || y > ctx->y_b)
    {
        fprintf(stderr,"Incorrect input\n");
        return -1;
    }
    int i = LowerBound(ctx->X, x, ctx->gamma.nx);
    int j = LowerBound(ctx->Y, y, ctx->gamma.ny);
	return EvaluatePolynomial(ctx->gamma.array[i][j], x - ctx->X[i], y - ctx->Y[j]);
}

void interpolation2_destroy(interpolation2_ctx ctx)
{
    destroy(ctx->gamma);
    free(ctx->X);
    free(ctx->Y);
	free(ctx);
}
