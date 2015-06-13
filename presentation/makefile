TEX = pdflatex -file-line-error
NOM = presentation

all: $(NOM).pdf

# Deux compilations consécutives dans tous les cas
# Pour mettre à jour la table des matieres
# Lors de la modification du .aux
$(NOM).pdf: $(NOM).tex
	$(TEX) $^ ; $(TEX) $^ > /dev/null

voir:
	nohup evince $(NOM).pdf

clean:
	rm -f	$(NOM).aux \
		$(NOM).toc \
		$(NOM).out \
		$(NOM).log

mrproper: clean
	rm -f   $(NOM).pdf

re: mrproper all