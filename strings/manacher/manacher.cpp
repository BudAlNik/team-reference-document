// returns vector ret of length (|s| * 2 - 1),
//   ret[i * 2] -- maximal length of palindrome with center in i-th symbol
//   ret[i * 2 + 1] -- maximal length of palindrome with center between i-th and (i + 1)-th symbols
vector<int> find_palindromes(string const& s) {
	string tmp;
	for (char c : s) {
		tmp += c;
		tmp += '!';
	}
	tmp.pop_back();

	int c = 0, r = 1;
	vector<int> rad(szof(tmp));
	rad[0] = 1;
	for (int i = 1; i < szof(tmp); ++i) {
		if (i < c + r) {
			rad[i] = min(c + r - i, rad[2 * c - i]);
		}
		while (i - rad[i] >= 0 && i + rad[i] < szof(tmp) && tmp[i - rad[i]] == tmp[i + rad[i]]) {
			++rad[i];
		}
		if (i + rad[i] > c + r) {
			c = i;
			r = rad[i];
		}
	}

	for (int i = 0; i < szof(tmp); ++i) {
		if (i % 2 == 0) {
			rad[i] = (rad[i] + 1) / 2 * 2 - 1;
		} else {
			rad[i] = rad[i] / 2 * 2;
		}
	}

	return rad;
}