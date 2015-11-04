import heapq
# http://www.redblobgames.com/pathfinding/a-star/implementation.html
class PriorityQueue:
    def __init__(self):
        self.elements = []
    
    def empty(self):
        return len(self.elements) == 0
    
    def put(self, item, priority):
        heapq.heappush(self.elements, (priority, item))
    
    def get(self):
        return heapq.heappop(self.elements)[1]

debug = False

class Block:
	pass
def genmoves(curstate):
	for bi in range(len(blocks)):
		for dire in [-1, 1]:
			newpos = blockmove(curstate, bi, dire)
			#print(bi, newpos, ingrid(newpos, bi))
			if ingrid(newpos, bi) and canmove(curstate, bi, newpos):
				yield (bi, dire)
def ingrid(newpos, bi):
	bl = blocks[bi]
	return newpos[0] >= 0 and newpos[0] + bl.height <= gridwidth \
		and newpos[1] >= 0 and newpos[1] + bl.width <= gridwidth
def canmove(curstate, mybi, newpos):
	for bi in range(len(blocks)):
		if bi == mybi:
			continue
		if collide(mybi, newpos, bi, blockpos(curstate, bi)):
			return False
	return True
def blockpos(curstate, bi):
	bl = blocks[bi]
	bv = curstate[bi] if bl.vertical else bl.v
	bh = curstate[bi] if not bl.vertical else bl.h
	return (bv, bh)
def blockmove(curstate, bi, d):
	bl = blocks[bi]
	bv, bh = blockpos(curstate, bi)
	bdv = d if bl.vertical else 0
	bdh = d if not bl.vertical else 0
	return (bv + bdv, bh + bdh)
def collide(bi, pos1, targetbi, pos2):
	bl = blocks[bi]
	bl2 = blocks[targetbi]
	horiz = pos1[1] < pos2[1]+bl2.width and pos1[1] + bl.width > pos2[1]
	vert = pos1[0] < pos2[0]+bl2.height and pos1[0] + bl.height > pos2[0]
	#print(bi, pos1, targetbi, pos2, horiz, vert)
	return horiz and vert
def move(curstate, m):
	newstate = list(curstate)
	newstate[m[0]] += m[1]
	return newstate
def board2s(board):
	return ",".join([str(a) for a in board])
def runstate(curstate, t):
	# http://www.redblobgames.com/pathfinding/a-star/implementation.html
	#print("runstate")
	current = board2s(curstate)
	costsofar = {current: 0}
	#camefrom = {current: None}
	frontier = PriorityQueue()
	frontier.put(curstate, 0)
	while not frontier.empty():
		curstate = frontier.get()
		if curstate[0] >= gridwidth - 2:
			return costsofar[board2s(curstate)]
		for m in genmoves(curstate):
			board = move(curstate, m)
			debug and print(m, board)
			next = board2s(board)
			current = board2s(curstate)
			newcost = costsofar[current] + 1
			if next not in costsofar or newcost < costsofar[next]:
				costsofar[next] = newcost
				priority = newcost
				frontier.put(board, priority)
				#camefrom[next] = m
			
		
def makestate():
	b = [0]*len(blocks)
	for bi in range(len(blocks)):
		b[bi] = blocks[bi].v if blocks[bi].vertical else blocks[bi].h
	return b

testcount = int(input())
for tes in range(testcount):
	blocks = [Block()]
	blocks[0].width = 2
	blocks[0].height = 1
	blocks[0].vertical = False
	blocks[0].v = 2
	blocks[0].h = int(input())
	gridwidth = int(input())
	browncount = int(input())
	for b in range(browncount):
		block = Block()
		s, d, r, c = input().split(" ")
		block.vertical = d == "V"
		if block.vertical:
			block.width = 1
			block.height = int(s)
		else:
			block.height = 1
			block.width = int(s)
		block.v = int(r)
		block.h = int(c)
		blocks.append(block)
	state = makestate()
	
	print(runstate(state, 1))

