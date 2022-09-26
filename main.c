#include <stdio.h>
#include "interpolation2.h"

int main()
{
	interpolation2_ctx	ctx;

	ctx = interpolation2_create(INTERPOLATION2_METHOD1,
				    10, 7, 1, -3.5, 8.1, 2.8, 3.9);

	printf("val: %lf\n", interpolation2_calculate(ctx, 2.7, 3));

	interpolation2_destroy(ctx);

	return 0;
}
