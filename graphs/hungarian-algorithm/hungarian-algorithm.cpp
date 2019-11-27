namespace hungary {
const int N = 210;

int a[N][N];
int ans[N];

int calc(int n, int m) {
	++n, ++m;
	vector<int> u(n), v(m), p(m), prev(m);
	for (int i = 1; i < n; ++i) {
		p[0] = i;
		int x = 0;
		vector<int> mn(m, INF);
		vector<int> was(m, 0);
		while (p[x]) {
			was[x] = 1;
			int ii = p[x], dd = INF, y = 0;
			for (int j = 1; j < m; ++j)
				if (!was[j]) {
					int cur = a[ii][j] - u[ii] - v[j];
					if (cur < mn[j])
						mn[j] = cur, prev[j] = x;
					if (mn[j] < dd)
						dd = mn[j], y = j;
				}
			for (int j = 0; j < m; ++j) {
				if (was[j])
					u[p[j]] += dd, v[j] -= dd;
				else
					mn[j] -= dd;
			}
			x = y;
		}
		while (x) {
			int y = prev[x];
			p[x] = p[y];
			x = y;
		}
	}
	for (int j = 1; j < m; ++j) {
		ans[p[j]] = j;
	}
	return -v[0];
}
// How to use:
// * Set values to a[1..n][1..m] (n <= m)
// * Run calc(n, m) to find minimum
// * Optimal edges are (i, ans[i]) for i = 1..n
// * Everything works on negative numbers
//
// !!! I don't understand this code, it's copypasted from e-maxx
} // namespace hungary