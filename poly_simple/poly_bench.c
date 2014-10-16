
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <windows.h>
#include <intrin.h>

/* asm implementation */

size_t safe_address(size_t nmemb, size_t size, size_t offset, int *overflow);


/* php implementation */
static __forceinline size_t safe_address_c(size_t nmemb, size_t size, size_t offset, int *overflow)
{
        size_t res = nmemb * size + offset;
        double _d  = (double)nmemb * (double)size + (double)offset;
        double _delta = (double)res - _d;

        if ((_d + _delta ) != _d) {
                *overflow = 1;
                return 0;
        }
        *overflow = 0;
        return res;
}


#define TESTS 4
#define REPEAT 1024

int
main(int argc, char **argv)
{

	int ovfl0, ovfl1;
	size_t iret0, iret1;
	__int64 delta = 0;

	int i, repeat;
	__int64 nmemb[TESTS] = {_I64_MAX, INT_MAX, 42, 32};
	__int64 size[TESTS] = {_I64_MAX, 20, 42, 32};
	__int64 offset[TESTS] = {_I64_MAX, UINT_MAX, 42, 32};

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

		__int64 elapsed_diff;
		int rnd;
		size_t lnmemb, lsize, loffset;

		printf("%d %d\n", i, repeat);
		
		srand((unsigned)time(NULL));
		rnd = rand();
		if (argc > 1) {
			rnd += atoi(argv[1]);
		}
		else {
			rnd += delta;
		}

		lnmemb = nmemb[i] + rnd;
		lsize = size[i] - rnd;
		loffset = offset[i] + rnd;


		QueryPerformanceCounter((LARGE_INTEGER *)&start);
		iret0 = safe_address(lnmemb, lsize, loffset, &ovfl0);
		QueryPerformanceCounter((LARGE_INTEGER *)&end);
		elapsed0 = end - start;

		/* This will help to randomize the result, processor should become some free circles */
		//Sleep(1);

		QueryPerformanceCounter((LARGE_INTEGER *)&start);
		iret1 = safe_address(lnmemb, lsize, loffset, &ovfl1);
		QueryPerformanceCounter((LARGE_INTEGER *)&end);
		elapsed1 = end - start;

		/* check if both overflow detection worked */
		if (ovfl0 != ovfl1) {
			printf("ERROR: unmatched overflow for nmemb=%I64u size=%I64u offset=%I64u\n", lnmemb, lsize, loffset);
			return 3;
		}

		/* now we're sure both resulted the same overflow flag */
		printf("ovfl %d: ", ovfl0);
		printf("nmemb=%I64u size=%I64u offset=%I64u ", lnmemb, lsize, loffset);

		elapsed_diff = elapsed0 - elapsed1;

		if (ovfl0) {
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

	}

	printf("\n\nmean elapsed_asm - elapsed_c=%.7f ticks\n", sum/(double)(TESTS*repeat));

	return 0;
}

