g = graphe("graphe1.txt")
g:afficherMatrice ()
g:set (0,0,512)
g:afficherMatrice ()
d = g:get(0,0)
print(d)
print("Nombre de sommets: ")
print(g.n)
g.n = 10
print(g.n)
g:afficherMatrice()

