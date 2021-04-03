// Rotate 3d point along axis on angle
/*
 * 2D
 * x' = x cos a - y sin a
 * y' = x sin a + y cos a
*/
struct quater {
	double w, x, y, z; // w + xi + yj + zk
	quater(double tw, const pt3 &v) : w(tw), x(v.x), y(v.y), z(v.z) { }
	quater(double tw, double tx, double ty, double tz) : w(tw), x(tx), y(ty), z(tz) { }
	pt3 vector() const {
		return {x, y, z};
	}
	quater conjugate() const {
		return {w, -x, -y, -z};
	}
	quater operator*(const quater &q2) {
		return {w * q2.w - x * q2.x - y * q2.y - z * q2.z, w * q2.x + x * q2.w + y * q2.z - z * q2.y, w * q2.y - x * q2.z + y * q2.w + z * q2.x, w * q2.z + x * q2.y - y * q2.x + z * q2.w};
	}
};
 
pt3 rotate(pt3 axis, pt3 p, double angle) {
	quater q = quater(cos(angle / 2), axis * sin(angle / 2));
	return (q * quater(0, p) * q.conjugate()).vector();
}