#include <stdio.h>

float sum_elements(float a[], unsigned length) {
	int i;
	float result = 0;

	for (i = 0; i < length - 1; i++)
		result += a[i];
	printf("The sum is : %f.\n", result);
}

int main() {
	float a[5] = {1.0, 2.0, 3.0, 4.1, 5.0};
	sum_elements(a, 5);
	printf("%d", (0u-1));
}