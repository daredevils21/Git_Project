clear all
close all
clc
format compact

%% Aller chercher les images en vertes
% pathImage = '..\..\images\asservissement_actif\bmp\';
pathImage = '..\..\images\\statique_zmax_version 1\bmp\';
pathBille = '..\..\images\';

% recuperation des noms des images dans un tableau
Dir = dir(pathImage); 
Dir = Dir(~cell2mat({Dir(:).isdir}));
liste = {Dir(:).name};

bille = imread(strcat(pathBille,'bille_verte.bmp'));

billeGreen = toGreen(bille);
rayonBille = length(billeGreen)/2;

%% Constantes
thetaMax = 15; % degres
fHz = 30; % 30 photos par seconde
rayonPlaque = 62.5/1000; % m
diametrePlaque = 2*rayonPlaque;
mSphere = 8; % g
g = 9.81;

%% Calculs physiques pour deplacement max
% Mecanique
accel = g*sind(thetaMax)/mSphere;
tDiametrePlaque = sqrt(diametrePlaque*2/accel);
vMax = accel*tDiametrePlaque;
% energie
v = sqrt(2*g*diametrePlaque*sind(thetaMax));

%% Boucle des differentes images
% for i=1:length(liste)
    
    imageA = imread(strcat(pathImage, char(liste(1))));
    imageGreen = toGreen(imageA);

    %printGreenImage(billeGreen);

    %% Trouver le cercle de la plaque (algo Mathieu)
    % valeurs de retour : 
    coteGauchePlaque = 40;
    coteDroitPlaque = 398;
    coteHautPlaque = 73;
    coteBasPlaque = 431;
    

    %% Calculs physiques pour deplacement max
    % On va peut-etre avoir besoin des gars d'elec pour la bonne vMax 
    vMaxParFrame = vMax/fHz;

    diametrePlaquePixel = coteDroitPlaque-coteGauchePlaque;
    centrePlaqueHorizontal = mean([coteGauchePlaque coteDroitPlaque]);

    vPixMaxParFrame = vMaxParFrame*diametrePlaquePixel/diametrePlaque;

    
    %% Premiere partie
%     if i==1
        % Premiere image : on fait une correlation sur toute la grosseur de la
        % plaque afin de trouver la balle
        imageCorr = imageGreen(coteHautPlaque:coteBasPlaque,coteGauchePlaque:coteDroitPlaque);
        position_bille = Correlation(imageCorr, billeGreen, rayonBille, [coteGauchePlaque, coteHautPlaque])        % printGreenImage(imageCorr);
        printCarreCorr(imageGreen, [coteGauchePlaque, coteHautPlaque, coteDroitPlaque, coteBasPlaque]);
        
        % Sorties
%         position_bille = [round(mean([188 209])) round(mean([230 251]))]; %hardcode pour l'instant
        posCarreCorr = trouverPosCercleCorr(position_bille, rayonBille, 30, -pi/4);
%     end
    
    %% Deuxieme partie
%     if((i==2)||(i==3))
        % Deuxieme et troisieme image : On fait une correlation de la balle avec un
        % cercle centre sur la balle et avec un rayon de vMax car on ne connaet pas
        % sa vitesse

        % devrait etre la prochaine image (2e)
        imageCorr = imageGreen(posCarreCorr(2):posCarreCorr(4),posCarreCorr(1):posCarreCorr(3));
        printCarreCorr(imageGreen, posCarreCorr);
%     end
    %% Troisieme partie
%     if i>=4
        % Quatrieme image et plus : e ce point, on connait la vitesse et
        % l'orientation de la vitesse de la balle, on fait donc une correlation de
        % la balle avec un cercle plus petit selon la vitesse courante de la balle
        % et on le place vers la direction oe la balle s'enligne.
%     end
% end
