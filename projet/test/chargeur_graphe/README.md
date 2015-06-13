####Chargeur graphe et exportateur de matrice laplacienne
----------------

Permet de charger un `.mtx`, et on y resort (dans le fichier de sortie `expor`) la matrice laplacienne associé au graphe que l'on peut ensuite charger sur scilab pour nos tests

Récuperer du TP5 de INFO501

`./tp5 GRAPHE.mtx`

avec `GRAPHE.mtx` fichier de données récupérer du site:

http://www.cise.ufl.edu/research/sparse/matrices/groups.html

puis à adapter à la forme suivante avant de passer à `./tp5` :

```
nSommets 12
oriente 0
value 0
complet 0
debutDefAretes
0 1
0 4
0 5
1 2
1 5
2 3
2 5
2 6
3 6
3 7
4 5
4 8
4 9
5 6
5 9
6 7
6 9
6 10
6 11
7 11
8 9
9 10
10 11
finDefAretes
```

juste à virer l'entête du .mtx et à la remplacer par l'entête

```
nSommets 12
oriente 0
value 0
complet 0
debutDefAretes
```

puis ajouter le `debutDefAretes` et le `finDefAretes`

NB temporaire juste pour les tests: Ajouter +1 au nombre de sommets pour les graphes venant de fichier .mtx