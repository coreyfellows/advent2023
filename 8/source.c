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

//#define DEBUG

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

uint parse_next_int(char** start, uint* value ) {
	while (is_digit(**start)) start++;
	while(**start && '\n' != **start && !is_digit(**start)) ++*start;
	if (!is_digit(**start)) {
		return 0;
	}

	*start += parse_int(*start, value);
	return 1;
}


struct Node {
	char* label;
	uint left;
	uint right;
};

struct Game {
	struct Node* nodes;
	char* instructions;
};

//#define SAMPLE

#ifndef SAMPLE
	#define NODE_COUNT 762
 	#define FILENAME "input.txt"
#else
	#define NODE_COUNT 3
	#define FILENAME "sample.txt"
#endif

uint find_node(struct Node* nodes, char* label) {
	for(uint i = 0; i < NODE_COUNT; ++i) {
		if (startswith(nodes[i].label, label)) {
			return i;
		}
	}
	return 0;
}

struct Game load_nodes() {
	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen(FILENAME, "r");

	uint idx = 0;
	char* instructions = NULL;

	struct Node* nodes = malloc(sizeof(struct Node)*NODE_COUNT);


	while( (read = getline(&lineptr, &n, f)) != -1 ) {
		if (!instructions) {
			instructions = lineptr;
			lineptr = NULL;
			continue;
		}

		if (*lineptr == '\n') {
			free(lineptr);
			lineptr = NULL;
			continue;
		};


		nodes[idx].label = malloc(4);
		nodes[idx].label[3] = 0;
		for (uint i = 0; i < 3; ++i) {
			nodes[idx].label[i] = lineptr[i];
		}

		free(lineptr);
		lineptr = NULL;
		idx+=1;
	}


	rewind(f);
	getline(&lineptr, &n, f);
	free(lineptr);

	idx = 0;
	while( (read = getline(&lineptr, &n, f)) != -1 ) {

		if (*lineptr == '\n') continue;

		struct Node* node = nodes + idx;


		char target_label[4] = {0,0,0,0};

		for (uint i = 0; i < 3; ++i) {
			target_label[i] = lineptr[7+i];
		}

		node->left = find_node(nodes, target_label);

		for (uint i = 0; i < 3; ++i) {
			target_label[i] = lineptr[12+i];
		}

		node->right = find_node(nodes, target_label);

		free(lineptr);
		lineptr = NULL;
		idx+=1;
	}

	fclose(f);
	struct Game g;
	g.nodes = nodes;
	g.instructions = instructions;
	return g;
}




void process_instruction(char instruction, uint* current_node, struct Game game) {
	if (instruction == 'L') {
		dprintf("Going LEFT from %s to", game.nodes[*current_node].label);
		*current_node = game.nodes[*current_node].left;
		dprintf(" %s\n", game.nodes[*current_node].label);

	}

	if (instruction == 'R') {
		dprintf("Going RIGHT from %s to", game.nodes[*current_node].label);
		*current_node = game.nodes[*current_node].right;
		dprintf(" %s\n", game.nodes[*current_node].label);
	}
}

void solution_1() {
	struct Game game = load_nodes();
	uint winner = find_node(game.nodes, "ZZZ");
	char* instruction_ptr = game.instructions;

	uint total = 0;

	uint current_node = find_node(game.nodes, "AAA");

	while(instruction_ptr) {
		++total;
		dprintf("Step %d ", total);

		process_instruction(*instruction_ptr, &current_node, game);

		instruction_ptr++;
		if (*instruction_ptr == '\n') {
			instruction_ptr = game.instructions;
		}

		if (current_node == winner) {
			dprintf("WINNER !\n");
			instruction_ptr = NULL;
		}
	}

	printf("solution 1 total: %d\n", total);
	free(game.nodes);
	free(game.instructions);
}

void solution_2() {
	struct Game game = load_nodes();
	//uint winner = find_node(game.nodes, "ZZZ");
	char* instruction_ptr = game.instructions;
	uint total = 0;
	uint current_nodes[6] = {
		find_node(game.nodes, "AAA"),
		find_node(game.nodes, "SHA"),
		find_node(game.nodes, "DLA"),
		find_node(game.nodes, "JVA"),
		find_node(game.nodes, "XLA"),
		find_node(game.nodes, "DNA")
	};

	uint periods[6] = {
		0,0,0,0,0,0
	};

	int periods_found = 0;
	while(instruction_ptr) {
		++total;

		for (uint i = 0; i < 6; i++){

			process_instruction(*instruction_ptr, current_nodes + i, game);
			if (game.nodes[current_nodes[i]].label[2] == 'Z' && periods[i] == 0) {
				periods[i] = total;
				periods_found++;
			}

		}

		instruction_ptr++;
		if (*instruction_ptr == '\n') {
			instruction_ptr = game.instructions;
		}

		if(periods_found == 6) {
			instruction_ptr = NULL;
		}
	}

	printf("solution 2: lcm(");
	for (int i = 0; i < 5; ++i) {
		printf("%d, ", periods[i]);
	}
	printf("%d) = 14935034899483\n ", periods[5]);

	free(game.nodes);
	free(game.instructions);
}

int main(void) {
	solution_1();
	solution_2();
	return 0;
}

