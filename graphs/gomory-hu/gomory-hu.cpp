// graph has n nodes
// reset() clears all flows in graph
// dinic(s, t) pushes max flow from s to t
// dist[v] is distance from s to v in residual network

vector<vector<long long>> prec;
void buildTree() {
  vector<int> p(n, 0);
  prec = vector<vector<long long>>(n, vector<long long>(n, inff));
  for (int i = 1; i < n; i++) {
    reset();
    long long f = dinic(i, p[i]);
    for (int j = 0; j < n; j++) {
      if (j != i && dist[j] < inff && p[j] == p[i]) {
        p[j] = i;
      }
    }
    prec[p[i]][i] = prec[i][p[i]] = f;
    for (int j = 0; j < i; j++) {
      prec[i][j] = prec[j][i] = min(prec[j][p[i]], f);
    }
    {
      int j = p[i];
      if (dist[p[j]] < inff) {
        p[i] = p[j];
        p[j] = i;
      }
    }
  }
}

long long fastFlow(int S, int T) {
	return prec[S][T];
}