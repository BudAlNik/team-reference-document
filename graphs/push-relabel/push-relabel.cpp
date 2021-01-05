struct edge_t {
	int to;
	int next;
	int64_t flow;
	int64_t capacity;
};

int main() {
	int n = input<int>();
	int m = input<int>();
	int S = 0;
	int T = n - 1;

	vector<edge_t> edges;
	vector<int> head(n, -1);

	auto add_edge = [&](int v, int u, int cap, int rcap) {
		edges.push_back(edge_t {u, head[v], 0, cap});
		head[v] = SZ(edges) - 1;
		edges.push_back(edge_t {v, head[u], 0, rcap});
		head[u] = SZ(edges) - 1;
	};
	
	for (int i = 0; i < m; ++i) {
		int v, u, cap;
		cin >> v >> u >> cap;
		--v, --u;
		add_edge(v, u, cap, 0);
	}
	
	vector<int> d(n);
	vector<int64_t> exc(n);
	d[S] = n;

	auto push_edge = [&](int e, int64_t W) {
		int to = edges[e].to;
		int from = edges[e ^ 1].to;
		edges[e].flow += W;
		edges[e ^ 1].flow -= W;
		exc[from] -= W;
		exc[to] += W;
	};

	auto global_relabel = [&]() {
		for (int v = 0; v < n; ++v)
			if (v != S and v != T)
				d[v] = -1;

		for (int fixed: {T, S}) {
			queue<int> q;
			q.push(fixed);
			while (not q.empty()) {
				int v = q.front();
				q.pop();
				for (int e = head[v]; e != -1; e = edges[e].next) {
					if (edges[e].to != S and edges[e^1].flow != edges[e^1].capacity and d[edges[e].to] == -1) {
						d[edges[e].to] = d[v] + 1;
						q.push(edges[e].to);
					}
				}
			}
		}

		for (int v = 0; v < n; ++v)
			if (d[v] == -1)
				d[v] = 2 * n - 1;
	};
	
	for (int e = head[S]; e != -1; e = edges[e].next) {
		push_edge(e, edges[e].capacity);
	}

	vector<char> in_queue(n, false);
	queue<int> que;

	for (int v = 0; v < n; ++v)
		if (v != S and v != T and exc[v] > 0) {
			in_queue[v] = 1;
			que.push(v);
		}

	int processed = 0;
	while (not que.empty()) {
		if (++processed >= 3 * n) {
			processed -= 3 * n;
			global_relabel();
		}
		
		int v = que.front();
		que.pop();
		in_queue[v] = false;
		
		if (exc[v] == 0)
			continue;

		int new_d = TYPEMAX(int);
		for (int e = head[v]; e != -1; e = edges[e].next) {
			if (edges[e].flow == edges[e].capacity)
				continue;

			if (exc[v] == 0)
				break;
				
			if (d[v] != d[edges[e].to] + 1) {
				new_d = min(new_d, 1 + d[edges[e].to]);
				continue;
			}

			int delta = min(edges[e].capacity - edges[e].flow, exc[v]);
			push_edge(e, delta);

			if (edges[e].flow < edges[e].capacity)
				new_d = min(new_d, 1 + d[edges[e].to]);

			if (exc[edges[e].to] > 0 and edges[e].to != S and edges[e].to != T and not in_queue[edges[e].to]) {
				que.push(edges[e].to);
				in_queue[edges[e].to] = 1;
			}
		}

		if (exc[v]) {
			que.push(v);
			in_queue[v] = true;
			d[v] = new_d;
		}
	}

	cout << exc[T] << "\n";
	for (int i = 0; i < SZ(edges); i += 2)
		cout << edges[i].flow << "\n";
	
	return 0;
}
