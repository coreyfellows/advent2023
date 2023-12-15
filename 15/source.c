#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
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

uint64_t advent_hash_1(const char** str) {
	uint64_t value = 0;

	while(**str && **str != ',' && **str != '\n') {
		dprintf("%c", **str);
		value = (17*(value+**str))%256;
		++*str;
	}
	dprintf(" = %ld\n", value);
	++*str;
	return value;

}

uint64_t advent_hash_2(const char** str, uint64_t* label_hash) {

	char label[20];
	uint64_t value = 0;
	uint64_t position = 0;
	while(**str && **str >= 'a' && **str <= 'z') {
		//printf("%c", **str);
		label[position++] = **str;
		value = (17*(value+**str))%256;
		++*str;
	}
	label[position] = 0;
	*label_hash = hash(label, position);
	//printf("%s = box: %ld hash %ld\n", label, value, *label_hash);
	return value;
}

struct Box {
	char focal_length;
	uint64_t label;
	struct Box* next;
} *boxes[256];


void render_box(uint64_t i){
	if (!boxes[i]) {
		return;
	}
	printf("Box %ld: ", i);
	struct Box* head = boxes[i];

	while(head) {
		printf("[%ld %c] ", head->label, head->focal_length);
		head = head->next;
	}
	printf("\n");
}

void add_lens(uint64_t i, uint64_t label, char focal_length) {
	dprintf("adding [%ld %c] to %ld\n", label, focal_length, i);
	if (!boxes[i]) {
		boxes[i] = malloc(sizeof(struct Box));
		boxes[i]->focal_length = focal_length;
		boxes[i]->next = NULL;
		boxes[i]->label = label;
		return;
	}

	struct Box* head = boxes[i];
	while(head) {
		if (head->label == label) {
			head->focal_length = focal_length;
			return;
		}

		if (!head->next) {
			head->next = malloc(sizeof(struct Box));
			head->next->focal_length = focal_length;
			head->next->next = NULL;
			head->next->label = label;
		}
		head = head->next;
	}
}

void remove_lens(uint64_t i, uint64_t label) {
	struct Box** head = boxes + i;
	if (!*head) {
		return;
	}

	while(*head) {
		if ((*head)->label == label){
			dprintf("removing %ld from %ld\n", label, i);
			struct Box* tmp = (*head)->next;
			free(*head);
			*head = tmp;
			return;
		}
		head = &(*head)->next;
	}

}

void process_instruction(const char** str, uint64_t i, uint64_t label) {
	char instruction = **str;
	++*str;
	if (instruction == '=') {
		char focal_length = **str;
		add_lens(i, label, focal_length);
		++*str;
	}
	if (instruction == '-') {
		remove_lens(i, label);
	}
	++*str;
}

void solution() {
	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen(FILENAME, "r");
	uint64_t total = 0;
	while( (read = getline(&lineptr, &n, f)) != -1 ) {
		dprintf("%s", lineptr);
		const char* head = lineptr;
		while (*head) {
			total += advent_hash_1(&head);
		}

		const char* head2 = lineptr;
		while (*head2) {
			uint64_t label_hash = 0;
			uint64_t box_number = advent_hash_2(&head2, &label_hash);
			process_instruction(&head2, box_number, label_hash);

			#ifdef DEBUG
			for (int i = 0; i < 256; ++i) {
				render_box(i);
			}
			#endif
		}

		lineptr = NULL;
		free(lineptr);
	}
	fclose(f);

	uint64_t total_2 = 0;
	for (int i = 0; i < 256; ++i) {
		uint box_count = 1;
		uint64_t box_score = 0;
		struct Box* head = boxes[i];
		while(head) {
			box_score += (i+1) * box_count * (head->focal_length-'0');
			box_count++;
			head=head->next;
		}
		if(box_score) dprintf("Box %ld score: %ld\n", i, box_score);
		total_2+=box_score;

		//render_box(i);
	}
	printf("Solution 1: %ld\n", total);
	printf("Solution 2: %ld\n", total_2);
}

int main(void) {
	memset(boxes, 0, sizeof(struct Box*) * 256);
	solution();
	return 0;
}

