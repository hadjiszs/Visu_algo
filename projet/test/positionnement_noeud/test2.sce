// Test avec la matrice suivante :
K3 = [
3	-1	0	0	-1	-1	0	0	0	0	0	0 ;
-1	3	-1	0	0	-1	0	0	0	0	0	0 ;
0	-1	4	-1	0	-1	-1	0	0	0	0	0 ;
0	0	-1	3	0	0	-1	-1	0	0	0	0 ;
-1	0	0	0	4	-1	0	0	-1	-1	0	0 ;
-1	-1	-1	0	-1	6	-1	0	0	-1	0	0 ;
0	0	-1	-1	0	-1	7	-1	0	-1	-1	-1 ;
0	0	0	-1	0	0	-1	3	0	0	0	-1 ;
0	0	0	0	-1	0	0	0	2	-1	0	0 ;
0	0	0	0	-1	-1	-1	0	-1	5	-1	0 ;
0	0	0	0	0	0	-1	0	0	-1	3	-1 ;
0	0	0	0	0	0	-1	-1	0	0	-1	3 ]

// graphe venant de : http://cosy.univ-reims.fr/~pdelisle/fichiers/info0501_14-15/Tp5_Info0501.pdf

dimension = size(K3);
dim = dimension(1); // matrice carré, en param 1 ou 2 -> pareil

// Calcul valeur/vecteur propre
B = eye(K3);
k = dim-1;
sigma = 'SM';

[d, v] = eigs(K3, B, k, sigma);

Xx = v(:,2);
Yy = v(:,3);
Zz = v(:,4);

// calcul matrice Zz2

unitaire = ones(dim);
Zz2 = Zz*unitaire;
Zz2 = eye(Zz2).*Zz2;
// fin calcul matrice Zz2

param3d1(Xx, Yy, Zz);

a = get("current_axes");
 
h = a.children;
h.line_mode = "off";
h.mark_mode = "on";
h.mark_size = 1;
h.mark_foreground = 5;

// affichage des aretes
for i = 1:dim
  for j = 1:dim
    if K3(i,j) == -1 then
      // tracer l'arete i j
      x1 = [ Xx(i); Xx(j) ];
      y1 = [ Yy(i); Yy(j) ];
      z1 = [ Zz(i); Zz(j) ];
      
      printf('\n{ { %ff, %ff, %ff } , { 0.0f, 0.0f, 1.0f } }', Xx(i), Yy(i), Zz(i));
	  printf('\n{ { %ff, %ff, %ff } , { 0.0f, 0.0f, 1.0f } }', Xx(j), Yy(j), Zz(j));
	  
      xarrows(x1, y1, z1, 0, 2);

    end
  end
end