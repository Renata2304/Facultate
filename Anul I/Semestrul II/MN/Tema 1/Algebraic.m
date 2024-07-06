function R = Algebraic(nume, d)
	% Functia care calculeaza vectorul PageRank folosind varianta algebrica de calcul.
	% Intrari: 
	%	-> nume: numele fisierului in care se scrie;
	%	-> d: probabilitatea ca un anumit utilizator sa continue navigarea la o pagina urmatoare.
	% Iesiri:
	%	-> R: vectorul de PageRank-uri acordat pentru fiecare pagina.
	fid = fopen(nume,'r');
	N=fscanf(fid,'%d',1);
	A=zeros(N);
	for i=1:N
		%citesc nr nodului, nr de linkuri si linkurile
    a=fscanf(fid,'%d',1);
		b=fscanf(fid,'%d',1);
		c=fscanf(fid,'%d',b);
		for x=1:b
			A(a,c(x))=1;
			if(a==c(x))
				A(a,c(x))=0;
				%daca are link catre el insusi se decrementeaza nr de linkuri
        b--;
			endif
		endfor
		%construiesc matricea de adiacenta
    k(i)=b;
  endfor

	O = ones(N,1);
	I = eye(N);
  
	for i = 1:N	
	  I(i,i) = 1 / k(i);
	endfor
  M = (I * A)';
  
  % calculez vectorul de Page Rank uri cu ajutorul factorizarilor Gram-Schmidt
	R = PR_Inv(eye(N) - d * M ) * (((1-d)/N) * O);	

endfunction