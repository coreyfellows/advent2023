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
	#define WIDTH 11
	#define HEIGHT 9
#else
	#define WIDTH 140
	#define HEIGHT 140
 	#define FILENAME "input.txt"
#endif




struct Node {
	uint visited;
	uint distance;
	char repr;
	uint inside;
} *grid;


char* megagrid;

struct Node* get_element(int x, int y) {
	if (x < 0 || x >= WIDTH) {
		return NULL;
	}

	if (y < 0 || y >= HEIGHT) {
		return NULL;
	}
	return grid + x + y*WIDTH;
}

char* get_mega_element(int x, int y) {
	if (x < 0 || x >= WIDTH*3) {
		return NULL;
	}

	if (y < 0 || y >= HEIGHT*3) {
		return NULL;
	}
	return megagrid+x + y*WIDTH*3;
}

void copy(uint x, uint y) {
	if (!get_element(x,y)->visited) return;

	char repr = get_element(x,y)->repr;
	*get_mega_element(x*3+1, y*3+1) = repr;

	if (repr == '|') {
		*get_mega_element(x*3+1, y*3+2) = repr;
		*get_mega_element(x*3+1, y*3) = repr;
	}

	if (repr == '-') {
		*get_mega_element(x*3+2, y*3+1) = repr;
		*get_mega_element(x*3+0, y*3+1) = repr;
	}

	if (repr == '7') {
		*get_mega_element(x*3+0, y*3+1) = repr;
		*get_mega_element(x*3+1, y*3+2) = repr;
	}

	if (repr == 'F') {
		*get_mega_element(x*3+2, y*3+1) = repr;
		*get_mega_element(x*3+1, y*3+2) = repr;
	}

	if (repr == 'J') {
		*get_mega_element(x*3+0, y*3+1) = repr;
		*get_mega_element(x*3+1, y*3+0) = repr;
	}

	if (repr == 'L') {
		*get_mega_element(x*3+2, y*3+1) = repr;
		*get_mega_element(x*3+1, y*3+0) = repr;
	}
}


void crawl_outside(uint x, uint y) {


	char* node = get_mega_element(x, y);
	if (!node || *node != ' ') {
		return;
	}

	*node = 'x';

	for (int u = -1; u < 2; ++u ) {
		for (int v = -1; v < 2; ++v ){
			if (u == v && u == 0) {
				continue;
			}
			crawl_outside(x+u, y+v);

		}
	}


}

uint next_node(uint* xptr, uint* yptr) {
	int x = *xptr;
	int y = *yptr;
	struct Node* n = get_element(x,y);

	if (n->repr == '|' && get_element(x,y+1) && !get_element(x,y+1)->visited) {
		++*yptr;
	}

	if (n->repr == '|' && get_element(x,y-1) && !get_element(x,y-1)->visited) {
		--*yptr;
	}

	if (n->repr == '-' && get_element(x+1,y) && !get_element(x+1,y)->visited) {
		++*xptr;
	}

	if (n->repr == '-' && get_element(x-1,y) && !get_element(x-1,y)->visited) {
		--*xptr;
	}

	if (n->repr == 'J' && get_element(x-1,y) && !get_element(x-1,y)->visited) {
		--*xptr;
	}

	if (n->repr == 'J' && get_element(x,y-1) && !get_element(x,y-1)->visited) {
		--*yptr;
	}

	if (n->repr == 'L' && get_element(x+1,y) && !get_element(x+1,y)->visited) {
		++*xptr;
	}

	if (n->repr == 'L' && get_element(x,y-1) && !get_element(x,y-1)->visited) {
		--*yptr;
	}

	if (n->repr == '7' && get_element(x-1,y) && !get_element(x-1,y)->visited) {
		--*xptr;
	}

	if (n->repr == '7' && get_element(x,y+1) && !get_element(x,y+1)->visited) {
		++*yptr;
	}

	if (n->repr == 'F' && get_element(x+1,y) && !get_element(x+1,y)->visited) {
		++*xptr;
	}

	if (n->repr == 'F' && get_element(x,y+1) && !get_element(x,y+1)->visited) {
		++*yptr;
	}

	return 1;

}

