/*
 * dummy_gabarit_version_etudiante.cpp
 *
 *  Created on: Jun 08, 2016
 *  Author: chaj1907, micj1901
 */


//aucun include externe local autre que la librairie boost (déjà installée) ou
//les include standard n'est permis. Tout doit tenir dans ce fichier. 
//Si vous utilisez une librairie externe le code source doit tenir ici.
#include <cstdint>
#include "image_processing_plugin.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

namespace
{
	const int IMAGE_WIDTH = 480;
	const int IMAGE_HEIGHT = 480;
	const int IMAGE_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * 3;

	const int RED = 0, GREEN = 1, BLUE = 2;

	const double MAX_SCALE = 0.5;
	const double DIAMETER_TOLERENCE = 1.05;
	const int FAST_CORR_COEF = 5;

	const int THETA = 15;
	const int FHZ = 30;

	const double RAYON_PLAQUE_M = 0.0625;

	const int BILLE_WIDTH = 27;
	const int BILLE_HEIGHT = 27;
	const int CORR_BILLE = 11996;
	const uint8_t IMAGE_BILLE_VERTE_INVERSE[] = {
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 6 , 1 ,15 , 1 ,14 , 1 , 3 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 ,10 , 6 ,28 ,15 ,27 ,14 , 4 , 3 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 ,58 ,43 ,93 ,70 ,104,74 ,101,70 ,69 ,37 ,33 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 ,73 ,58 ,115,93 ,133,104,130,101,101,69 ,65 ,33 , 1 , 1 , 1 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 1 , 1 , 1 , 98,93 ,178,163,205,193,199,201,196,190,173,136,116,72 ,40 , 1 , 1 , 1 , 0 , 0 , 0 ,
		0 , 0 , 1 , 1 , 1 , 1 ,102,98 ,193,178,217,205,197,199,202,196,210,173,159,116,78 ,40 , 1 , 1 , 1 , 0 , 0 ,
		0 , 0 , 1 , 1 ,95 ,102,199,202,223,228,206,205,189,190,189,200,215,220,188,164,114,76 ,28 , 1 , 1 , 0 , 0 ,
		0 , 1 , 1 , 1 ,88 ,95 ,196,199,217,223,207,206,187,189,178,189,209,215,212,188,150,114,55 ,28 , 1 , 1 , 0 ,
		0 , 1 ,42 ,57 ,168,183,211,206,136,170,68 ,134,84 ,166,106,176,101,200,183,200,166,131,70 ,39 , 8 , 1 , 0 ,
		0 , 1 ,26 ,42 ,153,168,216,211,103,136, 1 ,68 , 1 ,84 ,34 ,106, 1 ,101,165,183,200,166,100,70 ,14 , 8 , 0 ,
		0 , 6 ,79 ,106,198,211,197,168,83 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,162,186,203,184,106,71 ,20 , 4 , 0 ,
		0 , 4 ,52 ,79 ,184,198,226,197,163,83 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,137,162,221,203,139,106,35 ,20 , 1 ,
		2 ,10 ,89 ,127,208,221,223,218,171,141, 1 , 1 ,14 ,27 , 1 , 1 ,58 ,114,209,217,212,203,115,90 ,21 , 6 , 1 ,
		0 , 6 ,50 ,89 ,194,208,228,223,200,171, 1 , 1 , 1 ,14 , 1 , 1 , 1 ,58 ,201,209,221,212,139,115,36 ,21 , 1 ,
		0 ,10 ,82 ,120,194,216,222,220,126,50 , 1 , 1 , 1 , 1 , 1 , 1 ,42 ,83 ,217,223,209,197,97 ,80 ,12 , 1 , 0 ,
		0 , 6 ,44 ,82 ,171,194,224,222,201,126, 1 , 1 , 1 , 1 , 1 , 1 , 1 ,42 ,211,217,220,209,113,97 ,23 ,11 , 0 ,
		0 , 1 ,41 ,80 ,153,198,218,226,201,180,94 , 1 ,104,83 ,83 , 5 ,200,194,230,230,176,160,69 ,55 , 1 , 1 , 0 ,
		0 , 1 , 1 ,41 ,108,153,209,218,221,201,186,94 ,124,104,159,83 ,205,200,230,230,192,176,82 ,69 , 1 , 1 , 0 ,
		0 , 0 , 8 ,22 ,88 ,140,180,213,211,212,200,186,159,137,201,200,226,228,200,201,102,95 ,16 ,15 , 1 , 0 , 0 ,
		0 , 0 , 1 ,12 ,35 ,88 ,147,180,210,211,214,200,181,159,202,201,224,226,199,200,109,102,16 ,16 , 1 , 0 , 0 ,
		0 , 0 , 0 , 1 ,19 ,37 ,84 ,133,155,199,192,214,201,212,201,217,170,186,97 ,106,21 ,19 , 1 , 1 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 1 ,19 ,33 ,84 ,111,155,170,192,190,201,185,201,153,170,88 ,97 ,23 ,21 , 1 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 0 , 1 ,12 ,22 ,38 ,75 ,79 ,113,89 ,121,77 ,102,56 ,70 ,11 ,17 , 1 , 1 , 0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 0 , 0 , 1 ,10 , 1 ,38 ,44 ,79 ,56 ,89 ,51 ,77 ,41 ,56 , 5 , 9 , 1 , 0 , 0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 2 , 9 ,17 ,13 ,25 , 5 , 9 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 5 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };

