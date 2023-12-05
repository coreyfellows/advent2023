#include <stdio.h>
#include <stdlib.h>

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


struct Seed {
	struct Seed* next;
	uint value;
	uint processed;
};

struct Seed* newSeed(uint value) {
	struct Seed* seed = malloc(sizeof(struct Seed));
	seed->next = NULL;
	seed->value = value;
	seed->processed = 0;

	return seed;
}

struct Mapping {
	uint source;
	uint destination;
	uint range;
};

struct Mapping parse_mapping(char* line) {
	struct Mapping mapping;
	line += 1 + parse_int(line, &mapping.destination);
	line += 1 + parse_int(line, &mapping.source);
	line += 1 + parse_int(line, &mapping.range);
	return mapping;
}

void map(struct Mapping mapping, struct Seed* seed) {

	if (seed->processed) return;

	int normalized = (int)seed->value - (int)mapping.source;
	if (normalized >= 0 && normalized <= mapping.range) {
		seed->value = mapping.destination + normalized;
		seed->processed = 1;
	}
	dprintf("Mapping %d\n", seed->value);
}


struct Seed* parse_seeds(char* start) {

	while(*start != '\n' && !is_digit(*(start+1))) start++;

	struct Seed* head;
	struct Seed** headptr = &head;

	while(*start != '\n') {
		++start;
		*headptr = newSeed(0);

		uint value = 0;
		start += parse_int(start, &(*headptr)->value);

		dprintf("%d ", (*headptr)->value);

		headptr = &(*headptr)->next;
	}

	#ifdef DEBUG
	dprintf("\nSeed List:\n");
	struct Seed* iterator = head;
	while (iterator) {
		dprintf("%d ", iterator->value);
		iterator = iterator->next;
	}
	dprintf("\n");
	#endif

	return head;
}

struct Seed* parse_seeds_2(char* start, uint pair_index) {

	while(*start != '\n' && !is_digit(*(start+1))) start++;

	struct Seed* head;
	struct Seed** headptr = &head;
	uint idx = 0;
	while(*start != '\n') {
		++start;

		uint range_start = 0;
		uint range = 0;
		start += parse_int(start, &range_start) + 1;
		start += parse_int(start, &range);

		if (idx++ == pair_index) {
			while(range) {
				*headptr = newSeed(range_start + --range);
				headptr = &(*headptr)->next;
			}
		}
	}

	#ifdef DEBUG
	dprintf("\nSeed List:\n");
	struct Seed* iterator = head;
	while (iterator) {
		dprintf("%d ", iterator->value);
		iterator = iterator->next;
	}
	dprintf("\n");
	#endif

	return head;
}




uint process_pair(uint pair) {

	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen("input.txt", "r");

	int total = 0;

	struct Seed* seeds = NULL;

	while( (read = getline(&lineptr, &n, f)) != -1 ) {
		if (!seeds) {
			seeds = parse_seeds_2(lineptr, pair);
			continue;
		}

		if (*lineptr == '\n') {
			// not on a map, reset processed on our seeds
			printf("Not on a map, reset processed on our seeds\n");
			struct Seed* iterator = seeds;
			while (iterator) {
				iterator->processed = 0;
				iterator = iterator->next;
			}
		}

		if (*lineptr >= 'a' && *lineptr <= 'z')
		{
			printf("processing %s", lineptr);
		}

		if (!is_digit(*lineptr)) {
			continue;
		}

		struct Mapping mapping = parse_mapping(lineptr);

		dprintf("%d %d %d\n", mapping.destination, mapping.source, mapping.range);

		struct Seed* iterator = seeds;
		while (iterator) {
			map(mapping, iterator);
			iterator = iterator->next;
		}


		free(lineptr);
		lineptr = NULL;
	}

	struct Seed* iterator = seeds;
	uint lowest = 0xffffffff;
	while (iterator) {
		if (iterator->value < lowest) {
			lowest = iterator->value;
		}
		struct Seed* tmp = iterator;
		iterator = iterator->next;
		free(tmp);
	}

	printf("pair %d total: %d\n", pair, lowest);
	fclose(f);
	return lowest;
}

void solution_2() {
	uint lowest = 0xffffffff;
	for (uint i = 0; i < 10; ++i) {
		uint result = process_pair(i);
		if (result < lowest) {
			lowest = result;
		}
	}
	printf("solution 2 lowest: %d\n", lowest);
}


void solution_1() {

	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen("sample.txt", "r");

	int total = 0;

	struct Seed* seeds = NULL;

	while( (read = getline(&lineptr, &n, f)) != -1 ) {
		if (!seeds) {
			seeds = parse_seeds(lineptr);
			continue;
		}

		if (*lineptr == '\n') {
			// not on a map, reset processed on our seeds
			printf("Not on a map, reset processed on our seeds\n");
			struct Seed* iterator = seeds;
			while (iterator) {
				iterator->processed = 0;
				iterator = iterator->next;
			}
		}

		if (*lineptr >= 'a' && *lineptr <= 'z')
		{
			printf("processing %s", lineptr);
		}

		if (!is_digit(*lineptr)) {
			continue;
		}

		struct Mapping mapping = parse_mapping(lineptr);

		dprintf("%d %d %d\n", mapping.destination, mapping.source, mapping.range);

		struct Seed* iterator = seeds;
		while (iterator) {
			map(mapping, iterator);
			iterator = iterator->next;
		}


		free(lineptr);
		lineptr = NULL;
	}

	struct Seed* iterator = seeds;
	uint lowest = 0xffffffff;
	while (iterator) {
		if (iterator->value < lowest) {
			lowest = iterator->value;
		}
		iterator = iterator->next;
	}

	printf("solution 1 total: %d\n", lowest);
	fclose(f);
}


int main(void) {
	//solution_1();
	solution_2();
	return 0;
}

