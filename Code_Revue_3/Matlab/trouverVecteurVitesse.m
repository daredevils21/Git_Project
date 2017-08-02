function [vitesse, angle] = trouverVecteurVitesse(old, new)
    % Axes normaux          Axes de l'image
    %  Y                    ________> X
    %  ^                    |
    %  |                    |
    %  |                    v
    %  |_________> X        Y
    % L'axe des X reste pareil, l'axe des y est en négatif (donc on le
    % corrige pour l'angle.

    deltaX = new(1) - old(1);
    deltaY = -(new(2) - old(2));
    vitesse = sqrt(deltaX^2 + deltaY^2);
    angle = atan(deltaY/(deltaX+0.000000001)); % pour pas diviser par 0
    
    if isnan(angle) % atan(0) = NaN <- aucun mouvement en Y
        angle = 0;
    end
    
    % atan sort des valeurs de -pi/2 a pi/2 seulement, il faut additionner
    % pi pour etre dans le bon quadrant lorsque notre vecteur va a gauche.
    if deltaX < 0
        angle = angle + pi;
    end
end