library:
	gcc -shared -fPIC interpolation2.c Common.c HermitInterpolation.c CubicSpline.c -lm -o libinter2.so
compile:
	gcc main.c -Wl,-rpath='./' -L. -linter2 -lm -o main
test:
	gcc test.c -Wl,-rpath='./' -L. -linter2 -o test_inter
run_test:library test
	time ./test_inter
run_main:library compile
	time ./main $N