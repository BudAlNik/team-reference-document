vector<int> data(n);

// Adds value $val$ on position $pos$
auto addval = [&](int pos, int val) {
	while (pos < n) {
		data[pos] += val;
		pos |= (pos + 1);
	}
};

// Returns sum of values on half-interval $[0, pos)$
auto getsum = [&](int pos) {
	int ret = 0;
	while (pos) {
		ret += data[pos - 1];
		pos = pos & (pos - 1);
	}
	return ret;
};