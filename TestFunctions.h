#include <math.h>

static double f0(double x, double y)
{
    return x * y;
}

static double f0x(double x, double y)
{
    return y;
}

static double f0y(double x, double y)
{
    return x;
}

static double f0xy(double x, double y)
{
    return 1.0;
}

static double f1(double x, double y)
{
    return x * y;
}

static double f1x(double x, double y)
{
    return y;
}

static double f1y(double x, double y)
{
    return x;
}

static double f1xy(double x, double y)
{
    return 1.0;
}

static double f2(double x, double y)
{
    return x * y;
}

static double f2x(double x, double y)
{
    return y;
}

static double f2y(double x, double y)
{
    return x;
}

static double f2xy(double x, double y)
{
    return 1.0;
}

static double f3(double x, double y)
{
    return x * y;
}

static double f3x(double x, double y)
{
    return y;
}

static double f3y(double x, double y)
{
    return x;
}

static double f3xy(double x, double y)
{
    return 1.0;
}

static double f4(double x, double y)
{
    return sqrt(x * x + y * y);
}

static double f4x(double x, double y)
{
    return x / sqrt(x * x + y * y);
}

static double f4y(double x, double y)
{
    return y / sqrt(x * x + y * y);
}

static double f4xy(double x, double y)
{
    double tmp = sqrt(x * x + y * y);
    tmp *= tmp*tmp;
    return - x * y / tmp;
}

static double f5(double x, double y)
{
    return x * y;
}

static double f5x(double x, double y)
{
    return y;
}

static double f5y(double x, double y)
{
    return x;
}

static double f5xy(double x, double y)
{
    return 1.0;
}

static double f6(double x, double y)
{
    return x * y;
}

static double f6x(double x, double y)
{
    return y;
}

static double f6y(double x, double y)
{
    return x;
}

static double f6xy(double x, double y)
{
    return 1.0;
}

static double f7(double x, double y)
{
    return x * y;
}

static double f7x(double x, double y)
{
    return y;
}

static double f7y(double x, double y)
{
    return x;
}

static double f7xy(double x, double y)
{
    return 1.0;
}