
#include <stdio.h>


unsigned __int64 djb(const char *str, size_t len);


unsigned __int64 djb_c(const char *str, size_t len)
{
	size_t cnt = 0;
	unsigned __int64 hash = 5381;

	for (; cnt < len; cnt++) {
		hash = (hash << 5) + hash + str[cnt];
	}

	return hash;
}


#define STR "hello world"
#define LEN strlen(STR)

int
main(void)
{
	unsigned __int64 h0, h1;

	h1 = djb_c(STR, LEN);
	h0 = djb(STR, LEN);

	printf("%I64u %I64u %d %I64u\n", h0, h1, h0 > h1, h0 > h1 ? h0 - h1 : h1 - h0);

	return 1;
}

