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
thetaMax = 15; % degr�s
fHz = 30; % 30 photos par seconde
rayonPlaque = 62.5/1000; % m
diametrePlaque = 2*rayonPlaque;
mSphere = 8; % kg
g = 9.81;

%% Calculs physiques pour d�placement max
% M�canique
accel = g*sind(thetaMax)/mSphere;
tDiametrePlaque = sqrt(diametrePlaque*2/accel);
vMax = accel*tDiametrePlaque;
% �nergie
v = sqrt(2*g*diametrePlaque*sind(thetaMax));

% On va peut-�tre avoir besoin des gars d'�lec pour la bonne vMax 
vMaxParFrame = vMax/fHz;


diametrePlaquePixel = coteDroitPlaque-coteGauchePlaque;
centrePlaqueHorizontal = mean([coteGauchePlaque coteDroitPlaque]);

vPixMaxParFrame = vMaxParFrame*diametrePlaquePixel/diametrePlaque;

%% Premi�re partie
% Premi�re image : on fait une corr�lation sur toute la grosseur de la
% plaque afin de trouver la balle


% Sorties
position_bille = [round(mean([188 209])) round(mean([230 251]))]; %hardcod� pour l'instant
position_cercle_corr = trouverPosCercleCorr(position_bille, 0, 0);

%% Deuxi�me partie
% Deuxi�me et troisi�me image : On fait une corr�lation de la balle avec un
% cercle centr� sur la balle et avec un rayon de vMax car on ne conna�t pas
% sa vitesse

Correlation(billeGreen, position_cercle_corr,  vMax);

%% Troisi�me partie
% Quatri�me image et plus : � ce point, on connait la vitesse et
% l'orientation de la vitesse de la balle, on fait donc une corr�lation de
% la balle avec un cercle plus petit selon la vitesse courante de la balle
% et on le place vers la direction o� la balle s'enligne.

