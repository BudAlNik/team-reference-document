// assume p > 1
bool isprime(ll p) {
	const int a[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 0};
	ll d = p - 1;
	int cnt = 0;
	while (!(d & 1)) {
		d >>= 1;
		cnt++;
	}
	for (int i = 0; a[i]; i++) {
		if (p == a[i]) {
			return true;
		}
		if (!(p % a[i])) {
			return false;
		}
	}
	for (int i = 0; a[i]; i++) {
		ll cur = mpow(a[i], d, p); // a[i] ^ d (mod p)
		if (cur == 1) {
			continue;
		}
		bool good = false;
		for (int j = 0; j < cnt; j++) {
			if (cur == p - 1) {
				good = true;
				break;
			}
			cur = mult(cur, cur);
		}
		if (!good) {
			return false;
		}
	}
	return true;
}