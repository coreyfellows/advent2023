#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <math.h>
#include "../misc/misc.h"

#define DEBUG

#ifdef DEBUG
	#define dprintf(...) printf(__VA_ARGS__)
#else
	#define dprintf(...)
#endif

#define SAMPLE
#ifdef SAMPLE
	#define FILENAME "sample.txt"
	#define WIDTH 10
	#define HEIGHT 10
#else
	#define WIDTH 140
	#define HEIGHT 140
 	#define FILENAME "input.txt"
#endif

void solution() {
	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen(FILENAME, "r");

	while( (read = getline(&lineptr, &n, f)) != -1 ) {
		printf("%s", lineptr);
		lineptr = NULL;
		free(lineptr);
	}

	fclose(f);
}

int main(void) {

	struct timeval t0, t1;
	gettimeofday(&t0, 0);
	solution();
	gettimeofday(&t1, 0);
	long elapsed = (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
	printf("%ld", elapsed);
	return 0;
}