	const int BILLE_WIDTH_SCALED = 5;
	const int BILLE_HEIGHT_SCALED = 5;
	const uint8_t IMAGE_BILLE_VERTE_INVERSE_SCALED[] = {
		 0 , 7 ,38 ,19 , 0 ,
		15 ,143,169,164,40 ,
		62 ,161, 7 ,85 ,116,
		42 ,180,71 ,149,87 ,
		 1 ,72 ,139,108, 8 };
}

struct Coords
{
	Coords(int x_left = 0, int x_right = 0, int y_top = 0, int y_bottom = 0)
		: x_left(x_left), x_right(x_right), y_top(y_top), y_bottom(y_bottom)
	{}

	void operator+=(int value)
	{
		x_left -= value;
		x_right += value;
		y_top -= value;
		y_bottom += value;
	}

	int x_left;
	int x_right;
	int y_top;
	int y_bottom;
};

template<typename T>
struct Point
{
	Point(T x = 0, T y = 0)
		: x(x), y(y)
	{}

	T x;
	T y;
};

//Modifiez cette classe-ci, vous pouvez faire littéralement ce que vous voulez y compris la renommer
//à condition de faire un "replace all"
//à condition de conserver le constructeur par défaut et aucun autre
//le destructeur virtuel
//et à condition que vous conserviez les 2 fonctions publiques virtuelles telles quelles.
class DummyImageProcessingPlugin : public ImageProcessingPlugin
{
public:
	DummyImageProcessingPlugin(); 			//vous devez utiliser ce constructeur et aucun autre
	virtual ~DummyImageProcessingPlugin();
	
	/*! \brief Receive an image to process.
	 *
	 *  This function will be called every time we need the to send the X,Y position and differentials to
	 *  the **firmware**.
	 *
	 *  \param in_ptrImage Image data.
	 *  \param in_unWidth Image width (= 480).
	 *  \param in_unHeight Image height (= 480).
	 *  \param out_dXPos X coordinate (sub-)pixel position of the ball.
	 *  \param out_dYPos Y coordinate (sub-)pixel position of the ball.
	 *
	 */
	virtual void OnImage(const boost::shared_array<uint8_t> in_ptrImage, unsigned int in_unWidth, unsigned int in_unHeight,
			double & out_dXPos, double & out_dYPos);
			
	/*! \brief Receive an image to process.
	 *
	 *  This function will be called every time we need the to send the X,Y position and differentials to
	 *  the **firmware**.
	 *
	 *  \param in_dXPos X coordinate position of the ball in <arbitrary input units.
	 *  \param in_dYPos Y coordinate position of the ball.
	 *  \param out_dXDiff X speed of the ball in <input units> per second.
	 *  \param out_dYDiff Y speed of the ball in <input units> per second.
	 *
	 */
	virtual void OnBallPosition(double in_dXPos, double in_dYPos, double & out_dXDiff, double & out_dYDiff);

	void detectionPlaque(const boost::shared_array<uint8_t> in_ptrImage, Point<int> in_imageSize, 
		Coords& out_coords, Point<int>& out_center);

	void trouverPosBille(const boost::shared_array<uint8_t> in_ptrImage,
		const Point<int>& in_imageSize, const Point<int>& in_billeSize, const Point<int>& in_centrePlaque,
		int in_rayon, Coords& in_cercleCorr, Point<int>& out_positionBalle);

	void trouverPosBilleDecimated(const boost::shared_array<uint8_t> in_ptrImage,
		const Point<int>& in_imageSize, const Point<int>& in_billeSize, const Point<int>& in_centrePlaque,
		int in_rayon, Coords& in_cercleCorr, Point<int>& out_positionBalle);

	void correlation(const boost::shared_array<uint8_t> in_ptrImage, const uint8_t in_imageBille[],
		const Point<int>& in_imageSize, const Point<int>& in_billeSize, const Point<int>& in_centrePlaque,
		int in_rayon, const Coords& in_cercleCorr, const int in_rgb, Point<int>& out_positionBalle);

	void inverseImage(boost::shared_array<uint8_t> in_ptrImage,
		const Point<int>& in_imageSize, const Coords& in_cercleCorr);

	void decimation(const boost::shared_array<uint8_t> in_ptrImage, 
		const Point<int>& in_imageSize, const Coords& in_cercleCorr, int in_n);

	void trouverPosCercleCorr(const Point<int>& in_positionBalle, Coords& out_coords);

	void trouverPosCercleCorr(const Point<int>& in_positionBalle, const Point<double>& in_vitesse, 
		const Point<int>& in_centrePlaque, int in_rayon, Coords& out_coords);

private:

