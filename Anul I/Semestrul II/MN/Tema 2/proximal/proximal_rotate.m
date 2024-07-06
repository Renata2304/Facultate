function R = proximal_rotate(I, rotation_angle)
    % =========================================================================
    % Roteste imaginea alb-negru I de dimensiune m x n cu unghiul rotation_angle,
    % aplicând Interpolare Proximala.
    % rotation_angle este exprimat în radiani.
    % =========================================================================
    [m n nr_colors] = size(I);
    % Se converteste imaginea de intrare la alb-negru, daca este cazul.
    if nr_colors > 1
        R = -1;
        return
    endif
    % Obs:
    % Atunci când se aplica o scalare, punctul (0, 0) al imaginii nu se va deplasa.
    % În Octave, pixelii imaginilor sunt indexati de la 1 la n.
    % Daca se lucreaza în indici de la 1 la n si se inmultesc x si y cu s_x respectiv s_y,
    % atunci originea imaginii se va deplasa de la (1, 1) la (sx, sy)!
    % De aceea, trebuie lucrat cu indici în intervalul [0, n - 1].

    % TODO: Calculeaza cosinus si sinus de rotation_angle.
    s = sin(rotation_angle);
    c = cos(rotation_angle);
    % TODO: Initializeaza matricea finala.
    R = ones(m, n);
    
    % TODO: Calculeaza matricea de transformare.
    T = [c, -s; s, c]; 
    
    % TODO: Inverseaza matricea de transformare, FOLOSIND doar functii predefinite!
    T = T'; % T este ortogonala, deci T transpusa e si inversa lui T

    % Se parcurge fiecare pixel din imagine.
    for y = 0 : m - 1
        for x = 0 : n - 1
            % TODO: Aplica transformarea inversa asupra (x, y) si calculeaza x_p si y_p
            % din spatiul imaginii initiale.
            v0 = [x; y];  v1 = T * v0;
            xp = v1(1);   yp = v1(2);
            % TODO: Trece (xp, yp) din sistemul de coordonate [0, n - 1] în
            % sistemul de coordonate [1, n] pentru a aplica interpolarea.
            xp++;   yp++;
            % TODO: Daca xp sau yp se afla în exteriorul imaginii,
            % se pune un pixel negru si se trece mai departe.
            if (yp < 1 || xp < 1 || yp > m || xp > n)
              R(y + 1, x + 1) = 0;
              continue;
            endif
            % TODO: Afla punctele ce înconjoara(xp, yp).
            x1 = floor(xp);   y1 = floor(yp); 
            x2 = x1 + 1;      y2 = y1 + 1;
            
            if (y2 > m)
              y2 = m;
              y1 = m - 1;
            endif
            
            if (x2 > n)
              x2 = n;
              x1 = n - 1;
            endif
            % TODO: Calculeaza coeficientii de interpolare notati cu a
            % Obs: Se poate folosi o functie auxiliara în care sau se calculeze coeficientii,
            % conform formulei.
            a = proximal_coef(I, x1, y1, x2, y2);
            % TODO: Calculeaza valoarea interpolata a pixelului (x, y).
            R(y + 1, x + 1) = a(1) + a(2) * xp + a(3) * yp + a(4) * yp * xp;
        endfor
    endfor
    % TODO: Transforma matricea rezultata în uint8 pentru a fi o imagine valida.
    R = uint8(R);
endfunction
