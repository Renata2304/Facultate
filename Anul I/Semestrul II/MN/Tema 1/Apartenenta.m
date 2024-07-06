function y = Apartenenta(x, val1, val2)
	% Functia care primeste ca parametrii x, val1, val2 si care calculeaza valoarea functiei membru in punctul x.
	% Stim ca 0 <= x <= 1
  % calculez y in functie de intervalul in care se afla x
  if x < val1
    y = 0;
  elseif  x <= val2
    a = 1 / (val2 - val1);
    b = - val1 * a;
    y = a * x + b;
  elseif x <= 1
    y = 1;
  endif
endfunction