clear all
close all
clc
format compact

%% Aller chercher les images en vertes
pathImage = '..\..\images\asservissement_actif\bmp\';
pathBille = '..\..\images\';
imageA = imread(strcat(pathImage,'image_0.bmp'));
bille = imread(strcat(pathBille,'bille_verte.bmp'));

imageAGreen = toGreen(imageA);
billeGreen = toGreen(bille);

% printGreenImage(imageAGreen);

%% Trouver le cercle de la plaque (algo Mathieu)


%valeurs de retour : 
coteGauchePlaque = 40;
coteDroitPlaque = 398;


%% Constantes
thetaMax = 15; % degrés
fHz = 30; % 30 photos par seconde
rayonPlaque = 62.5/1000; % m
diametrePlaque = 2*rayonPlaque;
mSphere = 8; % kg
g = 9.81;

%% Calculs physiques pour déplacement max
% Mécanique
accel = g*sind(thetaMax)/mSphere;
tDiametrePlaque = sqrt(diametrePlaque*2/accel);
vMax = accel*tDiametrePlaque;
% Énergie
v = sqrt(2*g*diametrePlaque*sind(thetaMax));

% On va peut-être avoir besoin des gars d'élec pour la bonne vMax 
vMaxParFrame = vMax/fHz;


diametrePlaquePixel = coteDroitPlaque-coteGauchePlaque;
centrePlaqueHorizontal = mean([coteGauchePlaque coteDroitPlaque]);

vPixMaxParFrame = vMaxParFrame*diametrePlaquePixel/diametrePlaque;

%% Première partie
% Première image : on fait une corrélation sur toute la grosseur de la
% plaque afin de trouver la balle


% Sorties
position_bille = [round(mean([188 209])) round(mean([230 251]))]; %hardcodé pour l'instant
position_cercle_corr = trouverPosCercleCorr(position_bille, 0, 0);

%% Deuxième partie
% Deuxième et troisième image : On fait une corrélation de la balle avec un
% cercle centré sur la balle et avec un rayon de vMax car on ne connaît pas
% sa vitesse

Correlation(billeGreen, position_cercle_corr,  vMax);

%% Troisième partie
% Quatrième image et plus : À ce point, on connait la vitesse et
% l'orientation de la vitesse de la balle, on fait donc une corrélation de
% la balle avec un cercle plus petit selon la vitesse courante de la balle
% et on le place vers la direction où la balle s'enligne.

