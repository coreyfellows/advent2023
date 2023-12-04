#include <stdio.h>
#include <stdlib.h>

int is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_digit(char c) {
	return c >= '0' && c <= '9';
}


//#define dprintf(...) printf(__VA_ARGS__)
#define dprintf(...)

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

int ipow(int a, int b) {

	if (!b) {
		return 1;
	}
	int result = a;
	while (--b) {
		result *= a;
	}
	return result;
}
// parses int at start and stores result in result;
// returns length of the string representation of the number.
int parse_int(char* start, int* result) {
	if (*start > '9' || *start < '0') {
		*result = 0;
		return 0;
	}
	int power = parse_int(start+1, result);
	*result += ipow(10, power) * (*start - '0');
	return power + 1;
}


int find_winner(int winner, char* scratch_start) {
	while(*scratch_start && !is_digit(*scratch_start)) { scratch_start++;}
	if (!*scratch_start) return 0;

	int number = 0;
	scratch_start += parse_int(scratch_start, &number);
	if (number == winner) {
		return 1;
	}
	return find_winner(winner, scratch_start);
}

int number_of_winners(char* start) {

	while(*start++ != ':');
	char* winner_start = start;
	char* scratch_start = start;
	while(*scratch_start++ != '|');

	int winning_total = 0;
	while(*winner_start && *winner_start != '|') {
		while(*winner_start && *winner_start != '|' && !is_digit(*winner_start)) { winner_start++;}

		int winning_value;
		winner_start += parse_int(winner_start, &winning_value);
		int is_winner = find_winner(winning_value, scratch_start);
		if (is_winner) {
			winning_total += is_winner;
		}
	}

	return winning_total;
}

int solution_1() {

	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen("input.txt", "r");

	int total = 0;
	while( (read = getline(&lineptr, &n, f)) != -1 ) {
		int winners = number_of_winners(lineptr);
		if (winners) {
			total += ipow(2, winners-1);
		}
		free(lineptr);
		lineptr = NULL;
	}

	printf("solution 1 total: %d\n", total);
	fclose(f);
}


struct CardCounter {
	struct CardCounter* next;
	int card_count;
};

void increment_card_counter(struct CardCounter* head, int count, int value) {

	if (!count) return;

	if (!head->next) {
		head->next = malloc(sizeof(struct CardCounter));
		head->next->next = NULL;
		head->next->card_count = 1;
	}
	head = head->next;
	head->card_count += value;
	increment_card_counter(head, count-1, value);
}

void print_card_counter(struct CardCounter* head) {
	if (head) {
		dprintf("%d ", head->card_count);
		print_card_counter(head->next);
	}
}

int solution_2() {

	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen("input.txt", "r");

	int total = 0;
	struct CardCounter* counter = NULL;

	while( (read = getline(&lineptr, &n, f)) != -1 ) {

		if (!counter) {
			counter = malloc(sizeof(struct CardCounter));
			counter->next = NULL;
			counter->card_count = 1;
		}

		// incremenet our total card count.
		total += counter->card_count;

		int winners = number_of_winners(lineptr);
		// if we're a winner, increment the next N bonus cards with current bonus cards.
		if (winners) {
			increment_card_counter(counter, winners, counter->card_count);
		}

		dprintf("winners: %d ", winners);
		dprintf("total: %d counter:", total);
		print_card_counter(counter->next);
		dprintf("\n");

		//prepare for next loop.
		struct CardCounter* tmp = counter;
		counter = counter->next;

		free(tmp);
		free(lineptr);
		lineptr = NULL;
	}

	printf("solution 2 total: %d\n", total);
	fclose(f);
}

int main(void) {
	solution_1();
	solution_2();
	return 0;
}
