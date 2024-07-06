function [R1 R2] = PageRank(nume, d, eps)
	% Calculeaza indicii PageRank pentru cele 3 cerinte
	% Scrie fisierul de iesire nume.out
  data_in = fopen(nume,"r");
	% continui sa citesc datele din fisier, rand cu rand
  N=fscanf(data_in,"%d\n", 1);
	A=zeros(N);
  
	for i=1:N
		M = fgetl(data_in);
  endfor
  
  val1 = fscanf(data_in,"%f\n", 1);
  val2 = fscanf(data_in,"%f\n", 1);
  fclose(data_in);
  
  data_out = fopen([nume ".out"], "w");
  
  % incep afisarea in nume.out
  fprintf(data_out, "%d\n\n", N);
  
  % afisez R1 pe care l-am aflat cu Iterative
  R1 = Iterative(nume, d, eps);
  for i = 1:N
    fprintf(data_out, "%f\n", R1(i));
  endfor
  fprintf(data_out, "\n");
  
  % afisez R2 pe care l-am aflat cu Algebraic
  R2 = Algebraic(nume, d);
  for i = 1:N
    fprintf(data_out, "%f\n", R2(i));
  endfor
  fprintf(data_out, "\n");
  
  % retin datele din R2 
  PR1 = [[1:N]' R2];
  PR1 = sortrows(PR1, -2); % sortez descrescator PR1

  % afisez data din R2, folosindu-ma si de functiei Apartenenta
  for i = 1 : N
    fprintf(data_out, "%d %d %f\n", i, int32(PR1(i, 1)), Apartenenta(PR1(i, 2), val1, val2));  
  endfor

  fclose(data_out);
endfunction