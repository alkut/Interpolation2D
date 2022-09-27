void HermitInterpolation(double (*function)(double , double ), double (*derivative_x)(double , double ),
                         double (*derivative_y)(double , double ), double (*derivative_xy)(double , double ),
                         double *nest_x, double *nest_y, array4 gamma);
array4 FindF(double (*function)(double , double ), double (*derivative_x)(double , double ),
             double (*derivative_y)(double , double ), double (*derivative_xy)(double , double ),
             double *nest_x, double *nest_y, array4 gamma);
