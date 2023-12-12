#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


uint matches_range(char* start, int range) {
	if(*start == ' ' || *start == '.') {
		return 0;
	}

	while(range && (*start == '#' || *start == '?')){
		start++;
		range--;
	}

	if (range) return 0;
	//printf("%s %ld\n", start, range);

	return start[0] == ' ' || start[0] == '?' || start[0] == '.';
}

uint find_arrangements(char* start, uint* ranges) {
	printf("with range %ld starting at %s", *ranges, start);
	uint count = 0;
	if (*ranges == 0) {
		return 1;
	}

	while(*start == '?' || *start == '#' || *start == '.') {
		if (matches_range(start, *ranges)) {
			printf("matched range %ld at %s", *ranges, start);
			count += find_arrangements(start + (*ranges) + 1, ranges+1);
		}
		start++;
	}
	return count;
}


uint process_line(char* lineptr) {
	uint number_of_ranges = 1;

	char* ranges_start = lineptr;
	while(*ranges_start++ != ' ');


	char* tmp = ranges_start;
	while(*tmp++) {
		number_of_ranges+= *tmp == ',';
	}

	//dprintf("%s %d \n", ranges_start, number_of_ranges);

	uint* ranges = malloc(sizeof(uint) * (1+number_of_ranges));
	ranges[number_of_ranges] = 0;

	tmp = ranges_start;
	for (int i = 0; i< number_of_ranges; ++i) {
		tmp += 1 + parse_uint(tmp, ranges+i);
//		printf("%ld\n", ranges[i]);
	}

	uint arrangments = find_arrangements(lineptr, ranges);
	dprintf("%ld -> %s", arrangments, lineptr);
}

void solution() {
	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen(FILENAME, "r");

	while( (read = getline(&lineptr, &n, f)) != -1 ) {
		//printf("%s", lineptr);
		process_line(lineptr);
		lineptr = NULL;
		free(lineptr);
	}

	fclose(f);
}

int main(void) {

	// printf("should be 1\n");
	// printf("%ld\n", matches_range("#...", 1));
	// printf("%ld\n", matches_range("?...", 1));
	// printf("%ld\n", matches_range("#?...", 1));
	// printf("%ld\n", matches_range("#?...", 2));
	// printf("%ld\n", matches_range("?#...", 2));
	// printf("%ld\n", matches_range("?#?...", 3));
	// printf("%ld\n", matches_range("#?#...", 3));
	// printf("%ld\n", matches_range("?#?#...", 4));
	// printf("%ld\n", matches_range("#?#?...", 4));
	// printf("%ld\n", matches_range("???????", 2));
	// printf("%ld\n", matches_range("??????", 2));
	// printf("%ld\n", matches_range("?????", 2));
	// printf("%ld\n", matches_range("????", 2));
	// printf("%ld\n", matches_range("???", 2));
	// printf("%ld\n", matches_range("?? ", 2));

	// printf("%ld\n", matches_range("???????", 3));
	// printf("%ld\n", matches_range("??????", 3));
	// printf("%ld\n", matches_range("?????", 3));
	// printf("%ld\n", matches_range("????", 3));
	// printf("%ld\n", matches_range("??? ", 3));


	// printf("should be 0\n");
	// printf("%ld\n", matches_range("?#...", 1));
	// printf("%ld\n", matches_range("##...", 1));
	// printf("%ld\n", matches_range("?#?...", 1));
	// printf("%ld\n", matches_range(".?#?...", 2));
	// printf("%ld\n", matches_range("## 1,1,3", 3));


	solution();
	return 0;
}


