int LowerBound(double *arr, double x, int n);
double EvaluatePolynomial(double **arr, double dx, double dy);
void Calculate(double **gamma, double **A, double **TMP, double ** f, double dx, double dy);
void FindA(double **A, double h);

typedef struct array4
{
    double ****array;
    double *memory;
    int nx, ny;
} array4;

array4 create(int nx, int ny);

void destroy(array4 arr);

