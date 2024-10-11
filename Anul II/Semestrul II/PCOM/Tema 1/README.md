# Tema 1 PCOM - 2022-2023

Link: https://pcom.pages.upb.ro/tema1/

- echo_reply -> Această funcție primește un pachet de tip "echo request" și trimite înapoi un 
pachet de tip "echo reply". În funcție se inversează adresele MAC ale sursei și destinației,
se completează header-urile IP și ICMP și se trimite pachetul înapoi prin intermediul funcției
send_to_link.

- icmp_err -> Această funcție se ocupă de generarea de pachete ICMP de eroare, care sunt 
trimise înapoi la sursa unui pachet IP care a fost pierdut sau respins. Funcția completează 
header-urile IP și ICMP și apoi construiește un nou buffer care conține header-ul ICMP, 
header-ul IP al pachetului pierdut și primii 64 de biți din payload-ul original. Acest nou 
pachet este apoi trimis înapoi prin intermediul funcției send_to_link.

- cmp -> Această funcție sortează intrările din tabela de rutare în funcție de importanță, adică
în funcție de masca și prefixul rețelei. Ea primește doi pointeri de tip void *, care sunt 
transformați în pointeri const struct route_table_entry *. Funcția cmp este definită astfel încât
să ofere o sortare descrescătoare a intrărilor din tabela de rutare. Mai întâi se face o 
verificare a adresei IP în funcție de masca de rețea, iar apoi, dacă adresele IP sunt diferite, 
se face o verificare în funcție de prefixul de rețea. În cazul în care două intrări au aceeași 
adresă IP și aceeași masca de rețea, se consideră că adresa IP este mai mare dacă masca de rețea 
este mai mică, și astfel intrarea va fi plasată mai sus în lista sortată.

- get_best_route -> Această funcție primește o adresă IP de destinație și caută cea mai bună rută
în tabela de rutare. Tabela de rutare este sortată, iar căutarea se face prin metoda căutării 
binare, ceea ce face căutarea mai eficientă. În timpul căutării, se reține cea mai lungă potrivire
de masca adreselor IP găsite. Dacă nu este găsită nicio potrivire, se returnează NULL. Dacă se 
găsește o potrivire, se returnează adresa din tabela de rutare asociată acelei potriviri.

- get_arp_entry -> Această funcție caută o intrare ARP corespunzătoare unei adrese IP specifice 
în tabela ARP (adresa MAC asociată acelei adrese IP). Funcția parcurge toate intrările din tabela 
ARP și verifică dacă adresa IP căutată se potrivește cu adresa IP dintr-o anumită intrare. Dacă 
găsește o astfel de intrare, se returnează un pointer către intrarea respectivă din tabelul ARP. 
Dacă nu se găsește nicio intrare corespunzătoare, se returnează NULL.

- reply_arp -> Această funcție primește un pachet ARP și o coadă de pachete care așteaptă să fie 
trimise. Funcția adaugă adresa MAC a expeditorului pachetului ARP în tabela MAC, apoi procesează
toate pachetele din coada de așteptare. Pentru fiecare pachet, funcția determină cea mai bună 
rută către adresa de destinație și actualizează adresa MAC sursă și destinație în antetul Ethernet
în consecință. Pachetul este apoi trimis pe interfața corespunzătoare.

- no_next_hop -> Această funcție creează un pachet ARP de tip cerere și îl trimite prin interfața
indicată de cel mai bun router. Se aloca memorie pentru o structura de tip ether_header si una 
de tip arp_header, iar apoi se completeaza campurile acestora cu informatii necesare pentru a 
trimite pachetul. Pachetul este apoi copiat in buffer-ul "buf" si trimis prin intermediul 
interfetei "best_router->interface".

- main -> În prima parte se face inițializarea tabelelor de rutare și de adresă MAC. Apoi, se 
primesc pachete de pe orice interfață și se verifică dacă pachetul curent este un pachet IP sau ARP. 
    
    În cazul în care este un pachet ARP, se verifică tipul de pachet primit. Dacă este un pachet 
ARP de tip request, se construiește un pachet ARP de tip reply cu adresele MAC și IP inversate, iar 
acesta este trimis pe interfața corespunzătoare. Dacă este un pachet ARP de tip reply, se răspunde 
cu adăugarea adresei MAC corespunzătoare adresei IP în tabela ARP și se trimite orice pachet din
coada corespunzătoare adresei IP respectiva.
    
    În cazul în care este un pachet IP, se verifică suma de control și se caută cea mai bună rută 
pentru destinație, în funcție de tabela de rutare ordonată. Dacă nu există o rută validă, se trimite
un mesaj ICMP cu eroarea "Destination unreachable". În caz contrar, se verifică TTL-ul pachetului 
și, dacă este mai mic sau egal cu 1, se trimite un mesaj ICMP cu eroarea "Time exceeded". Dacă 
pachetul trebuie transmis mai departe, se decrementează TTL-ul și se recalculează suma de control,
apoi se caută adresa MAC a următorului hop în tabela ARP. 
    
    Dacă această adresă nu este disponibilă, se adaugă vechiul buffer in coada predefinită și apoi
se apelează functia no_next_hop, explicată mai sus.
    În caz contrar, se actualizează adresa sursă și adresa destinație în antetul Ethernet,
apoi pachetul este trimis pe interfața corespunzătoare.