	Coords prochaineCorr;
	Point<int> lastPos;
	Point<double> lastVitesse;
	double accelerationMSec;
	double vitesseMaxMSec;
	double pixPerM;
	bool isPosKnown;
	bool isCollision;
	bool isDecimatedRequired;
};

DummyImageProcessingPlugin::DummyImageProcessingPlugin()
	: isPosKnown(false), isCollision(false), isDecimatedRequired(false)
{
	accelerationMSec = 5.0 / 7.0 * 9.81 * sin(THETA);
	double t = sqrt((RAYON_PLAQUE_M * 2) / accelerationMSec);
	vitesseMaxMSec = accelerationMSec * t;
}

DummyImageProcessingPlugin::~DummyImageProcessingPlugin()
{

}

void DummyImageProcessingPlugin::OnImage(const boost::shared_array<uint8_t> in_ptrImage, unsigned int in_unWidth, 
		unsigned int in_unHeight, double & out_dXPos, double & out_dYPos)
{
	Coords coordsPlaque;
	Point<int> centrePlaque;
	Point<int> posBille;

	Point<int> sizeImage(in_unWidth, in_unHeight);
	Point<int> sizeBille(BILLE_WIDTH, BILLE_HEIGHT);

	detectionPlaque(in_ptrImage, sizeImage, coordsPlaque, centrePlaque);

	int rayonPlaque = (coordsPlaque.x_right - coordsPlaque.x_left + coordsPlaque.y_bottom - coordsPlaque.y_top) / 4;
	pixPerM = rayonPlaque / RAYON_PLAQUE_M;

	bool isPlaqueScanned = false;
    do
    {
	    if (isPosKnown)
	    {
		    if (isCollision)
		    {
				isCollision = false;
				trouverPosBilleDecimated(in_ptrImage, sizeImage, sizeBille, centrePlaque, rayonPlaque, prochaineCorr, posBille);
				if (posBille.x == -1 && posBille.y == -1)
				{
				    isPosKnown = false;
					prochaineCorr += BILLE_WIDTH / 2;
					inverseImage(in_ptrImage, sizeImage, prochaineCorr);
				}
				else
				{
			        trouverPosCercleCorr(posBille, prochaineCorr);		
					isDecimatedRequired = true;
				}
		    }
		    else
		    {
				if (isDecimatedRequired)
				{
					isDecimatedRequired = false;
					trouverPosBilleDecimated(in_ptrImage, sizeImage, sizeBille, centrePlaque, rayonPlaque, prochaineCorr, posBille);
				}
				else
				{
					trouverPosBille(in_ptrImage, sizeImage, sizeBille, centrePlaque, rayonPlaque, prochaineCorr, posBille);
				}
			    
				if (posBille.x == -1 && posBille.y == -1)
				{
				    isPosKnown = false;
					prochaineCorr += BILLE_WIDTH / 2;
					inverseImage(in_ptrImage, sizeImage, prochaineCorr);
				}
				else
				{	
			        lastVitesse.x = (posBille.x - lastPos.x) * FHZ;
			        lastVitesse.y = (posBille.y - lastPos.y) * FHZ;
			        trouverPosCercleCorr(posBille, lastVitesse, centrePlaque, rayonPlaque, prochaineCorr);			
				}
		    }
	    }
	    else
	    {
			trouverPosBilleDecimated(in_ptrImage, sizeImage, sizeBille, centrePlaque, rayonPlaque, coordsPlaque, posBille);
			isPlaqueScanned = true;

			if (posBille.x != -1 && posBille.y != -1)
			{
				trouverPosCercleCorr(posBille, prochaineCorr);
				isPosKnown = true;
				isDecimatedRequired = true;
			}
	    }
	}
	while (!isPlaqueScanned && !isPosKnown);
	
	lastPos = posBille;

	out_dXPos = posBille.x;
	out_dYPos = posBille.y;
}

void DummyImageProcessingPlugin::OnBallPosition(double in_dXPos, double in_dYPos, double & out_dXDiff, double & out_dYDiff)
{
	out_dXDiff = lastVitesse.x;
	out_dYDiff = lastVitesse.y;
}

