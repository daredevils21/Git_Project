% Fonction de tests seulement
function [] = printCarre(greenImage, coords)
    zeroArr = zeros(size(greenImage));
    newImage(:,:,1) = zeroArr;
    newImage(:,:,2) = greenImage;
    newImage(:,:,3) = zeroArr;
    
    newImage(coords(2):coords(4), coords(1), 1) = 255;
    newImage(coords(2):coords(4), coords(3), 1) = 255;
    newImage(coords(2), coords(1):coords(3), 1) = 255;
    newImage(coords(4), coords(1):coords(3), 1) = 255;
    
    newImage(coords(2):coords(4), coords(1), 2) = 0;
    newImage(coords(2):coords(4), coords(3), 2) = 0;
    newImage(coords(2), coords(1):coords(3), 2) = 0;
    newImage(coords(4), coords(1):coords(3), 2) = 0;
    
    newImage = newImage/256;
    figure
    image(newImage);
end
