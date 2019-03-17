namespace fft {
	int const BP = 20, SZ = 1 << BP;
	int const INV_SZ = mpow(SZ, MOD - 2);
	int perm[SZ], roots[SZ];
	int arr1[SZ], arr2[SZ];

	void fft(int* arr) {
		for (int i = 0; i < SZ; ++i) {
			if (perm[i] > i) {
				swap(arr[i], arr[perm[i]]);
			}
		}

		for (int i = 1, diff_pow = SZ >> 1; i < SZ; i <<= 1, diff_pow >>= 1) {
			for (int j = 0; j < SZ; j += i * 2) {
				int cur_pow = 0;
				for (int k = 0; k < i; ++k) {
					int b = mult(arr[j + i + k], roots[cur_pow]);
					arr[j + i + k] = sum(arr[j + k], MOD - b);
					add(arr[j + k], b);
					cur_pow += diff_pow;
				}
			}
		}
	}

	void fill_arr(vector<int> const& a, int* arr) {
		for (int i = 0; i < SZ; ++i) {
			if (i < (int) a.size()) {
				arr[i] = a[i];
			} else {
				arr[i] = 0;
			}
		}
	}

	vector<int> mult(vector<int> const& a, vector<int> const& b) {
		fill_arr(a, arr1);
		fft(arr1);
		fill_arr(b, arr2);
		fft(arr2);
		for (int i = 0; i < SZ; ++i) {
			arr1[i] = mult(arr1[i], arr2[i]);
		}
		fft(arr1);
		reverse(arr1 + 1, arr1 + SZ);
		vector<int> ret;
		for (int i = 0; i < SZ; ++i) {
			ret.push_back(mult(arr1[i], INV_SZ));
		}
		while (ret.back() == 0) {
			ret.pop_back();
		}
		return ret;
	}

	void init() {
		int rt = 646; // this is precalculated $2^{20}$-th root of $1$ for MOD = $998\,244\,353$
		/*
		for (int i = 0; i < MOD; ++i) {
			if (mpow(i, 1 << (BP - 1)) == MOD - 1) {
				rt = i;
				break;
			}
		}
		*/

		roots[0] = 1;
		for (int i = 1; i < SZ; ++i) {
			perm[i] = (perm[i >> 1] >> 1) | ((i & 1) << (BP - 1));
			roots[i] = mult(roots[i - 1], rt);
		}
	}
}