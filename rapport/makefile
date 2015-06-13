TEX = pdflatex -file-line-error
NOM = rapport

all: pdf bib pdf

pdf:	
	$(TEX) $(NOM).tex ; $(TEX) $(NOM).tex > /dev/null

bib:
	bibtex $(NOM)

voir:
	nohup evince $(NOM).pdf

clean:
	rm -f	$(NOM).aux \
		$(NOM).toc \
		$(NOM).bbl \
		$(NOM).blg \
		$(NOM).out \
		$(NOM).log

mrproper: clean
	rm -f   $(NOM).pdf

re: mrproper all
