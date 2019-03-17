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

#include "pref_function.cpp"

vector<int> slow_calc_pref(string s) {
	vector<int> ret(szof(s));
	for (int i = 0; i < szof(s); ++i) {
		for (int j = 0; j < i + 1; ++j) {
			if (s.substr(0, j) == s.substr(i - j + 1, j)) {
				ret[i] = j;
			}
		}
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

		assert(calc_pref(s) == slow_calc_pref(s));
	}

	for (int i = 0; i < 1000; ++i) {
		int l = rnd(1, 100);
		string s = "";
		int maxc = rnd(0, 5);
		for (int j = 0; j < l; ++j) {
			s += (char) ('a' + rnd(0, maxc));
		}

		assert(calc_pref(s) == slow_calc_pref(s));
	}

	cout << "Length; Average time (ms)\n";
	for (int l = 1; ; l += 1000000) {
		cerr << "Running length " << l << endl;
		ll t = 0;
		int cnt = 0;
		while (true) {
			string s = "";
			for (int j = 0; j < l; ++j) {
				s += (char) rnd(32, 126);
			}
			auto start = std::chrono::high_resolution_clock::now();
			calc_pref(s);
			auto end = std::chrono::high_resolution_clock::now();
			t += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

			++cnt;
			
			if (t > 1e9 || cnt > 1000) {
				break;
			}
		}

		cout << l << "; " << t / (double) 1000000 / cnt << "\n";

		if (l >= 10000000 && cnt <= 10) {
			break;
		}
	}
}