void solution_1() {
	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen(FILENAME, "r");


	uint y = 0;
	uint start_x = 0;
	uint start_y = 0;
	while( (read = getline(&lineptr, &n, f)) != -1 ) {

		for (int x = 0; x < WIDTH; ++x) {
			struct Node* n = get_element(x,y);
			n->repr = lineptr[x];
			n->visited = 0;
			n->distance = 0;
			n->inside = 0;

			if (lineptr[x] == 'S') {
				start_x = x;
				start_y = y;
				n->visited = 1;
			}
		}
		y++;
		lineptr = NULL;
		free(lineptr);
	}

	struct Node* adjacent_left = get_element(start_x-1, start_y);
	struct Node* adjacent_right = get_element(start_x+1, start_y);
	struct Node* adjacent_up = get_element(start_x, start_y-1);
	struct Node* adjacent_down = get_element(start_x, start_y+1);

	struct Node* start_node = get_element(start_x, start_y);

	if (adjacent_up && !(adjacent_up->repr == '|' || adjacent_up->repr == 'F' || adjacent_up->repr == '7')) adjacent_up = NULL;
	if (adjacent_down && !(adjacent_down->repr == '|' || adjacent_down->repr == 'J' || adjacent_down->repr == 'L')) adjacent_down = NULL;

	if (adjacent_left && !(adjacent_left->repr == '-' || adjacent_left->repr == 'F' || adjacent_left->repr == 'L')) adjacent_left = NULL;
	if (adjacent_right && !(adjacent_right->repr == '-' || adjacent_right->repr == 'J' || adjacent_right->repr == '7')) adjacent_right = NULL;

	if (adjacent_down && adjacent_up) start_node->repr = '|';
	if (adjacent_left && adjacent_right) start_node->repr = '-';

	if (adjacent_down && adjacent_left) start_node->repr = '7';
	if (adjacent_down && adjacent_right) start_node->repr = 'F';

	if (adjacent_up && adjacent_right) start_node->repr = 'L';
	if (adjacent_up && adjacent_left) start_node->repr = 'J';



	uint x_1 = start_x, x_2 = start_x;
	uint y_1 = start_y, y_2 = start_y;
	char d1, d2;
	if (start_node->repr == 'F') {
		x_1++;
		y_2++;
		d1 = 'e';
		d2 = 's';
	}

	if (start_node->repr == 'J') {
		x_1--;
		y_2--;
	}

	uint distance = 0;
	while(!get_element(x_1, y_1)->visited && !get_element(x_2, y_2)->visited) {

		get_element(x_1, y_1)->visited = 1;
		get_element(x_1, y_1)->distance = ++distance;

		get_element(x_2, y_2)->visited = 1;
		get_element(x_2, y_2)->distance = distance;
		next_node(&x_1, &y_1);
		next_node(&x_2, &y_2);
	}


	printf("\n\nOriginal Map\n\n");
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			printf("%c", get_element(x,y)->repr);
		}
		printf("\n");
	}


	printf("\n\nVisited elements\n\n");
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			printf("%d", get_element(x,y)->visited);
		}
		printf("\n");
	}

	printf("\n\nDistance (a-z looping back to a)\n\n");
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			if (get_element(x,y)->distance) {
				printf("%c", ((get_element(x,y)->distance- 1)%26) + 'a' );
			} else {
				printf(".");
			}
		}
		printf("\n");
	}

	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			copy(x,y);
		}
	}

	crawl_outside(0,0);
	uint total = 0;
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			if (*get_mega_element(x*3+1, y*3+1) == ' ') {
				*get_mega_element(x*3+1, y*3+1) = '@';
				total++;
			}
		}
	}

	printf("\n\nExpanded grid with @s representing interior spaces\n\n");
	for (int y = 0; y < HEIGHT*3; ++y) {
		for (int x = 0; x < WIDTH*3; ++x) {
			printf("%c", *get_mega_element(x,y));
		}
		printf("\n");
	}

	printf("distance: %d\n", distance);
	printf("total: %d\n", total);
	fclose(f);
}

int main(void) {
	grid = malloc(sizeof(struct Node)*WIDTH*HEIGHT);
	megagrid = malloc(WIDTH*HEIGHT*9);
	memset(megagrid, ' ', WIDTH*HEIGHT*9);
	solution_1();
	free(megagrid);
	free(grid);
	return 0;
}

