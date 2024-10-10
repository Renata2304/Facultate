#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int nr, N, a, b, max_n, min_n, max_ab, min_ab;
    FILE *out = fopen("output", "w");
    
    max_n = 20000; min_n = 10000;
    nr = rand() % max_n + min_n;

    fprintf(out, "%d\n", nr);

    max_ab = 100000; min_ab = 1;

    for (int i = 0; i < nr; i++) {
        N = rand() % 5;
        a = rand() % max_ab + min_ab;
        b = rand() % max_ab + min_ab;
        fprintf(out, "%d%c", N, 9);
        switch (N) {
            case 0:
                fprintf(out, "%d%c", a, 9);
                break;
            case 1:
                fprintf(out, "%d%c", a, 9);
                break;
            case 2:
                fprintf(out, "%d%c", a, 9);
                break;
            case 3:
                fprintf(out, "%d%c%d%c", a, 9, b, 9);
                break;
            case 4:
                break;
            default:
                break;
        }
        fprintf(out, "\n");
    }
    fclose(out);
}