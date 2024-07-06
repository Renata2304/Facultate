Tema 2 - PCLP 1 --> Anul 1 / Semestrul 1 https://ocw.cs.pub.ro/courses/programare/teme_2021/tema2_2021_cbd

### Task1:

La primul task m-am folosit 5 functii pentru a rezolva cerintele: ch_to_int, (care transforma un sir de caractere format din cifre, intr-un numar), poz_cif_max (care determina pozitia in sirul de caractere pe care se afla cifra maxima - folosita la cazul in care cuvintele incep cu litera a), palindrom si prim (amandoua folosite in cazul in care un cucant incepe cu b, prima verificand daca un numar de tip int este palindrom si a doua verificand daca un numar de timp int este prim - amandoua returneaza 1 in caz afisrmativ si 0 in caz contrar) si suma (functie folosita pentru cazul in care un cuvant incepe cu litera c si care determina suma cu cele k cifre, luate din k in k si returneaza in final suma).

Incep prin a initializa coltul din stanga sus al matricei si citesc tot sirul de cuvinte. Cu ajutorul functiei strtok impart sirul in cuvinte si rezolv fiecare cerinta, testand prima litera a cuvantului, ajutandu-ma pe urma de functiile enumerate mai sus. Folosesc un contor - pas, care la fiecare mutare creste cu cate o unitate si modifica matricea.

In final, afisez matricea.

### Task2:

Pentru a rezolva cerintele ma folosesc de codul ascii al caracterelor. Daca se trece de pragul inferior (48,65 sau 97) se adauga lungimea intervalului (26 pt litere sau 10 pt cifre) si se scade cheia. In caz contrat se scade cheia direct.

La taskul 2 folosesc 2 functii: decod ( pentru a decripta cele doua siruri foarte lungi de la cerinta 2.3 addition; functia decodifica doar sirurile formate din cifre) si invers (care invarte elementele unui sir [ex: v={123} -> v={321}]). 

Incep prin a citi codul (ceasar, vigenere sau addition) si in functie de prima litera (c, v sau a) se rezolva cerintele. 
- Pentru cazul CAESAR -> se foloseste de fiecare data restul impartirii la 26 respectiv 10 (26 pt litere si 10 pt numere) si se foloseste regula de mai sus, utilizand codul ascii.
- Pentru cazul VIGENERE -> se procedeaza asemanator ca la cazul 2.1, dar se ia pe rand fiecare litera si se converteste in int (scazand 'a', 'A' sau '0').
- Pentru cazul ADDITION -> se decodeaza cele doua numere foarte mari si apoi se inverseaza pentru a putea face suma. In cazul in care un sir inca mai am cifre ramase, iar in final se afiseaza noul sir inversat.

### Task3:

In cadrul citirii, citesc cate un paragraf si apoi il adaug la finalul sirului sir_cuvinte si adaug cate un spatiu dupa. Cu ajutorul strtok ului impart sirul in cuvinte, sarind separatorii si le pun intr-un vector de cuvinte (cate doua cuvinte, despartite prin spatiu). Parcurg vectorul de cuvinte cu doi vectori, iar atunci cand gasesc doua perechi de cuvinte identice, sterg una din ele si modific vectorul nr in care retine frecventa fiecarei perechi. In final se afiseaza n (nr de perechi ramase) pe cate un rand o pereche si frecventa sa. 
