function B = PR_Inv(A)
	% Functia care calculeaza inversa matricii A folosind factorizari Gram-Schmidt
	% Se va inlocui aceasta linie cu descrierea algoritmului de inversare
  [n] = size(A, 1); 
  Q = zeros(n); 
  R = zeros (n); 
  % calculez R si Q pentru o matrice A folosind factorizari Gram-Schmidt
  for i = 1 :n
  R(i, i) = norm(A(:, i), 2); 
  Q(:, i) = A(:, i) / R(i, i); 
    for j = i + 1 :n
      R(i, j) = Q(:, i)' * A(:, j);
      A(:, j) = A(:, j) - Q(:, i) * R(i, j); 
    endfor 
  endfor 
  
  Q = Q'; % tranpun matricea Q
  
  % folosind formulele din lab 2 calculez inversa lui A si o salvez in B
  for k = 1 : n
    b = Q(:, k); 
    for i = n : -1 : 1
      sum = 0; 
      for j = i + 1:n
        sum = sum + R(i, j) * x(j); 
      endfor
      x(i) = (b(i) - sum) / R(i, i); 
    endfor
    B(:, k) = x; 
  endfor 
  
endfunction