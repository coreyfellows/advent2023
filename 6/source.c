#include <stdio.h>
#include <stdlib.h>

#include <math.h>

int is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_digit(char c) {
	return c >= '0' && c <= '9';
}

#define uint unsigned int

#define DEBUG

#ifdef DEBUG
	#define dprintf(...) printf(__VA_ARGS__)
#else
	#define dprintf(...)
#endif

int startswith(char* string, char* start) {
	// if we've fallen off the word, we've found it
	if (!*start) {
		return 1;
	}

	// if we've fallen off the string we haven't
	if(!*string) {
		return 0;
	}
	//and if the strings dont match, we havent found it either.
	if (*string != *start) {
		return 0;
	}

	return startswith(string+1, start+1);
}

uint ipow(uint a, uint b) {

	if (!b) {
		return 1;
	}
	uint result = a;
	while (--b) {
		result *= a;
	}
	return result;
}
// parses int at start and stores result in result;
// returns length of the string representation of the number.
uint parse_int(char* start, uint* result) {
	if (*start > '9' || *start < '0') {
		*result = 0;
		return 0;
	}
	uint power = parse_int(start+1, result);
	*result += ipow(10, power) * (*start - '0');
	return power + 1;
}

uint parse_next_int(char** start, uint* found ) {
	while (is_digit(**start)) start++;
	while(**start && '\n' != **start && !is_digit(**start)) ++*start;
	if (!is_digit(**start)) {
		*found = 0;
		return 0;
	}

	*found = 1;
	uint out = 0;
	*start += parse_int(*start, &out);
	return out;
}


uint find_solutions(double time, double distance) {
	uint max_time = (uint)floor(-.01+(time + sqrt((time*time) - (4.0*distance)))/2.0);
	uint min_time = (uint)ceil(.01+(time - sqrt((time*time) - (4.0*distance)))/2.0);
	dprintf("%d - %d = %d\n", max_time, min_time, 1+max_time-min_time);
	return 1+max_time-min_time;
}


void solution_1() {

	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen("input.txt", "r");

	int total = 1;

	char* times = NULL;
	char* distances = NULL;
	while( (read = getline(&lineptr, &n, f)) != -1 ) {
		if (startswith(lineptr, "Time")) {
			times = lineptr;
		}
		if (startswith(lineptr, "Distance")) {
			distances = lineptr;
		}
		lineptr = NULL;
	}

	uint found = 1;
	while (found) {
		uint time = parse_next_int(&times, &found);
		uint distance = parse_next_int(&distances, &found);
		if (found) {
			total *= find_solutions((double)time, (double)distance);
		}
	}
	printf("solution 1: %d", total);
	fclose(f);
}

void solution_2() {
	printf("solution 2: %d", find_solutions(40817772.0, 219101213651089.0));
}

int main(void) {
	solution_1();
	solution_2();
	return 0;
}

