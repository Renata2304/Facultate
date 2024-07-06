#include "utils.h"

void SolveTask3() {
    char sep[10] = " .,!?;\n", cuv2[5001][1001] = {0};
    char sir_cuvinte[10001] = " ", s[50001];
    int i = 0, j = 0, nr[1001] = {0}, n = 1;

    // citire
    fgets(sir_cuvinte, sizeof(s), stdin);
    while (fgets(s, sizeof(s), stdin)) {
        strncat(sir_cuvinte, s, strlen(s));
        sir_cuvinte[strlen(sir_cuvinte)] = ' ';
    }
    // impartirea sirului in cuvinte
    char *cuvant_curent = strtok(sir_cuvinte, sep);
    // scrierea separata a primului cuvant
    strncpy(cuv2[0], cuvant_curent, strlen(cuvant_curent));
    strncat(cuv2[0], " ", 1);
    cuvant_curent = strtok(NULL, sep);
    // 
    while (cuvant_curent)
    {   strncat(cuv2[n-1], cuvant_curent, strlen(cuvant_curent));
        strncpy(cuv2[n], cuvant_curent, strlen(cuvant_curent));
        strncat(cuv2[n], " ", 1);
        n++;
        cuvant_curent = strtok(NULL, sep);
    }
    // stergerea perechilor care se repeta
    for (i=0; i < n-1; i++)
        for (j=i+1; j < n; j++)
        {   if (strcmp(cuv2[i], cuv2[j]) == 0)
            {   nr[i]++;
                for (int k=j; k < n; k++) {
                    strncpy(cuv2[k], cuv2[k+1], strlen(cuv2[k+1]));
                    cuv2[k][strlen(cuv2[k+1])] = '\0';
                }
                n--;
            }
        }
    // afisare
    printf("%d\n", n-1);
    for (i=0; i < n-1; i++)
        printf("%s %d\n", cuv2[i], nr[i]+1);
}
