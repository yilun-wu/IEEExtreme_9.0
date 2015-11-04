//#define PRINT_SET cout << "new set: " << endl; for (auto rm : occ_rooms) cout << rm << ' '; cout << endl;
#define PRINT_SET ;
//#define DEB(a) a
#define DEB(a)
#include <iostream>
#include <algorithm>
#include <deque>
#include <unordered_map>
#include <unordered_set>
using namespace std;

using Choice = pair<int,int>;

int main() {
	int tests;
	cin >> tests;
	while (tests--) {
		int rooms, ppl;
		cin >> rooms >> ppl;

		unordered_map<int,int> room_popularity;
		deque<Choice> pplchoices;

		for (int p = 0; p < ppl; ++p) {
			int room, alt;
			cin >> room >> alt;

			pplchoices.push_back(make_pair(room,alt));
			room_popularity[room]++;
			room_popularity[alt]++;
		}


		// sort based on room popularity
		auto pop_sort = [&room_popularity](const Choice& a, const Choice& b){
			return (room_popularity[a.first]+room_popularity[a.second]) > 
				   (room_popularity[b.first]+room_popularity[b.second]);
		};

		sort(begin(pplchoices), end(pplchoices), pop_sort);


		int max_tickets = 2;	// can give at least 2 out, even for 2 most popular rooms
		unordered_set<int> occ_rooms;
		// most popular choices
		occ_rooms.insert(pplchoices[0].first); 
		occ_rooms.insert(pplchoices[0].second); 
		// second most popular choices
		occ_rooms.insert(pplchoices[1].first);
		occ_rooms.insert(pplchoices[1].second);
		pplchoices.pop_front();
		pplchoices.pop_front();

		PRINT_SET;


		while (max_tickets <= (int)occ_rooms.size()) {
			if (pplchoices.empty()) break;	// exhausted all
			// try to choose the most overlapping room (already in occ_room)
			bool double_overlap = false;
			for (auto itr = pplchoices.begin(); itr != pplchoices.end(); ++itr) {
				// first pass try to get double overlap
				if (occ_rooms.find(itr->first) != occ_rooms.end() &&
					occ_rooms.find(itr->second) != occ_rooms.end()) {

					DEB(cout << "double overlap: " << itr->first << ' ' << itr->second << endl);
					double_overlap = true;
					pplchoices.erase(itr);
					break;
				}
			}
			// no room choice is already double covered by set, find the most popular single cover
			bool single_overlap = false;
			if (!double_overlap) {
				for (auto itr = pplchoices.begin(); itr != pplchoices.end(); ++itr) {
					if (occ_rooms.find(itr->first) != occ_rooms.end() ||
						occ_rooms.find(itr->second) != occ_rooms.end()) {

						DEB(cout << "single overlap: " << itr->first << ' ' << itr->second << endl);
						single_overlap = true;
						// add to set
						occ_rooms.insert(itr->first);
						occ_rooms.insert(itr->second);
						pplchoices.erase(itr);

						// reset popularity
						room_popularity.clear();
						for (auto itr = begin(pplchoices); itr != end(pplchoices); ++itr) {
							room_popularity[itr->first]++;
							room_popularity[itr->second]++;
						}
						sort(begin(pplchoices), end(pplchoices), pop_sort);
						break;
					}
				}
			}
			// no overlap, just insert most popular one
			if (!single_overlap && !double_overlap) {
				occ_rooms.insert(pplchoices[0].first);
				occ_rooms.insert(pplchoices[0].second);
				DEB(cout << "no overlap: " << pplchoices[0].first << ' ' << pplchoices[0].second << endl);
				pplchoices.pop_front();
				
				// reset popularity
				room_popularity.clear();
				for (auto itr = begin(pplchoices); itr != end(pplchoices); ++itr) {
					room_popularity[itr->first]++;
					room_popularity[itr->second]++;
				}
				sort(begin(pplchoices), end(pplchoices), pop_sort);
			}

			// updated set
			PRINT_SET;

			++max_tickets;
		}
		if (max_tickets > (int)occ_rooms.size()) --max_tickets;
		max_tickets = min(ppl, max_tickets);
		max_tickets = min(rooms, max_tickets);
		cout << max_tickets << endl;
	}
}