void DummyImageProcessingPlugin::detectionPlaque(const boost::shared_array<uint8_t> in_ptrImage, Point<int> in_imageSize,
	Coords& out_coords, Point<int>& out_center)
{
	const unsigned int imageWidthX3 = in_imageSize.x * 3;
	const unsigned int halfImageHeight = in_imageSize.y / 2;
	const unsigned int thirdImageWidth = in_imageSize.y / 3;
	const unsigned int thirdImageHeight = in_imageSize.y / 3;
	const unsigned int twoThirdImageWidth = thirdImageWidth * 2;
	const unsigned int twoThirdImageHeight = thirdImageHeight * 2;
	const unsigned int imageSize = imageWidthX3 * in_imageSize.y;
	
	int index_X[] = { 0 , 0 };
	int index_Y[] = { 0 , 0 };
	double intensity_X[] = { 0 , 0 };
	double intensity_Y[] = { 0 , 0 };

	int gx = 0, gy = 0;
	double g = 0;
	unsigned int efficient_index_minus = 0;
	unsigned int efficient_index_plus = 0;

	// Detection du dessus de la plaque

	double max = 0;
	double max_chosen = 0;
	unsigned int pos = imageWidthX3 * 1.5 + GREEN;

	for (unsigned int j = 1; j < thirdImageHeight; j++)
	{
		efficient_index_minus = pos - 3;
		efficient_index_plus = pos + 3;

		gx = in_ptrImage[efficient_index_minus - imageWidthX3]
			+ in_ptrImage[efficient_index_minus]
			+ in_ptrImage[efficient_index_minus + imageWidthX3]
			- in_ptrImage[efficient_index_plus - imageWidthX3]
			- in_ptrImage[efficient_index_plus]
			- in_ptrImage[efficient_index_plus + imageWidthX3];

		efficient_index_minus = pos - imageWidthX3;
		efficient_index_plus = pos + imageWidthX3;

		gy = in_ptrImage[efficient_index_minus - 3]
			+ in_ptrImage[efficient_index_minus]
			+ in_ptrImage[efficient_index_minus + 3]
			- in_ptrImage[efficient_index_plus - 3]
			- in_ptrImage[efficient_index_plus]
			- in_ptrImage[efficient_index_plus + 3];

		g = sqrt(gx*gx + gy*gy);

		if (g > max * MAX_SCALE)
		{
			intensity_Y[0] += g;
			max_chosen = g;
			index_Y[0] = j;
			if (max_chosen > max)
			{
				max = max_chosen;
			}
		}
		pos += imageWidthX3;
	}

	// Detection du dessous de la plaque

	max = 0;
	max_chosen = 0;
	pos = imageSize - imageWidthX3 * 1.5 + GREEN;

	for (unsigned int j = in_imageSize.y - 2; j > twoThirdImageHeight; j--)
	{
		efficient_index_minus = pos - 3;
		efficient_index_plus = pos + 3;

		gx = in_ptrImage[efficient_index_minus - imageWidthX3]
			+ in_ptrImage[efficient_index_minus]
			+ in_ptrImage[efficient_index_minus + imageWidthX3]
			- in_ptrImage[efficient_index_plus - imageWidthX3]
			- in_ptrImage[efficient_index_plus]
			- in_ptrImage[efficient_index_plus + imageWidthX3];

		efficient_index_minus = pos - imageWidthX3;
		efficient_index_plus = pos + imageWidthX3;

		gy = in_ptrImage[efficient_index_minus - 3]
			+ in_ptrImage[efficient_index_minus]
			+ in_ptrImage[efficient_index_minus + 3]
			- in_ptrImage[efficient_index_plus - 3]
			- in_ptrImage[efficient_index_plus]
			- in_ptrImage[efficient_index_plus + 3];

		g = sqrt(gx*gx + gy*gy);

		if (g > max * MAX_SCALE)
		{
			intensity_Y[1] += g;
			max_chosen = g;
			index_Y[1] = j;
			if (max_chosen > max)
			{
				max = max_chosen;
			}
		}
		pos -= imageWidthX3;
	}

	// Detection du cote gauche de la plaque

	max = 0;
	max_chosen = 0;
	pos = (halfImageHeight - 1) * imageWidthX3 + 3 + GREEN;

	for (unsigned int i = 1; i < thirdImageWidth; i++)
	{
		efficient_index_minus = pos - 3;
		efficient_index_plus = pos + 3;

		gx = in_ptrImage[efficient_index_minus - imageWidthX3]
			+ in_ptrImage[efficient_index_minus]
			+ in_ptrImage[efficient_index_minus + imageWidthX3]
			- in_ptrImage[efficient_index_plus - imageWidthX3]
			- in_ptrImage[efficient_index_plus]
			- in_ptrImage[efficient_index_plus + imageWidthX3];

		efficient_index_minus = pos - imageWidthX3;
		efficient_index_plus = pos + imageWidthX3;

		gy = in_ptrImage[efficient_index_minus - 3]
			+ in_ptrImage[efficient_index_minus]
			+ in_ptrImage[efficient_index_minus + 3]
			- in_ptrImage[efficient_index_plus - 3]
			- in_ptrImage[efficient_index_plus]
			- in_ptrImage[efficient_index_plus + 3];

		g = sqrt(gx*gx + gy*gy);

		if (g > max * MAX_SCALE)
		{
			intensity_X[0] += g;
			max_chosen = g;
			index_X[0] = i;
			if (max_chosen > max)
			{
				max = max_chosen;
			}
		}
		pos += 3;
	}

	// Detection du cote droit de la plaque

	max = 0;
	max_chosen = 0;
	pos = halfImageHeight * imageWidthX3 - 6 + GREEN;

	for (unsigned int i = in_imageSize.x - 2; i > twoThirdImageWidth; i--)
	{
		efficient_index_minus = pos - 3;
		efficient_index_plus = pos + 3;

		gx = in_ptrImage[efficient_index_minus - imageWidthX3]
			+ in_ptrImage[efficient_index_minus]
			+ in_ptrImage[efficient_index_minus + imageWidthX3]
			- in_ptrImage[efficient_index_plus - imageWidthX3]
			- in_ptrImage[efficient_index_plus]
			- in_ptrImage[efficient_index_plus + imageWidthX3];

		efficient_index_minus = pos - imageWidthX3;
		efficient_index_plus = pos + imageWidthX3;

		gy = in_ptrImage[efficient_index_minus - 3]
			+ in_ptrImage[efficient_index_minus]
			+ in_ptrImage[efficient_index_minus + 3]
			- in_ptrImage[efficient_index_plus - 3]
			- in_ptrImage[efficient_index_plus]
			- in_ptrImage[efficient_index_plus + 3];

		g = sqrt(gx*gx + gy*gy);

		if (g > max * MAX_SCALE)
		{
			intensity_X[1] += g;
			max_chosen = g;
			index_X[1] = i;
			if (max_chosen > max)
			{
				max = max_chosen;
			}
		}
		pos -= 3;
	}

	int diametre_X = index_X[1] - index_X[0];
	int diametre_Y = index_Y[1] - index_Y[0];

	if (diametre_X > diametre_Y * DIAMETER_TOLERENCE)
	{
		if (intensity_Y[0] > intensity_Y[1])
		{
			index_Y[0] = index_Y[1] - diametre_X;
		}
		else
		{
			index_Y[1] = index_Y[0] + diametre_X;
		}
		diametre_Y = diametre_X;
	}
	else if (diametre_Y > diametre_X * DIAMETER_TOLERENCE)
	{
		if (intensity_X[0] > intensity_X[1])
		{
			index_X[0] = index_X[1] - diametre_Y;
		}
		else
		{
			index_X[1] = index_X[0] + diametre_Y;
		}
		diametre_X = diametre_Y;
	}

	out_center.x = (index_X[0] + index_X[1]) / 2;
	out_center.y = (index_Y[0] + index_Y[1]) / 2;

	out_coords.x_left = index_X[0];
	out_coords.x_right = index_X[1];
	out_coords.y_top = index_Y[0];
	out_coords.y_bottom = index_Y[1];
}

