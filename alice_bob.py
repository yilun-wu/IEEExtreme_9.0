debug = False
n = int(input())
rpswin = {}
rpsdefeats = {}
def d(a, b):
	rpswin[(a, b)] = True
	rpswin[(b, a)] = False
	if not b in rpsdefeats:
		rpsdefeats[b] = [a]
	else:
		rpsdefeats[b].append(a)
d("Scissors", "Paper")
d("Paper", "Rock")
d("Rock", "Lizard")
d("Lizard", "Spock")
d("Spock", "Scissors")
d("Scissors", "Lizard")
d("Lizard", "Paper")
d("Paper", "Spock")
d("Spock", "Rock")
d("Rock", "Scissors")
def solve(ass, bs, n):
	a = ass
	b = bs
	scores = [0, 0]
	ties = 0
	game = 0
	pastmoves = []
	pastscores = []
	while True:
		pastmoves.append((a, b))
		pastscores.append((scores[0], scores[1], ties))
		tied = a == b
		if not tied:
			winner = 0 if rpswin[a, b] else 1
			scores[winner] += 1
		else:
			winner = -1
			ties += 1
		debug and print("game", game, a, b, tied, winner, scores)
		# alice
		if tied:
			achoose = rpsdefeats[a]
			a = achoose[0] if rpswin[(achoose[0], achoose[1])] else achoose[1]
		elif winner != 0:
			achoose = rpsdefeats[b]
			a = achoose[0] if rpswin[(achoose[0], achoose[1])] else achoose[1]
		
		#bob
		if b != "Spock":
			b = "Spock"
		else:
			if tied:
				b = "Lizard"
			elif winner == 1:
				b = "Rock"
			elif winner == 0:
				b = "Paper"
		game += 1
		if n == game:
			break
		if (a, b) in pastmoves:
			pastindex = pastmoves.index((a, b))
			# looped around
			looplen = game - pastindex
			l = (n - pastindex) // looplen
			rem = (n - pastindex) % looplen
			beforeloopsc = pastscores[pastindex]
			pastscores.append((scores[0], scores[1], ties))
			sc = pastscores[-1]
			debug and print("Looping?", n, game, a, b, "pastindex", pastindex)
			#print(pastmoves)
			debug and print("score", sc, "bef", beforeloopsc, "rem", rem)
			p1ends = beforeloopsc[0] + (sc[0]-beforeloopsc[0])*l + (pastscores[pastindex + rem][0]-beforeloopsc[0])
			p2ends = beforeloopsc[1] + (sc[1]-beforeloopsc[1])*l + (pastscores[pastindex + rem][1]-beforeloopsc[1])
			tiends = beforeloopsc[2] + (sc[2]-beforeloopsc[2])*l + (pastscores[pastindex + rem][2]-beforeloopsc[2])
			debug and print("projected: ", p1ends, p2ends, tiends)
			#p1stub, p2stub, tiesstub = solve(a, b, n % game)
			#return (p1endloop + p1stub, p2endloop + p2stub, tiesloop + tiesstub)
			return p1ends, p2ends, tiends
	return scores[0], scores[1], ties
def solves(a, b, n):
	p1s, p2s, ti = solve(a, b, n)
	debug and print("score:", p1s, p2s, ti)
	if p1s > p2s:
		print("Alice wins, by winning", p1s, "game(s) and tying", ti, "game(s)")
	elif p2s > p1s:
		print("Bob wins, by winning", p2s, "game(s) and tying", ti, "game(s)")
	else:
		print("Alice and Bob tie, each winning", p1s, "game(s) and tying", ti, "game(s)")
for ts in range(n):
	ass, bs, n = input().split(" ")
	solves(ass, bs, int(n))
