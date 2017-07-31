/*
 * Code d'exemple pour charger une image de format rgb raw.
 * Exemple:
 *
 * load_image image.rgb
 *
 */

#include <boost/shared_array.hpp>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;
/*
template <typename Txt, typename Val>
void disp(Txt txt, Val val)
{
	cout << txt << " = " << val << endl;
}

template <typename Txt, typename Val, typename... Args>
void disp(Txt txt, Val val, Args... args)
{
	cout << txt << " = " << val << " :: ";
	disp(args...);
}

int main( int argc, char **argv)
{
	if(argc != 2)
	{
		cerr << "Erreur: Vous devez specifier seulement l'image a charger" << endl;
		return EXIT_FAILURE;
	}

	// Open image file
	ifstream image_file(argv[1], ios::binary);
	if(!image_file)
	{
		cerr << "Erreur: Chemin de l'image invalide" << endl;
		return EXIT_FAILURE+1;
	}

	// Check file size
	image_file.seekg(0, image_file.end);
	if((unsigned int)image_file.tellg() != IMAGE_SIZE)
	{
		cerr << "Erreur: La taille de l'image specifiee est incorrecte" << endl;
		return EXIT_FAILURE+2;
	}
	image_file.seekg(0, image_file.beg);

	// Read file
	boost::shared_array<uint8_t> image(new uint8_t[IMAGE_SIZE]);
	image_file.read(reinterpret_cast<char *>(image.get()), IMAGE_SIZE);

	DummyImageProcessingPlugin dipp;

	Coords coordsPlaque;
	Point<int> center;

	dipp.detectionPlaque(image, coordsPlaque, center);

	int rayonPlaque = (coordsPlaque.x_right - coordsPlaque.x_left + coordsPlaque.y_bottom - coordsPlaque.y_top) / 4;
	int pixPerM = rayonPlaque / RAYON_PLAQUE_M;

	Coords cercleCorr;
	
	dipp.trouverPosCercleCorr(Point<int>(100, 100), Point<double>(0.1, 100), pixPerM, cercleCorr);

	disp("pixPerM", pixPerM);
	disp("xl", cercleCorr.x_left, "xr", cercleCorr.x_right);

	

	dipp.inverseImage(image, Coords(113, 187, 113, 187));

	FILE *imageFile;

	imageFile = fopen("transformedIM.ppm", "wb");
	if (imageFile == NULL) {
		perror("ERROR: Cannot open output file");
		exit(EXIT_FAILURE);
	}

	fprintf(imageFile, "P3\n");
	fprintf(imageFile, "%d %d\n", IMAGE_WIDTH, IMAGE_HEIGHT);   // dimensions
	fprintf(imageFile, "255\n"); // Max pixel

	for (int i = 0; i < IMAGE_SIZE; i+=3) {
		fprintf(imageFile, "%d %d %d\n", image[i], image[i + GREEN], image[i + 2]);
	}
	fclose(imageFile);



	// Votre code ici: l'image est un tableau lineaire de uint8.
	// Chaque pixel contient 3 uint8 soit les composantes: Red, Green, Blue (expliquant le "*3" dans IMAGE_SIZE)
	// Les pixels sont sotckes en mode: row-major order.
	// L'outil convert de imagemagick peut etre interessant pour convertir le format d'image si requis:
	// convert -depth 8 -size 480x480 test.rgb test.png

	//system("pause");
	return EXIT_SUCCESS;
}*/