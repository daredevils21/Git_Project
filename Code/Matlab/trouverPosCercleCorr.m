function [posCercleCorr] = trouverPosCercleCorr(posBille, rayonBille, vitesse, angle, vMax)
    facteur = 0.5;
    facVitesse = facteur*vitesse;
    posCentreCercleCorr(1) = posBille(1) + facVitesse*cos(angle);
    posCentreCercleCorr(2) = posBille(2) - facVitesse*sin(angle);
    rayonBille = 2*rayonBille;
    if vitesse == 0 % Si on mets la vitesse max partout alentours (aucun angle spécifié)
        posCercleCorr = [round(posCentreCercleCorr(1) - vMax - rayonBille), ...
                         round(posCentreCercleCorr(2) - vMax - rayonBille), ...
                         round(posCentreCercleCorr(1) + vMax + rayonBille), ...
                         round(posCentreCercleCorr(2) + vMax + rayonBille)];
    else % Si on connait la vitesse et on sait sa direction
        posCercleCorr = [round(posCentreCercleCorr(1) - vitesse - rayonBille), ...
                         round(posCentreCercleCorr(2) - vitesse - rayonBille), ...
                         round(posCentreCercleCorr(1) + vitesse + rayonBille), ...
                         round(posCentreCercleCorr(2) + vitesse + rayonBille)];
    end
    
    % On s'assure que les coordonnees depassent pas les rebords de l'image
    for i = 1:4
        if posCercleCorr(i) > 480
            posCercleCorr(i) = 480;
        else if posCercleCorr(i) < 0
            posCercleCorr(i) = 0;
        end
    end
    
end