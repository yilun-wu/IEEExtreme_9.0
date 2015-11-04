// #define DEB
#ifdef DEB
	#define DEBUG(x) {cout << x << endl;}
	#define PRINTBOARD(b) {for (int i = 0; i < (int)b.size(); ++i) cout << setw(3) << i << ':' << b[i]; cout << endl;}
#else
	#define DEBUG(x)
	#define PRINTBOARD(b)
#endif
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <unordered_map>
#include <cassert>
#include <unordered_set>
using namespace std;

bool congested(const vector<int>& players, int at) {
	int occ = 0;
	for (int pos : players)
		if (pos == at) ++occ;
	return (occ < 2)? false : true;
}

int main() {
	int M;
	cin >> M;
	vector<string> rawboard;
	for (int i = 0; i < M; ++i) {
		string line;
		cin >> line;
		rawboard.push_back(line);
	}

	vector<int> board(M*M+1);	// 0 indicates nothing, negative number is a character, positive number is a jump
	board[0] = -1;

	int ltr = 1;	// alternatives 
	int step = 1;
	unordered_map<char, int> jumps;
	int heads = 0, tails = 0;
	for (int i = rawboard.size()-1; i >= 0; --i) {
		// bottom up, so will see feet of snake and bunny before their heads
		int j = 0;
		if (ltr == -1) j = M-1;
		for (; j < M && j >= 0; j += ltr) {
			char c = rawboard[i][j];
			if (c == '-') board[step] = 0;
			else if (c >= '0' && c <= '9') {
				DEBUG("Bunny spotted at step " << step << " (" << i << "," << j << ")");
				// bunny
				if (jumps.find(c) != jumps.end()) {
					// head of bunny
					DEBUG("Bunny head comes from " << jumps[c]);
					board[step] = 0;	// nothing happens at head
					board[jumps[c]] = step;	// set the feet to point to here
					jumps[c] = step;
					++heads;
				}
				else {
					DEBUG("Bunny feet");
					// feet of bunny
					board[step] = 0;	// to be relabelled afterwards
					jumps[c] = step;	// mark so we can find it later
					++tails;
				}
			}
			else {
				DEBUG("Snake spotted at step " << step << " (" << i << "," << j << ")");
				// snake
				if (jumps.find(c) != jumps.end()) {
					DEBUG("Snake head points back " << jumps[c]);
					// head of snake
					board[step] = jumps[c];	// go back to tail
					++heads;
				}
				else {
					DEBUG("Snake tail ");
					board[step] = 0;
					jumps[c] = step;
					++tails;
				}
			}
			++step;
		}

		ltr = -ltr;
	}
	PRINTBOARD(board);
	assert(tails == heads);
	// printboard_2d(board, M);

	int ppl;
	cin >> ppl;
	vector<int> players(ppl);
	int player = 0;
	bool double_roll = false;
	// guarenteed that someone will win
	while (true) {
		int move;
		if (double_roll) {
			if (cin >> move)
				double_roll = false;
			else	// end of file
				break;
		} 
		else {
			int d1, d2;
			if (cin >> d1) {
				if (cin >> d2) {
					move = d1 + d2;
					if (d1 == d2) 
						double_roll = true;
				}
				else
					break;
			}
			else
				break;
		}

		unordered_set<int> prev_dests;
		// is allowed to move back to inital position, since that is before rolling and moving
		// they can only "move into" an EVIL CYCLE
		// prev_dests.insert(players[player]);	// initial position

		int dest = players[player] + move;
		DEBUG("Player " << player+1 << " rolled " << move << " going from " << players[player] << " to " << dest);
		players[player] = dest;
		// game termination if player went past
		if (dest >= M*M) {
			players[player] = M*M;
			break;
		}
		while (prev_dests.find(dest) == prev_dests.end() && (board[dest] != 0 || congested(players, dest))) {
			prev_dests.insert(dest);	// becomes a prev dest
			// can never have congestion at a special square
			if (congested(players, dest)) {
				DEBUG("Some player on square " << dest << " moving forward");
				++dest;
			}
			else if (board[dest] != 0) {
				DEBUG("Special square, moving to " << board[dest]);
				dest = board[dest];
			}
			players[player] = dest;
		}
		if (prev_dests.find(dest) != prev_dests.end()) {
			// returned back to original destination, must be a cycle
			cout << "PLAYER " << player+1 << " WINS BY EVIL CYCLE!\n";
			return 0;
		}
		// finally make the move
		players[player] = dest;

		// game termination if a player went past M*M
		if (dest >= M*M) {
			players[player] = M*M;
			break;
		}

		// make another move
		if (double_roll)
			continue;

		player = (player+1) % ppl;
	}

	// print final positions of the players
	for (int pos : players)
		cout << pos << ' ';
	cout << endl;
}