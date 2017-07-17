clear all
close all
clc
format compact

%% Aller chercher les images en vertes
pathImage = '..\..\images\asservissement_actif\bmp\';
pathBille = '..\..\images\';

%r�cuperation des noms des images dans un tableau
Dir = dir(pathImage); 
Dir = Dir(~cell2mat({Dir(:).isdir}));
liste = {Dir(:).name};

bille = imread(strcat(pathBille,'bille_verte.bmp'));

billeGreen = toGreen(bille);
rayonBille = length(billeGreen)/2;

 %% Constantes
 thetaMax = 15; % degr�s
 fHz = 30; % 30 photos par seconde
 rayonPlaque = 62.5/1000; % m
 diametrePlaque = 2*rayonPlaque;
 mSphere = 8; % g
 g = 9.81;
 
 %% Calculs physiques pour d�placement max
 % M�canique
 accel = g*sind(thetaMax)/mSphere;
 tDiametrePlaque = sqrt(diametrePlaque*2/accel);
 vMax = accel*tDiametrePlaque;
 % �nergie
 v = sqrt(2*g*diametrePlaque*sind(thetaMax));

%% Boucle des diff�rentes images
for i=1:length(liste)
    
    imageA = imread(strcat(pathImage, liste(i)));
    imageGreen = toGreen(imageA);

    %printGreenImage(billeGreen);

    %% Trouver le cercle de la plaque (algo Mathieu)
    %valeurs de retour : 
    coteGauchePlaque = 40;
    coteDroitPlaque = 398;
    coteHautPlaque = 73;
    coteBasPlaque = 430;
    

    %% Calculs physiques pour d�placement max
    % On va peut-�tre avoir besoin des gars d'�lec pour la bonne vMax 
    vMaxParFrame = vMax/fHz;

    diametrePlaquePixel = coteDroitPlaque-coteGauchePlaque;
    centrePlaqueHorizontal = mean([coteGauchePlaque coteDroitPlaque]);

    vPixMaxParFrame = vMaxParFrame*diametrePlaquePixel/diametrePlaque;

    
    %% Premi�re partie
    if i==1
        % Premi�re image : on fait une corr�lation sur toute la grosseur de la
        % plaque afin de trouver la balle
        imageCorr = imageGreen(coteHautPlaque:coteBasPlaque,coteGauchePlaque:coteDroitPlaque);
        position_bille = Correlation(imageCorr, billeGreen);
        % printGreenImage(imageCorr);

        % Sorties
        position_bille = [round(mean([188 209])) round(mean([230 251]))]; %hardcod� pour l'instant
        posCarreCorr = trouverPosCercleCorr(position_bille, rayonBille, 30, -pi/4);
    end
    
    %% Deuxi�me partie
    if((i==2)||(i==3))
        % Deuxi�me et troisi�me image : On fait une corr�lation de la balle avec un
        % cercle centr� sur la balle et avec un rayon de vMax car on ne conna�t pas
        % sa vitesse

        % devrait �tre la prochaine image (2e)
        imageCorr = imageGreen(posCarreCorr(2):posCarreCorr(4),posCarreCorr(1):posCarreCorr(3));
        printCarreCorr(imageGreen, posCarreCorr);
    end
    %% Troisi�me partie
    if i>=4
        % Quatri�me image et plus : � ce point, on connait la vitesse et
        % l'orientation de la vitesse de la balle, on fait donc une corr�lation de
        % la balle avec un cercle plus petit selon la vitesse courante de la balle
        % et on le place vers la direction o� la balle s'enligne.
    end
end
