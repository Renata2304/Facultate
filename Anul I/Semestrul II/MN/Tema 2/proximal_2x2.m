function out = proximal_2x2(f, STEP = 0.1)
    % ===================================================================================
    % Aplica Interpolare Proximala pe imaginea 2 x 2 f cu puncte intermediare echidistante.
    % f are valori cunoscute în punctele (1, 1), (1, 2), (2, 1) ?i (2, 2).
    % Parametrii:
    % - f = imaginea ce se va interpola;
    % - STEP = distan?a dintre dou? puncte succesive.
    % ===================================================================================
    
    % TODO: Defineste coordonatele x si y ale punctelor intermediare.
    x_int = [1 : STEP :2];
    y_int = [1 : STEP :2];
    % Se afl? num?rul de puncte.
%prin urmatoarele 2 randuri de mai jos determin dimensiunile celor 4 sferturi ale matricei rezultat
    len = 1/STEP; % trec din subunitar in supraunitar
    len = len/2;
    
    n = length(x_int);
    % TODO: Cele 4 puncte încadratoare vor fi aceleasi pentru toate punctele din interior.
    a = f(1);    b = f(2);   c = f(3);    d = f(4);
    % TODO: Initializeaza rezultatul cu o matrice nula n x n.
    out = zeros(n, n);
    % Se parcurge fiecare pixel din imaginea finala.
    % fomez matricea finala, fixand pixel cu pixel in funtie de parte din matrice 
    % in care ma aflu
    for i = 1 : n
        for j = 1 : n
          if i <= len && j <= len
            out(i,j) = a;
          elseif i <= len && j > len
            out(i,j) = c;
          elseif i > len && j <= len
            out(i,j) = b;
          else 
            out(i,j) = d;
          endif
        endfor
    endfor

endfunction