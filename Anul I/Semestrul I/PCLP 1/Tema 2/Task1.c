#include "utils.h"

// functie care transforma un sir de caractere format din cifre intr-un numar
int ch_to_int(char *s) {
    int x = 0;
    for (int i=1; i < strlen(s); i++)
        x = x*10 + (s[i]-48);
    return x;
}
// fucnctie pentru cazul in care un cuvant incepe cu litera 'a' si care
// determina pozitia in sirul de caractere pe care se afla cifra maxima
int poz_cif_max(char s[]) {
    int pozmax = 0, valmax = 0;
    for (int i=1; s[i]; i++)
        if ((s[i]-48) > valmax) {  // daca s-a gait o noua cifra maxima
            valmax = s[i] - 48;
            pozmax = i;
        }
    return pozmax;
}
// functie care verifica daca un numar e palindrom
int palindrom(int x) {
    int x_oglindit = 0, x_copie = x;
    while (x) {  // formarea oglinditului
        x_oglindit = x_oglindit*10 + x%10;
        x = x/10;
    }
    if (x_oglindit == x_copie) return 1;  // numarul e palindrom
    return 0;
}
// functie care verifica daca un numar e prim
int prim(int x) {
    if (x == 0 || x == 1) return 0;
    for (int d=2; d*d <= x; d++)
        if (x%d == 0) return 0;
    return 1;  // numarul e prim
}
int suma(char s[], int k, int n) {
    int sum = 0, nr = k, i = 0;
    while (nr) {
        sum = sum + (s[i]-48);
        i = (i+k)%n; nr--;
    }
    return sum;
}

void SolveTask1() {
    // declarari variabile
    int n, m, mat[1000][1000]={0}, pas = 2, i = 0, j = 0;
    char sir_cuvinte[5000]={0};
    mat[0][0] = 1;  // initializare colt stanga sus

    // citire
    scanf("%d %d\n", &n, &m);
    fgets(sir_cuvinte, sizeof(sir_cuvinte), stdin);
    // impartirea sirului in cuvinte
    char *cuvant_curent = strtok(sir_cuvinte, " \n");

    // rezolvare task1, luand fiecare cuvant in parte
    while (cuvant_curent) {
       // cazul in care cuvantul incepe cu 'a'
        if (cuvant_curent[0] == 'a') {
            int pozmax =  poz_cif_max(cuvant_curent);
            if (pozmax == 1) {
                mat[i][++j] = pas;  // pas la dreapta
            } else {
                if (pozmax == 2) {
                    mat[--i][j] = pas;  // pas in stanga
                } else {
                    if (pozmax == 3) {
                        mat[i][--j] = pas;  // pas in sus
                    } else {
                        mat[++i][j] = pas;  // pas in jos
                      }
                  }
              }
        } else {  // cazul in care cuvantul incepe cu 'b'
            if (cuvant_curent[0] == 'b') {
                int k = ch_to_int(cuvant_curent); int x = k%100;
                // salvare output-uri
                int rez_palindrom = palindrom(k), rez_prim = prim(x);
                if (rez_palindrom == 1) {  // daca k e palindrom
                    if (rez_prim == 1) {
                         mat[i][--j] = pas;  // pas in stanga
                    } else {
                        if (rez_prim == 0) {
                            mat[i][++j] = pas;  // pas in dreapta
                        }
                      }
                } else {
                    if (rez_prim == 1) {
                        mat[--i][j] = pas;  // pas in sus
                    } else {
                        if (rez_prim == 0) mat[++i][j] = pas;  // pas in jos
                      }
                  }
            } else {  // cazul in care cuvantul incepe cu 'c'
                if (cuvant_curent[0] == 'c') {
                    int nr = cuvant_curent[1]-48, k = cuvant_curent[2]-48;
                    // stergerea primelor doua cifre
                    char sir2[101];
                    strncpy(sir2, cuvant_curent+3, strlen(cuvant_curent));
                    int S = suma(sir2, k, nr);
                    if (S%4 == 0) {
                        mat[i][--j] = pas;  // pas la stanga
                    } else {
                        if (S%4 == 1) {
                            mat[--i][j] = pas;  // pas in sus
                        } else {
                            if (S%4 == 2) {
                                mat[i][++j] = pas;  // pas in dreapta
                            } else {
                                mat[++i][j] = pas;  // pas in jos
                            }
                          }
                      }
                }
              }
          }
            pas++;
            cuvant_curent = strtok(NULL, " \n");
    }
    // afisare
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) printf("%d ", mat[i][j]);
        printf("\n");
    }
}
