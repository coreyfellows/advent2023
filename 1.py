#!/usr/bin/python3
import re
with open("1.txt", "r") as f:
    lines = list(map(str.strip, f.readlines()))


def solution_one(printit):
    total = 0
    for line in lines:
        matches = re.findall("(\d)", line)
        result = int(f"{matches[0]}{matches[-1]}")
        total += result

        if printit:
            print(f"{line} => {result}")

    return total

#part 1
print(solution_one(False))
#part 2

mappings = {
    "one": "1",
    "two": "2",
    "three": "3",
    "four": "4",
    "five": "5",
    "six": "6",
    "seven": "7",
    "eight": "8",
    "nine": "9"
}

def converter(first, second):
    first = mappings.get(first, first)
    second = mappings.get(second, second)
    return int(f"{first}{second}")

def solution_two(printit):
    total = 0
    words_forward = "|".join(list(mappings.keys()))
    words_backwards = words_forward[::-1]
    words_forward = f"({words_forward}|\d)"
    words_backwards = f"({words_backwards}|\d)"

    for line in lines:
        first = re.search(words_forward, line).group()
        second = re.search(words_backwards, line[::-1]).group()[::-1]
        result = converter(first, second)
        total += result

        if printit:
            print(f"{line} => {result}")

    return total

print(solution_two(False))
