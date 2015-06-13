###Compilation
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

---
```
qmake --version
```
>Doit être > 5.4 pour l'utilisation de QOpenGLWidget entre autres

---
```
qmake compile.pro
make
```
>Génération du makefile puis compilation

---
```
export LC_NUMERIC=C
```
>Au cas où Qt aurait modifié le locale

---
