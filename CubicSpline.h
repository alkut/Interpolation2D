#include "Common.h"

void CubicSplineInterpolation(double (*Function)(double , double ), double *X, double *Y, array4 gamma);
array4 FindF_(double (*Function)(double , double ), double *nest_x, double *nest_y, array4 gamma);
double **FindG(double *nest_x, int nx);
double **FindF__(double (*Function)(double , double ), double *nest_x, double *nest_y, int nx, int ny);
matrix3 FindA_(double *nest_x, int nx);
matrix3 FindC(double *nest_x, int nx);