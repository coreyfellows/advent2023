#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include "../misc/misc.h"


//#define DEBUG

#ifdef DEBUG
	#define dprintf(...) printf(__VA_ARGS__)
#else
	#define dprintf(...)
#endif

//#define SAMPLE
#ifdef SAMPLE
	#define FILENAME "sample.txt"
#else
 	#define FILENAME "input.txt"
#endif


uint calcuate_differences(int* line, uint count) {
	uint zeros = 0;
	for (int i = 0; i < count; ++i) {
		dprintf("%d ", line[i]);
		if (line[i] == 0 ) zeros++;
	}

	dprintf("\n");

	if (zeros == count) {
		return 0;
	}

	for (int i = 1; i < count; ++i) {
		line[i-1] = line[i] - line[i-1];
	}
	return 1;
}

int* parse_line(char* lineptr, uint* length) {
	int* int_array = NULL;
	*length = 0;

	uint int_count = 1;
	char* tmp = lineptr;

	while (*tmp && *tmp != '\n') *tmp++ == ' ' && int_count++;
	int_array = malloc(sizeof(int) * int_count);
	for (uint i = 0; i < int_count; ++i) {
		lineptr += 1 + parse_int(lineptr, int_array + i);
	}
	*length = int_count;
	return int_array;
}

int process_line_1(int* line, uint count ) {

	int last = line[count-1];

	if (!calcuate_differences(line, count)) {
		//they're all zeroes, base case.
		return 0;
	}
	// find differences of next line and add it to the last value of this line.
	return last + process_line_1(line, count-1);
}

int process_line_2(int* line, uint count ) {

	int first = line[0];
	if (!calcuate_differences(line, count)) {
		//they're all zeroes, base case.
		return 0;
	}
	return first - process_line_2(line, count-1);
}


void solution_1() {
char* lineptr = NULL;
	int total = 0;
	int total_2 = 0;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen(FILENAME, "r");
	while( (read = getline(&lineptr, &n, f)) != -1 ) {

		uint count = 0;
		int* line = parse_line(lineptr, &count);
		total += process_line_1(line, count);
		free(line);
		line = parse_line(lineptr, &count);
		total_2 += process_line_2(line, count);
		free(line);

		lineptr = NULL;
		free(lineptr);
	}
	printf("solution 1: %d\n", total);
	printf("solution 1: %d\n", total_2);
	fclose(f);
}

int main(void) {
	solution_1();
	return 0;
}

