struct SVG {
	FILE *out;
	ld sc = 50;
	void open() {
		out = fopen("image.svg", "w");
		fprintf(out, "svg xmlns='http://www.w3.org/2000/svg' viewBox='-1000 -1000 2000 2000'>\n");
	}
	void line(point a, point b) {
		a = a * sc, b = b * sc;
		fprintf(out, "line x1='%f' y1='%f' x2='%f' y2='%f' stroke='black'/>\n", a.x, -a.y, b.x, -b.y);
	}
	void circle(point a, ld r = -1, string col = "red") {
		r = sc * (r == -1 ? 0.3 : r);
		a = a * sc;
		fprintf(out, "circle cx='%f' cy='%f' r='%f' fill='%s'/>\n", a.x, -a.y, r, col.c_str());
	}
	void text(point a, string s) {
		a = a * sc;
		fprintf(out, "text x='%f' y='%f' font-size='100px'>%s</text>\n", a.x, -a.y, s.c_str());
	}
	void close() {
		fprintf(out, "</svg>\n");
		fclose(out);
		out = 0;
	}
	~SVG() {
		if (out) {
			close();
		}
	}
} svg;