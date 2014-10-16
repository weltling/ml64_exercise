
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <windows.h>
#include <intrin.h>

/* asm implementation */
//__int8 int64_mul_ovfl(__int64 a, __int64 b, __int64 *lval);

__int8 mult(__int64 a, __int64 b, __int64 *lval, double *dval);
/* XXX in further some tries to embed a mixed c variant, not used however */

/*{
	__int8 ovfl;

	ovfl = int64_mul_ovfl(a, b, lval);

	if (ovfl) {
		*dval = (double)a * (double)b;
	}

	return ovfl;
}*/

/*{
	__m128d ad, bd, rd;
	__debugbreak();
	__int8 ovfl = 0;

	ad = _mm_cvtsi64_ss(ad, a);
	bd = _mm_cvtsi64_ss(bd, b);
	rd = _mm_mul_ss(ad, bd);

	//ovfl = rd > _I64_MAX || rd < _I64_MIN;

	if (ovfl) {
		//*dval = rd;
	} else {
		*lval = _mm_cvttss_si64(rd);
	}

	return ovfl;
}*/


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

#define TESTS 24
#define REPEAT 16

int
main(int argc, char **argv)
{

	__int8 ovfl0, ovfl1;
	__int64 iret0, iret1;
	double dret0, dret1;
	__int64 delta = 0;

	int i, j, repeat;
	__int64 val[TESTS] = {1, 1024, 42, 33, 77, _I64_MAX, _I64_MIN, INT_MAX, INT_MIN, SIZE_MAX, 0, UINT_MAX, 13
		-0, 1023, _I64_MAX*_I64_MAX, _I64_MIN - _I64_MIN, 12, 77, 33, 234532, -1000, 2345, -42 };

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

	if (2 > argc) {
		printf("input some delta: ");
		scanf("%I64d", &delta);
	}

	for (repeat = 0; repeat < REPEAT; repeat++)
	for (i = 0; i < TESTS; i++) {
		for (j = 0; j < TESTS; j++) {

			__int64 elapsed_diff, a, b;
			int rnd;
			
			srand((unsigned)time(NULL));
			rnd = rand();
			if (argc > 1) {
				rnd += atoi(argv[1]);
			}
			else {
				rnd += delta;
			}

			a = val[i] - rnd;
			b = val[j] + rnd;
			/*a = 2147452546;
			b = 2147514748;*/
			/*a = -45483;
			b = -9223372036854730325;*/
			/*__debugbreak();*/


			QueryPerformanceCounter((LARGE_INTEGER *)&start);
			ovfl0 = mult(a, b, &iret0, &dret0);
			QueryPerformanceCounter((LARGE_INTEGER *)&end);
			elapsed0 = end - start;

			/* This will help to randomize the result, processor should become some free circles */
			Sleep(1);

			QueryPerformanceCounter((LARGE_INTEGER *)&start);
			ovfl1 = mult_c(a, b, &iret1, &dret1);
			QueryPerformanceCounter((LARGE_INTEGER *)&end);
			elapsed1 = end - start;

			/* check if both overflow detection worked */
			if (ovfl0 != ovfl1) {
				printf("ERROR: unmatched overflow for %I64d and %I64d\n", val[i], val[j]);
				return 3;
			}

			/* now we're sure both resulted the same overflow flag */
			printf("ovfl %d: ", ovfl0);
			printf("a=%I64d b=%I64d ", a, b);

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

