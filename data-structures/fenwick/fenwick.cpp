const int MAXN = 1001;
vector<ll> arr(MAXN);

// adds $val$ at position $x$
auto fenwick_add = [&](int x, ll val) {
	for (; x < MAXN; x = x | (x + 1))
		arr[x] += val;
};

// returns $\sum\limits_{i = 0}^{x} arr[i]$ ($x$ is inclusive)
auto fenwick_get = [&](int x) {
	ll res = 0;
	for (; x >= 0; x = (x & (x + 1)) - 1)
		res += arr[x];
	return res;
};