all: generate-document build-pdf clean

build-pdf:
	pdflatex -shell-escape reference-document.tex
	pdflatex -shell-escape reference-document.tex

generate-document:
	python3 generate-document.py

clean:
	rm *.aux *.log *.toc *.out
	rm -r _minted-*
