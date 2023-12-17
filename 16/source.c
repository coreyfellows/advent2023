#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>

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
	#define WIDTH 110
	#define HEIGHT 110
 	#define FILENAME "input.txt"
#endif

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

struct Node {
	char val;
	bool visited;
	bool has_reflected[4];
} grid[WIDTH*HEIGHT];

struct Node* get_element(int64_t x, int64_t y) {
	if (x < 0 || x >= WIDTH) {
		return NULL;
	}

	if (y < 0 || y >= HEIGHT) {
		return NULL;
	}
	return grid + x + y*WIDTH;
}

char d(char direction) {
	switch(direction) {
		case NORTH:
			return 'n';
			break;
		case SOUTH:
			return 's';
			break;
		case EAST:
			return 'e';
			break;
		case WEST:
			return 'w';
			break;
	}
	return 0;
}
int dx(char direction) {
	if (direction == NORTH || direction == SOUTH) return 0;
	if (direction == WEST) return -1;
	if (direction == EAST) return 1;
}

int dy(char direction) {
	if (direction == EAST || direction == WEST) return 0;
	if (direction == NORTH) return -1;
	if (direction == SOUTH) return 1;
}

char rotate_cw(char direction) {
	return (direction + 1)%4;
}
char rotate_ccw(char direction) {
	if(direction == 0) { return 3;}
	return direction - 1;
}

char transform_direction(struct Node* node, char direction) {

	if (node->has_reflected[direction]) {
		return -1;
	}

	if (node->val == '\\') {

		node->has_reflected[direction] = true;
		if (direction%2 == 0) {
			return rotate_ccw(direction);
		} else {
			return rotate_cw(direction);
		}
	}

	if (node->val == '/') {
		node->has_reflected[direction] = true;
		if (direction%2) {
			return rotate_ccw(direction);
		} else {
			return rotate_cw(direction);
		}
	}
	return direction;
}

bool should_split(struct Node* node, char direction) {
	if (node->val == '|' && (direction == EAST || direction == WEST)) return true;
	if (node->val == '-' && (direction == NORTH || direction == SOUTH)) return true;
	return false;
}

uint64_t score_and_reset() {
	uint64_t total = 0;
	for (int y = 0; y< HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			struct Node* element = get_element(x,y);
			if(get_element(x,y)->visited){
				total++;
				element->visited = 0;
				memset(element->has_reflected, 0, sizeof(bool) * 4);
			}
		}
	}
	return total;
}

void render(int cx, int cy) {
	dprintf("\n");
	for (int y = 0; y< HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			if (cx == x && cy == y) {
				dprintf("*");
			}
			else {
				if (get_element(x,y)->visited) {
					dprintf("#");
				} else {
					dprintf("%c", get_element(x,y)->val);
				}
			}
		}
		dprintf("\n");
	}
}

void traverse(int64_t x, int64_t y, char direction) {
	struct Node* element = NULL;
	while (element = get_element(x,y)) {
		element->visited = true;
		direction = transform_direction(element, direction);
		if(-1 == direction) {
			return;
		}
		if (should_split(element, direction)){
			char new_direction = rotate_cw(direction);
			traverse(x+dx(new_direction), y+dy(new_direction), new_direction);
			direction = rotate_ccw(direction);
		}
		x+=dx(direction);
		y+=dy(direction);
	}
}

void solution() {
	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen(FILENAME, "r");

	uint64_t y;
	while( (read = getline(&lineptr, &n, f)) != -1 ) {
		for (int x = 0; x < WIDTH; ++x) {
			struct Node* element = get_element(x,y);
			element->visited = 0;
			element->val = lineptr[x];
			memset(element->has_reflected, 0, sizeof(bool) * 4);

		}
		y++;
		lineptr = NULL;
		free(lineptr);
	}
	traverse(0,0, EAST);
	printf("Total 1: %ld\n", score_and_reset());

	uint64_t max_score = 0;
	for (uint64_t x = 0; x < WIDTH; x++) {
		traverse(x, 0, SOUTH);
		uint64_t score = score_and_reset();
		if (score > max_score) {
			max_score = score;
		}
		traverse(x, HEIGHT-1, NORTH);
		score = score_and_reset();
		if (score > max_score) {
			max_score = score;
		}
		traverse(0, x, EAST);
		score = score_and_reset();
		if (score > max_score) {
			max_score = score;
		}
		traverse(0, WIDTH-x, WEST);
		score = score_and_reset();
		if (score > max_score) {
			max_score = score;
		}
	}
	printf("Total 2: %ld\n", max_score);

	fclose(f);
}

int main(void) {

	struct timeval t0, t1;
	gettimeofday(&t0, 0);
	solution();
	gettimeofday(&t1, 0);
	long elapsed = (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
	printf("\n\n%ld microseconds\n", elapsed);
	return 0;
}


