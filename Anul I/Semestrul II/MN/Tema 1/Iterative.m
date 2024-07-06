function R = Iterative(nume, d, eps)
% Functia care calculeaza matricea R folosind algoritmul iterativ.
% Intrari:
% -> nume: numele fisierului din care se citeste;
% -> d: coeficentul d, adica probabilitatea ca un anumit navigator sa continue navigarea (0.85 in cele mai multe cazuri)
% -> eps: eruarea care apare in algoritm.
% Iesiri:
% -> R: vectorul de PageRank-uri acordat pentru fiecare pagina.
  R = output_precision(7); 
  new_R = output_precision(7); 
  % citesc datele din fisierul dat ca parametru
  data = dlmread(nume," \n"); 
  N = data(1,1); 
  A = zeros(N); 
  K = zeros(N); 
  L = zeros(N, 1); 
  
  % generez matricea de adiacenta A
  for i = 1:N 
    for j = 1: data(i + 1, 2) 
      x = data(i + 1, j + 2); 
      if i ~= x 
        K(i, i) = K(i, i) + 1;
        L(i) = L(i) + 1; 
        A(i, x) = 1;
      endif
    endfor
  endfor

  new_R = ones(N, 1) * (1 / N); 
  cst = (1 - d) / N; 
  
  % transform datele de pe diagonala lui K
  for i=1:N
    K(i, i) = 1/K(i, i); 
  endfor 
  
  % calculez matricea M
  M = (K * A)'; 
  M = d * M; 
  
  % caluclez R pana cand aceasta scade sub eroarea data
  while norm(new_R - R, 2) > eps
    R = new_R; 
    new_R = M * R + cst * ones(N, 1); 
  endwhile 
  
endfunction