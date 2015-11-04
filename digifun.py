def solve(a):
	i = 1
	while len(a) != int(a):
		a = str(len(a))
		i += 1
	return i
while True:
	a = input().strip()
	if a == "END":
		break
	print(solve(a))
