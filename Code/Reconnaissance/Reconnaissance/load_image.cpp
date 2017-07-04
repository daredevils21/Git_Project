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

namespace {

	const unsigned int IMAGE_WIDTH = 480;
	const unsigned int IMAGE_HEIGHT = 480;
	const unsigned int IMAGE_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * 3;
	const unsigned int RED = 0;
	const unsigned int GREEN = 1;
	const unsigned int BLUE = 2;
};

unsigned int bluePic[IMAGE_SIZE];

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

	// Aller chercher un pixel
	image[(4 * IMAGE_WIDTH + 14) * 3 + BLUE];



	// Create blue image
	for (int i = 0; i < IMAGE_SIZE; i+=3) {
		bluePic[i + RED] = 0;
		bluePic[i + GREEN] = 0;
		bluePic[i + BLUE] = image[i + BLUE];
	}

	FILE *imageFile;
	unsigned int red;
	unsigned int green;
	unsigned int blue;

	imageFile = fopen("bluePic.ppm", "wb");
	if (imageFile == NULL) {
		perror("ERROR: Cannot open output file");
		exit(EXIT_FAILURE);
	}

	fprintf(imageFile, "P3\n");               // P6 filetype
	fprintf(imageFile, "%d %d\n", IMAGE_WIDTH, IMAGE_HEIGHT);   // dimensions
	fprintf(imageFile, "255\n");              // Max pixel

	for (int i = 0; i < IMAGE_SIZE; i+=3) {
		fprintf(imageFile, "%d %d %d\n", /*image[i + RED]*/ 0, /*bluePic[i + GREEN]*/ 0, image[i + BLUE]);
	}
	fclose(imageFile);



	// Votre code ici: l'image est un tableau lineaire de uint8.
	// Chaque pixel contient 3 uint8 soit les composantes: Red, Green, Blue (expliquant le "*3" dans IMAGE_SIZE)
	// Les pixels sont sotckes en mode: row-major order.
	// L'outil convert de imagemagick peut etre interessant pour convertir le format d'image si requis:
	// convert -depth 8 -size 480x480 test.rgb test.png

	int index = 0;

	//for (int i = 0; i < IMAGE_HEIGHT; i++) {
	//	for (int j = 0; j < IMAGE_WIDTH; j++) {
	//		index = i * IMAGE_WIDTH + j * 3;
	//		pixels[i][j].r = (int)image[index];
	//		pixels[i][j].g = (int)image[index + 1];
	//		pixels[i][j].b = (int)image[index + 2];
	//	}
	//}

	//system("pause");
	return EXIT_SUCCESS;
}