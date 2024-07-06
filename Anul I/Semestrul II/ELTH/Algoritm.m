function [Pc Pg] = Algoritm(M)
  [m n] = size(M);
  Pc = 0;
  Pg = 0;
  
  for j = 1:n
    Pc = Pc + M(5,j) * M(1,j) * M(1,j);
    Pg = Pg + M(3, j) * M (1, j) - M(4, j) * M(2, j);
  endfor
  
endfunction

