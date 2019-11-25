python3 generate-document.py
pdflatex -shell-escape reference-document.tex
pdflatex -shell-escape reference-document.tex
rm *.aux *.log *.toc
rm _minted-* -r