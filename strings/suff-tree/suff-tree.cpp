#include <bits/stdc++.h>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

const int N = 1e5, VN = 2 * N;

char s[N + 1];
map<char, int> t[VN];
int l[VN], r[VN], p[VN]; // edge p[v] -> v matches to [l[v], r[v]) of string
int cc, n, suf[VN], vn = 2, v = 1, pos; // going by edge from p[v] to v, now standing in pos

void go(int v) {
	int no = cc++;
	for (auto p : t[v]) {
		v = p.second;
		printf("%d %d %d\n", no, l[v], min(n, r[v]));
		go(v);
	}
}

int main() {
	assert(freopen("suftree.in", "r", stdin));
	assert(freopen("suftree.out", "w", stdout));

	gets(s);
	forn(i, 127) t[0][i] = 1; // 0 = fictitious, 1 = root
	l[1] = -1;
	for (n = 0; s[n]; n++) {
		char c = s[n];
		auto new_leaf = [&](int v) {
			p[vn] = v, l[vn] = n, r[vn] = N, t[v][c] = vn++;
		};
	go:;
		if (r[v] <= pos) {
			if (!t[v].count(c)) {
				new_leaf(v), v = suf[v], pos = r[v];
				goto go;
			}
			v = t[v][c], pos = l[v] + 1;
		} else if (c == s[pos]) {
			pos++;
		} else {
			int x = vn++;
			l[x] = l[v], r[x] = pos, l[v] = pos;
			p[x] = p[v], p[v] = x;
			t[p[x]][s[l[x]]] = x, t[x][s[pos]] = v;
			new_leaf(x);
			v = suf[p[x]], pos = l[x];
			while (pos < r[x])
				v = t[v][s[pos]], pos += r[v] - l[v];
			suf[x] = (pos == r[x] ? v : vn);
			pos = r[v] - (pos - r[x]);
			goto go;
		}
	}
	printf("%d\n", vn - 1);
	go(1);
}