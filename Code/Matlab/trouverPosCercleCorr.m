function [posCercleCorr] = trouverPosCercleCorr(posBille, vitesse, angle)
    facteur = 0.5;
    xCercleCorr = posBille(1) + facteur*vitesse*cos(angle);
    yCercleCorr = posBille(2) + facteur*vitesse*sin(angle);
    posCercleCorr = [xCercleCorr yCercleCorr];
end