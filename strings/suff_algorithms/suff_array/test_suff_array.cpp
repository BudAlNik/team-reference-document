#include <bits/stdc++.h>
#define ff first
#define ss second
#define szof(x) ((int)x.size())

using namespace std;
typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
int const INF = (int)1e9 + 1e3;
ll const INFL = (ll)1e18 + 1e6;
mt19937 tw(9450189);
uniform_int_distribution<ll> ll_distr;
ll rnd(ll a, ll b) { return ll_distr(tw) % (b - a + 1) + a; }

#include "suff_array.cpp"

vector<int> slow_suff_array(string s) {
	vector<pair<string, int>> order;
	for (int i = 0; i < szof(s); ++i) {
		order.push_back({s.substr(i), i});
	}

	sort(order.begin(), order.end());

	vector<int> ret;
	for (auto& p : order) {
		ret.push_back(p.ss);
	}
	return ret;
}

int main() {
	cerr << setprecision(5) << fixed;
	cout << setprecision(5) << fixed;
	
	for (int i = 0; i < 1000; ++i) {
		int l = rnd(1, 10);
		string s = "";
		for (int j = 0; j < l; ++j) {
			s += (char) rnd(32, 126);
		}

		assert(suff_array(s) == slow_suff_array(s));
	}

	for (int i = 0; i < 1000; ++i) {
		int l = rnd(1, 100);
		string s = "";
		int maxc = rnd(0, 5);
		for (int j = 0; j < l; ++j) {
			s += (char) ('a' + rnd(0, maxc));
		}

		assert(suff_array(s) == slow_suff_array(s));
	}

	cout << "Length; Average time (ms)\n";
	for (int l = 1; ; l = min(1000000, l + 100000)) {
		cerr << "Running length " << l << endl;
		ll t = 0;
		int cnt = 0;
		while (true) {
			string s = "";
			for (int j = 0; j < l; ++j) {
				s += (char) rnd(32, 126);
			}
			auto start = std::chrono::high_resolution_clock::now();
			suff_array(s);
			auto end = std::chrono::high_resolution_clock::now();
			t += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

			++cnt;
			
			if (t > 1e9) {
				break;
			}
		}

		cout << l << "; " << t / (double) 1000000 / cnt << "\n";

		if (l == 1000000) {
			break;
		}
	}
}
