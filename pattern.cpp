// #define DEBUG(x) {cout << x << endl;}
#define DEBUG(x)
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// implementation of KMP with O(n + k) where n and k are the lengths of strings to match
// returns iterator to the start of the word, else s.end()
// good for small # of possible values and resusable subpatterns
// builds backtracking table for use below
template <typename Indexable>
std::vector<int> kmp_table(const Indexable& w) {
    size_t start_pos {};    // current matched up to from start
    std::vector<int> backtrack(w.size()); 
    backtrack[0] = -1;
    backtrack[1] = 0;

    for (size_t pos = 2; pos < w.size();) {
        if (w[pos - 1] == w[start_pos]) {           // substring continues
            ++start_pos;
            backtrack[pos] = start_pos;
            ++pos;
        }
        else if (start_pos > 0) start_pos = backtrack[start_pos];   // fall back to last substring
        else {                              // can't fall back
            backtrack[pos] = 0;
            ++pos;
        }
    }
    return backtrack;
}
template <typename Vec>
void print_vec(const Vec& vec) {
	for (auto c : vec) cout << c << ' ';
	cout << endl;
}
int main() {
	int T;
	cin >> T;
	while (T--) {
		string str;
		cin >> str;
		// terminating sentinel
		str += '0';
		DEBUG("str:" << str);
		auto kmp = kmp_table(str);
		// print_vec(kmp);
		if (kmp[str.size()-1] == 0) cout << str.size() - 1 << endl;
		else {
			cout << str.size() - kmp[str.size()-1] - 1 << endl;
		}
	}
}