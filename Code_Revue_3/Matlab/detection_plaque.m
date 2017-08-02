function [Coords, Centre, Rayon] = detection_plaque(greenImage)

    % Constantes
    [IMAGE_WIDTH,IMAGE_HEIGHT] = size(greenImage);

    HALF_IMAGE_WIDTH = IMAGE_WIDTH/2;
    HALF_IMAGE_HEIGHT = IMAGE_HEIGHT/2;

    THIRD_IMAGE_WIDTH = IMAGE_WIDTH/3;
    THIRD_IMAGE_HEIGHT = IMAGE_HEIGHT/3;

    MAX_SCALE = 0.5;
    DIAMETER_TOLERENCE = 1.05;

    % Variables
    max = [ 0 0 ];
    max_chosen = [ 0 0 ];
    index_X = [ 0 0 ];
    index_Y = [ 0 0 ];
    intensity_X = [ 0 0 ];
    intensity_Y = [ 0 0 ];

    % Detection du dessus de la plaque
    for j = 2 : THIRD_IMAGE_HEIGHT
        gx = greenImage(j-1, HALF_IMAGE_WIDTH-1) ...
            + 2*greenImage(j, HALF_IMAGE_WIDTH-1) ...
            + greenImage(j+1, HALF_IMAGE_WIDTH-1) ...
            - greenImage(j-1, HALF_IMAGE_WIDTH+1) ...
            - greenImage(j, HALF_IMAGE_WIDTH+1) ...
            - greenImage(j+1, HALF_IMAGE_WIDTH+1);

        gy = greenImage(j-1, HALF_IMAGE_WIDTH-1) ...
            + 2*greenImage(j-1, HALF_IMAGE_WIDTH) ...
            + greenImage(j-1, HALF_IMAGE_WIDTH+1) ...
            - greenImage(j+1, HALF_IMAGE_WIDTH-1) ...
            - greenImage(j+1, HALF_IMAGE_WIDTH) ...
            - greenImage(j+1, HALF_IMAGE_WIDTH+1);

        g = sqrt(double(gx*gx + gy*gy));

        if g > max(1)*MAX_SCALE
            intensity_Y(1) = intensity_Y(1)+g;
            max_chosen(1) = g;
            index_Y(1) = j;
            if max_chosen(1) > max(1)
                max(1) = max_chosen(1);
            end
        end
    end

    % Detection du dessous de la plaque
    for j = IMAGE_HEIGHT - 1 : -1 : THIRD_IMAGE_HEIGHT*2
        gx = greenImage(j-1, HALF_IMAGE_WIDTH-1) ...
            + 2*greenImage(j, HALF_IMAGE_WIDTH-1) ...
            + greenImage(j+1, HALF_IMAGE_WIDTH-1) ...
            - greenImage(j-1, HALF_IMAGE_WIDTH+1) ...
            - greenImage(j, HALF_IMAGE_WIDTH+1) ...
            - greenImage(j+1, HALF_IMAGE_WIDTH+1);

        gy = greenImage(j-1, HALF_IMAGE_WIDTH-1) ...
            + 2*greenImage(j-1, HALF_IMAGE_WIDTH) ...
            + greenImage(j-1, HALF_IMAGE_WIDTH+1) ...
            - greenImage(j+1, HALF_IMAGE_WIDTH-1) ...
            - greenImage(j+1, HALF_IMAGE_WIDTH) ...
            - greenImage(j+1, HALF_IMAGE_WIDTH+1);

        g = sqrt(double(gx*gx + gy*gy));

        if g > max(2)*MAX_SCALE
            intensity_Y(2) = intensity_Y(2)+g;
            max_chosen(2) = g;
            index_Y(2) = j;
            if max_chosen(2) > max(2)
                max(2) = max_chosen(2);
            end
        end
    end

    max = [ 0 0 ];
    max_chosen = [ 0 0 ];

    % Detection du cote gauche de la plaque
    for j = 2 : THIRD_IMAGE_WIDTH
        gx = greenImage(HALF_IMAGE_HEIGHT-1, j-1) ...
            + 2*greenImage(HALF_IMAGE_HEIGHT, j-1) ...
            + greenImage(HALF_IMAGE_HEIGHT+1, j-1) ...
            - greenImage(HALF_IMAGE_HEIGHT-1, j+1) ...
            - greenImage(HALF_IMAGE_HEIGHT, j+1) ...
            - greenImage(HALF_IMAGE_HEIGHT+1, j+1);

        gy = greenImage(HALF_IMAGE_HEIGHT-1, j-1) ...
            + 2*greenImage(HALF_IMAGE_HEIGHT-1, j) ...
            + greenImage(HALF_IMAGE_HEIGHT-1, j+1) ...
            - greenImage(HALF_IMAGE_HEIGHT+1, j-1) ...
            - greenImage(HALF_IMAGE_HEIGHT+1, j) ...
            - greenImage(HALF_IMAGE_HEIGHT+1, j+1);

        g = sqrt(double(gx*gx + gy*gy));

        if g > max(1)*MAX_SCALE
            intensity_X(1) = intensity_X(1)+g;
            max_chosen(1) = g;
            index_X(1) = j;
            if max_chosen(1) > max(1)
                max(1) = max_chosen(1);
            end
        end
    end

    % Detection du cote droite de la plaque
    for j = IMAGE_WIDTH-1 : -1 : THIRD_IMAGE_WIDTH*2
        gx = greenImage(HALF_IMAGE_HEIGHT-1, j-1) ...
            + 2*greenImage(HALF_IMAGE_HEIGHT, j-1) ...
            + greenImage(HALF_IMAGE_HEIGHT+1, j-1) ...
            - greenImage(HALF_IMAGE_HEIGHT-1, j+1) ...
            - greenImage(HALF_IMAGE_HEIGHT, j+1) ...
            - greenImage(HALF_IMAGE_HEIGHT+1, j+1);

        gy = greenImage(HALF_IMAGE_HEIGHT-1, j-1) ...
            + 2*greenImage(HALF_IMAGE_HEIGHT-1, j) ...
            + greenImage(HALF_IMAGE_HEIGHT-1, j+1) ...
            - greenImage(HALF_IMAGE_HEIGHT+1, j-1) ...
            - greenImage(HALF_IMAGE_HEIGHT+1, j) ...
            - greenImage(HALF_IMAGE_HEIGHT+1, j+1);

        g = sqrt(double(gx*gx + gy*gy));

        if g > max(2)*MAX_SCALE
            intensity_X(2) = intensity_X(2)+g;
            max_chosen(2) = g;
            index_X(2) = j;
            if max_chosen(2) > max(2)
                max(2) = max_chosen(2);
            end
        end
    end
    
    diametre_X = index_X(2) - index_X(1);
    diametre_Y = index_Y(2) - index_Y(1);
    
    if diametre_X > diametre_Y * DIAMETER_TOLERENCE
        if intensity_Y(1) > intensity_Y(2)
            index_Y(1) = index_Y(2) - diametre_X;
        else
            index_Y(2) = index_Y(1) + diametre_X;
        end
        diametre_Y = diametre_X;
    elseif diametre_Y > diametre_X * DIAMETER_TOLERENCE
        if intensity_X(1) > intensity_X(2)
            index_X(1) = index_X(2) - diametre_Y;
        else
            index_X(2) = index_X(1) + diametre_Y;
        end
        diametre_X = diametre_Y; 
    end
    
    center_X = (index_X(1) + index_X(2))/2;
    center_Y = (index_Y(1) + index_Y(2))/2;
    
    Coords = [index_X(1) index_Y(1) index_X(2) index_Y(2)];
    Centre = [center_X center_Y];
    Rayon = (diametre_X + diametre_Y) / 4;
end
