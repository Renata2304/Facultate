function A = bicubic_coef(f, Ix, Iy, Ixy, x1, y1, x2, y2)
    % =========================================================================
    % Calculeaz? coeficien?ii de Interpolare Bicubic? pentru 4 puncte al?turate
    % =========================================================================
    % formez cele 4 matrici 2x2 pe care le voi folosi pentru a forma A
    A1 = [f(y1, x1), f(y1, x2); f(y2, x1), f(y2, x2)];
    A2 = [Iy(y1, x1), Iy(y1, x2); Iy(y2, x1), Iy(y2, x2)];
    A3 = [Ix(y1, x1), Ix(y1, x2); Ix(y2, x1), Ix(y2, x2)];
    A4 = [Ixy(y1, x1), Ixy(y1, x2); Ixy(y2, x1), Ixy(y2, x2)];
    
    % am folosit double intrucat ajuta la compilarea testelor
    A1 = double(A1);  A2 = double(A2);  A3 = double(A3);  A4 = double(A4);
    
    % folosesc matricile date in cerinta, pe care le fac double
    a1 = [1 0 0 0; 0 0 1 0; -3 3 -2 -1; 2 -2 1 1];
    a2 = [1 0 -3 2; 0 0 3 -2; 0 1 -2 1; 0 0 -1 1];
    
    a1 = double(a1);  a2 = double(a2);
    
    % TODO: Calculeaz? matricea final?.
    A = a1 * [A1 A3; A2 A4]' * a2;
    
endfunction