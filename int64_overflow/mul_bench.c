
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <windows.h>

/* asm implementation */
__int8 mult(__int64 a, __int64 b, __int64 *lval, double *dval);

/* php implementation */
__int8
mult_c(__int64 a, __int64 b, __int64 *lval, double *dval)
{
	__int8 ovfl;
	__int64 lres;
	double dres, delta;

	lres  = a * b;
	dres  = (double)a * (double)b;
	delta = (double)lres - dres;

	if (ovfl = ((dres + delta) != dres)) {
		*dval = dres;
	} else {
		*lval = lres;
	}

	return ovfl;
}

#define TESTS 10
#define REPEAT 128

int
main(int argc, char **argv)
{

	__int8 ovfl0, ovfl1;
	__int64 iret0, iret1;
	double dret0, dret1;

	int i, j, repeat;
	__int64 val[TESTS] = {1, 1024, 42, 33, 77, _I64_MAX, _I64_MIN, INT_MAX, INT_MIN, SIZE_MAX};

	/* timing related */
	__int64 sum = 0;
	__int64 freq, start, end;
	__int64 elapsed0, elapsed1;


	/*srand((unsigned)time(NULL));
	for (i = 0; i < TESTS; i++) {
		val[i] -= rand();
		val[i] += rand();
	}*/

	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&freq)) {
		return;
	}

	for (repeat = 0; repeat < REPEAT; repeat++)
	for (i = 0; i < TESTS; i++) {
		for (j = 0; j < TESTS; j++) {

			__int64 elapsed_diff;
			int rnd = rand();
			if (argc > 1) {
				rnd += atoi(argv[1]);
			}

			QueryPerformanceCounter((LARGE_INTEGER *)&start);
			ovfl0 = mult(val[i]-rnd, val[j]+rnd, &iret0, &dret0);
			QueryPerformanceCounter((LARGE_INTEGER *)&end);
			elapsed0 = end - start;

			QueryPerformanceCounter((LARGE_INTEGER *)&start);
			ovfl1 = mult_c(val[i]-rnd, val[j]+rnd, &iret1, &dret1);
			QueryPerformanceCounter((LARGE_INTEGER *)&end);
			elapsed1 = end - start;

			/* check if both overflow detection worked */
			if (ovfl0 != ovfl1) {
				printf("ERROR: unmatched overflow for %I64d and %I64d\n", val[i], val[j]);
				return 3;
			}

			/* now we're sure both resulted the same overflow flag */
			printf("ovfl %d: ", ovfl0);

			elapsed_diff = elapsed0 - elapsed1;

			if (ovfl0) {
				if (dret0 != dret1) {
					printf("ret0!=ret1  %f!=%f ", dret0, dret1);
					return 3;
				} else {
					printf("ret0=%f ", dret0);
					printf("ret1=%f ", dret1);
				}
			} else {
				if (iret0 != iret1) {
					printf("ret0!=ret1  %I64d!=%I64d ", iret0, iret1);
					return 3;
				} else {
					printf("ret0=%I64d ", iret0);
					printf("ret1=%I64d ", iret1);
				}
			}

			printf("\n");


			sum += elapsed_diff;

			/*printf("ovfl0=%d ", ovfl0);
			if (ovfl0) {
				printf("ret=%f\n", dret0);
			} else {
				printf("ret=%I64d\n", iret0);
			}
			

			printf("ovfl1=%d ", ovfl1);
			if (ovfl1) {
				printf("ret=%f\n", dret1);
			} else {
				printf("ret=%I64d\n", iret1);
			}*/
		}
	}

	printf("\n\nmean elapsed_asm - elapsed_c=%.7f ticks\n", sum/(double)(TESTS*TESTS));

	return 0;
}

