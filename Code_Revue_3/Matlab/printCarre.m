% Fonction de tests seulement
function [] = printCarre(imageA, coords, titre, coords2)
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
    
    if nargin > 3
        imageA(coords2(2):coords2(4), coords2(1), 1) = 0;
        imageA(coords2(2):coords2(4), coords2(3), 1) = 0;
        imageA(coords2(2), coords2(1):coords2(3), 1) = 0;
        imageA(coords2(4), coords2(1):coords2(3), 1) = 0;

        imageA(coords2(2):coords2(4), coords2(1), 2) = 0;
        imageA(coords2(2):coords2(4), coords2(3), 2) = 0;
        imageA(coords2(2), coords2(1):coords2(3), 2) = 0;
        imageA(coords2(4), coords2(1):coords2(3), 2) = 0;

        imageA(coords2(2):coords2(4), coords2(1), 3) = 255;
        imageA(coords2(2):coords2(4), coords2(3), 3) = 255;
        imageA(coords2(2), coords2(1):coords2(3), 3) = 255;
        imageA(coords2(4), coords2(1):coords2(3), 3) = 255;
    end
    
    figure
    image(imageA);
    title(titre)
end
