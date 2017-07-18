clear all
close all
clc
format compact

%% Aller chercher les images en vertes
% recuperation des noms des dossiers images dans un tableau
pathBille = '..\..\images\';
Dir = dir(pathBille); 
Dir = Dir(cell2mat({Dir(:).isdir}));
dossier = {Dir(:).name};
for i=1:(length(dossier)-2)
    dossier(i) = dossier(i+2);
end
dossier(13:14) = [];


% recuperation des noms des images dans un tableau
% pathImage = '..\..\images\asservissement_actif\bmp\';
pathImage = strcat('..\..\images\',char(dossier(7)),'\bmp\');
dir = dir(pathImage); 
dir = dir(~cell2mat({dir(:).isdir}));
liste = {dir(:).name};

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
    
    imageA = imread(strcat(pathImage, char(liste(14))));
    imageGreen = toGreen(imageA);

    %printGreenImage(billeGreen);

    %% Trouver le cercle de la plaque
    
    coordsPlaque = detection_plaque(imageGreen)
    
    %% Calculs physiques pour deplacement max
    % On va peut-etre avoir besoin des gars d'elec pour la bonne vMax 
    vMaxParFrame = vMax/fHz;

    diametrePlaquePixel = coordsPlaque(3)-coordsPlaque(1);
    centrePlaqueHorizontal = mean([coordsPlaque(1) coordsPlaque(3)]);

    vPixMaxParFrame = vMaxParFrame*diametrePlaquePixel/diametrePlaque;

    
    %% Premiere partie
%     if i==1
        % Premiere image : on fait une correlation sur toute la grosseur de la
        % plaque afin de trouver la balle
        
        % Image a correler
        imageCorr = imageGreen(coordsPlaque(2):coordsPlaque(4),coordsPlaque(1):coordsPlaque(3));
        printCarre(imageGreen, coordsPlaque);
        % Position de la bille et afficher
        position_bille = Correlation(imageCorr, billeGreen, rayonBille, [coordsPlaque(1), coordsPlaque(2)])        % printGreenImage(imageCorr);
        carreBille = trouverPosCercleCorr(position_bille, rayonBille, 0,0);
        printCarre(imageGreen, carreBille)
        
        % Sorties
        posCarreCorr = trouverPosCercleCorr(position_bille, rayonBille, 30, -pi/4);
%     end
    
    %% Deuxieme partie
%     if((i==2)||(i==3))
        % Deuxieme et troisieme image : On fait une correlation de la balle avec un
        % cercle centre sur la balle et avec un rayon de vMax car on ne connaet pas
        % sa vitesse

        % Afficher le carre de corelation
        imageCorr = imageGreen(posCarreCorr(2):posCarreCorr(4),posCarreCorr(1):posCarreCorr(3));
        printCarre(imageGreen, posCarreCorr);
%     end
    %% Troisieme partie
%     if i>=4
        % Quatrieme image et plus : e ce point, on connait la vitesse et
        % l'orientation de la vitesse de la balle, on fait donc une correlation de
        % la balle avec un cercle plus petit selon la vitesse courante de la balle
        % et on le place vers la direction oe la balle s'enligne.
%     end
% end
