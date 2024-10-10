# Multimi 1

Problema 2 - Multimi 1

Structura arhiva:
- folderul "testare" -> fișierul in care se afla testele care vor fi rulate.
- 2.pdf -> pdf-ul proiectului
- "TestTime.xslx -> documentul in Excel in care s-au adăugat toate datele colectate de la executarea celor doi algoritmi
- folder "out" -> fișierele de output ale algoritmilor
- folder "in" -> cele 100 de fișiere de input ale algoritmilor
- Hash.c, AVL.c -> fișierele .c care implementează cele doua structuri de date
- Makefile
- timeAVl, timeHash -> fișierele text in care s-a salvat timpul de execuție al structurilor de date
- test.in -> testul care va fi rulat de către cele doua programe
- folder "rand_generator" -> folderul in care se afla fișierul de generare a testelor 

în cadrul creării testelor se va genera un număr random (N) si cu ajutorul unui for de la 0 la N-1 se vor genera N numere (a,b), specific numărului de acțiuni care vor trebui aplicate asupra celor două structuri. 

Surse cod:
- AVL -> https://ocw.cs.pub.ro/courses/sda-ab/laboratoare/07 Algoritmul s-a implementat în cadrul laboratorului
- Tabele de dispersie ->  https://www.digitalocean.com/community/tutorials/hash-table-in-c-plus-plus priority queue.