void DummyImageProcessingPlugin::trouverPosBille(boost::shared_array<uint8_t> in_ptrImage, 
	const Point<int>& in_imageSize, const Point<int>& in_billeSize, const Point<int>& in_centrePlaque, 
	int in_rayon, Coords& in_cercleCorr, Point<int>& out_positionBalle)
{
	const int halfBille = in_billeSize.y / 2;

	Coords rectCorr(in_cercleCorr);
	rectCorr += halfBille;

	if (rectCorr.x_left < 0)
	{
		in_cercleCorr.x_left -= rectCorr.x_left;
		rectCorr.x_left = 0;
	}
	if (rectCorr.x_right >= in_imageSize.x)
	{
		in_cercleCorr.x_right += in_imageSize.x - rectCorr.x_right - 1;
		rectCorr.x_right = in_imageSize.x - 1;
	}
	if (rectCorr.y_top < 0)
	{
		in_cercleCorr.y_top -= rectCorr.y_top;
		rectCorr.y_top = 0;
	}
	if (rectCorr.y_bottom >= in_imageSize.y)
	{
		in_cercleCorr.y_bottom += in_imageSize.y - rectCorr.y_bottom - 1;
		rectCorr.y_bottom = in_imageSize.y - 1;
	}

	inverseImage(in_ptrImage, in_imageSize, rectCorr);

	correlation(in_ptrImage, IMAGE_BILLE_VERTE_INVERSE, in_imageSize, in_billeSize, 
		in_centrePlaque, in_rayon, in_cercleCorr, GREEN, out_positionBalle);
}

