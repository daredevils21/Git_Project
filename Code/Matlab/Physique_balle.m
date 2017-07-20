clc
clear
close all

pathImage = '..\..\images\vitesse_max_version_3\bmp\';
pathBille = '..\..\images\';

image = imread(strcat(pathImage,'image_1253.bmp'));
bille = imread(strcat(pathBille,'bille_verte.bmp'));

% Calcul des limites physiques
[center, rayon] = detection_plaque(image);

rayon_plaque_reel = 0.0625;
p_per_m = rayon /rayon_plaque_reel;
accel = 5/7 * 9.81 * sind(5);
p_accel = accel * p_per_m;

v_max = accel * sqrt((rayon*2)/(0.5*p_accel)) * p_per_m;

% Début de calcul dynamique

x = 0;
vx = 0;
dt = 1/30;

for t = 0 : 300
    x = x + vx * dt + 0.5 * p_accel * dt^2;
    if x > rayon*2
        break
    end
    vx = vx + p_accel * dt;
end