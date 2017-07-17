function [] = printGreenImage(greenImage)
    zeroArr = zeros(size(greenImage));
    newImage(:,:,1) = zeroArr;
    newImage(:,:,2) = greenImage;
    newImage(:,:,3) = zeroArr;
    newImage = newImage/256;
    image(newImage);
%     imwrite(newImage, 'greenImage.bmp');
end
