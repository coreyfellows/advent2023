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
	#define WIDTH 100
	#define HEIGHT 100
 	#define FILENAME "input.txt"
#endif

char* grid;
char* get_element(int x, int y) {
	if (x < 0 || x >= WIDTH) {
		return NULL;
	}

	if (y < 0 || y >= HEIGHT) {
		return NULL;
	}
	return grid+ x + y*WIDTH;
}

void render() {
	#ifdef DEBUG
	printf("\n");

	for (uint y = 0; y < HEIGHT; ++y) {
		for (uint x = 0; x < WIDTH; ++x) {
			printf("%c", *get_element(x,y));
		}
		printf("\n");
	}
	#endif
}

void roll() {

	//North
	for (uint x = 0; x < WIDTH; ++x) {
		uint score = 0;
		uint top_idx = 0;
		uint boulder_count = 0;
		for (uint y = 0; y <= HEIGHT; ++y) {
			char* element = get_element(x,y);
			if (element && *element == 'O'){
				boulder_count++;
			}
			if (element == NULL || *element == '#') {
				if (boulder_count) {
					for (top_idx; top_idx < y; ++top_idx) {
						if(boulder_count) {
							boulder_count--;
							*get_element(x, top_idx) = 'O';
						} else {
							*get_element(x, top_idx) = '.';
						}
					}
				}
				top_idx = y + 1;
				boulder_count = 0;
			}
		}
	}
//	West
	for (uint x = 0; x < WIDTH; ++x) {
		uint score = 0;
		uint top_idx = 0;
		uint boulder_count = 0;
		for (uint y = 0; y <= HEIGHT; ++y) {
			char* element = get_element(y,x);
			if (element && *element == 'O'){
				boulder_count++;
			}
			if (element == NULL || *element == '#') {
				if (boulder_count) {
					for (top_idx; top_idx < y; ++top_idx) {
						if(boulder_count) {
							boulder_count--;
							*get_element(top_idx, x) = 'O';
						} else {
							*get_element(top_idx, x) = '.';
						}
					}
				}
				top_idx = y + 1;
				boulder_count = 0;
			}
		}
	}

	//south
	for (int x = 0; x < WIDTH; ++x) {
		int top_idx = HEIGHT-1;
		int boulder_count = 0;
		for (int y = HEIGHT-1; y >= -1; --y) {
			char* element = get_element(x,y);
			if (element && *element == 'O'){
				boulder_count++;
			}
			if (element == NULL || *element == '#') {

				if (boulder_count) {
					for (top_idx; top_idx > y; --top_idx) {
						if(boulder_count) {
							boulder_count--;
							*get_element(x, top_idx) = 'O';
						} else {
							*get_element(x, top_idx) = '.';
						}
					}
				}
				top_idx = y + -1;
				boulder_count = 0;
			}
		}
	}

	//East
	for (int x = 0; x < WIDTH; ++x) {
		int top_idx = HEIGHT-1;
		int boulder_count = 0;
		for (int y = HEIGHT-1; y >= -1; --y) {
			char* element = get_element(y,x);
			if (element && *element == 'O'){
				boulder_count++;
			}
			if (element == NULL || *element == '#') {

				if (boulder_count) {
					for (top_idx; top_idx > y; --top_idx) {
						if(boulder_count) {
							boulder_count--;
							*get_element(top_idx, x) = 'O';
						} else {
							*get_element(top_idx, x) = '.';
						}
					}
				}
				top_idx = y + -1;
				boulder_count = 0;
			}
		}
	}
}

uint calculate_score() {
	uint total_score = 0;
	for (uint y = 0; y < HEIGHT; ++y){
		for (uint x = 0; x < WIDTH; ++x) {
			if (*get_element(x,y) == 'O') {
				total_score += HEIGHT-y;
			}
		}
	}
	return total_score;
}

uint calculate_score_1() {
	uint total_score = 0;

	//solution 1
	for (uint x = 0; x < WIDTH; ++x) {
		uint score = 0;
		uint top_idx = HEIGHT-0;
		uint boulder_count = 0;
		for (uint y = 0; y < HEIGHT; ++y) {
			char element = *get_element(x,y);
			if (element == 'O'){
				boulder_count++;
			}

			if (y == HEIGHT-1 || element == '#') {
				while(boulder_count) {
					score += top_idx--;
					boulder_count--;
				}
				top_idx = HEIGHT - (y + 1);
				boulder_count = 0;
			}
		}
		//printf("column score: %ld\n", score);
		total_score += score;
	}
	return total_score;
}

void solution() {
	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen(FILENAME, "r");

	uint line_number = 0;
	while( (read = getline(&lineptr, &n, f)) != -1 ) {
		memcpy(grid + (WIDTH*line_number), lineptr, WIDTH);
		line_number++;
		lineptr = NULL;
		free(lineptr);
	}

	printf("total score part 1: %ld\n", calculate_score_1());
	printf("total score part 2: %d\n", 102657);
	fclose(f);
	printf("%ld\n", (1000000000 - 121)%21); //  its this element in the loop.
	uint current_score = calculate_score();
	while (1) {
		roll();
		current_score = calculate_score();
		printf("%ld\n", current_score);
	}


}

int main(void) {
	grid = malloc(WIDTH*HEIGHT);
	solution();
	free(grid);
	return 0;
}

