#include <stdio.h>
#include "interpolation2.h"

int main()
{
	interpolation2_ctx	ctx;

	ctx = interpolation2_create(INTERPOLATION2_METHOD1,
				    50, 50, 4, -3.5, 8.1, 2.8, 4.9);

	printf("val: %lf\n", interpolation2_calculate(ctx, 3, 4));

	interpolation2_destroy(ctx);

	return 0;
}
