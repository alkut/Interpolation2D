#ifndef COMMON_
#define COMMON_
int LowerBound(double *arr, double x, int n);
double EvaluatePolynomial(double **arr, double dx, double dy);
void Calculate(double **gamma, double **A, double **TMP, double ** f, double dx, double dy);
void FindA(double **A, double h);

double ** alloc(int n, int m);
void dealloc(double** ptr, int n);
typedef struct matrix3
{
    double *main, *up, *down;
    int n;
} matrix3;

matrix3 create3(int n);
void gauss(matrix3 m, double **OutMatrix);
void destroy3(matrix3 m);


typedef struct array4
{
    double ****array;
    double *memory;
    int nx, ny;
} array4;

array4 create(int nx, int ny);

void destroy(array4 arr);

#endif
