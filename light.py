from fractions import gcd	
def debug(*args):
	print(*args)
	return

def lcm(a,b):
	return a * (b // gcd(a,b))
def lcm_prime(a,b):
	return a * b

tests = int(input())

for t in range(tests):
	line = [int(a) for a in input().split()]
	switches = line[0]
	n = line[1]
	grems = line[2:]

	# print(grems)
	conflicts = {}
	on = 0
	for i in range(len(grems)):
		on += switches // grems[i]
		debug(on)
		this_conflicts = set()
		# test against all previous ones for conflict
		for j in range(i):
			conflict = lcm_prime(grems[i],grems[j])
			if conflict <= switches and conflict not in this_conflicts:
				this_conflicts.add(conflict)
				if conflict in conflicts:
					conflicts[conflict] += 1
				else:
					conflicts[conflict] = 1

		debug(conflicts)

	# has lcm of all conflicts, now find how it affected times
	conflicts2 = {}
	for conflict,times in conflicts.items():
		tmp = set()
		for cc in conflicts2:
			cc2 = lcm(conflict, cc)
			if cc2 <= switches:
				tmp.add(cc2)
		for cc2 in tmp:
			if cc2 in conflicts2:
				conflicts2[cc2] += 1
			else:
				conflicts2[cc2] = 1

		conflicts2[conflict] = 1
		print(conflicts2)

		if times % 2 == 1:	# odd number of times conflict means switched off
			on -= (switches // conflict) * (times + 1) # overcounted all previous times
		else:
			on -= (switches // conflict) * times		# overcounted all except the last time

	# take care of overcounted lcms
	for conflict, times in conflicts2.items():
		debug("{}: {}".format(conflict, switches // conflict))
		if times >= 2:
			on += (switches // conflict) * times * 2
		# else:
			# on += 
	print(on)