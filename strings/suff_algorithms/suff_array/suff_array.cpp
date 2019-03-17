vector<int> suff_array(string s) {
	s += '\0';
	int n = s.size();
	vector<int> classes(s.begin(), s.end()), new_classes(n);
	vector<int> order(n), new_order(n);
	iota(order.begin(), order.end(), 0);

	function<int(int)> mod = [&](int num) {
		if (num >= n) {
			return num - n;
		}
		return num;
	};

	vector<int> from(max(n, 128) + 1);
	for (int num : classes) {
		from[num + 1]++;
	}

	for (int i = 1; i < (int) from.size(); ++i) {
		from[i] += from[i - 1];
	}

	for (int i = 0; i < n; i == 0 ? i = 1 : i <<= 1) {
		for (int j = 0; j < n; ++j) {
			int pos = mod(order[j] - i + s.size());
			new_order[from[classes[pos]]++] = pos;
		}

		swap(order, new_order);

		int cnt = -1;
		for (int j = 0; j < n; ++j) {
			if (j == 0 || classes[order[j]] != classes[order[j - 1]] || classes[mod(order[j] + i)] != classes[mod(order[j - 1] + i)]) {
				++cnt;
				from[cnt] = j;
			}
			new_classes[order[j]] = cnt;
		}

		swap(classes, new_classes);
	}

	order.erase(order.begin());
	return order;
}
