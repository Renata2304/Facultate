#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// functie pentru transformarea unui numar din binar in zecimal
int bitidec (unsigned int n)
{ int n1=0, p2=1;
  while(n)
  { n1 = n1 + p2*(n%10);
    n = n/10; p2 = p2 *2;
  }
  return n1;
}

int main()
{   unsigned int inst, N=0, dim=0, nr=0, nri, rez=0;
    char op[9]={0};
    unsigned short y;

    scanf("%u", &inst);

    // calculare N in decimal
    for(int i=31; i>=29; i--)
       N = N*10 + ((inst & (1<<i)) >> i); // gasirea N pe biti
    N = bitidec(N) + 1;
    //printf("%u ", N); // afisare

    // determinare + afisare op
    for(int i = 28; i >= 28 - 2*N; i--)
    { int x = x*10 + ((inst & (1<<i)) >> i);
      nr++; // formarea unui numar x, din cate doi biti consecutivi      if(nr%2==0)
      { // completarea unui vector de caractere, in functie de cate doi biti alaturati
        if(x==0) op[nr/2] = '+';
        else if(x==1) op[nr/2] = '-';
        else if(x==10) op[nr/2] = '*';
        else op[nr/2] = '/';
        x = 0; // trecerea la un nou numar x
        //printf("%c ", op[nr/2]); // afisare
      }
    }

    // calculare dim
    for(int i=29 - nr; i>=26 - nr; i--)
        dim = dim*10 + ((inst & (1<<i)) >> i);
    dim = bitidec(dim) + 1;
    //printf("%u\n", dim); // afisare

    // determinarea numerelor de introdus
    nri = ceil(((N+1)*dim) / 16.0);
    //printf("%u\n", nri);
    //printf("Introduceti numar: ");

    nr = 0; int o = 0; int x = 0, Nr = 0,  j = 0;
    // citire celor ((N+1)*dim)/16 - 1 numere
    while(nri) // determinarea celor N+1 operanti
    { if(Nr == 0 && j == 0) // am inceput sa construim un nou termen
      { for(j=15; j>=15-dim+1; j--)
       { scanf("%hu", &y); nri--;
         x += ((y & (1<<j)) >> j) << (dim - Nr - 1);
         Nr++;
         if(Nr==dim) // daca x are dim biti
         { x = bitidec(x); // transformarea lui x din binar in zecimal
           if(nr == 0) rez = x; // daca s-a calculat primul operant
           else // daca nu suntem la primul operant
           {  if(op[nr] == '+') rez = rez + x;
              else if(op[nr] == '-') rez = rez - x;
              else if(op[nr] == '*') rez = rez * x;
              else if(op[nr] == '/') { if(x == 0) rez = 0;
                                       else rez = rez / x;
                                      }

           }
           nr++;
           Nr = 0; x = 0; // trecerea la un nou x
         }
        }
      }
       else if(Nr!=0 && j==0) // inca construim un termen si trecem la urmatorul numar
       { scanf("%hu", &y); nri--;
         for(j=15; j>=0; j--)
         x += ((y & (1<<j)) >> j) << (dim - Nr - 1);
         Nr++;
         if(Nr==dim) // daca x are dim biti
         { x = bitidec(x); // transformarea lui x din binar in zecimal
           if(nr == 0) rez = x; // daca s-a calculat primul operant
           else // daca nu suntem la primul operant
           {  if(op[nr] == '+') rez = rez + x;
              else if(op[nr] == '-') rez = rez - x;
              else if(op[nr] == '*') rez = rez * x;
              else if(op[nr] == '/') { if(x == 0) rez = 0;
                                       else rez = rez / x;
                                      }

           }
           nr++;
           Nr = 0; x = 0; // trecerea la un nou x
         }
        }
        else // construim un nou termen, dar inca mai exista biti in numarul citit curent
       { for(j=15-Nr+1; j>=0; j--)
         x += ((y & (1<<j)) >> j) << (dim - Nr - 1);
         Nr++;
         if(Nr==dim) // daca x are dim biti
         { x = bitidec(x); // transformarea lui x din binar in zecimal
           if(nr == 0) rez = x; // daca s-a calculat primul operant
           else // daca nu suntem la primul operant
           {  if(op[nr] == '+') rez = rez + x;
              else if(op[nr] == '-') rez = rez - x;
              else if(op[nr] == '*') rez = rez * x;
              else if(op[nr] == '/') { if(x == 0) rez = 0;
                                       else rez = rez / x;
                                      }

           }
           nr++;
           Nr = 0; x = 0; // trecerea la un nou x
         }
        }
      }

    printf("%u\n", rez);
    return 0;
}
