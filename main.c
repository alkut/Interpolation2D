#include <stdio.h>
#include "interpolation2.h"

int main()
{
    int nx = 50, ny = 50;
    double x = 4.18, y = 3.76845;
	interpolation2_ctx	ctx;

	ctx = interpolation2_create(INTERPOLATION2_METHOD1,
				    nx, ny, 6, -3.5, 8.1, 2.8, 4.9);

	printf("val: %lf\n", interpolation2_calculate(ctx, x, y));


    interpolation2_ctx	ctx2;

    ctx2 = interpolation2_create(INTERPOLATION2_METHOD2,
                                nx, ny, 4, -3.5, 8.1, 2.8, 4.9);

    printf("val: %lf\n", interpolation2_calculate(ctx2, x, y));

	interpolation2_destroy(ctx);
    interpolation2_destroy(ctx2);

	return 0;
}
