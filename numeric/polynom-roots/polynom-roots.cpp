const double EPS = 1e-9;
double cal(const vector<double> &coef, double x) {
	double e = 1, s = 0;
	for (double i : coef) s += i * e, e *= x;
	return s;
}
 
int dblcmp(double x) {
	if (x < -EPS) return -1;
	if (x > EPS) return 1;
	return 0;
}
 
double find(const vector<double> &coef, double l, double r) {
	int sl = dblcmp(cal(coef, l)), sr = dblcmp(cal(coef, r));
	if (sl == 0) return l;
	if (sr == 0) return r;
	for (int tt = 0; tt < 100 && r - l > EPS; ++tt) {
		double mid = (l + r) / 2;
		int smid = dblcmp(cal(coef, mid));
		if (smid == 0) return mid;
		if (sl * smid < 0) r = mid;
		else l = mid;
	}
	return (l + r) / 2;
}
 
vector<double> rec(const vector<double> &coef, int n) {
	vector<double> ret; // c[0]+c[1]*x+c[2]*x^2+...+c[n]*x^n, c[n]==1
	if (n == 1) {
		ret.push_back(-coef[0]);
		return ret;
	}
	vector<double> dcoef(n);
	for (int i = 0; i < n; ++i) dcoef[i] = coef[i + 1] * (i + 1) / n;
	double b = 2; // fujiwara bound
	for (int i = 0; i <= n; ++i) b = max(b, 2 * pow(fabs(coef[i]), 1.0 / (n - i)));
	vector<double> droot = rec(dcoef, n - 1);
	droot.insert(droot.begin(), -b);
	droot.push_back(b);
	for (int i = 0; i + 1 < droot.size(); ++i) {
		int sl = dblcmp(cal(coef, droot[i])), sr = dblcmp(cal(coef, droot[i + 1]));
		if (sl * sr > 0) continue;
		ret.push_back(find(coef, droot[i], droot[i + 1]));
	}
	return ret;
}
 
vector<double> solve(vector<double> coef) {
	int n = coef.size() - 1;
	while (coef.back() == 0) coef.pop_back(), --n;
	for (int i = 0; i <= n; ++i) coef[i] /= coef[n];
	return rec(coef, n);
}