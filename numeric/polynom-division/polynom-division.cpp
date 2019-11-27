poly inv(poly A, int n) // returns A^-1 mod x^n
{
	assert(sz(A) && A[0] != 0);
	A.cut(n);

	auto cutPoly = [](poly &from, int l, int r) {
		poly R;
		R.v.resize(r - l);
		for (int i = l; i < r; ++i) {
			if (i < sz(from))
				R[i - l] = from[i];
		}
		return R;
	};

	function<int(int, int)> rev = [&rev](int x, int m) -> int {
		if (x == 1)
			return 1;
		return (1 - rev(m % x, x) * (ll)m) / x + m;
	};

	poly R({rev(A[0], mod)});
	for (int k = 1; k < n; k <<= 1) {
		poly A0 = cutPoly(A, 0, k);
		poly A1 = cutPoly(A, k, 2 * k);
		poly H = A0 * R;
		H = cutPoly(H, k, 2 * k);
		poly R1 = (((A1 * R).cut(k) + H) * (poly({0}) - R)).cut(k);
		R.v.resize(2 * k);
		forn(i, k) R[i + k] = R1[i];
	}
	return R.cut(n).norm();
}

pair<poly, poly> divide(poly A, poly B) {
	if (sz(A) < sz(B))
		return {poly({0}), A};

	auto rev = [](poly f) {
		reverse(all(f.v));
		return f;
	};

	poly q =
			rev((inv(rev(B), sz(A) - sz(B) + 1) * rev(A)).cut(sz(A) - sz(B) + 1));
	poly r = A - B * q;

	return {q, r};
}