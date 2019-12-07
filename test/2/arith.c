#include <stdio.h>

int div16(int x) {
	int bias = (x >> 31) & 0xF;
	/* 保证向0舍入。 */
	return (x + bias) >> 4;
}

int main() {
	int x = -2**32;
	printf("%ld\n", x);
	return 0;
}