import math
def make_combs(combs):
	num = 0
	combs.sort()
	if combs[1] == combs[2] and combs[2] == combs[0]:
		return 2*combs[1] - math.ceil(combs[1]/2)
	if combs[1] == combs[2]:
		return combs[0] + (combs[1] - math.ceil(combs[0]/2))
	if combs[2] - combs[1] > combs[0]:
		return combs[0] + combs[1]
	else:
		num = combs[2] - combs[1]
		combs[2] -= num
		combs[0] -= num
		num += combs[0]
		num += (combs[1] - math.ceil(combs[0]/2) )
		return num

n = int(input())
for i in range(n):
	line = [int(a) for a in input().split()]
	shells = line[0]
	combs = make_combs(line[1:])
	print(min(shells, combs))
