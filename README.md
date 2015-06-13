Framework pour la visualisation d'algorithmes intéractifs
=========================================================

Base de framework permettant de visualiser en 2D ou 3D l'execution d'un algorithme sur une structure de donnée défini (graphe / arbre / tableau / etc)

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

####Description

>les algorithmes semi ou complètement intéractifs permettent de combiner les connaissances d'un expert aux capacités de calcul d'une machine pour solutionner un problème. Le but de ce projet serait de proposer un framework permettant de mettre en place un algorithme (semi-)intéractif en permettant à l'utilisateur de visualiser son déroulement et agir sur celui-ci aux moments où il l'aura désiré. Pour ce faire, la plateforme proposée devra mettre à disposition de manière générique les moyens de brancher un algorithme sur celui-ci (notion de template) tout en fournissant les représentations standard de solution (graphe, arbre, vecteur, ...) pour visualisation mais aussi les briques pour en créer d'autres. Il faudra bien sûr réfléchir à l'ergonomique de l'affichage pour le déroulement de l'algorithme (pas à pas, taux de rafraichissement, ...).

####Rendu

#####Vidéo
- [DFS sur un graphe](https://raw.githubusercontent.com/hadjiszs/Visu_algo/master/demo/billboard.webm)
- [Autres](https://github.com/hadjiszs/Visu_algo/tree/master/demo)

#####Image
![DFS](http://i.imgur.com/cTnOiS6.png)
