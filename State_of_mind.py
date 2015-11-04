import math
statenum, inputnum = [int(a) for a in input().strip().split(" ")]
states = [0]*statenum
class State:
	pass
def parseexp(b):
	a = b.split("/")
	c = a[0].split(",")
	d = []
	for ex in c:
		e = ex.split("=")
		d.append((ord(e[0])-ord("A"), int(e[1])))
	return (int(a[1]), d)
def newstate(state, inputs):
	stateinfo = states[state]
	for tran in stateinfo.trans:
		for exp in tran[1]:
			if inputs[exp[0]] != exp[1]:
				break
		else:
			return tran[0]
	return state
def dumpline(o, i, n):
	endi = i + 16
	if endi > len(o):
		endi = len(o)
	markers = ["___", "***"]
	outstr = "".join([markers[a[n]] for a in o[i:endi]])
	outname = (chr(n + ord("A")) + "     ") if n != len(o[i])-2 else "OUT   "
	return outname + outstr
for i in range(statenum):
	stateinfo = input().strip().split(" ")
	state = State()
	state.output = int(stateinfo[0])
	state.trans = [parseexp(b) for b in stateinfo[2:]]
	states[i] = state
timesteps, state = [int(a) for a in input().strip().split(" ")]
outputs = []
for t in range(timesteps):
	inputs = [int(a) for a in input().strip().split(" ")]
	state = newstate(state, inputs)
	outputs.append(inputs + [states[state].output, state])
numoutputlines = math.ceil(len(outputs) / 16)
for i in range(numoutputlines):
	starti = i*16
	endi = min(len(outputs), starti + 16)
	out = outputs[starti]
	print("Tick #" + str(starti + 1))
	for varnum in range(len(out) - 1):
		print(dumpline(outputs, starti, varnum))
	states = "".join(["{: 3d}".format(o[-1]) for o in outputs[starti:endi]])
	print("STATE " + states)
	print()
