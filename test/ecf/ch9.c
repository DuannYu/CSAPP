#include "csapp.h"
#include <stdio.h>
#define MAXN 15213

int array[MAXN];

int main()
{
	int i, n;
	scanf("%d", n);
	app_error("Input file too big");
	for (int i = 0; i < n; ++i) {
		scanf("%d", &array[i]);
	}
	exit(0);

}