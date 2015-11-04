#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
using namespace std;
// #define DEBUG(x) {cout << x << endl;}
#define DEBUG(x) ;
#define NEXT_TURN {DEBUG(st.pi << "->" << st.i << ' ' << st.a << ' ' << st.b << ' ' << st.c);\
				next_turn.push(st);\
				traceback.push_back(st);\
			}

struct State {
	int a,b,c;
	int i,pi;
	bool operator==(const State& o) const {return a == o.a && b == o.b && c == o.c;}
};

int main() {
	// total number less than 3^11 possibilities, 11 moves
	// each turn can have at most 3 turns
	vector<State> traceback;
	traceback.reserve((int)pow(3,11));

	// BFS essentially on possibilities
	queue<State> to_look;
	int a,b,c;
	cin >> a >> b >> c;
	State st {a,b,c,0,0};

	// index for traceback
	int k = 1;
	to_look.push(st);
	traceback.push_back(st);

	for (int t = 0; t < 12; ++t) {
		DEBUG("turn " << t);
		queue<State> next_turn;	
		while (!to_look.empty()) {
			State prob = to_look.front();
			to_look.pop();
			// test against termination
			if (prob.a == 0 || prob.b == 0 || prob.c == 0) {
				// end game, do traceback
				DEBUG("game terminated on turn " << t);
				stack<State> prev_turns;
				prev_turns.push(prob);
				while (prob.i != 0) {
					DEBUG("k " << prob.i);
					DEBUG(traceback.size());
					prob = traceback[prob.pi];
					DEBUG("next k " << prob.i);
					prev_turns.push(prob);
				}
				while (!prev_turns.empty()) {
					prob = prev_turns.top();
					prev_turns.pop();
					cout << prob.a << ' '  << prob.b << ' ' << prob.c << endl;
				}
				return 0;
			}
			// add back to queue
			if (prob.a <= prob.b) {	// (1 2)
				st = State {prob.a * 2, prob.b - prob.a, prob.c, k++, prob.i};
				NEXT_TURN;
			}
			if (prob.a <= prob.c) { // (1 3)
				st = State {prob.a * 2, prob.b, prob.c - prob.a, k++, prob.i};
				NEXT_TURN;
			}
			if (prob.b <= prob.a) { // (2 1)
				st = State {prob.a - prob.b, prob.b * 2, prob.c, k++, prob.i};
				NEXT_TURN;
			}
			if (prob.b <= prob.c) { // (2 3)
				st = State {prob.a, prob.b * 2, prob.c - prob.b, k++, prob.i};
				NEXT_TURN;
			}
			if (prob.c <= prob.a) { // (3 1)
				st = State {prob.a - prob.c, prob.b, prob.c * 2, k++, prob.i};
				NEXT_TURN;
			}
			if (prob.c <= prob.b) { // (3 2)
				st = State {prob.a, prob.b - prob.c, prob.c * 2, k++, prob.i};
				NEXT_TURN;
			}			
		}
		to_look = move(next_turn);
	}
	cout << "Ok\n";
}