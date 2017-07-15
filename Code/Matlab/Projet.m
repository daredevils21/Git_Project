clear all
close all
clc
format compact

pathImage = '..\..\images\asservissement_actif\bmp\';
pathBille = '..\..\images\';
imageA = imread(strcat(pathImage,'image_0.bmp'));
bille = imread(strcat(pathBille,'bille_verte.bmp'));

imageAGreen = toGreen(imageA);
billeGreen = toGreen(bille);

% printGreenImage(imageAGreen);
