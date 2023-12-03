#include <stdio.h>
#include <stdlib.h>

int is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_digit(char c) {
	return c >= '0' && c <= '9';
}

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


int check_game(char* game_start) {
	if (!startswith(game_start, "Game")) {
		return 0;
	}
	//advance to game id
	game_start += 5;
	int game_id = 0;
	//get game id and advance to first number in the game.
	game_start += 2 + parse_int(game_start, &game_id);

	while(*game_start) {
		int number = 0;
		game_start += 1 + parse_int(game_start, &number);
		if(
			(startswith(game_start, "blue") && number > 14) ||
			(startswith(game_start, "green") && number > 13) ||
			(startswith(game_start, "red") && number > 12)
			) {
			return 0;
		}
		//advance to next number or end of line
		while (*game_start && !is_digit(*game_start)) game_start++;
	}
	return game_id;
}

int check_game_fewest(char* game_start) {

	if (!startswith(game_start, "Game")) {
		return 0;
	}
	//advance to game id
	game_start += 5;
	int game_id = 0;
	//get game id and advance to first number in the game.
	game_start += 2 + parse_int(game_start, &game_id);

	int red = 0;
	int blue = 0;
	int green = 0;

	while(*game_start) {
		int number = 0;
		game_start += 1 + parse_int(game_start, &number);
		if(startswith(game_start, "blue") && number > blue) {
			blue = number;
		}

		if(startswith(game_start, "red") && number > red) {
			red = number;
		}

		if(startswith(game_start, "green") && number > green) {
			green = number;
		}

		while (*game_start && !is_digit(*game_start)) game_start++;
	}

	return red * green * blue;
}



int solution() {

	char* lineptr = NULL;
	size_t n = 0;
	ssize_t read = 0;
	FILE* f = fopen("input.txt", "r");

	unsigned int total_1 = 0;
	unsigned int total_2 = 0;
	while( (read = getline(&lineptr, &n, f)) != -1 ) {

		total_1 += check_game(lineptr);
		total_2 += check_game_fewest(lineptr);

		free(lineptr);
		lineptr = NULL;
	}
	fclose(f);
	printf("solution 1: %d\n", total_1);
	printf("solution 2: %d\n", total_2);

}

int main(void) {
	solution();
	return 0;
}