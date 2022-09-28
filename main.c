#include <stdio.h>
#include <math.h>
#include "interpolation2.h"

int main()
{
    int nx = 50, ny = 50;
    double x = 0.18, y = 0.76845;
	interpolation2_ctx	ctx;

	ctx = interpolation2_create(INTERPOLATION2_METHOD1,
				    nx, ny, 6, -0.5, 0.8, -1.1, 0.9);

	printf("val: %lf\n", interpolation2_calculate(ctx, x, y));


    interpolation2_ctx	ctx2;

    ctx2 = interpolation2_create(INTERPOLATION2_METHOD2,
                                nx, ny, 6, -0.5, 0.8, -1.1, 0.9);

    printf("val: %lf\n", interpolation2_calculate(ctx2, x, y));

    printf("true val: %lf\n", exp(x * x - y * y));

	interpolation2_destroy(ctx);
    interpolation2_destroy(ctx2);

	return 0;
}
