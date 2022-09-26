#include <stdlib.h>
#include <stdio.h>
#include "Common.h"
#include "interpolation2.h"

struct interpolation2_ctx_inner {
	int	method;
	int	n;
	double	x_a, x_b,
		y_a, y_b;
	double	*f_a;
    double *X, *Y;
    array4 gamma;
    double **A, **TMP;
	double	(*f)(double, double);
};

static double f1(double x, double y)
{
	return x * y;
}

static double f3(double x, double y)
{
	return 1 / (1 + y * y);
}

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
    res_ptr->gamma = create(n_x, n_y);

	switch (k) {
	case INTERPOLATION2_F_X:
		res_ptr->f = f1;
		break;
	default:
		res_ptr->f = f3;
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
