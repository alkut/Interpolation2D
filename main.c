#include <stdio.h>
#include <math.h>
#include "interpolation2.h"
#include "TestFunctions.h"

int main()
{
    const double (*Functions[8])(double, double) = {f0, f1, f2, f3, f4, f5, f6, f7};
    int nx = 50, ny = 50, k = 6;
    double x = 0.76845, y = 0.08, x_a = -0.5, x_b = 0.8, y_a = -1.1, y_b = 0.9;

	interpolation2_ctx	ctx;

	ctx = interpolation2_create(INTERPOLATION2_METHOD1,
				    nx, ny, k, x_a, x_b, y_a, y_b);

	printf("val1: %lf\n", interpolation2_calculate(ctx, x, y));


    interpolation2_ctx	ctx2;

    ctx2 = interpolation2_create(INTERPOLATION2_METHOD2,
                                nx, ny, k, x_a, x_b, y_a, y_b);

    printf("val2: %lf\n", interpolation2_calculate(ctx2, x, y));

    printf("true val: %lf\n", Functions[k](x, y));

	interpolation2_destroy(ctx);
    interpolation2_destroy(ctx2);

	return 0;
}
