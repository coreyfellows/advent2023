#pragma once
#define uint unsigned long

uint is_alpha(char c);
uint is_digit(char c);

uint startswith(char* string, char* start);
uint ipow(uint a, uint b);

// parses int at start and stores result in result;
// returns length of the string representation of the number.
uint parse_uint(char* start, uint* result);
uint parse_int(char* start, long* result);
