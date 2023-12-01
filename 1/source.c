#include <stdio.h>
#include <stdlib.h>

const char* digits[9] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

int is_word(char* string, char* word) {
	// if we've fallen off the word, we've found it
	if (!*word) {
		return 1;
	}

	// if we've fallen off the string we haven't
	if(!*string) {
		return 0;
	}
	//and if the strings dont match, we havent found it either.
	if (*string != *word) {
		return 0;
	}

	return is_word(string+1, word+1);
}

int get_digit(char* ptr) {
	if (*ptr >= '0' && *ptr <= '9') {
		return *ptr - '0';
	}

	return 0;
}

int get_digit_with_words(char* ptr) {
	int digit = get_digit(ptr);
	if (digit) {
		return digit;
	}
	for (int i = 0; i < 9; ++i) {
		if (is_word(ptr, digits[i])) {
			return i + 1;
		}
	}
}

int solution(int (*get_digit_ptr)(char*)) {

	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen("data.txt", "r");

	unsigned int total = 0;
	while( (read = getline(&lineptr, &n, f)) != -1 ) {

		char* idx = lineptr;
		unsigned int first_digit = 0;
		unsigned int second_digit = 0;

		while ( *idx ) {
			int current_digit = get_digit_ptr(idx++);
			if (!current_digit) {
				continue;
			}

			if (!first_digit) {
				first_digit = current_digit;
			}
			second_digit = current_digit;
		}

		unsigned int digit = first_digit * 10 + second_digit;
		printf("%d => %s", digit, lineptr);
		total += digit;

		free(lineptr);
		lineptr = NULL;
	}
	return total;

}

void main(void) {
	int total_1 = solution(get_digit);
	int total_2 = solution(get_digit_with_words);
	printf("solution 1: %d\n", total_1);
	printf("solution 2: %d\n", total_2);
	return 0;
}
