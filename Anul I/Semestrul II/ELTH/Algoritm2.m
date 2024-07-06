function[X] = Algoritm2( M, b, err )
  # Aflu coeficientii din X intr-o relatie de tipul 
  # A*X = b
  X = inv(M) * b;
endfunction