function [posBille] = Correlation(image, bille, rayonBille, coinHautGaucheImage)
        % Faire la correlation entre l'image de la bille et l'image
        % courante
        corr = normxcorr2(bille, image);
        
        % Afficher la correlation
        figure
        mesh(corr);
        
        % Trouver l'indice de la bille dans l'image courante reduite
        ind = find(corr==(max(max(corr))));
        [x y] = ind2sub(size(corr),ind);
        
        % On trouve la position de la bille dans l'image originelle.
        % L'indice trouve est celui dans le coin en bas a droite de la
        % bille.
        posBille(1) = round(y + coinHautGaucheImage(1) - rayonBille);
        posBille(2) = round(x + coinHautGaucheImage(2) - rayonBille);
end
