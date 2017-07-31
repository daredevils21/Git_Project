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
using namespace std;

namespace
{
	const unsigned int IMAGE_WIDTH = 480;
	const unsigned int IMAGE_HEIGHT = 480;

	const unsigned int IMAGE_WIDTH_X3 = IMAGE_WIDTH * 3;
	const unsigned int IMAGE_HEIGHT_X3 = IMAGE_HEIGHT * 3;

	const unsigned int HALF_IMAGE_WIDTH = IMAGE_WIDTH / 2;
	const unsigned int HALF_IMAGE_HEIGHT = IMAGE_HEIGHT / 2;

	const unsigned int THIRD_IMAGE_WIDTH = IMAGE_WIDTH / 3;
	const unsigned int THIRD_IMAGE_HEIGHT = IMAGE_HEIGHT / 3;

	const unsigned int TWO_THIRD_IMAGE_WIDTH = 2 * THIRD_IMAGE_WIDTH;
	const unsigned int TWO_THIRD_IMAGE_HEIGHT = 2 * THIRD_IMAGE_HEIGHT;

	const unsigned int IMAGE_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * 3;
	const unsigned int GREEN = 1;

	const double MAX_SCALE = 0.5;
	const double DIAMETER_TOLERENCE = 1.05;

	const unsigned int THETA = 15;
	const unsigned int FHZ = 30;

	const double RAYON_PLAQUE_M = 0.0625;

	const int BILLE_WIDTH = 27;
	const int BILLE_HEIGHT = 27;
	const int IMAGE_BALLE_VERTE_INVERSE[] = {
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
}

struct Coords
{
	Coords(int x_left = 0, int x_right = 0, int y_top = 0, int y_bottom = 0)
		: x_left(x_left), x_right(x_right), y_top(y_top), y_bottom(y_bottom)
	{}

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

	void detectionPlaque(const boost::shared_array<uint8_t> in_ptrImage, Coords& out_coords, Point<int>& out_center);

	void trouverPosBille(const boost::shared_array<uint8_t> in_ptrImage, Coords& in_cercleCorr, Point<int>& out_positionBalle);

	void inverseImage(boost::shared_array<uint8_t> in_ptrImage, Coords& in_cercleCorr);

	void trouverPosCercleCorr(Point<int>& in_positionBalle, int in_pixPerM, Coords& out_coords);

	void trouverPosCercleCorr(Point<int>& in_positionBalle, Point<double>& in_vitesse, 
			int in_pixPerM, Coords& out_coords);

private:

