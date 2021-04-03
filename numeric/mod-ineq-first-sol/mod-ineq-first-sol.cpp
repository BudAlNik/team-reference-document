// returns value of (p + step * x), i.e. number of steps x = (ans - p) / step (mod mod)
int smart_calc(int mod, int step, int l, int p) {
	if (p < l) {
		return p;
	}
	int d = (mod - p + step - 1) / step;
	int np = (p + d * step) % mod;
	if (np < l) {
		return np;
	}
	int res = smart_calc(step, mod % step, l, l + step - 1 - np);
	return l - 1 - res;
}