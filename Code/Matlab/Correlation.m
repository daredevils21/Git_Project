function [posBille] = Correlation(image, bille, rayonBille, coinGaucheImage)
        truc = normxcorr2(bille, image);
        
        figure
        mesh(truc);
        ind = find(truc==(max(max(truc))));
        [posBilleDansCorrX posBilleDansCorrY] = ind2sub(size(truc),ind);
        
        posBille(1) = round(posBilleDansCorrY + coinGaucheImage(1) - rayonBille);
        posBille(2) = round(posBilleDansCorrX + coinGaucheImage(2) - rayonBille);
end
