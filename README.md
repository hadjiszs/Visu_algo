Framework pour la visualisation d'algorithmes intéractifs
=========================================================

#####Technologie utilisée
- C++11
    - Windows: `MinGW 4.9.1`
    - Debian sid: `g++ (Debian 4.9.1-19) 4.9.1`
- Qt5.4
    - Windows: `QtCreator 3.3.1`
    - Debian sid: `qmake version (5.4.0)`
- OpenGL3.3
    - `GLSL 330`
- GLM 0.9.6 (manipulation matricielle, pour matrice projection/model/view)
- GSL 1.16 (pour le calcul des valeurs/vecteurs propres pour la methode spectral graph drawing)
- Scilab pour test

####Résumé

>les algorithmes semi ou complètement intéractifs permettent de combiner les connaissances d'un expert aux capacités de calcul d'une machine pour solutionner un problème. Le but de ce projet serait de proposer un framework permettant de mettre en place un algorithme (semi-)intéractif en permettant à l'utilisateur de visualiser son déroulement et agir sur celui-ci aux moments où il l'aura désiré. Pour ce faire, la plateforme proposée devra mettre à disposition de manière générique les moyens de brancher un algorithme sur celui-ci (notion de template) tout en fournissant les représentations standard de solution (graphe, arbre, vecteur, ...) pour visualisation mais aussi les briques pour en créer d'autres. Il faudra bien sûr réfléchir à l'ergonomique de l'affichage pour le déroulement de l'algorithme (pas à pas, taux de rafraichissement, ...).

####Sources

######Force directed graph drawing
- http://arxiv.org/abs/1201.3011
- http://fr.wikipedia.org/wiki/Force-based_layout

######Spectral graph drawing
- http://www.sciencedirect.com/science/article/pii/S089812210500204X
- http://www.wisdom.weizmann.ac.il/~verify/publications/2003/Kor03.pdf (partie 3 essentiellement)
- http://www.research.att.com/export/sites/att_labs/groups/infovis/res/legacy_papers/DBLP-journals-camwa-Koren05.pdf (page 9 essentiellement)
- http://en.wikipedia.org/wiki/Laplacian_matrix pour un peu mieux comprendre la représentation par matrice de laplace
- http://www.cs.yale.edu/homes/spielman/sgta/SpectTut.pdf premier test sur scilab basé sur l'exemple à partir de slide 7

######Banque de joli graphe artistique

- http://www.cise.ufl.edu/research/sparse/matrices/HB/index.html
- http://www.cise.ufl.edu/research/sparse/matrices/groups.html

>récupérable sous format MTX, que l'on peut ensuite représenter en matrice laplacienne
>impossible pour l'instant d'avoir un aperçu des graphes avec un nombre de noeuds supérieur à ~100 en passant par Scilab.

######Autre banque de graphe intéressant
- http://konect.uni-koblenz.de/networks/
- http://networkrepository.com/index.php
- http://snap.stanford.edu/data/
- http://www-personal.umich.edu/~mejn/netdata/
- http://law.di.unimi.it/datasets.php