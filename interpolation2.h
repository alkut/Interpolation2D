#ifndef __INTERPOLATION2_H
#define __INTERPOLATION2_H

struct interpolation2_ctx_inner;
typedef struct interpolation2_ctx_inner *interpolation2_ctx;

#define INTERPOLATION2_METHOD1	1
#define INTERPOLATION2_METHOD2	2

#define INTERPOLATION2_F_1		0
#define INTERPOLATION2_F_X		1
#define INTERPOLATION2_F_Y		2
#define INTERPOLATION2_F_XY		3
#define INTERPOLATION2_F_SQRX2Y2	4
#define INTERPOLATION2_F_X2Y2		5
#define INTERPOLATION2_F_EX2Y2		6
#define INTERPOLATION2_1DIVX2Y2		7

interpolation2_ctx interpolation2_create(int method, int n_x, int n_y, int k,
					 double x_a, double x_b,
					 double y_a, double y_b);

double interpolation2_calculate(interpolation2_ctx ctx, double x, double y);

void interpolation2_destroy(interpolation2_ctx ctx);

#endif
