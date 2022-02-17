vector<int> build_suffarr(string s) {
	int n = szof(s);
	auto norm = [&](int num) {
		if (num >= n) {
			return num - n;
		}
		return num;
	};
	vector<int> classes(s.begin(), s.end()), n_classes(n);
	vector<int> order(n), n_order(n);
	iota(order.begin(), order.end(), 0);
	vector<int> cnt(max(szof(s), 128));
	for (int num : classes) {
		cnt[num + 1]++;
	}
	for (int i = 1; i < szof(cnt); ++i) {
		cnt[i] += cnt[i - 1];
	}

	for (int i = 0; i < n; i = i == 0 ? 1 : i * 2) {
		for (int pos : order) {
			int pp = norm(pos - i + n);
			n_order[cnt[classes[pp]]++] = pp;
		}
		int q = -1;
		pii prev = {-1, -1};
		for (int j = 0; j < n; ++j) {
			pii cur = {classes[n_order[j]], classes[norm(n_order[j] + i)]};
			if (cur != prev) {
				prev = cur;
				++q;
				cnt[q] = j;
			}
			n_classes[n_order[j]] = q;
		}
		swap(n_classes, classes);
		swap(n_order, order);
	}
	return order;
}

void solve() {
	string s;
	cin >> s;
	s += "$";
	auto suffarr = build_suffarr(s);

	vector<int> where(szof(s));
	for (int i = 0; i < szof(s); ++i) {
		where[suffarr[i]] = i;
	}

	vector<int> lcp(szof(s));
	int cnt = 0;
	for (int i = 0; i < szof(s); ++i) {
		if (where[i] == szof(s) - 1) {
			cnt = 0;
			continue;
		}
		cnt = max(cnt - 1, 0);
		int next = suffarr[where[i] + 1];
		while (i + cnt < szof(s) && next + cnt < szof(s) && s[i + cnt] == s[next + cnt]) {
			++cnt;
		}
		lcp[where[i]] = cnt;
	}
}