void DummyImageProcessingPlugin::trouverPosBilleDecimated(boost::shared_array<uint8_t> in_ptrImage,
	const Point<int>& in_imageSize, const Point<int>& in_billeSize, const Point<int>& in_centrePlaque,
	int in_rayon, Coords& in_cercleCorr, Point<int>& out_positionBalle)
{
	const int halfBilleNorm = in_billeSize.y / (2 * FAST_CORR_COEF);

	int dx = (in_cercleCorr.x_right - in_cercleCorr.x_left + 1) % FAST_CORR_COEF;
	int dy = (in_cercleCorr.y_bottom - in_cercleCorr.y_top + 1) % FAST_CORR_COEF;

	if (dx != 0)
	{
		in_cercleCorr.x_right += FAST_CORR_COEF - dx;
	}
	if (dy != 0)
	{
		in_cercleCorr.y_bottom += FAST_CORR_COEF - dy;
	}

	Coords rectCorr(in_cercleCorr);
	rectCorr += FAST_CORR_COEF * halfBilleNorm;

	if (rectCorr.x_left < 0)
	{
		in_cercleCorr.x_left -= rectCorr.x_left;
		in_cercleCorr.x_right -= rectCorr.x_left;
		rectCorr.x_right -= rectCorr.x_left;
		rectCorr.x_left = 0;
	}
	else if (rectCorr.x_right >= in_imageSize.x)
	{
		in_cercleCorr.x_right += in_imageSize.x - rectCorr.x_right - 1;
		in_cercleCorr.x_left += in_imageSize.x - rectCorr.x_right - 1;
		rectCorr.x_left += in_imageSize.x - rectCorr.x_right - 1;
		rectCorr.x_right = in_imageSize.x - 1;
	}
	else
	{
		int mod5x = rectCorr.x_left % 5;
		if (mod5x != 0)
		{
			in_cercleCorr.x_right += FAST_CORR_COEF - mod5x;
			rectCorr.x_right += FAST_CORR_COEF - mod5x;
			in_cercleCorr.x_left += FAST_CORR_COEF - mod5x;
			rectCorr.x_left += FAST_CORR_COEF - mod5x;
		}
	}

	if (rectCorr.y_top < 0)
	{
		in_cercleCorr.y_top -= rectCorr.y_top;
		in_cercleCorr.y_bottom -= rectCorr.y_top;
		rectCorr.y_bottom -= rectCorr.y_top;
		rectCorr.y_top = 0;
	}
	else if (rectCorr.y_bottom >= in_imageSize.y)
	{
		in_cercleCorr.y_bottom += in_imageSize.y - rectCorr.y_bottom - 1;
		in_cercleCorr.y_top += in_imageSize.y - rectCorr.y_bottom - 1;
		rectCorr.y_top += in_imageSize.y - rectCorr.y_bottom - 1;
		rectCorr.y_bottom = in_imageSize.y - 1;
	}
	else
	{
		int mod5y = rectCorr.y_top % 5;
		if (mod5y != 0)
		{
			in_cercleCorr.y_top += FAST_CORR_COEF - mod5y;
			rectCorr.y_top += FAST_CORR_COEF - mod5y;
			in_cercleCorr.y_bottom += FAST_CORR_COEF - mod5y;
			rectCorr.y_bottom += FAST_CORR_COEF - mod5y;
		}
	}

	inverseImage(in_ptrImage, in_imageSize, rectCorr);

	decimation(in_ptrImage, in_imageSize, rectCorr, FAST_CORR_COEF);

	correlation(in_ptrImage, IMAGE_BILLE_VERTE_INVERSE_SCALED,
		Point<int>(in_imageSize.x / FAST_CORR_COEF, in_imageSize.y / FAST_CORR_COEF),
		Point<int>(BILLE_WIDTH_SCALED, BILLE_HEIGHT_SCALED),
		Point<int>(in_centrePlaque.x / FAST_CORR_COEF, in_centrePlaque.y / FAST_CORR_COEF),
		in_rayon / FAST_CORR_COEF, 
		Coords(in_cercleCorr.x_left / FAST_CORR_COEF, in_cercleCorr.x_right / FAST_CORR_COEF, 
			in_cercleCorr.y_top / FAST_CORR_COEF, in_cercleCorr.y_bottom / FAST_CORR_COEF), RED, out_positionBalle);

	out_positionBalle.x *= FAST_CORR_COEF;
	out_positionBalle.y *= FAST_CORR_COEF;

	correlation(in_ptrImage, IMAGE_BILLE_VERTE_INVERSE, in_imageSize, in_billeSize, in_centrePlaque, in_rayon,
		Coords(out_positionBalle.x - FAST_CORR_COEF, out_positionBalle.x + FAST_CORR_COEF,
			out_positionBalle.y - FAST_CORR_COEF, out_positionBalle.y + FAST_CORR_COEF), GREEN, out_positionBalle);
}

