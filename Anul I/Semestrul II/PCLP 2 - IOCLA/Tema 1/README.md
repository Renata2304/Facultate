# Tema1-PCLP2
Tema 1 - PCLP1 --> Anul 1/ Semestrul 2

add_last -> in cadrul functiei add_last, in cazul in care datele sunt corecte, aloc memorie pt arr. In cazul in care arr este gol ii aloc memorie cat pentru data, iar in cazut in arr nu este gol ii realoc memorie de (memoria din arr + dimensiunea datelor din data). Apoi adaug la finalul lui arr ceea ce se gasea in data si incrementez len.

add_at -> in cadrul functiei add_at, in cazul in care datele sunt corecte, daca data trebuie adaugat la finalul lui arr, atunci se va apela functia add_last. In caz contrar, realoc memorie pentru arr. In final, fac loc pe pozitia pe care trebuie introdus data, apoi mut datele din data in arr, pe pozitia stabilita si incrementez len.

find -> in cadrul functiei find, in cazul in care datele sunt corecte, parcurg arr pana la pozitia la care trebuie sa afisez. Retin si afisez tipul de structura care trebuie afisata, apoi in functie de tip, voi retine in mod diferit sumele dedicate. Apoi, afisez numele si sumele dedicate, in functie de tipul de structura.

print -> ma folosesc de find pentru a face o afisare mai usoara si printr-un for in care folosesc un int de la 1 la len apelez functia find pentru a afisa structura de la pozitia i

delete_at -> in cadrul functiei delete_at, in cazul in care datele sunt corecte, parcurg arr pana la pozitia de pe care trebuie stearsa structura data. Daca trebuie stearsa ultima structura atunci se va decrementa len si se va iesi din program. In caz contrar, se retine numarul de octeti care sunt inaintea structurii care trebuie sterse. In final, se copiaza cu (dimensiunea structurii care trebuie stearsa) nr de octeti la stanga restul structurilor, suprascriindu-se cea care trebuia stearsa. In final se decrementeaza len.

main -> in cadrul main-ului ma folosesc de un while cu ajutorul caruia tot citesc comenzi si folosesc strstr pentru a stii in care din cazuri ma aflu. In functie de caz, se vor apela functiile corespunzatoare. In cazul lui insert, se va construi data cu ajutorul datelor citite. 
