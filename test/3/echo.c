#include <stdio.h>

/* Implementation of library fucntion gets() */
char *gets(char *s) {
	int c;
	char *dest = s;
	while ((c = getchar()) != '\n' && c != EOF)
		*dest++ = c;
	if ( c == EOF && dest == s)
		/* No characters read */
		return NULL;
	*dest++ = '\0';
	return s;
}

void echo() {
	char buf[8];
	gets(buf);
	puts(buf); 
}