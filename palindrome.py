from functools import reduce
def mtod(year, m):
	if m == 1 or m == 3 or m == 5 or m == 7 or m == 8 or m == 10 or m == 12:
		return 31
	if m == 2:
		return 29 if (year % 4 == 0 and (year % 100 != 0 or year % 400 == 0)) else 28
	return 30
def revstr(a):
	return "".join(a[::-1])
YYYY="@"
MM="!"
DD="#"
YY="$"
def domdy(a):
	return a.replace("yyyy", YYYY).replace("dd", DD).replace("MM", MM).replace("yy", YY)
def prefformit(a, year, month, day):
	out = ""
	for i in a:
		if i == "M":
			out += str(month)
		elif i == MM:
			out += ("" if month >= 10 else "0") + str(month)
		elif i == "d":
			out += str(day)
		elif i == DD:
			out += ("" if day >= 10 else "0") + str(day)
		elif i == YY:
			y = year % 100
			out += pad2(y)
		elif i == YYYY:
			out += str(year)
	return out
def pad2(h):
	return ("" if h >= 10 else "0") + str(h)
def suffix(i, hour, minute, second):
	out = ""
	if i == AMPM:
		h = hour % 12
		h = h if h != 0 else 12
		out += str(h)
	elif i == AMPMP:
		h = hour % 12
		h = h if h != 0 else 12
		out += pad2(h)
	elif i == HR:
		out += str(hour)
	elif i == HRP:
		out += pad2(hour)
	return out

def gethrs(p, i):
    return range(0, 24)


mdy = [domdy(a) for a in "Mdyy, MMdyy, Mddyy, MMddyy, Mdyyyy, MMdyyyy, " \
	"Mddyyyy, MMddyyyy, dMyy, dMMyy, ddMyy, ddMMyy, dMyyyy, " \
	"dMMyyyy, ddMyyyy, ddMMyyyy".split(", ")]
#print(mdy)

AMPM="0"
AMPMP="1"
HR="2"
HRP="3"
postfix=[(AMPM, 12), (AMPMP, 12), (HR, 24), (HRP, 24)]

def palin(year):
	palins = set()
	#pd1 = set()
	for prefform in mdy:			
		for month in range(1, 13 if MM in prefform else 10):
			for day in range(1, mtod(year, month) + 1 if DD in prefform else 10):
				pref = prefformit(prefform, year, month, day)
				#if month == 1 and day == 1:
				#	print(prefform, pref)
				second = int(pref[0:2][::-1])
				if second >= 60:
					continue
				minute = int(pref[2:4][::-1])
				if minute >= 60:
					continue
				minsec = pad2(minute) + pad2(second)
				for suffixform in postfix:
					hourmax = suffixform[1]
					for hour in gethrs(pref, suffixform[0]):
						back = suffix(suffixform[0], hour, minute, second) + minsec
						res = pref + back
						moment = (month, day, hour, minute, second)
						if res == revstr(res):
							#if month == 1 and day == 1:
							#	pd1.add(moment)
							palins.add(moment)
	#pd1_ = list(pd1)
	#pd1_.sort()
	#for p in pd1_:
	#	print(p)
	#print(len(pd1_))
	return len(palins)
#import time
#print(time.time())
for tc in range(int(input())):
	print(palin(int(input())))
#print(time.time())