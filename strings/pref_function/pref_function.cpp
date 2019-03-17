vector<int> calc_pref(string s) {
	vector<int> pref(szof(s));
	for (int i = 1; i < szof(s); ++i) {
		pref[i] = pref[i - 1];
		while (pref[i] && s[pref[i]] != s[i]) {
			pref[i] = pref[pref[i] - 1];
		}
		if (s[pref[i]] == s[i]) {
			++pref[i];
		}
	}
	return pref;
}