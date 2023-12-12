SAMPLE = False

if SAMPLE:
	filename = 'sample.txt'
else:
	filename = 'input.txt'


def matches_range(line, i, range):
	initial_idx = i
	if i >= len(line) or line[i] == '.':
		#print("over edge or its a .", len(line), i, line[i])
		return False

	while range and i < len(line) and (line[i] == '#' or line[i] == '?'):
		i += 1
		range -= 1

	if range:
		return False

	return (i >= len(line) or line[i] == '?' or line[i] == '.') and (initial_idx == 0 or line[initial_idx-1] != '#')

# ?##???.??#?
def is_valid_arrangement(line, ranges, index_stack):
	for idx, char in enumerate(line):
		if char != '#':
			continue
		found = False
		for r, dest_idx in zip(ranges, index_stack):
			if idx >= dest_idx and idx < dest_idx+r:
				 found = True

		if not found:
			return False

	return True

def find_arrangements(line, linei, ranges, rangei, index_stack, visited, invalid):

	if rangei >= len(ranges):
		rendered = ",".join(map(str, index_stack))
		if  rendered in visited or rendered in invalid:
			return 0
		if is_valid_arrangement(line, ranges, index_stack):
			#render_solution(ranges, index_stack, len(line))
			visited.add(rendered)
			return 1
		invalid.add(rendered)
		return 0

	while linei < len(line):
		if matches_range(line, linei, ranges[rangei]):

			index_stack.append(linei)
			find_arrangements(line, linei+ranges[rangei]+1, ranges, rangei+1, index_stack, visited, invalid)
			index_stack.pop()
		linei += 1


def render_solution(ranges, solution, total_length):

	globalidx = 0
	repr = ""
	for range, idx in zip(ranges, solution):
		if globalidx < idx:
			repr += "."*(idx-globalidx)
			globalidx += (idx-globalidx)
		repr += range*"#"
		globalidx += range
	if len(repr) < total_length:
		repr += "."*(total_length-len(repr))
	print(repr)


def solution1():

	with open(filename, "r") as f:
		lines = f.readlines()

	count = 0
	for line in lines:
		line, ranges = line.split(' ')
		ranges = [int(i) for i in ranges.split(',')]
		print(line, ranges)
		visited = set()
		invalid = set()
		index_stack = []
		find_arrangements(line, 0, ranges, 0, index_stack, visited, invalid)
		visited = [[int(i) for i in solution.split(',')] for solution in visited]
		for solution in sorted(visited):
			render_solution(ranges, solution, len(line))
		print(len(visited))
		count += len(visited)

	print("total solutions:", count)

def solution2():

	with open(filename, "r") as f:
		lines = f.readlines()

	count = 0
	for line in lines:
		line, ranges = line.split(' ')
		line = "?".join([line,]*5)
		ranges = [int(i) for i in ranges.split(',')]
		ranges = ranges * 5
		print(line, ranges)
		visited = set()
		invalid = set()
		index_stack = []
		find_arrangements(line, 0, ranges, 0, index_stack, visited, invalid)
		visited = [[int(i) for i in solution.split(',')] for solution in visited]
		# for solution in sorted(visited):
		# 	render_solution(ranges, solution, len(line))
		print(len(visited))
		count += len(visited)

	print("total solutions:", count)

solution2()

# print("should be 1\n")
# print( matches_range("#...",0, 1))
# print( matches_range("?...",0, 1))
# print( matches_range("#?...",0, 1))
# print( matches_range("#?...",0, 2))
# print( matches_range("?#...",0, 2))
# print( matches_range("?#?...",0, 3))
# print( matches_range("#?#...",0, 3))
# print( matches_range("?#?#...",0, 4))
# print( matches_range("#?#?...",0, 4))
# print( matches_range("???????",0, 2))
# print( matches_range("??????",0, 2))
# print( matches_range("?????",0, 2))
# print( matches_range("????",0, 2))
# print( matches_range("???",0, 2))
# print( matches_range("??",0, 2))

# print( matches_range("???????",0, 3))
# print( matches_range("??????",0, 3))
# print( matches_range("?????",0, 3))
# print( matches_range("????",0, 3))
# print( matches_range("???",0, 3))


# print("should be 0\n")
# print( matches_range("?#...",0, 1))
# print( matches_range("##...",0, 1))
# print( matches_range("?#?...",0, 1))
# print( matches_range(".?#?...",0, 2))
# print( matches_range("## 1,1,3",0, 3))
# print( matches_range("?###????????", 2, 3))

