// returns vector ret of length (|s| * 2 - 1),
//   ret[i * 2] -- maximal length of palindrome with center in i-th symbol
//   ret[i * 2 + 1] -- maximal length of palindrome with center between i-th and (i + 1)-th symbols
vector<int> find_palindromes(string const& s) {
	string t(szof(s) * 2 - 1, '$');
	for (int i = 0; i < szof(s); ++i) {
		t[i * 2] = s[i];
	}

	int c = 0, r = 1;
	vector<int> d(szof(t));
	d[0] = 1;
	for (int i = 1; i < szof(t); ++i) {
		if (i < c + r) {
			d[i] = min(c + r - i, d[2 * c - i]);
		}
		while (i - d[i] >= 0 && i + d[i] < szof(t) && t[i - d[i]] == t[i + d[i]]) {
			++d[i];
		}
		if (i + d[i] > c + r) {
			c = i;
			r = d[i];
		}
	}

	for (int i = 0; i < szof(t); ++i) {
		if (i % 2 == 0) {
			d[i] = (d[i] + 1) / 2 * 2 - 1;
		} else {
			d[i] = d[i] / 2 * 2;
		}
	}

	return d;
}