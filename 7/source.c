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


struct Hand {
	uint bid;
	uint first_order;
	char* repr;
};

uint card_to_int(char card) {
	if(is_digit(card)) {
		return card - '0' - 2;
	}

	if (card == 'T') return 8;
	if (card == 'J') return 9;
	if (card == 'Q') return 10;
	if (card == 'K') return 11;
	if (card == 'A') return 12;
}

uint card_to_int_joker(char card) {
	if(is_digit(card)) {
		return card - '0' - 1;
	}

	if (card == 'J') return 0;
	if (card == 'T') return 9;
	if (card == 'Q') return 10;
	if (card == 'K') return 11;
	if (card == 'A') return 12;
}

void print_first_order(uint value) {
	switch (value)
	{
	case 6:
		dprintf("Five of a Kind");
		break;

	case 5:
		dprintf("Four of a Kind");
		break;
	case 4:
		dprintf("Full House");
		break;
	case 3:
		dprintf("Three of a kind");
		break;
	case 2:
		dprintf("Two Pair");
		break;
	case 1:
		dprintf("Pair");
		break;
	case 0:
		dprintf("High Card");
		break;
	default:
		break;
	}
}


int create_joker_hand(char* hand, struct Hand* target) {

	target->repr = hand;

	char cards[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};

	while(*hand != ' ') {
		cards[card_to_int_joker(*(hand++))]++;
	}

	parse_next_int(&hand, &(target->bid));

	uint counts[5] = {0,0,0,0};
	for (int i = 1; i < 13; ++i) {
		if(cards[i]) counts[cards[i]-1]++;

		dprintf("%d ", cards[i]);
	}
	dprintf(" bid: %d\n", target->bid);
	uint jokers = cards[0];
	dprintf("jokers: %d\n", jokers);
	for (int i = 4; i >= 0; --i) {
		if (counts[i] && jokers) {
			counts[i]--;
			counts[i+jokers]++;
			jokers = 0;
		}
		dprintf("%d ", counts[i]);
	}

	target->first_order = 0;

	if (counts[4] || cards[0] == 5) { //five of a kind
		target->first_order = 6;
	}

	if (counts[3]) { //four of a kind
		target->first_order = 5;
	}

	if (counts[2] && counts[1]) { //full house
		target->first_order = 4;
	}

	if (counts[2] && !counts[1]) { //three of a kind
		target->first_order = 3;
	}

	if (counts[1] == 2) { //two pair
		target->first_order = 2;
	}

	if (!counts[2] && counts[1] == 1) { //pair
		target->first_order = 1;
	}

	print_first_order(target->first_order);
	dprintf("\n");
	return 0;
}


int create_hand(char* hand, struct Hand* target) {

	target->repr = hand;

	char cards[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};

	while(*hand != ' ') {
		cards[card_to_int(*(hand++))]++;
	}

	parse_next_int(&hand, &(target->bid));

	int counts[5] = {0,0,0,0};
	for (int i = 0; i < 13; ++i) {
		if(cards[i]) counts[cards[i]-1]++;

		dprintf("%d ", cards[i]);
	}
	dprintf(" bid: %d\n", target->bid);
	for (int i = 0; i < 5; ++i) {
		dprintf("%d ", counts[i]);
	}
	target->first_order = 0;
	if (counts[4]) { //five of a kind
		target->first_order = 6;
	}

	if (counts[3]) { //four of a kind
		target->first_order = 5;
	}

	if (counts[2] && counts[1]) { //full house
		target->first_order = 4;
	}

	if (counts[2] && !counts[1]) { //three of a kind
		target->first_order = 3;
	}

	if (counts[1] == 2) { //two pair
		target->first_order = 2;
	}

	if (!counts[2] && counts[1] == 1) { //pair
		target->first_order = 1;
	}

	print_first_order(target->first_order);
	dprintf("\n");
	return 0;
}


int handcmp(const void* _a, const void* _b) {
	const struct Hand* a = _a;
	const struct Hand* b = _b;
	if (b->first_order != a->first_order) return a->first_order - b->first_order;

	for (int i = 0; i < 5; ++i) {
		uint bidx = card_to_int(b->repr[i]);
		uint aidx = card_to_int(a->repr[i]);
		if (bidx != aidx) {
			return aidx - bidx;
		}
	}
	return 0;
}


int handcmp_joker(const void* _a, const void* _b) {
	const struct Hand* a = _a;
	const struct Hand* b = _b;
	if (b->first_order != a->first_order) return a->first_order - b->first_order;

	for (int i = 0; i < 5; ++i) {
		uint bidx = card_to_int_joker(b->repr[i]);
		uint aidx = card_to_int_joker(a->repr[i]);
		if (bidx != aidx) {
			return aidx - bidx;
		}
	}
	return 0;
}


#define HAND_COUNT 1000
#define FILENAME "input.txt"

//#define HAND_COUNT 5
//#define FILENAME "sample.txt"


void solution_1() {
	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen(FILENAME, "r");

	int total = 0;

	struct Hand* hands = malloc(sizeof(struct Hand)*HAND_COUNT);
	int idx = 0;
	while( (read = getline(&lineptr, &n, f)) != -1 ) {

		create_hand(lineptr, hands+idx++);
		lineptr = NULL;
	}

	qsort(hands, HAND_COUNT, sizeof(struct Hand), handcmp);

	for (int i = 0; i < HAND_COUNT; ++i) {
		print_first_order(hands[i].first_order);
		dprintf(" %s", hands[i].repr);
		free(hands[i].repr);
		total += (i+1) * hands[i].bid;
	}
	printf("solution 1: %d\n", total);
	free(hands);
	fclose(f);
}


void solution_2() {
	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen(FILENAME, "r");

	int total = 0;

	struct Hand* hands = malloc(sizeof(struct Hand)*HAND_COUNT);
	int idx = 0;
	while( (read = getline(&lineptr, &n, f)) != -1 ) {

		create_joker_hand(lineptr, hands+idx++);
		lineptr = NULL;
	}

	qsort(hands, HAND_COUNT, sizeof(struct Hand), handcmp_joker);

	for (int i = 0; i < HAND_COUNT; ++i) {
		print_first_order(hands[i].first_order);
		dprintf(" %s", hands[i].repr);
		free(hands[i].repr);
		total += (i+1) * hands[i].bid;
	}
	printf("solution 2: %d\n", total);
	free(hands);
	fclose(f);
}

int main(void) {
	solution_1();
	solution_2();
	return 0;
}