	double accelerationMSec;
	double vitesseMaxMSec;
	double pixPerM;
};

DummyImageProcessingPlugin::DummyImageProcessingPlugin()
{
	accelerationMSec = 5 / 7 * 9.81 * sin(THETA);
	double t = sqrt((RAYON_PLAQUE_M * 2) / accelerationMSec);
	vitesseMaxMSec = accelerationMSec * t;
}

DummyImageProcessingPlugin::~DummyImageProcessingPlugin()
{
//Insérez votre code ici
}

void DummyImageProcessingPlugin::OnImage(const boost::shared_array<uint8_t> in_ptrImage, unsigned int in_unWidth, 
		unsigned int in_unHeight, double & out_dXPos, double & out_dYPos)
{
	Coords coordsPlaque;
	Point<int> center;

	detectionPlaque(in_ptrImage, coordsPlaque, center);

	int rayonPlaque = (coordsPlaque.x_right - coordsPlaque.x_left + coordsPlaque.y_bottom - coordsPlaque.y_top) / 4;
	pixPerM = rayonPlaque / RAYON_PLAQUE_M;

	out_dXPos = -1.0;
	out_dYPos = -1.0;
}

void DummyImageProcessingPlugin::OnBallPosition(double in_dXPos, double in_dYPos, double & out_dXDiff, double & out_dYDiff)
{
	//insérez votre code ici
	out_dXDiff = 0.0;
	out_dYDiff = 0.0;
}

void DummyImageProcessingPlugin::detectionPlaque(const boost::shared_array<uint8_t> in_ptrImage, Coords& out_coords, 
		Point<int>& out_center)
{
	int index_X[] = { 0 , 0 };
	int index_Y[] = { 0 , 0 };
	double intensity_X[] = { 0 , 0 };
	double intensity_Y[] = { 0 , 0 };

	int gx = 0;
	int gy = 0;
	double g = 0;
	int efficient_index_minus = 0;
	int efficient_index_plus = 0;

	// Detection du dessus de la plaque

	double max = 0;
	double max_chosen = 0;
	int pos = HALF_IMAGE_WIDTH * 3 + IMAGE_WIDTH_X3 + GREEN;

	for (int j = 1; j < THIRD_IMAGE_HEIGHT; j++)
	{
		efficient_index_minus = pos - 3;
		efficient_index_plus = pos + 3;

		gx = in_ptrImage[efficient_index_minus - IMAGE_WIDTH_X3]
			+ in_ptrImage[efficient_index_minus]
			+ in_ptrImage[efficient_index_minus + IMAGE_WIDTH_X3]
			- in_ptrImage[efficient_index_plus - IMAGE_WIDTH_X3]
			- in_ptrImage[efficient_index_plus]
			- in_ptrImage[efficient_index_plus + IMAGE_WIDTH_X3];

		efficient_index_minus = pos - IMAGE_WIDTH_X3;
		efficient_index_plus = pos + IMAGE_WIDTH_X3;

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
		pos += IMAGE_WIDTH_X3;
	}

	// Detection du dessous de la plaque

	max = 0;
	max_chosen = 0;
	pos = IMAGE_SIZE - HALF_IMAGE_WIDTH * 3 - IMAGE_WIDTH_X3 + GREEN;

	for (int j = IMAGE_HEIGHT - 2; j > TWO_THIRD_IMAGE_HEIGHT; j--)
	{
		efficient_index_minus = pos - 3;
		efficient_index_plus = pos + 3;

		gx = in_ptrImage[efficient_index_minus - IMAGE_WIDTH_X3]
			+ in_ptrImage[efficient_index_minus]
			+ in_ptrImage[efficient_index_minus + IMAGE_WIDTH_X3]
			- in_ptrImage[efficient_index_plus - IMAGE_WIDTH_X3]
			- in_ptrImage[efficient_index_plus]
			- in_ptrImage[efficient_index_plus + IMAGE_WIDTH_X3];

		efficient_index_minus = pos - IMAGE_WIDTH_X3;
		efficient_index_plus = pos + IMAGE_WIDTH_X3;

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
		pos -= IMAGE_WIDTH_X3;
	}

	// Detection du cote gauche de la plaque

	max = 0;
	max_chosen = 0;
	pos = (HALF_IMAGE_HEIGHT - 1) * IMAGE_WIDTH_X3 + 3 + GREEN;

	for (int i = 1; i < THIRD_IMAGE_WIDTH; i++)
	{
		efficient_index_minus = pos - 3;
		efficient_index_plus = pos + 3;

		gx = in_ptrImage[efficient_index_minus - IMAGE_WIDTH_X3]
			+ in_ptrImage[efficient_index_minus]
			+ in_ptrImage[efficient_index_minus + IMAGE_WIDTH_X3]
			- in_ptrImage[efficient_index_plus - IMAGE_WIDTH_X3]
			- in_ptrImage[efficient_index_plus]
			- in_ptrImage[efficient_index_plus + IMAGE_WIDTH_X3];

		efficient_index_minus = pos - IMAGE_WIDTH_X3;
		efficient_index_plus = pos + IMAGE_WIDTH_X3;

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
	pos = HALF_IMAGE_HEIGHT * IMAGE_WIDTH_X3 - 6 + GREEN;

	for (int i = IMAGE_WIDTH - 2; i > TWO_THIRD_IMAGE_WIDTH; i--)
	{
		efficient_index_minus = pos - 3;
		efficient_index_plus = pos + 3;

		gx = in_ptrImage[efficient_index_minus - IMAGE_WIDTH_X3]
			+ in_ptrImage[efficient_index_minus]
			+ in_ptrImage[efficient_index_minus + IMAGE_WIDTH_X3]
			- in_ptrImage[efficient_index_plus - IMAGE_WIDTH_X3]
			- in_ptrImage[efficient_index_plus]
			- in_ptrImage[efficient_index_plus + IMAGE_WIDTH_X3];

		efficient_index_minus = pos - IMAGE_WIDTH_X3;
		efficient_index_plus = pos + IMAGE_WIDTH_X3;

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

void DummyImageProcessingPlugin::trouverPosBille(const boost::shared_array<uint8_t> in_ptrImage, Coords& in_cercleCorr,
	Point<int>& out_positionBalle)
{
	int halfBille = BILLE_HEIGHT / 2;

	Coords rectCoor(in_cercleCorr.x_left - halfBille,
		in_cercleCorr.x_right + halfBille,
		in_cercleCorr.y_top - halfBille,
		in_cercleCorr.y_bottom - halfBille);

	if (rectCoor.x_left < 0)
	{
		in_cercleCorr.x_left -= rectCoor.x_left;
		rectCoor.x_left = 0;
	}
	if (rectCoor.x_right >= IMAGE_WIDTH)
	{
		in_cercleCorr.x_right += IMAGE_WIDTH - rectCoor.x_right - 1 ;
		rectCoor.x_right = IMAGE_WIDTH - 1;
	}
	if (rectCoor.y_top < 0)
	{
		in_cercleCorr.y_top -= rectCoor.y_top;
		rectCoor.y_top = 0;
	}
	if (rectCoor.y_bottom >= IMAGE_HEIGHT)
	{
		in_cercleCorr.y_bottom += IMAGE_HEIGHT - rectCoor.y_bottom - 1;
		rectCoor.y_bottom = IMAGE_HEIGHT - 1;
	}

	inverseImage(in_ptrImage, rectCoor);

	int indexBalle;
	int conditionBilleX;

	const int conditionBilleY = BILLE_HEIGHT * BILLE_WIDTH;

	int lastPos1;
	int lastPos2;
	int lastPos3;
	int pos = in_cercleCorr.y_top * IMAGE_WIDTH_X3 + in_cercleCorr.x_left * 3 + GREEN;
	int conditionX = in_cercleCorr.y_top * IMAGE_WIDTH_X3 + in_cercleCorr.x_right * 3;
	
	const int conditionY = (in_cercleCorr.y_bottom + 1) * IMAGE_WIDTH_X3;
	const int leftCorner = halfBille * (IMAGE_WIDTH_X3 + 3);

	while (pos < conditionY)
	{
		lastPos1 = pos;

		while (pos <= conditionX)
		{
			lastPos2 = pos;
			pos -= leftCorner;
			
			indexBalle = 0;
			conditionBilleX = BILLE_WIDTH;

			int corr = 0;

			while (indexBalle < conditionBilleY)
			{
				lastPos3 = pos;
				while (indexBalle < conditionBilleX)
				{
					corr = in_ptrImage[pos] * IMAGE_BALLE_VERTE_INVERSE[indexBalle];
					indexBalle++;
					pos += 3;
				}
				conditionBilleX += BILLE_WIDTH;
				pos = lastPos3 + IMAGE_WIDTH_X3;
			}
			pos = lastPos2 + 3;
		}
		conditionX += IMAGE_WIDTH_X3;
		pos = lastPos1 + IMAGE_WIDTH_X3;
	}
}

void DummyImageProcessingPlugin::inverseImage(boost::shared_array<uint8_t> in_ptrImage, Coords& in_cercleCorr)
{
	int lastPos;
	int pos = in_cercleCorr.y_top * IMAGE_WIDTH_X3 + in_cercleCorr.x_left * 3 + GREEN;
	int conditionY = (in_cercleCorr.y_bottom + 1) * IMAGE_WIDTH_X3;
	int conditionX = in_cercleCorr.y_top * IMAGE_WIDTH_X3 + in_cercleCorr.x_right * 3;

	while (pos < conditionY)
	{
		lastPos = pos;

		while (pos <= conditionX)
		{
			in_ptrImage[pos] = 255 - in_ptrImage[pos];
			pos += 3;
		}

		conditionX += IMAGE_WIDTH_X3;
		pos = lastPos + IMAGE_WIDTH_X3;
	}
}

void DummyImageProcessingPlugin::trouverPosCercleCorr(Point<int>& in_positionBalle, int in_pixPerM, Coords& out_coords)
{
	int deplacement_max = (vitesseMaxMSec / FHZ + 0.5 * accelerationMSec / (FHZ * FHZ)) * in_pixPerM;
	out_coords.x_left = in_positionBalle.x - deplacement_max;
	out_coords.x_right = in_positionBalle.x + deplacement_max;
	out_coords.y_top = in_positionBalle.y - deplacement_max;
	out_coords.y_bottom = in_positionBalle.y + deplacement_max;
}

void DummyImageProcessingPlugin::trouverPosCercleCorr(Point<int>& in_positionBalle, Point<double>& in_vitesse,
	int in_pixPerM, Coords& out_coords)
{
	out_coords.x_left = in_positionBalle.x + (in_vitesse.x / FHZ - 0.5 * accelerationMSec / (FHZ * FHZ)) * in_pixPerM;
	out_coords.x_right = in_positionBalle.x + (in_vitesse.x / FHZ + 0.5 * accelerationMSec / (FHZ * FHZ)) * in_pixPerM;
	out_coords.y_top = in_positionBalle.y + (in_vitesse.y / FHZ - 0.5 * accelerationMSec / (FHZ * FHZ)) * in_pixPerM;
	out_coords.y_bottom = in_positionBalle.y + (in_vitesse.y / FHZ + 0.5 * accelerationMSec / (FHZ * FHZ)) * in_pixPerM;
}


int main(int argc, char **argv)
{
	if (argc != 2)
	{
		cerr << "Erreur: Vous devez specifier seulement l'image a charger" << endl;
		return EXIT_FAILURE;
	}

	// Open image file
	ifstream image_file(argv[1], ios::binary);
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
	boost::shared_array<uint8_t> image(new uint8_t[IMAGE_SIZE]);
	image_file.read(reinterpret_cast<char *>(image.get()), IMAGE_SIZE);

	DummyImageProcessingPlugin dipp;

	Coords coordsPlaque;
	Point<int> center;

	dipp.detectionPlaque(image, coordsPlaque, center);

	Point<int> posBalle;

	dipp.trouverPosBille(image, Coords(400, 420, 100, 120), posBalle);

	FILE *imageFile;

	imageFile = fopen("transformedIM.ppm", "wb");
	if (imageFile == NULL) {
		perror("ERROR: Cannot open output file");
		exit(EXIT_FAILURE);
	}

	fprintf(imageFile, "P3\n");
	fprintf(imageFile, "%d %d\n", IMAGE_WIDTH, IMAGE_HEIGHT);   // dimensions
	fprintf(imageFile, "255\n"); // Max pixel

	for (int i = 0; i < IMAGE_SIZE; i += 3) {
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