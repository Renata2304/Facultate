function out = proximal_rotate_RGB(img, rotation_angle)
    % =========================================================================
    % Aplica Interpolarea Proximala pentru a roti o imagine RGB cu un unghi dat.
    % =========================================================================
    
    % TODO: Extrage canalul rosu al imaginii.
    R = img(:, :, 1);
    % TODO: Extrage canalul verde al imaginii.
    G = img(:, :, 2);
    % TODO: Extrage canalul albastru al imaginii.
    B = img(:, :, 3);
    % TODO: Aplica rotatia pe fiecare canal al imaginii.
    R = proximal_rotate(R, rotation_angle);
    G = proximal_rotate(G, rotation_angle);
    B = proximal_rotate(B, rotation_angle);
    % TODO: Formeaza imaginea finala concatenând cele 3 canale de culori.
    out(:, :, 1) = R;
    out(:, :, 2) = G;
    out(:, :, 3) = B;
endfunction