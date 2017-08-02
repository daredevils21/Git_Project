clc
clear
close all

pathImage = '..\..\images\vitesse_max_version_3\bmp\';
pathBille = '..\..\images\';

image = imread(strcat(pathImage,'image_1253.bmp'));
bille = imread(strcat(pathBille,'bille_verte.bmp'));

% Calcul des limites physiques
[coord, center, rayon] = detection_plaque(image);

theta = 15;
fHz = 30;
rayon_plaque_reel = 0.0625;

accel = 5/7 * 9.81 * sind(theta);
t = sqrt((rayon_plaque_reel*2)/accel)

p_per_m = rayon / rayon_plaque_reel;
p_accel = accel * p_per_m;
p_v_max = p_accel * t;
v_max = accel * t;
dx_max_per_frame = p_v_max / fHz; 

% Début de calcul dynamique

pos_balle = [ 100 100 ];
rayon_bille = 12;
vx = p_v_max;
vy = p_v_max;

dt = 1/fHz;

delta_x_max = p_v_max * dt + 0.5 * p_accel * dt^2;
