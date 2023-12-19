#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
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

struct Set {
	int32_t x;
	int32_t y;
	struct Set* next;
} *set = NULL;


bool _in_set(int32_t x, int32_t y, struct Set* _set) {
	if(!_set) return false;
	if (_set->x == x && _set->y == y) return true;
	return _in_set(x, y, _set->next);
}

bool in_set(int32_t x, int32_t y) {
	return _in_set(x,y, set);
}

void add_to_set(int32_t x, int32_t y) {
	if (!in_set(x,y)) {
		struct Set* tmp = set;
		set = malloc(sizeof(struct Set));
		set->next = tmp;
		set->x = x;
		set->y = y;
	}
}

void _free_set(struct Set* _set) {
	if (_set && _set->next) {
		_free_set(_set->next);
	}
	free(_set);
}

void free_set() {
	_free_set(set);
}

int32_t dx(char direction) {
	if (direction == 'D' || direction == 'U') return 0;
	if (direction == 'R') return 1;
	if (direction == 'L') return -1;
}

int32_t dy(char direction) {
	if (direction == 'R' || direction == 'L') return 0;
	if (direction == 'D') return 1;
	if (direction == 'U') return -1;
}



uint32_t process(bool render) {
	struct Set *iter = set;
	int32_t minx = 10000000;
	int32_t maxx = -1000000;
	int32_t miny = 10000000;
	int32_t maxy = -1000000;
	while(iter) {
		if (iter->x > maxx) maxx=iter->x;
		if (iter->x < minx) minx=iter->x;
		if (iter->y > maxy) maxy=iter->y;
		if (iter->y < miny) miny=iter->y;
		iter = iter->next;
	}

	int32_t width = (maxx - minx + 1);
	int32_t height = (maxy - miny + 1);
	char* grid = malloc( width * height * sizeof(char));
	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			if (in_set(x+minx,y+miny)) grid[x + width*y] = '#';
			else grid[x + width*y] = '.';
			if(render) printf("%c", grid[x + width*y]);
		}
		if(render) printf("\n");
	}


	for (size_t y = 1; y < height-1; y++) {
		bool inside = false;
		for (size_t x = 0; x < width; x++) {
			if (grid[x + width*y] == '.'){
				if (inside) grid[x + width*y] = 'F';
				continue;
			}

			bool above = false;
			bool below = false;
			while (grid[x + width*y] == '#' && x < width){
				if (grid[x + width*(y-1)] == '#') {
					above = true;
				}
				if (grid[x + width*(y+1)] == '#') {
					below = true;
				}
				x++;
			};
			x--;
			if (above && below) inside = !inside;
		}
	}

	if(render) printf("\n");
	uint32_t count = 0;
	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			if (grid[x + width*y]!='.') count++;
			if(render) printf("%c", grid[x + width*y]);

		}
		if(render) printf("\n");
	}
	return count;
}

uint32_t x_product(uint32_t xa, uint32_t ya, uint32_t xb, uint32_t yb) {
	return xa * yb - ya * xb;
}

void solution() {
	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen(FILENAME, "r");

	int32_t x = 0;
	int32_t y = 0;

	add_to_set(x,y);

	while( (read = getline(&lineptr, &n, f)) != -1 ) {
		char direction = *lineptr;
		uint64_t length = 0;
		parse_uint(lineptr+2, &length);
		while(length) {
			length--;
			x += dx(direction);
			y += dy(direction);
			add_to_set(x,y);
		}
		lineptr = NULL;
		free(lineptr);
	}



	//printf("%d\n\n", process(false));

	fclose(f);
}

int main(void) {


	printf("%d", x_product(1,0,0,-1));

	// struct timeval t0, t1;
	// gettimeofday(&t0, 0);
	// solution();
	// gettimeofday(&t1, 0);
	// long elapsed = (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
	// printf("%ld", elapsed);
	return 0;
}

