clc
clear
close all

GREEN = 2;

image = imread('C:\Users\mat_8\Desktop\S4\Projet\git\detection\images\vitesse_max_version_3\bmp\image_1253.bmp');
bille = imread('C:\Users\mat_8\Desktop\S4\Projet\git\detection\images\bille_verte.bmp');

bille = 255 - bille; 

[center, rayon] = detection_plaque(image);

bille_diametre = length(bille);
bille_rayon = bille_diametre/2;

rectX_Left = int32(center(1) - rayon + bille_rayon + 50);
rectX_Right = int32(center(1) + rayon - bille_rayon - 50);
rectY_Top = int32(center(2) - rayon + bille_rayon + 50);
rectY_Bottom = int32(center(2) + rayon - bille_rayon - 50);

my_corr = zeros(480,480);

smallest = 2000000000;
pos = 0;

for y = rectY_Top : bille_diametre : rectY_Bottom
    for x = rectX_Left : bille_diametre : rectX_Right
        for yy = 1 : bille_diametre
            for xx = 1 : bille_diametre
                my_corr(y,x) = my_corr(y,x) + int32(image(y-bille_rayon+yy, x-bille_rayon+xx, GREEN)) ...
                    * int32(bille(yy,xx,GREEN));
            end
        end
        if my_corr(y,x) < smallest
            smallest = my_corr(y,x);
            pos = [ y, x ];
        end
    end
end

for y = pos(1)-bille_diametre : pos(1)+bille_diametre
    for x = pos(2)-bille_diametre : pos(2)+bille_diametre
        for yy = 1 : bille_diametre
            for xx = 1 : bille_diametre
                my_corr(y,x) = my_corr(y,x) + int32(image(y-bille_rayon+yy, x-bille_rayon+xx, GREEN)) ...
                    * int32(bille(yy,xx,GREEN));
            end
        end
        if my_corr(y,x) < smallest
            smallest = my_corr(y,x);
            pos = [ y, x ];
        end
    end
end
