#include <iostream>
#include <deque>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
	long long miles, num_cows;
	cin >> miles >> num_cows;

	deque<pair<long long,int>> cows;
	while (num_cows--) {
		long long cow_mile, cow_lane;
		cin >> cow_lane >> cow_mile;
		cows.push_back(make_pair(cow_mile,cow_lane));
	}
	auto cow_sort = [](const pair<long long,int>& a, const pair<long long,int>& b){return a.first < b.first;};
	sort(cows.begin(), cows.end(), cow_sort);
	// for all cows closing a lane
	int blocked = 0x0;
	for (int i = 0; i < cows.size(); ++i) {
		// all blocked
		// cout << blocked << endl;
		blocked |= 1 << (cows[i].second - 1);
		if (blocked == 0xF) {
			cout << 0 << endl;
			return 0;
		}
		if (i+1 < cows.size() && cows[i].first != cows[i+1].first) // no longer on same mile
			blocked = 0x0;
	}

	if (miles > 100000000) {
		cout << 0 << endl;
		return 0;
	}

	// for (auto cow : cows) {
	// 	cout << cow.first << ' ' << cow.second << endl;
	// }

	vector<long long> pos(4), nxtpos(4);
	pos[0] = 1;	// start with 1 way to reach starting point
	long long cow_mile = -1;
	int cow_lane = 0;
	if (!cows.empty()) {
		auto cur_cow = cows.front();
		cows.pop_front();
		cow_mile = cur_cow.first;
		cow_lane = cur_cow.second;
	}

	long long modulo = 1000000007;
	for (long long mile = 1; mile < miles; ++mile) {
		nxtpos[0] = (pos[0] + pos[1]) % modulo;	// leftmost
		nxtpos[1] = (pos[0] + pos[1] + pos[2]) % modulo;
		nxtpos[2] = (pos[1] + pos[2] + pos[3]) % modulo;
		nxtpos[3] = (pos[2] + pos[3]) % modulo;
		if (mile == cow_mile-1) {
			while (mile == cow_mile-1) {
				// cout << "cow " << cow_mile << ' ' << cow_lane << endl;
				nxtpos[cow_lane-1] = 0;	// reset lane to be 0
				auto cur_cow = cows.front();
				cows.pop_front();
				cow_mile = cur_cow.first;
				cow_lane = cur_cow.second;
			} 
			// nxtpos[cow_lane-1] = 0;
			if (cow_mile == mile && cows.empty()) { // ran out of cows
				cow_mile = -1;
			}
		}
		// cout << "mile " << mile+1 << ": "; 
		for (int i = 0; i < 4; ++i) {
			pos[i] = nxtpos[i];
		// 	cout << pos[i] << ' ';
		}
		// cout << endl;
	}
	cout << pos[0] << endl;
}