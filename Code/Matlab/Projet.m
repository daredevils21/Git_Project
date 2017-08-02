clear all
close all
clc
format compact

%% Aller chercher les images en vertes
% recuperation des noms des dossiers images dans un tableau
num_folder = 4;

pathBille = '..\..\images\';
Dir = dir(pathBille); 
Dir = Dir(cell2mat({Dir(:).isdir}));
dossier = {Dir(:).name};
for i=1:(length(dossier)-2)
    dossier(i) = dossier(i+2);
end
dossier(13:14) = [];

% recuperation des noms des images dans un tableau
pathImage = strcat('..\..\images\',char(dossier(num_folder)),'\bmp\');
dir = dir(pathImage); 
dir = dir(~cell2mat({dir(:).isdir}));
liste = {dir(:).name};
liste = sort_nat(liste);
bille = imread(strcat(pathBille,'bille_verte2.bmp'));

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
vMaxParFrame = vMax/fHz;
% energie
v = sqrt(2*g*diametrePlaque*sind(thetaMax));

%% Boucle des differentes images
for i=1:length(liste)
    
    imageA = imread(strcat(pathImage, char(liste(i))));
    imageGreen = toGreen(imageA);
    
    %% Trouver le cercle de la plaque
    [coordsPlaque, centrePlaque, rayonPlaque] = detection_plaque(imageGreen);
    diametrePlaquePixel = coordsPlaque(3)-coordsPlaque(1);
    vPixMaxParFrame = vMaxParFrame*diametrePlaquePixel/diametrePlaque;
    
    %% Premiere partie
    if i==1
        % Premiere image : on fait une correlation sur toute la grosseur de la
        % plaque afin de trouver la balle
        
        msgErreur = ' ';
        % Image a correler
        carreCorr = imageGreen(coordsPlaque(2):coordsPlaque(4),coordsPlaque(1):coordsPlaque(3));
        
        % Position de la bille et afficher
        [position_bille, max] = Correlation(carreCorr, billeGreen, rayonBille, [coordsPlaque(1), coordsPlaque(2)], centrePlaque, rayonPlaque);
        seuilBille = max * 0.75;
        
        % Sorties
        posCarreCorr = trouverPosCercleCorr(position_bille, rayonBille, 0, 0, vPixMaxParFrame);
                
        carreBille = [round(position_bille(1)- rayonBille), ...
                      round(position_bille(2) - rayonBille), ...
                      round(position_bille(1) + rayonBille), ...
                      round(position_bille(2) + rayonBille)];
        printCarre(imageA, carreBille, 'Position de la bille')
    else
        % A ce point, on connait la vitesse et
        % l'orientation de la vitesse de la balle, on fait donc une correlation de
        % la balle avec un cercle plus petit selon la vitesse courante de la balle
        % et on le place vers la direction ou la balle s'enligne.
        
        vieille_pos = position_bille;
        msgErreur = ' ';
        carreCorr = imageGreen(posCarreCorr(2):posCarreCorr(4),posCarreCorr(1):posCarreCorr(3));
        [position_bille, max] = Correlation(carreCorr, billeGreen, rayonBille, [posCarreCorr(1), posCarreCorr(2)], centrePlaque, rayonPlaque);
        max2 = max;
        for j = 1:2
            if (max < seuilBille && max2 < seuilBille)
                posCarreCorr = coordsPlaque;
                carreCorr = imageGreen(posCarreCorr(2):posCarreCorr(4),posCarreCorr(1):posCarreCorr(3));
                [position_bille, max2] = Correlation(carreCorr, billeGreen, rayonBille, [posCarreCorr(1), posCarreCorr(2)], centrePlaque, rayonPlaque);
                if (max2 < seuilBille)
                    position_bille = vieille_pos;
                    msgErreur = ' Bille non trouvée';
                end
            end
        end
        
        [vPix(i), angle_vitesse(i)] = trouverVecteurVitesse(vieille_pos, position_bille);
        vit(i) = vPix(i)*diametrePlaque/diametrePlaquePixel;
        
        % Affichage
        carreBille = [round(position_bille(1)- rayonBille), ...
                     round(position_bille(2) - rayonBille), ...
                     round(position_bille(1) + rayonBille), ...
                     round(position_bille(2) + rayonBille)];
        printCarre(imageA, carreBille, [char(liste(i)); 'Vitesse : '; string(vPix(i)); 'pixel/frame ', 'Angle : '; string(angle_vitesse(i)); ' rad'], posCarreCorr)
        
        posCarreCorr = trouverPosCercleCorr(position_bille, rayonBille, 0, angle_vitesse(i), vPixMaxParFrame);
    end
    disp(strcat('Position :', msgErreur, '( ', string(position_bille(1)), ', ', string(position_bille(2)), ')'));
end
