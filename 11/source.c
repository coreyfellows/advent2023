#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	#define WIDTH 10
	#define HEIGHT 10
#else
	#define WIDTH 140
	#define HEIGHT 140
 	#define FILENAME "input.txt"
#endif

struct Galaxy {
	uint x;
	uint y;
};

struct Galaxy* galaxies = NULL;

char* grid;

struct Galaxy* find_galaxy(uint x, uint y, uint count) {
	for (uint i = 0; i < count; ++i) {
		if (galaxies[i].x == x && galaxies[i].y == y) {
			return galaxies+i;
		}
	}
	return NULL;
}

char* get_element(int x, int y) {
	if (x < 0 || x >= WIDTH) {
		return NULL;
	}

	if (y < 0 || y >= HEIGHT) {
		return NULL;
	}
	return grid + x + y*WIDTH;
}

uint calculate_distance(struct Galaxy* a, struct Galaxy* b) {
	return abs(a->x - b->x) + abs(a->y - b->y);
}

void solution(uint age) {
	grid = malloc(WIDTH*HEIGHT);
	memset(grid, '.', WIDTH*HEIGHT);
	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen(FILENAME, "r");

	uint y = 0;
	while( (read = getline(&lineptr, &n, f)) != -1 ) {
		memcpy(get_element(0, y++), lineptr, WIDTH);
		lineptr = NULL;
		free(lineptr);
	}


	uint galaxy_count = 0;
	for (uint i=0; i < WIDTH*HEIGHT; ++i) {
		if(grid[i] == '#'){
			galaxy_count += 1;
		}
	}
	dprintf("galaxy count = %d\n", galaxy_count);

	galaxies = malloc(sizeof(struct Galaxy) * galaxy_count);


	dprintf("finding galaxies\n");
	uint galaxy_idx = 0;
	for (uint y = 0; y < HEIGHT; ++y) {
		for (uint x = 0; x < WIDTH; ++x) {
			char element = *get_element(x,y);
			if (element == '#') {
				dprintf("#");
				galaxies[galaxy_idx].x = x;
				galaxies[galaxy_idx].y = y;
				galaxy_idx++;
			} else {
				dprintf(" ");
			}
		}
		dprintf("\n");
	}

	// transform our galaxies
	uint ybump = 0;
	uint xbump = 0;
	for (uint y = 0; y < HEIGHT; ++y)
	{
		uint g = 0;
		for (uint x = 0; x < WIDTH; ++x){
			if (*get_element(x,y) == '#') g++;
		}
		if (g) continue;

		for (uint gi = 0; gi < galaxy_count; ++gi) {
			struct Galaxy* galaxy = galaxies + gi;
			if (galaxy->y >= y+ybump) {

				galaxy->y+=age;
			}
		}
		ybump+=age;
		dprintf("line %d has no galaxies\n", y);
	}

	for (uint x = 0; x < WIDTH; ++x)
	{
		uint g = 0;
		for (uint y = 0; y < HEIGHT; ++y){
			if (*get_element(x,y) == '#') g++;
		}
		if (g) continue;

		for (uint gi = 0; gi < galaxy_count; ++gi) {
			struct Galaxy* galaxy = galaxies + gi;
			if (galaxy->x >= x+xbump) {
				galaxy->x+=age;
			}
		}
		xbump+=age;
		dprintf("column %d has no galaxies\n", x);
	}

	#ifdef DEBUG
	printf("\nrender transformed galaxies\n");
	for (int y = 0; y < WIDTH+ybump; ++y) {
		for (int x = 0; x < WIDTH+xbump; ++x) {
			struct Galaxy* galaxy = find_galaxy(x,y, galaxy_count);
			if (galaxy) {
				printf("#");
			} else {
				printf(".");
			}
		}
		printf("\n");
	}
	#endif

	uint total_distance = 0;
	for (uint i = 0; i < galaxy_count; ++i) {

		for (uint sub_i = i+1; sub_i < galaxy_count; ++sub_i) {
			total_distance += calculate_distance(galaxies+i, galaxies+sub_i);
		}
	}
	printf("total %ld\n", total_distance);
	free(grid);
	free(galaxies);
	fclose(f);
}

int main(void) {
	grid = malloc(WIDTH*HEIGHT);
	memset(grid, '.', WIDTH*HEIGHT);
	solution(2-1);
	solution(1000000-1);
	return 0;
}

