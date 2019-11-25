import os
import sys
import subprocess
import hashlib

content = ""
errors_cnt = 0
warnings_cnt = 0

chart_template = open("chart-template.tex").read()

def get_mited_format(ext):
	if ext == "cpp":
		return "[mathescape, breaklines, tabsize=4, frame=lines, linenos=true]{c++}"
	elif ext == "py":
		return "[mathescape, breaklines, tabsize=4]{python}"


section_names = ["section", "subsection", "subsubsection", "paragraph"]
def add_section(depth, name):
	global content
	assert(depth <= len(section_names))
	name.strip()
	content += "\\" + section_names[depth - 1] + "{" + name + "}\n"


def add_plaintext(text, color=None):
	global content
	text.strip()
	if color == None:
		content += text + "\n"
	else:
		content += "\\textcolor{%s}{%s}" % (color, text)


def add_minted(format, path):
	global content
	content += "\\inputminted" + format + "{" + path + "}\n"


def add_verbatim(text):
	global content
	content += "\\begin{verbatim}\n"
	content += text
	content += "\\end{verbatim}\n"


def add_error(text):
	global content, errors_cnt
	errors_cnt += 1
	content += "\\textcolor{red}{\\textbf{Error:} " + text + "}\n"


def add_warning(text):
	global content, warnings_cnt
	warnings_cnt += 1
	content += "\\textcolor{orange}{\\textbf{Warning:} " + text + "}\n"


def escape_latex(s):
	s = s.replace("_", "\\_")
	return s


def parse_csv(s):
	table = []
	for row in s.split("\n"):
		if row.strip() == "":
			continue
		table.append([])
		for s in row.split("; "):
			table[-1].append(s.strip())
	return table


def add_chart(table):
	global content
	cur_chart = chart_template
	cur_chart = cur_chart.replace("$xlabel", table[0][0])
	cur_chart = cur_chart.replace("$ylabel", table[0][1])
	coords = ""
	for i in range(1, len(table)):
		coords += "(%s, %s)\n" % (table[i][0], table[i][1])

	cur_chart = cur_chart.replace("$coordinates", coords)

	content += "\\begin{center}"
	content += cur_chart
	content += "\\end{center}"


def handle_test_outp(test_outp):
	add_chart(parse_csv(test_outp))


def run_test(path, file):
	print("Testing " + os.path.join(path, file), file=sys.stderr)
	test = os.path.join(path, "test_" + file)
	cache = os.path.join(path, "test_" + file + ".cache")
	file = os.path.join(path, file)
	hash_file = hashlib.md5(open(file).read().encode("utf-8")).hexdigest()
	hash_test = hashlib.md5(open(test).read().encode("utf-8")).hexdigest()
	if os.path.isfile(cache):
		print("Checking cache", file=sys.stderr)
		cache_inp = open(cache)

		if cache_inp.readline().strip() == hash_file and cache_inp.readline().strip() == hash_test:
			print("Relevant cache", file=sys.stderr)
			add_plaintext("\\textcolor{violet}{\\checkmark Tests passed}")
			handle_test_outp(cache_inp.read())
			return
		
		print("Outdated cache, removing", file=sys.stderr)
		os.remove(cache)

	if subprocess.call("g++ -Wall -Wextra -Wshadow -fno-stack-limit -std=c++11 -O2 -o tmp %s" % test, shell=True):
		print("Compilation error", file=sys.stderr)
		add_error("Compilation error while compiling test")
	else:
		print("Compiled", file=sys.stderr)
		proc = subprocess.Popen("./tmp", shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
		if proc.wait():
			print("Tests failed", file=sys.stderr)
			add_error("Tests failed")
		else:
			add_plaintext("\\textcolor{violet}{\\checkmark Tests passed}")
			test_outp = proc.stdout.read().decode("utf-8")
			print("Writing cache")
			cache_outp = open(cache, "w")
			print(hash_file, file=cache_outp)
			print(hash_test, file=cache_outp)
			print(test_outp, file=cache_outp)
			handle_test_outp(test_outp)
			

verification_flag = True

def dfs(path, depth):
	if depth > 0:
		name = "\\textbf{No name} (\\texttt{%s})" % (escape_latex(os.path.basename(path)))
		if os.path.isfile(os.path.join(path, "name")):
			name = open(os.path.join(path, "name")).read().strip()
		add_section(depth, name)

	if os.path.isfile(os.path.join(path, "info.tex")):
		add_plaintext(open(os.path.join(path, "info.tex")).read())

	dirs = []
	files = []

	for file in os.listdir(path):
		if file == "info.tex" or file == "name" or file.startswith("test") or file.startswith("_") or file.startswith(".") or file.endswith(".cache"):
			continue

		if os.path.isdir(os.path.join(path, file)):
			dirs.append(file)
		elif os.path.isfile(os.path.join(path, file)):
			if file.endswith(".tex"):
				files.append((file[:-4], 0))
			else:
				files.append((file, 1))

	if depth > 0:
		if len(files) == 0 and len(dirs) == 0:
			add_warning("Leaf directory without any information")
		files.sort()

		for file in files:
			if file[1] == 0:
				add_plaintext(open(os.path.join(path, file[0] + ".tex")).read())
			else:
				format = get_mited_format(file[0].split(".")[-1])
				add_minted(format, os.path.join(path, file[0]))
				if verification_flag:
					if os.path.isfile(os.path.join(path, "test_" + file[0])):
						run_test(path, file[0])
					else:
						add_warning("No tests")
					

	for dir in dirs:
		dfs(os.path.join(path, dir), depth + 1)

dfs(".", 0)

template = open("template.tex").read()

if not verification_flag:
	verification = "\\textcolor{orange}{Without verification}"
else:
	if errors_cnt == 0:
		verification = "No errors"
	elif errors_cnt == 1:
		verification = "\\textcolor{red}{1 error}"
	else:
		verification = "\\textcolor{red}{%d errors}" % errors_cnt

	verification += ", "

	if warnings_cnt == 0:
		verification += "No warnings"
	elif warnings_cnt == 1:
		verification += "\\textcolor{orange}{1 warning}"
	else:
		verification += "\\textcolor{orange}{%d warnings}" % warnings_cnt


print(template.replace("% Content", content).replace("% Verification", verification), file=open("reference-document.tex", "w"))