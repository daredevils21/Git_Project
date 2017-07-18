function [posBille] = Correlation(image, bille, rayonBille, coinHautGaucheImage, centrePlaque, rayonPlaque)
    % Faire la correlation entre l'image de la bille et l'image
    % courante
    corr = normxcorr2(bille, image);

    % Afficher la correlation
    figure
    mesh(corr);

    % Do-while pour aller chercher le maximum sur la plaque (et non sur
    % le contour)
    distanceCentreBille = rayonPlaque + 1;
    while distanceCentreBille >= rayonPlaque 
        % Trouver l'indice de la bille dans l'image courante reduite
        ind = find(corr==(max(max(corr))));
        [x,y] = ind2sub(size(corr),ind);


        % On trouve la position de la bille dans l'image originelle.
        % L'indice trouve est celui dans le coin en bas a droite de la
        % bille.
        posBille(1) = round(y + coinHautGaucheImage(1) - rayonBille);
        posBille(2) = round(x + coinHautGaucheImage(2) - rayonBille);

        % Distance entre la bille et le centre de la plaque
        distanceCentreBille = sqrt((posBille(1)-centrePlaque(1))^2 + (posBille(2)-centrePlaque(2))^2)
        if distanceCentreBille > rayonPlaque
            corr(x,y) = 0;
        end
%             distanceCentreBille = rayonPlaque - 1;
    end
end
