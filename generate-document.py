import os
import sys
import subprocess
import hashlib

content = ""
errors_cnt = 0
warnings_cnt = 0

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
	print(format, path, file=sys.stderr)
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


def walk_directories(path, depth):
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
		if file == "info.tex" or file == "name" or file.startswith("."):
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

	for dir in dirs:
		walk_directories(os.path.join(path, dir), depth + 1)

walk_directories(".", 0)

template = open("template.tex").read()

verification = ""
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