% Fonction de tests seulement
function [] = printCarre(imageA, coords, titre)
%     zeroArr = zeros(size(imageA));
%     newImage(:,:,1) = zeroArr;
%     newImage(:,:,2) = imageA;
%     newImage(:,:,3) = zeroArr;
    
    imageA(coords(2):coords(4), coords(1), 1) = 0;
    imageA(coords(2):coords(4), coords(3), 1) = 0;
    imageA(coords(2), coords(1):coords(3), 1) = 0;
    imageA(coords(4), coords(1):coords(3), 1) = 0;
    
    imageA(coords(2):coords(4), coords(1), 2) = 0;
    imageA(coords(2):coords(4), coords(3), 2) = 0;
    imageA(coords(2), coords(1):coords(3), 2) = 0;
    imageA(coords(4), coords(1):coords(3), 2) = 0;
    
    imageA(coords(2):coords(4), coords(1), 3) = 255;
    imageA(coords(2):coords(4), coords(3), 3) = 255;
    imageA(coords(2), coords(1):coords(3), 3) = 255;
    imageA(coords(4), coords(1):coords(3), 3) = 255;
    
%     imageA = imageA/256;
    figure
    image(imageA);
    title(titre)
end
