#include <stdio.h>
#include <stdlib.h>

// functie pentru transformarea unui numar din binar in zecimal
int bitidec (int n)
{ int n1=0, p2=1;
  while(n)
  { n1 = n1 + p2*(n%10);
    n = n/10; p2 = p2 *2;
  }
  return n1;
}
int main()
{   unsigned int inst, x=0, N=0, dim=0, mask, r, p2=1, nr=0;
    char op[9]={0};

    scanf("%u", &inst);

    // calculare N in decimal
    for(int i=31; i>=29; i--)
       N = N*10 + ((inst & (1<<i)) >> i); // gasirea N pe biti
    N = bitidec(N) + 1;
    printf("%u ", N); // afisare

    // determinare + afisare op
    for(int i = 28; i >= 28 - 2*N; i--)
    { x = x*10 + ((inst & (1<<i)) >> i);
      nr++; // formarea unui numar x, din cate doi biti consecutivi
      if(nr%2==0)
      { // completarea unui vector de caractere, in functie de cate doi biti alaturati
        if(x==0) op[nr/2] = '+';
        else if(x==1) op[nr/2] = '-';
        else if(x==10) op[nr/2] = '*';
        else op[nr/2] = '/';
        x = 0; // trecerea la un nou numar x
        //printf("%c ", op[nr/2]); // afisare
      }
    }
    for(int i=1; i<=nr/2; i++) printf("%c ", op[i]);

    // calculare dim
    for(int i=29 - nr; i>=26 - nr; i--)
        dim = dim*10 + ((inst & (1<<i)) >> i);
    dim = bitidec(dim) + 1;
    printf("%u\n", dim); // afisare
    return 0;
}
