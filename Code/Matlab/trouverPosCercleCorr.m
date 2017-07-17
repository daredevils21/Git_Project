function [posCercleCorr] = trouverPosCercleCorr(posBille, rayonBille, vitesse, angle)
    facteur = 0.5;
    facVitesse = facteur*vitesse;
    posCentreCercleCorr(1) = posBille(1) + facVitesse*cos(angle);
    posCentreCercleCorr(2) = posBille(2) - facVitesse*sin(angle);
    
    posCercleCorr = [round(posCentreCercleCorr(1) - facVitesse - rayonBille), ...
                     round(posCentreCercleCorr(2) - facVitesse - rayonBille), ...
                     round(posCentreCercleCorr(1) + facVitesse + rayonBille), ...
                     round(posCentreCercleCorr(2) + facVitesse + rayonBille)];
end