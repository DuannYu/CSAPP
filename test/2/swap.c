#include <stdio.h>

void inplace_swap(int *x, int *y) {

	*y = *x ^ *y;
	*x = *x ^ *y;
	*y = *x ^ *y;
}

void reverse_array(int a[], int cnt) {
	int first, last;
	for (first = 0, last = cnt-1; first < last; first++, last--)
		inplace_swap(&a[first], &a[last]);

	for (first = 0; first < cnt; first++)
		printf("%d ", a[first]);
	printf("\n");
}

int main() {
	int a[5] = {1, 2, 3, 4, 5};
	// inplace_swap(&ix, &iy);
	// printf("After swap: %d & %d\n", ix, iy);
	reverse_array(a, 5);
}

