function [] = printGreenImage(greenImage)
    zeroArr = zeros(480, 480);
    newImage(:,:,1) = zeroArr;
    newImage(:,:,2) = greenImage;
    newImage(:,:,3) = zeroArr;
    newImage = newImage/256;
    image(newImage);
    imwrite(newImage, 'greenImage.bmp');
end
