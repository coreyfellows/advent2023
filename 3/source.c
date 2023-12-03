#include <stdlib.h>
#include <stdio.h>

const int line_count = 140;
const int line_width = 141;

int is_digit(char c) {
	return c >= '0' && c <= '9';
}

char* find_int_start(char* position) {
	if (!is_digit(*position)) {
		return NULL;
	}
	while (is_digit(*(position-1))) position--;

	return position;
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


static char *readcontent(const char *filename)
{
	char *fcontent = NULL;
	int fsize = 0;
	FILE *fp;

	fp = fopen(filename, "r");
	if(fp) {
		fseek(fp, 0, SEEK_END);
		fsize = ftell(fp);
		rewind(fp);

		fcontent = (char*) malloc(sizeof(char) * fsize);
		fread(fcontent, 1, fsize, fp);

		fclose(fp);
	}
	return fcontent;
}

int check(char* position, int x, int y, int def, int* found) {
	//move pointer
	position = position + x + line_width*y;

	//find start of integer
	position = find_int_start(position);

	if (position) {
		int value;
		int length = parse_int(position, &value);
		//blank that number to prevent double counts.
		for (int i = 0; i < length; i++) {
			position[i] = '.';
		}
		printf("%d, ", value);
		++*found;
		return value;
	}
	return def;
}


int process_line(char* array, int y) {
	int total = 0;
	for (int x = 0; x < 140; ++x) {
		char* position = array + x + line_width*y;

		if (is_digit(*position) || *position == '.') {
			continue;
		}
		for (int sub_y = -1; sub_y < 2; sub_y++) {

			for (int sub_x = -1; sub_x < 2; sub_x++) {
				if (sub_x == 0 && sub_y == 0) {
					continue;
				}
				int found = 0;
				total += check(position, sub_x, sub_y, 0, &found);
			}
		}
	}
	return total;
}

int process_line_gear(char* array, int y) {
	int total = 0;


	for (int x = 0; x < 140; ++x) {
		char* position = array + x + line_width*y;

		if (*position != '*') {
			continue;
		}
		int num_found = 0;
		int ratio = 1;
		for (int sub_x = -1; sub_x < 2; sub_x++) {
			for (int sub_y = -1; sub_y < 2; sub_y++) {
				if (sub_x == 0 && sub_y == 0) {
					continue;
				}

				ratio *= check(position, sub_x, sub_y, 1, &num_found);
			}
		}
		if (num_found == 2) {
			printf(" = %d", ratio);
			total += ratio;
		}
		printf("\n");
	}
	return total;
}

int solution() {
	int total_parts = 0;
	int total_gear = 0;

	char* array = readcontent("input.txt");
	for (int i = 1; i < line_count-1; ++i) {
		total_parts += process_line(array, i);
	}
	free(array);

	//this is a destructive process, it removes used numbers from grid.
	array = readcontent("input.txt");
	for (int i = 1; i < line_count-1; ++i) {
		total_gear += process_line_gear(array, i);
	}
	free(array);

	printf("\n total parts: %d", total_parts);
	printf("\n total gear: %d", total_gear);
}

int main(void) {
	solution();
	return 0;
}