void DummyImageProcessingPlugin::correlation(const boost::shared_array<uint8_t> in_ptrImage, const uint8_t in_imageBille[],
	const Point<int>& in_imageSize, const Point<int>& in_billeSize, const Point<int>& in_centrePlaque,
	int in_rayon, const Coords& in_cercleCorr, const int in_rgb, Point<int>& out_positionBalle)
{
	const int halfBille = in_billeSize.y / 2;
	const int imageWidthX3 = in_imageSize.x * 3;
	const int rayonBilleMoinsPlaque = in_billeSize.y - in_rayon;
	
	unsigned int indexBalle;
	unsigned int conditionBilleX;

	const unsigned int conditionBilleY = in_billeSize.y * in_billeSize.x;

	unsigned int lastPos1, lastPos2, lastPos3;
	unsigned int pos = in_cercleCorr.y_top * imageWidthX3 + in_cercleCorr.x_left * 3 + in_rgb;
	unsigned int conditionX = in_cercleCorr.y_top * imageWidthX3 + in_cercleCorr.x_right * 3;
	
	const unsigned int conditionY = (in_cercleCorr.y_bottom + 1) * imageWidthX3;
	const unsigned int leftCorner = halfBille * (imageWidthX3 + 3);

	unsigned int max = 0, corr = 0;
	double scale = 0;
	unsigned int indexX = 0, indexY = 0;
	unsigned int dx = 0, dy = 0, d = 0, dMax = 0;

	while (pos < conditionY)
	{
		lastPos1 = pos;

		while (pos <= conditionX)
		{
			lastPos2 = pos;
			pos -= leftCorner;
			indexBalle = 0;
			conditionBilleX = in_billeSize.x;

			corr = 0;

			while (indexBalle < conditionBilleY)
			{
				lastPos3 = pos;
				while (indexBalle < conditionBilleX)
				{
					corr += in_ptrImage[pos] * in_imageBille[indexBalle];
					indexBalle++;
					pos += 3;
				}
				conditionBilleX += in_billeSize.x;
				pos = lastPos3 + imageWidthX3;
			}

			pos = lastPos2 + 3;

			if (max < corr)
			{
				indexX = pos / 3;
				indexY = indexX / in_imageSize.y;
				indexX = indexX % in_imageSize.x;
				dy = indexY - in_centrePlaque.y;
				dx = indexX - in_centrePlaque.x;
				d = sqrt(dy*dy + dx*dx);

				if (d < in_rayon)
				{
					if (in_rgb == RED)
					{
						if (d + in_billeSize.x > in_rayon)
						{
							scale = (d + rayonBilleMoinsPlaque) / (in_billeSize.x / 2.0);
							if (max < corr / scale)
							{
								max = corr / scale;
								dMax = d;
								out_positionBalle.x = indexX;
								out_positionBalle.y = indexY;
							}
						}
						else
						{
							max = corr;
							dMax = d;
							out_positionBalle.x = indexX;
							out_positionBalle.y = indexY;
						}
					}
					else
					{
						max = corr;
						dMax = d;
						out_positionBalle.x = indexX;
						out_positionBalle.y = indexY;
					}
				}
			}
			pos += 3;
		}
		conditionX += imageWidthX3;
		pos = lastPos1 + imageWidthX3;
	}

	max /= (in_billeSize.x * in_billeSize.y);
	double rate = max / double(CORR_BILLE);

	if (rate < 0.7 && in_rgb == GREEN)
	{
		out_positionBalle.x = -1;
		out_positionBalle.y = -1;
	}
}

void DummyImageProcessingPlugin::inverseImage(boost::shared_array<uint8_t> in_ptrImage, 
		const Point<int>& in_imageSize, const Coords& in_cercleCorr)
{
	const int imageWidthX3 = in_imageSize.x * 3;
	
	unsigned int lastPos;
	unsigned int pos = in_cercleCorr.y_top * imageWidthX3 + in_cercleCorr.x_left * 3 + GREEN;
	unsigned int conditionY = (in_cercleCorr.y_bottom + 1) * imageWidthX3;
	unsigned int conditionX = in_cercleCorr.y_top * imageWidthX3 + (in_cercleCorr.x_right + 1) * 3;

	int moyenne = 0;

	while (pos < conditionY)
	{
		lastPos = pos;

		while (pos < conditionX)
		{
			in_ptrImage[pos] = 255 - in_ptrImage[pos];
			//moyenne += in_ptrImage[pos];
			pos += 3;
		}

		conditionX += imageWidthX3;
		pos = lastPos + imageWidthX3;
	}

	//int corrSize = (in_cercleCorr.x_right - in_cercleCorr.x_left) * (in_cercleCorr.y_bottom - in_cercleCorr.y_top);
	//moyenne /= corrSize;
	//int moyBille = SOMME_BILLE / corrSize;
	//cout << "Moyenne = " << moyenne << endl;
	//cout << "Moyenne Bille = " << moyBille << endl;
}

void DummyImageProcessingPlugin::decimation(const boost::shared_array<uint8_t> in_ptrImage, 
	const Point<int>& in_imageSize,	const Coords& in_cercleCorr, int in_n)
{
	const int imageWidthX3 = in_imageSize.x * 3;
	const Point<int> outputSize((in_cercleCorr.x_right - in_cercleCorr.x_left + 1) * 3 / in_n, 
		(in_cercleCorr.y_bottom - in_cercleCorr.y_top + 1) / in_n);

	unsigned int lastPos;
	
	unsigned int pos = in_cercleCorr.y_top * imageWidthX3 + in_cercleCorr.x_left * 3 + GREEN;
	unsigned int conditionY = (in_cercleCorr.y_bottom + 1) * imageWidthX3;
	unsigned int conditionX = in_cercleCorr.y_top * imageWidthX3 + (in_cercleCorr.x_right + 1) * 3;

	unsigned int index = (in_cercleCorr.y_top / in_n) * (imageWidthX3 / in_n) + 3*(in_cercleCorr.x_left/in_n) , lastIndex;
	unsigned int i = 0, j = 0;
	while (pos < conditionY)
	{
		lastIndex = index;
		lastPos = pos;
		while (pos < conditionX)
		{
			if (i == 0 && j == 0)
			{
				in_ptrImage[index] = 0;
			}
			
			in_ptrImage[index] += in_ptrImage[pos] / (in_n * in_n);
			pos += 3;
			i++;
			if (i / in_n == 1)
			{
				index += 3;
				i = 0;
			}
		}
		j++;
		index = lastIndex;
		if (j / in_n == 1)
		{
			index += imageWidthX3 / in_n;
			j = 0;
		}
		conditionX += imageWidthX3;
		pos = lastPos + imageWidthX3;
	}
}

