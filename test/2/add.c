#include <stdio.h>

/* Determine whether arguments can be added without overflow */
int uadd_ok(unsigned x, unsigned y) {
	unsigned sum = x+y;
	return sum >= x;
}

/* Determine whether arguments can be added without overflow */
int tadd_ok(int x, int y) {
	int sum = x+y;
	int neg_over = x <  0 && y <  0 && sum >= 0;
	int pos_over = x <  0 && y >  0 && sum <  0;
	return !neg_over && !pos_over;
}

/* Determine whether arguments can be multiplied without overflow */
int tmult_ok(int x, int y) {
	int p = x*y;
	/* Either x is zero, or dividing p by x gives y */
	return !x || p/x == y;
}

int tmult_ok64(int x, int y) {
	/* Compute product without overflow */
	int64_t pll = (int64_t) x*y;
	/* See if casting to int preserves value */
	return pll == (int) pll;
}


int main() {
	printf("result is %d.\n", uadd_ok(12,1));
	return 0;
}

