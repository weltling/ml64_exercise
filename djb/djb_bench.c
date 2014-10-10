
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <intrin.h>
#include <time.h>


/* asm implementation */
unsigned __int64 djb(const char *str, size_t len);

/* c implementation with unroll */
static __forceinline unsigned __int64 djb_c(const char *str, size_t len)
{
        register unsigned __int64 hash = 5381;

        /* variant with the hash unrolled eight times */
        for (; len >= 8; len -= 8) {
                hash = ((hash << 5) + hash) + *str++;
                hash = ((hash << 5) + hash) + *str++;
                hash = ((hash << 5) + hash) + *str++;
                hash = ((hash << 5) + hash) + *str++;
                hash = ((hash << 5) + hash) + *str++;
                hash = ((hash << 5) + hash) + *str++;
                hash = ((hash << 5) + hash) + *str++;
                hash = ((hash << 5) + hash) + *str++;
        }
        switch (len) {
                case 7: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
                case 6: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
                case 5: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
                case 4: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
                case 3: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
                case 2: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
                case 1: hash = ((hash << 5) + hash) + *str++; break;
                case 0: break;
		default: __assume(0);
        }
        return hash;
}

/* simple c implementation */
/*unsigned __int64 djb_c(const char *str, size_t len)
{
	size_t cnt = 0;
	unsigned __int64 hash = 5381;

	for (; cnt < len; cnt++) {
		hash = (hash << 5) + hash + str[cnt];
	}

	return hash;
}*/

/* helpers */
void
init_rand(void)
{
	static __int64 start = 0;
	__int64 freq, end;

	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&freq)) {
		return;
	}

	if (0 == start) {
		QueryPerformanceCounter((LARGE_INTEGER *)&start);
		srand((unsigned)time(NULL));
	} else {
		QueryPerformanceCounter((LARGE_INTEGER *)&end);
		srand((unsigned)((end - start)+rand()));
		start = end;
	}
}

#define LEN 64
char *
get_random_str(size_t *len)
{
	char *hello;
	size_t i;

	*len = LEN;
	init_rand();
	//*len = rand();

	hello = (char *)malloc(sizeof(char) * *len);

	for (i = 0; i < *len; i++) {
		hello[i] = 'A' + (rand() % 26);
	}

	return hello;
}


/* test */

static void
testit(double *diff1)
{
	char *hello;
	size_t len;
	unsigned __int64 h0 = 0, h1 = 0;
	__int64 freq, start, end;
	double elapsed0, elapsed1;
	int i = 0;
	FILETIME tod;

	hello = get_random_str(&len);

	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&freq)) {
		return;
	}

	QueryPerformanceCounter((LARGE_INTEGER *)&start);
	h0 = djb(hello, len);
	QueryPerformanceCounter((LARGE_INTEGER *)&end);
	elapsed0 = ((end - start)*1000000)/freq;

	QueryPerformanceCounter((LARGE_INTEGER *)&start);
	h1 = djb_c(hello, len);
	QueryPerformanceCounter((LARGE_INTEGER *)&end);
	elapsed1 = ((end - start)*1000000)/freq;

	*diff1 = elapsed0-elapsed1;

	if (h0 != h1) {
		printf("\nERROR: hash_asm != hash_c, %I64u != %I64u\n\n", h0, h1);
		exit(3);
	}

	printf("hash ok=%d len=%d, hash_asm = %I64u, hash_c=%I64u\n", h0 == h1, len, h0, h1);
	printf("elapsed_asm = %.3fus, elapsed_c=%.3fus, lapsed_asm-elapsed_c=%.3fus\n", elapsed0, elapsed1, *diff1);

	free(hello);

}

#define ROUNDS 1024
int
main(void)
{
	double sum1 = 0;
	int i;

	for (i = 0; i < ROUNDS; i++) {
		double tmp1;

		printf("\n");
		printf("round %d from %d:\n", i, ROUNDS);

		testit(&tmp1);

		sum1 += tmp1;

//		printf("round %d from %d: sum1=%.2fus, sum2=%.2fus\n", i, ROUNDS, sum1, sum2);
	}

	printf("\n");
	printf("mean elapsed_asm-elapsed_c=%.2fus\n", sum1/ROUNDS);

	return 0;	
}

