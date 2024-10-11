# Tema 4 PCOM - 2022-2023


Am scris mult mai multe comentarii ajutatoare in cod. Fisierele request.c si request.h sunt
in mare parte rezolvate in cadrul laboratorului 9. Am mai adaugat in antetul celor doua functii
deja existente un char* token pentru carintele care necesita acest lucru (atunci cand se adauga
si headerul Authorization). In celelalte cazuri in care se apeleaza functiile se poate pune NULL 
si astfel nu se va interfera cu rezolvare cerintei. De asemenea, pentru cazul de delete_request
am mai adaugat o functie foarte asemanatoare cu compute_get_request, modificand doar randurile
necesare din GET in DELETE.

Am folosit parserul in C de pe linkul https://github.com/kgabis/parson

- cazul register si cazul login: cele doua sunt asemanatoare intrucat se pirmesc datele de intrare,
se verifica sa nu existe spatii in username sau in parola, se face obiectul json, se face requestul.
In cazul register, se testeaza daca username-ul este luat deja, atunci se intoarce o eroare.
In cazul login, se testeazadaca exista erori, iar in caz contrar, se retin cokkie-urile.

- cazul logout: se trimite requestul, se delogheaza userul (logged_in = 0) si se elibereaza 
cookie-urile. Se afiseaza dupa caz Success sau No use is logged in!, in cazul in care logged_in = 0;

- cazul enter_library: se testeaza daca userul este logat (se afiseaza eroare, in caz contrar), se face 
request, apoi se salveaza raspunsul si se imparte dupa "{" si ", care mai apoi se salveaza in token. 
Se seteaza library = 1, pentru urmatoarele taskuri.

- cazul get_books: se testeaza daca userul are acces la biblioteca (se afiseaza eroare, in caz 
contrar), se face request, apoi se salveaza raspunsul si se imparte dupa "[{" pentru a salva cartile.
In final se afiseaza toate cartile.

- cazul get_book: se testeaza daca userul are acces la biblioteca (se afiseaza eroare, in caz contrar),
se salveaza headerului Authorization, se citeste si verifica id-ul cartii. In final, se cauta cartea
in baza de date a clientului si se afiseaza cartea cu acel token, daca exista. 

- cazul add_book: se testeaza daca userul are acces la biblioteca (se afiseaza eroare, in caz contrar),
se salveaza headerului Authorization, se citesc datele de intrare si se verifica. Daca sunt corecte,
se creeaza obiectul json, se trimite cererea si in cazul in care nu au fost erori, se adauga in baza
de date cartea. In caz contrar, se vor afisa erorile aferente. 

- cazul delete_book: asemanator ca in cazul add_book, se fac cam aceeasi pasi, iar in final se cauta ruta 
catre carte si daca totul a fost corect, se trimite request de tip delete, iar daca nu exista erori in 
rapuns, se sterge cartea. 

- cazul exit: se iese din while.