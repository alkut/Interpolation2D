#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "interpolation2.h"

double test(double x, double y);

double test(double x, double y)
{
   return 1.0 / (25.0 * (x*x + y*y) + 1);   
}

double g(double x, double y);

double g(double x, double y)
{
    x += 0;
    y += 0;
    return 1.0;
}

double g2(double x, double y);

double g2(double x, double y)
{
    x += 0;
    y += 0;
    return y;
}

void Show_error(interpolation2_ctx ctx, double start_pointX, double end_pointX, double start_pointY, double end_pointY, int countX, int countY, double (*function)(double, double));

void Show_error(interpolation2_ctx ctx, double start_pointX, double end_pointX, double start_pointY, double end_pointY, int countX, int countY, double (*function)(double, double))
{
    double ans, stepX, stepY, tmp, tmpX, tmpY;
    ans = 0.0;
    stepX = (end_pointX - start_pointX) / (countX - 1);
    stepY = (end_pointY - start_pointY) / (countY - 1);
    
    for (int i = 0; i < countX - 1; ++i)
    {
        for (int j = 0; j < countY - 1; ++j)
        {
        tmpX = start_pointX + i * stepX;
        tmpY = start_pointY + j * stepY;
        tmp = fabs(function(tmpX, tmpY) - interpolation2_calculate(ctx, tmpX, tmpY));
        if (ans < tmp)
            ans = tmp;
        }
    }
    
    printf("C0 Norm of error: %10.15e\n", ans);
    
}

int main(void)
{
    int nx = 5, ny = 5, k = 2;
    double x_a = -0.5, x_b = 0.8, y_a = -1.1, y_b = 0.9;

	interpolation2_ctx	ctx;

	ctx = interpolation2_create(INTERPOLATION2_METHOD1,
				    ny, nx, k, x_a, x_b, y_a, y_b);

	Show_error(ctx, x_a, x_b, y_a, y_b, 6, 6, g2);

	interpolation2_destroy(ctx);

	return 0;
}
