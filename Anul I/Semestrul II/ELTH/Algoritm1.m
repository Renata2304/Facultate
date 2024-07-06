function [Pc Pg] = Algoritm1(M)
  # m-am folosit de urmatoare matrice cu datele din circuitul meu
  # M = [5 2 3 9 6 4 10 10; -8 8 18 -10 18 8 -28 20; 8 0 0 10 0 0 0 0; 
  #       0 0 0 0 0 0 10 0; 0 4 6 0 3 2 0 2 ]
  [m n] = size(M);
  Pc = 0;
  Pg = 0;
  for j = 1:n
    Pc = Pc + M(5,j) * M(1,j) * M(1,j);
    Pg = Pg + M(3, j) * M (1, j) - M(4, j) * M(2, j);
  endfor
endfunction