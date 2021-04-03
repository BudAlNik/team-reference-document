poly transform(poly P, bool inverse) {
	for (len = 1; 2 * len <= degree(P); len <<= 1) {
		for (i = 0; i < degree(P); i += 2 * len) {
			for (j = 0; j < len; j++) {
				u = P[i + j];
				v = P[i + len + j];
				
				if (!inverse) {
					P[i + j] = v;
					P[i + len + j] = u + v;
				} else {
					P[i + j] = -u + v;
					P[i + len + j] = u;
				}
			}
		}
	}
	
	return P;
}