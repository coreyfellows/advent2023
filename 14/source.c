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

struct Set {
	uint hash;
	struct Set* next;
} *set = NULL;


uint _in_set(uint h, struct Set* _set) {
	if(!_set) return 0;
	if (_set->hash == h) return _set->hash;
	return _in_set(h, _set->next);
}

uint in_set() {
	return _in_set(hash(grid, WIDTH*HEIGHT), set);
}


void add_to_set() {
	if (!in_set()) {
		uint h = hash(grid, WIDTH*HEIGHT);
		struct Set* tmp = set;
		set = malloc(sizeof(struct Set));
		set->next = tmp;
		set->hash = h;
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
	fclose(f);
	uint loop_start = 0;
	uint loop_length = 0;
	uint pre_loop = 0;
	while (1) {
		roll();
		uint hash_in_set = in_set();
		printf("%lu\n", hash_in_set);
		add_to_set();
		if (!loop_start && hash_in_set) {
			loop_start = hash_in_set;
			loop_length++;
			continue;
		}
		pre_loop++;

		if (loop_start && loop_start != hash_in_set) {
			loop_length++;
			continue;
		}

		if (loop_start && loop_start == hash_in_set)
		{

			break;
		}
	}

	printf("pre-loop %ld\n", pre_loop);
	printf("loop length %ld\n", loop_length);

	uint more_iters = (1000000000-pre_loop)%loop_length - 1;
	printf("more_iters %ld\n", more_iters);
	while(more_iters--) {
		roll();
	}
	printf("total score part 2: %ld\n", calculate_score());



}

int main(void) {
	grid = malloc(WIDTH*HEIGHT);
	solution();

	free(grid);
	free_set();
	return 0;
}

