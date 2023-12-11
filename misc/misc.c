#include "misc.h"

uint is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

uint is_digit(char c) {
	return c >= '0' && c <= '9';
}


uint startswith(char* string, char* start) {
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
uint parse_uint(char* start, uint* result) {
	if (*start > '9' || *start < '0') {
		*result = 0;
		return 0;
	}
	uint power = parse_uint(start+1, result);
	*result += ipow(10, power) * (*start - '0');
	return power + 1;
}


uint parse_int(char* start, long* result) {

	int length = 0;
	int multiplier = 1;
	if (*start == '-') {
		start++;
		multiplier = -1;
		length++;
	}

	length += parse_uint(start, result);
	*result *= multiplier;
	return length;
}