void DummyImageProcessingPlugin::trouverPosCercleCorr(const Point<int>& in_positionBalle, Coords& out_coords)
{
	int deplacement_max = (vitesseMaxMSec / FHZ + 0.5 * accelerationMSec / (FHZ * FHZ)) * pixPerM;
	out_coords.x_left = in_positionBalle.x - deplacement_max;
	out_coords.x_right = in_positionBalle.x + deplacement_max;
	out_coords.y_top = in_positionBalle.y - deplacement_max;
	out_coords.y_bottom = in_positionBalle.y + deplacement_max;
}

void DummyImageProcessingPlugin::trouverPosCercleCorr(const Point<int>& in_positionBalle, const Point<double>& in_vitesse, 
		const Point<int>& in_centrePlaque, int in_rayon,  Coords& out_coords)
{
	double accelerationWeight = 0.5 * accelerationMSec * pixPerM / (FHZ * FHZ);

	int centerX = in_positionBalle.x + in_vitesse.x / FHZ;
	int centerY = in_positionBalle.y + in_vitesse.y / FHZ;
	double distanceX = centerX - in_centrePlaque.x;
	double distanceY = centerY - in_centrePlaque.y;

	double distanceCentre = sqrt(distanceX*distanceX + distanceY*distanceY);

	if (distanceCentre <= in_rayon)
	{
		out_coords.x_left = centerX - accelerationWeight;
		out_coords.x_right = centerX + accelerationWeight;
		out_coords.y_top = centerY - accelerationWeight;
		out_coords.y_bottom = centerY + accelerationWeight;
	}
	else
	{
		trouverPosCercleCorr(in_positionBalle, out_coords);
		isCollision = true;
	}
}

int getImage(string& argv, boost::shared_array<uint8_t> image)
{
	// Open image file
	
	ifstream image_file(argv, ios::binary);
	if (!image_file)
	{
		cerr << "Erreur: Chemin de l'image invalide" << endl;
		return EXIT_FAILURE + 1;
	}

	// Check file size
	image_file.seekg(0, image_file.end);
	if ((unsigned int)image_file.tellg() != IMAGE_SIZE)
	{
		cerr << "Erreur: La taille de l'image specifiee est incorrecte" << endl;
		return EXIT_FAILURE + 2;
	}
	image_file.seekg(0, image_file.beg);

	// Read file
	image_file.read(reinterpret_cast<char *>(image.get()), IMAGE_SIZE);
	
	return 0 ;
}

int main(int argc, char **argv)
{
	string path = "C:\\Users\\mat_8\\Desktop\\S4\\Projet\\git\\detection\\images\\images_test\\images_test_c++\\rgb\\";

	Point<double> posBalle;
	Point<double> vitBalle;
	DummyImageProcessingPlugin dipp;
	boost::shared_array<uint8_t> image(new uint8_t[IMAGE_SIZE]);
	
	string fullPath;
	string imageName;
	
	auto start = std::chrono::high_resolution_clock::now();

	// operation to be timed ...
	
	auto finish = std::chrono::high_resolution_clock::now();

	for (int i = 1; i <= 192; i++)
	{
	    if ( i < 10 )
	    {
	        imageName = "image_00" + std::to_string(i);
	    }
	    else if ( i < 100 )
	    {
	        imageName = "image_0" + std::to_string(i);
	    }
	    else
	    {
	        imageName = "image_" + std::to_string(i);
	    }
	    
		fullPath = path + imageName + ".rgb";
		getImage(fullPath, image);

		start = std::chrono::high_resolution_clock::now();

		dipp.OnImage(image, IMAGE_WIDTH, IMAGE_HEIGHT, posBalle.x, posBalle.y);
		dipp.OnBallPosition(posBalle.x, posBalle.y, vitBalle.x, vitBalle.y);

		finish = std::chrono::high_resolution_clock::now();

		cout << i << ".\t" << imageName << "\t P: (" << posBalle.x << ", " << posBalle.y << ")\tV: (" 
			 << vitBalle.x << ", " << vitBalle.y << ")\tTemps : " 
			 << chrono::duration_cast<chrono::nanoseconds>(finish - start).count() / 1000000.0 << "ms" << endl;
	}
	return EXIT_SUCCESS;
}

//ne rien modifier passé ce commentaire
//ne rien modifier passé ce commentaire
//ne rien modifier passé ce commentaire
//ne rien modifier passé ce commentaire
//ne rien modifier passé ce commentaire
//ne rien modifier passé ce commentaire
//ne rien modifier passé ce commentaire
//ne rien modifier passé ce commentaire

extern "C"
{
	ImageProcessingPlugin * Load();
	void Unload( ImageProcessingPlugin * in_pPlugin );
}

void Unload( ImageProcessingPlugin * in_pPlugin )
{
	delete in_pPlugin;
}

ImageProcessingPlugin * Load()
{
	//si vous changez le nom de la classe asssurez-vous de le changer aussi ci-dessous
	return new DummyImageProcessingPlugin;
}
