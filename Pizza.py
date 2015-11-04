# http://mathworld.wolfram.com/CatalanNumber.html
# http://mathworld.wolfram.com/EulersPolygonDivisionProblem.html
# https://raw.githubusercontent.com/LemonPi/sal/master/algo/numeric.h
import sys
def catalanrev(target):
	res = 1 # C_0 = 1
	i = 1
	while res < target:
		res = (2 * (2*i + 1) * res) // (i + 2)
		i += 1
	return i
for l in sys.stdin:
	print(catalanrev(int(l)) + 2)
