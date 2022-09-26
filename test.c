#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "interpolation2.h"

#define ASSERT_CM(val, msg) assert((#msg, val))

void run_tests(void);

int main()
{
	run_tests();

	return 0;
}

#define TEST_NEGATIVE_INIT(m, n_x, n_y, k, x_a, x_b, y_a, y_b) { \
		puts("    Wrong arguments test - m=" #m ", n_x=" #n_x ", n_y=" #n_y ", k=" #k \
				     ", Xab x Yab = [" #x_a ";" #x_b "] x [" #y_a ";" #y_b "]"); \
		interpolation2_ctx ctx = interpolation2_create(m, n_x, n_y, k, x_a, x_b, y_a, y_b); \
		ASSERT_CM(!ctx, \
			  interpolation2_create(m, n_x, n_y, k, x_a, x_b, y_a, y_b) must return 'NULL'); \
		puts("    Ok");}

#define RUN_TEST(name) { \
			puts("TEST start    : " #name); \
			test_##name(); \
			puts("TEST finished : " #name);}

void test_negative_init(void)
{
	/* wrong method */
	TEST_NEGATIVE_INIT(0, 5, 5, 3, 0, 5, 0, 5)
	TEST_NEGATIVE_INIT(3, 5, 5, 3, 0, 5, 0, 5)
	/* wrong n count */
	TEST_NEGATIVE_INIT(1, 2, 5, 3, 0, 5, 0, 5)
	TEST_NEGATIVE_INIT(1, 5, 2, 3, 0, 5, 0, 5)
	/* wrong func number */
	TEST_NEGATIVE_INIT(1, 5, 5, -2, 0, 5, 0, 5)
	TEST_NEGATIVE_INIT(1, 5, 5, 8, 0, 5, 0, 5)
	/* wrong [a, b] */
	TEST_NEGATIVE_INIT(1, 5, 5, 3, 5, 0, 0, 5)
	TEST_NEGATIVE_INIT(1, 5, 5, 3, 0, 5, 5, 0)
}

#define TEST_PERFORMANCE(m, n_x, n_y, k, x_a, x_b, y_a, y_b, req) { \
		puts("    Performance - m=" #m ", n_x=" #n_x ", n_y=" #n_y ", k=" #k \
				     ", Xab x Yab = [" #x_a ";" #x_b "] x [" #y_a ";" #y_b "]; reqs=" #req); \
		struct timespec	tv1, tv2, tv3; \
		int tmp1, tmp2; double x_len = (x_b) - (x_a), y_len = (y_b) - (y_a); \
		timespec_get(&tv1, TIME_UTC); \
		interpolation2_ctx ctx = interpolation2_create(m, n_x, n_y, k, x_a, x_b, y_a, y_b); \
		timespec_get(&tv2, TIME_UTC); \
		for (int i = 0; i < (req); i++) \
			interpolation2_calculate(ctx, (x_a) + x_len * i / (req), (y_a) + y_len * ((req) - i) / (req)); \
		timespec_get(&tv3, TIME_UTC); \
		tmp1 = (tv2.tv_sec - tv1.tv_sec) * 1000 + tv2.tv_nsec / 1000000 - tv1.tv_nsec / 1000000; \
		tmp2 = (tv3.tv_sec - tv2.tv_sec) * 1000 + tv3.tv_nsec / 1000000 - tv2.tv_nsec / 1000000; \
		printf("    Time: %d.%03d -  %d.%03d\n", tmp1 / 1000, tmp1 % 1000, tmp2 / 1000, tmp2 % 1000); \
		interpolation2_destroy(ctx);}

void test_performance(void)
{
	/* 10^7 approximations */
	TEST_PERFORMANCE(1, 100, 100, 5, -100, 200, -200, 100, 10000000)
	TEST_PERFORMANCE(2, 100, 100, 5, -100, 200, -200, 100, 10000000)
	/* 10^7 known points */
	TEST_PERFORMANCE(1, 3000, 3000, 5, -100, 200, -200, 100, 1)
	TEST_PERFORMANCE(2, 3000, 3000, 5, -100, 200, -200, 100, 1)
	/* 10^7 & 10^7 */
	TEST_PERFORMANCE(1, 3000, 3000, 3, -100, 200, -200, 100, 10000000)
	TEST_PERFORMANCE(2, 3000, 3000, 5, -100, 200, -200, 100, 10000000)
}

void run_tests(void)
{
	RUN_TEST(negative_init)
	RUN_TEST(performance)
}
