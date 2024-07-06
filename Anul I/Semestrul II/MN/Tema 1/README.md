# Tema-1-MN
Tema 1 - MN --> Anul1 - Semestrul2

Apartenenta.m -> in cadrul functiei am calculat y in functie de cele 3 cazuri din cerinta: y=0 daca x<val1, y=1 daca x apartine intervalului (val1, 1] si in rest se calculeaza y cu ajutorul formulei de aflare a valorii functiei membru in punctul x.

Algebraic.m -> in cadrul functiei am inceput prin a citi din fisier numarul de noduri, nr de linkuri al fiecarui nod si linkurile acestuia catre alte noduri. In for -ului de citire construiesc totodata si matricea de adiecenta pentru graf. Calculez matricea M cu ajutorul matricei unitate (care are pe diagonala principala valori din matricea de adiacenta) si A. In final, calculez vectorul de Page Rank uri cu ajutorul factorizarilor Gram-Schmidt.

Iterative.m -> in cadrul functiei incep prin a citi datele din fisier si a construi matricea de adiacenta A. Transform datele de pe diagonala principala din matricea K in care se retin gradele exterioare ale nodurilor din graf. Apoi, determin matricea M si, in cele din urma calculez R pana cand norma dintre vechiul R si noul R trece de eroarea data ca parametru.

Pr_Inv -> in cadrul functiei culculez R si Q pentru matricea A primita ca parametru. Apoi, folosind formulele din laboratorul 2 am calculat inversa lui A si am salvat-o in B.

PageRank.m -> in cadrul functiei citesc rand cu rand datele din fisierul dat ca parametru. R1 va fi obtinut cu ajutorul functiei Iterative, iar R2, cu ajutorul functiei Algebraic. Amandoua se afiseaza. In PR1 se salveaza pe prima coloana numele de la 1 la N si pe a doua coloana, datele din R2. Apoi, matricea se ordoneaza descrescator dupa datele din R2. Cu ajutorul unui for de la 1 la N se afiseaza in nume.out se afiseaza pe rand, nr nodului, valoarea lui din R2 si se afiseaza rezultatul care rezulta in urma apelului functiei Apartenenta in nodul respectiv.