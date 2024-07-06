#include "utils.h" 
/* Pentru a rezolva cerintele ma folosesc de codul ascii al caracterelor.
Daca se trece de pragul inferior (48,65 sau 97) se adauga lungimea intervalului
(26 pt litere sau 10 pt cifre) si se scade cheia. In caz contrat se scade cheia
direct.
*/
// functie de decodificare cifre
void decod(char s[], int cheie) {
    for (int i=0; s[i]; i++) {
        char c = s[i];
        int x = cheie%10;
        if (c-x < 48) {
            c = c + 10 - x;
        } else {
            c = c - x;
          }
        s[i] = c;
    }
}
// functie care inverseaza elementele intre ele, invartind vectorul
void invers(char s[]) {
    for (int i = 0; i < strlen(s)/2; i++) {
        char aux = s[i];
        s[i] = s[strlen(s)-i-1];
        s[strlen(s)-i-1] = aux;
    }
}
void SolveTask2() {
    // variabile
    char cod[10] = {0}, text[101] = {0};
    char n1[1001] = {0}, n2[1001] = {0}, n3[1001] = {0};
    int cheie = 0, rest = 0, i;
    // citire
    scanf("%s\n", cod);
    // decodificarea pe cazuri
    if (cod[0] == 'c') {  // cazul 2.1 - caesar
        // citire cheie+text criptat
        scanf("%d\n%s", &cheie, text);
        for (i = 0; i < strlen(text); i++) {
            // daca text[i] este litera mica
            if ((text[i] >= 'a' && text[i] <= 'z')) {
                char c = text[i];
                int x = cheie%26;
                if (c-x < 97) {
                    c = c + 26 - x;
                } else {
                    c = c - x;
                  }
                text[i] = c;
            } else {  // daca text[i] este litera mare
                if (text[i] >= 'A' && text[i] <= 'Z') {
                    char c = text[i];
                    int x = cheie%26;
                    if (c-x < 65) {
                        c = c + 26 - x;
                    } else {
                        c = c - x;
                      }
                    text[i] = c;
                } else {  // daca text[i] este numar
                    int c = text[i];
                    int x = cheie%10;
                    if (c-x < 48) {
                        c = c + 10 - x;
                    } else {
                        c = c - x;
                      }
                    text[i] = c;
                }
            }
        }
      // afisare
      printf("%s\n", text);
    } else {
        if (cod[0] == 'v') {  // cazul 2.2 - vigenere
            char cheie_majuscule[1001]={0};
            // citire cheie+text criptat
            scanf("%s\n%s", cheie_majuscule, text);
            // salvarea dimensiunii cheii, pentru a folosi pozitiile majuculelor
            int n = strlen(cheie_majuscule);
            for (i = 0; i < strlen(text); i++) {
            // daca text[i] este litera mica
                if ((text[i] >= 'a' && text[i] <= 'z')) {
                    char c = text[i];
                    int x = (cheie_majuscule[i%n] - 'A') % 26;
                    if (c-x < 97) {
                        c = c + 26 - x;
                    } else {
                        c = c - x;
                      }
                    text[i] = c;
                } else {  // daca text[i] este litera mare
                    if (text[i] >= 'A' && text[i] <= 'Z') {
                    char c = text[i];
                    int x = (cheie_majuscule[i%n] - 'A') % 26;
                    if (c-x < 65) {
                        c = c + 26 - x;
                    } else {
                        c = c - x;
                      }
                    text[i] = c;
                    } else {  // daca text[i] este numar
                        int c = text[i];
                        int x = (cheie_majuscule[i%n] - 'A') % 10;
                        if (c-x < 48) {
                            c = c + 10 - x;
                        } else {
                            c = c - x;
                        }
                        text[i] = c;
                    }
                }
            }
            // afisare
            printf("%s\n", text);
        } else {  // cazul 2.3 - addition (folosing cazul 2.1)
            // citire cheie + cele doua numere
            scanf("%d\n%s\n%s", &cheie, n1, n2);
            decod(n1, cheie);  // decriptare n1
            decod(n2, cheie);  // decriptare n2
            // inversare siruri
            invers(n1); invers(n2);
            // crearea celui de-al treilea sir, cu suma dintre n1 si n2
            for (i=0; i < strlen(n1) && i < strlen(n2); i++) {
                // calcularea sumei cifra cu cifra
                int x = n1[i]-48, y = n2[i]-48, s;
                s = x+y+rest;
                n3[i] = (s%10)+48;
                rest = s/10;
            }
            // daca sirurile sunt de lungimi diferite se calculeaza separat
            if (i < strlen(n1)) {
                while (i < strlen(n1)) {
                    rest = rest+(n1[i]-48);
                    n3[i] = (rest%10)+48;
                    i++; rest = rest/10;  // trecerea la un nou rest
                }
            }
            if (i < strlen(n2)) {
                while (i < strlen(n2)) {
                    rest = rest+(n2[i]-48);
                    n3[i] = (rest%10)+48;
                    i++; rest = rest/10;
                }
            }
            if (rest) n3[i] = rest+48;  // daca a mai ramas un rest
            invers(n3);
            while (n3[0] == '0')
                strncpy(n3, n3+1, sizeof(n3)-2);
            printf("%s\n", n3);  // afisare
            }
      }
}
