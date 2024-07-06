Vaideanu Renata - Georgia 
312 CD 

Am scris mult mai multe comentarii in program, acestea insotind TODO-urile deja existente.

PROXIMAL:

- proximal 2x2(f, STEP) -> Folosesc formula (1/STEP)/2 pentru a afla numarul de pixeli din fiecare 
sfert al noii matricii, 1/STEP ma ajuta sa trec din subunitar in supraunitar si impart apoi la 2 
pentru a determina exact dele 4 "parti" ale noii matricei. Retin cei 4 pixeli initiali. Parcurg 
fiecare pixel din matricea rezultat (pe  care am initializat-o cu 0) si pun unul din cei 4 pixeli
retinuti, in functie de sfertul de matrice in care ma aflu.

- proximal 2x2 RGB(f, STEP) -> Extrag separat R, G si B din matricea originala si aplic 
algoritmul de la proximal 2x2, iar in final pun cele 3 culori in matricea finala.

- proximal resize(I, p, q) -> Calculez factorii de scalare si formez magtricea T, pe care apoi
o inversez. Parcurg pixel cu pixel matricea finala si atribui fiecarui pixel din noua imagine,
pixelul din vechea imagine. Trec din [0, n-1] in [1, n], incrementand xp si yp si calculez 
valoarea pixelului din imaginea finala. In final transform matricea rezultata în uint8 pentru 
a fi o imagine valida.

- proximal resize RGB(I, p, q) -> Extrag separat R, G si B din matricea originala si aplic 
algoritmul de la proximal resize, iar in final pun cele 3 culori in matricea finala.

- proximal_coef -> Folosesc aceasta functie pentru a calcula coeficientii a pentru Interpolarea 
Proximala intre punctele(x1, y1), (x1, y2), (x2, y1) si (x2, y2). Folosesc formulele date in
cerinta si determin matricea A si vectorul b. In final determin coreficientii cu ajutorul functiei
a = A \ b.

- proximal rotate(I, angle) -> Incep prin a calcula cosinusul si sinusul de rotation_angle, 
intializez matricea finala si calculez matricea de transformare, pe care o si inversez. Aplic un 
algoritm similar cu cel de la resize: parcurg fiecare pixel din matricea finala si ii atribui un 
pixel din vechea matrice, iar apoi trec [0, n-1] in [1, n]. In cazul in care xp sau yp se afla 
in exteriorul noii imagini, se pune un pixel negru. Aflu punctele care înconjoara(xp, yp), avand 
grija sa actualizez x1, x2 si y1, y2 daca este necesar. Ma folosesc de proximal_coef pentru a afla
coeficientii, conform formulei si calculez valoarea interpolata a pixelului (x, y). In final 
transform matricea rezultata în uint8 pentru a fi o imagine valida.

- proximal rotate RGB(I, angle) -> Extrag separat R, G si B din matricea originala si aplic 
algoritmul de la proximal rotate, iar in final pun cele 3 culori in matricea finala.

Dupa cate se poate vedea si mai sus, algoritmii RGB sunt aceeasi, indiferent de cerinta.


BICUBIC:

- fx, fy, fxy -> Toate calculeaza derivatele in punctele date, conform formulelor.

- precalc d(I) -> Zerorizez matricile ce contin derivate si le transform in double si apoi le 
calculez pe fiecare in parte, cu ajutorul functiilor fx, fy, fxy, dupa caz.

- bicubic coef(f, Ix, Iy, Ixy, x1, y1, x2, y2) -> Am format cele 4 matrici rezultat, de 2x2, pe 
care le-am transformat in double, iar apoi le transform in double. Folosesc, apoi, matricile date
in cerinta (si acestea fiind schimbate in double) si calculez matricea finala, conform relatiei
date. 

- bicubic resize(I, p, q) -> Acest algoritm este, din nou, foarte asemanator cu cel de la proximal:
calculez factorii de scalare, calculez matricea de transformare, pe care o si inversez. Parcurg
fiecare pixel si transformarea inversa asupra (x, y), calculand x_p si y_p din spatiul imaginii
initiale, trec [0, n-1] in [1, n]. Aflu punctele care înconjoara(xp, yp), avand grija sa 
actualizez x1, x2 si y1, y2 daca este necesar. Ma folosesc de bicubic_coef pentru a afla
coeficientii, conform formulei si trec coordonatele (xp, yp) in patratul unitate, scazand 
(x1, y1), apoi calculez valoarea interpolata a pixelului (x, y) si aflu matricea rezultat.
In final transform matricea rezultata în uint8 pentru a fi o imagine valida.

- bicubic resize RGB(I, p, q) -> Extrag separat R, G si B din matricea originala si aplic 
algoritmul de la bicubic resize, iar in final pun cele 3 culori in matricea finala.