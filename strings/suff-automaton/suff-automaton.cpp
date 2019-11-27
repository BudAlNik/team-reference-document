struct state {
	state() { std::fill(next, next + 26, -1); }

	int len = 0, link = -1;
	bool term = false;

	int next[26];
};

vector<state> st;
int last;

void sa_init() {
	last = 0;
	st.clear();
	st.resize(1);
}

void sa_extend(char c) {
	int cur = st.size();
	st.resize(st.size() + 1);

	st[cur].len = st[last].len + 1;
	int p;
	for (p = last; p != -1 && st[p].next[c - 'a'] == -1; p = st[p].link)
		st[p].next[c - 'a'] = cur;
	if (p == -1)
		st[cur].link = 0;
	else {
		int q = st[p].next[c - 'a'];
		if (st[p].len + 1 == st[q].len)
			st[cur].link = q;
		else {
			int clone = st.size();
			st.resize(st.size() + 1);
			st[clone].len = st[p].len + 1;
			std::copy(st[q].next, st[q].next + 26, st[clone].next);
			st[clone].link = st[q].link;
			for (; p != -1 && st[p].next[c - 'a'] == q; p = st[p].link)
				st[p].next[c - 'a'] = clone;
			st[q].link = st[cur].link = clone;
		}
	}
	last = cur;
}

for (int v = last; v != -1; v = st[v].link) // set termination flag.
	st[v].term = 1;
