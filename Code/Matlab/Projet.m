clear all
close all
clc
format compact

pathImage = 'C:\Users\Utilisteur\Programmation\S4\Projet\Git_Project\images\asservissement_actif\bmp\';
pathBille = 'C:\Users\Utilisteur\Documents\MATLAB\S4Projet\';
imageA = imread(strcat(pathImage,'image_0.bmp'));
bille = imread(strcat(pathBille,'bille.bmp'));

imageAGreen = toGreen(imageA);
billeGreen = toGreen(bille);

% printGreenImage(greenImage);
