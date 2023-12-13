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

//#define SAMPLE
#ifdef SAMPLE
	#define FILENAME "sample.txt"
	#define WIDTH 10
	#define HEIGHT 10
#else
	#define WIDTH 140
	#define HEIGHT 140
 	#define FILENAME "input.txt"
#endif



struct Grid {
	uint* horizontal;
	uint* vertical;
	uint width;
	uint height;
};

void free_grid(struct Grid* grid){
	free(grid->horizontal);
	free(grid->vertical);
	free(grid);
}

struct Grid* read_grid(FILE* f) {
	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	ssize_t total_read = 0;
	uint line_count = 0;
	while( (read = getline(&lineptr, &n, f)) != -1 ) {
		total_read += read;
		if (*lineptr == '\n') {
			break;
		}
		line_count += 1;
		lineptr = NULL;
		free(lineptr);
	}

	fseek(f, -1 * total_read, SEEK_CUR);
	if (line_count == 0) {
		return NULL;
	}

	struct Grid* grid = malloc(sizeof(struct Grid));
	grid->horizontal = malloc(sizeof(uint) * line_count);
	grid->height = line_count;
	uint y = 0;

	while( (read = getline(&lineptr, &n, f)) != -1 ) {
		if (*lineptr == '\n') {
			break;
		}

		uint value = 0;
		char* iter = lineptr;
		grid->width = 0;
		while(*iter && *iter != '\n') {
			grid->width++;
			value <<= 1;
			if (*iter == '#')
				value |= 1;
			iter++;
		}
		grid->horizontal[y++] = value;
		dprintf("%ld\t%s", value, lineptr);
		lineptr = NULL;
		free(lineptr);
	}

	grid->vertical = malloc(sizeof(uint) * grid->width);

	for (int x = 0; x < grid->width; ++x) {
		uint value = 0;
		for (int y = 0; y < grid->height; ++y) {
			value <<= 1;
			if (grid->horizontal[y] & 1 << grid->width-x-1) {
				value |= 1;
			}
		}
		dprintf("%ld ", value);
		grid->vertical[x] = value;
	}

	dprintf("\nwidth: %ld, height: %ld\n\n", grid->width, grid->height);


	return grid;
}

uint count_bits(uint n)
{
    uint count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

uint smudge_match(uint a, uint b) {
	return count_bits(a ^ b) == 1;
}

uint check_smudged_solution(uint* range, uint width, uint start) {
	uint left = start-1;
	uint right = start;
	dprintf("checking solution at %ld\n", start);
	uint smudge_used = 0;
	while (left != -1 && right < width){
		dprintf("%ld == %ld\n", range[left], range[right]);
		if (range[left] != range[right]) {
			if (smudge_used) return 0;
			smudge_used = smudge_match(range[left], range[right]);
			if (!smudge_used) return 0;
		}
		left--; right++;
	}
	dprintf("found solution\n");
	return smudge_used;

}


uint check_solution(uint* range, uint width, uint start) {
	uint left = start-1;
	uint right = start;
	dprintf("checking solution at %ld\n", start);
	while (left != -1 && right < width){
		dprintf("%ld == %ld\n", range[left], range[right]);
		if (range[left] != range[right]) return 0;
		left--; right++;
	}
	dprintf("found solution\n");
	return 1;

}

uint solve(uint* range, uint width) {
	for (int i = 1; i < width; ++i) {
		if (check_solution(range, width, i)) {
			return i;
		}
	}
	return 0;
}

uint solve_smudge(uint* range, uint width) {
	for (int i = 1; i < width; ++i) {
		if (check_smudged_solution(range, width, i)) {
			return i;
		}
	}
	return 0;
}


void solution() {

	FILE* f = fopen(FILENAME, "r");
	struct Grid* grid = NULL;
	uint total = 0;
	uint total_smudged = 0;
	while(grid = read_grid(f)) {

		dprintf("Checking vertical rows\n");
		uint solution = solve(grid->vertical, grid->width);
		if (!solution) {
			dprintf("Checking horizontal rows\n");
			solution = 100 * solve(grid->horizontal, grid->height);
		}
		total += solution;

		dprintf("solution: %ld\n", solution);
		dprintf("Checking vertical rows\n");
		uint solution_smudged = solve_smudge(grid->vertical, grid->width);
		if (!solution_smudged) {
			dprintf("Checking horizontal rows\n");
			solution_smudged = 100 * solve_smudge(grid->horizontal, grid->height);
		}
		dprintf("smudged solution: %ld\n", solution_smudged);
		total_smudged += solution_smudged;

		free_grid(grid);

	}

	printf("\ntotal: %ld\n", total);
	printf("\ntotal smudge: %ld\n", total_smudged);
	fclose(f);
}

int main(void) {
	solution();
	return 0;
}

