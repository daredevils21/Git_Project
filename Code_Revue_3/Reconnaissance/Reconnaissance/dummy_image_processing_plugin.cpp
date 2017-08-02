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
#include <direct.h>
#define GetCurrentDir _getcwd

using namespace std;
namespace
{
	const int IMAGE_WIDTH = 480;
	const int IMAGE_HEIGHT = 480;

	const int IMAGE_WIDTH_X3 = IMAGE_WIDTH * 3;
	const int IMAGE_HEIGHT_X3 = IMAGE_HEIGHT * 3;

	const int HALF_IMAGE_WIDTH = IMAGE_WIDTH / 2;
	const int HALF_IMAGE_HEIGHT = IMAGE_HEIGHT / 2;

	const int THIRD_IMAGE_WIDTH = IMAGE_WIDTH / 3;
	const int THIRD_IMAGE_HEIGHT = IMAGE_HEIGHT / 3;

	const int TWO_THIRD_IMAGE_WIDTH = 2 * THIRD_IMAGE_WIDTH;
	const int TWO_THIRD_IMAGE_HEIGHT = 2 * THIRD_IMAGE_HEIGHT;

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

	void trouverPosBille(const boost::shared_array<uint8_t> in_ptrImage,
		const Point<int>& in_imageSize, const Point<int>& in_billeSize, const Point<int>& in_centrePlaque,
		int in_rayon, Coords& in_cercleCorr, Point<int>& out_positionBalle);

	void trouverPosBille2(const boost::shared_array<uint8_t> in_ptrImage,
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
//Insérez votre code ici
}

void DummyImageProcessingPlugin::OnImage(const boost::shared_array<uint8_t> in_ptrImage, unsigned int in_unWidth, 
		unsigned int in_unHeight, double & out_dXPos, double & out_dYPos)
{
	Coords coordsPlaque;
	Point<int> centrePlaque;
	Point<int> posBille;

	detectionPlaque(in_ptrImage, coordsPlaque, centrePlaque);

	int rayonPlaque = (coordsPlaque.x_right - coordsPlaque.x_left + coordsPlaque.y_bottom - coordsPlaque.y_top) / 4;
	pixPerM = rayonPlaque / RAYON_PLAQUE_M;

	if (isPosKnown)
	{
		if (isCollision)
		{
			isCollision = false;
			trouverPosBille2(in_ptrImage, Point<int>(in_unWidth, in_unHeight), Point<int>(BILLE_WIDTH, BILLE_HEIGHT),
				centrePlaque, rayonPlaque, prochaineCorr, posBille);
			trouverPosCercleCorr(posBille, prochaineCorr);
		}
		else if (isDecimatedRequired)
		{
			isDecimatedRequired = false;
			trouverPosBille2(in_ptrImage, Point<int>(in_unWidth, in_unHeight), Point<int>(BILLE_WIDTH, BILLE_HEIGHT),
				centrePlaque, rayonPlaque, prochaineCorr, posBille);
			lastVitesse.x = (posBille.x - lastPos.x) * FHZ;
			lastVitesse.y = (posBille.y - lastPos.y) * FHZ;
			trouverPosCercleCorr(posBille, lastVitesse, centrePlaque, rayonPlaque, prochaineCorr);
		}
		else
		{
			trouverPosBille(in_ptrImage, Point<int>(in_unWidth, in_unHeight), Point<int>(BILLE_WIDTH, BILLE_HEIGHT),
				centrePlaque, rayonPlaque, prochaineCorr, posBille);
			lastVitesse.x = (posBille.x - lastPos.x) * FHZ;
			lastVitesse.y = (posBille.y - lastPos.y) * FHZ;
			trouverPosCercleCorr(posBille, lastVitesse, centrePlaque, rayonPlaque, prochaineCorr);
		}
	}
	else
	{
		trouverPosBille2(in_ptrImage, Point<int>(in_unWidth, in_unHeight), Point<int>(BILLE_WIDTH, BILLE_HEIGHT),
			centrePlaque, rayonPlaque, coordsPlaque, posBille);
		trouverPosCercleCorr(posBille, prochaineCorr);
		isPosKnown = true;
	}
	lastPos = posBille;

	out_dXPos = posBille.x;
	out_dYPos = posBille.y;
}

void DummyImageProcessingPlugin::OnBallPosition(double in_dXPos, double in_dYPos, double & out_dXDiff, double & out_dYDiff)
{
	if (isPosKnown)
	{

	}
	else
	{
		lastPos.x = in_dXPos;
		lastPos.y = in_dYPos;
	}

	out_dXDiff = lastVitesse.x;
	out_dYDiff = lastVitesse.y;
}

void DummyImageProcessingPlugin::detectionPlaque(const boost::shared_array<uint8_t> in_ptrImage, Coords& out_coords, 
		Point<int>& out_center)
{
	int index_X[] = { 0 , 0 };
	int index_Y[] = { 0 , 0 };
	double intensity_X[] = { 0 , 0 };
	double intensity_Y[] = { 0 , 0 };

	int gx = 0, gy = 0;
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

void DummyImageProcessingPlugin::trouverPosBille(boost::shared_array<uint8_t> in_ptrImage, 
	const Point<int>& in_imageSize, const Point<int>& in_billeSize, const Point<int>& in_centrePlaque, 
	int in_rayon, Coords& in_cercleCorr, Point<int>& out_positionBalle)
{
	const int halfBille = in_billeSize.y / 2;

	Coords rectCorr(in_cercleCorr.x_left - halfBille,
		in_cercleCorr.x_right + halfBille,
		in_cercleCorr.y_top - halfBille,
		in_cercleCorr.y_bottom + halfBille);

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

void DummyImageProcessingPlugin::trouverPosBille2(boost::shared_array<uint8_t> in_ptrImage,
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

	Coords rectCorr(in_cercleCorr.x_left - FAST_CORR_COEF * halfBilleNorm,
		in_cercleCorr.x_right + FAST_CORR_COEF * halfBilleNorm,
		in_cercleCorr.y_top - FAST_CORR_COEF * halfBilleNorm,
		in_cercleCorr.y_bottom + FAST_CORR_COEF * halfBilleNorm);

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
	const int rayonBillePlaque = in_billeSize.y + in_rayon;
	
	int indexBalle;
	int conditionBilleX;

	const int conditionBilleY = in_billeSize.y * in_billeSize.x;

	int lastPos1, lastPos2, lastPos3;
	int pos = in_cercleCorr.y_top * imageWidthX3 + in_cercleCorr.x_left * 3 + in_rgb;
	int conditionX = in_cercleCorr.y_top * imageWidthX3 + in_cercleCorr.x_right * 3;
	
	const int conditionY = (in_cercleCorr.y_bottom + 1) * imageWidthX3;
	const int leftCorner = halfBille * (imageWidthX3 + 3);

	int max = 0, scale = 0;
	int indexX = 0, indexY = 0;
	int dx = 0, dy = 0;

	while (pos < conditionY)
	{
		lastPos1 = pos;

		while (pos <= conditionX)
		{
			lastPos2 = pos;
			pos -= leftCorner;
			indexBalle = 0;
			conditionBilleX = in_billeSize.x;

			int corr = 0;

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
				int d = sqrt(dy*dy + dx*dx);

				if (d < in_rayon)
				{
					if (in_rgb == RED)
					{
						if (d + in_billeSize.x > in_rayon)
						{
							scale = (d - rayonBillePlaque) / halfBille;
							if (max < corr / scale)
							{
								max = corr / scale;
								out_positionBalle.x = indexX;
								out_positionBalle.y = indexY;
							}
						}
						else
						{
							max = corr;
							out_positionBalle.x = indexX;
							out_positionBalle.y = indexY;
						}
					}
					else
					{
						max = corr;
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
}

void DummyImageProcessingPlugin::inverseImage(boost::shared_array<uint8_t> in_ptrImage, 
		const Point<int>& in_imageSize, const Coords& in_cercleCorr)
{
	const int imageWidthX3 = in_imageSize.x * 3;
	
	int lastPos;
	int pos = in_cercleCorr.y_top * imageWidthX3 + in_cercleCorr.x_left * 3 + GREEN;
	int conditionY = (in_cercleCorr.y_bottom + 1) * imageWidthX3;
	int conditionX = in_cercleCorr.y_top * imageWidthX3 + (in_cercleCorr.x_right + 1) * 3;

	while (pos < conditionY)
	{
		lastPos = pos;

		while (pos < conditionX)
		{
			in_ptrImage[pos] = 255 - in_ptrImage[pos];
			pos += 3;
		}

		conditionX += imageWidthX3;
		pos = lastPos + imageWidthX3;
	}
}

void DummyImageProcessingPlugin::decimation(const boost::shared_array<uint8_t> in_ptrImage, 
	const Point<int>& in_imageSize,	const Coords& in_cercleCorr, int in_n)
{
	const int imageWidthX3 = in_imageSize.x * 3;
	const Point<int> outputSize((in_cercleCorr.x_right - in_cercleCorr.x_left + 1) * 3 / in_n, 
		(in_cercleCorr.y_bottom - in_cercleCorr.y_top + 1) / in_n);

	int lastPos;
	
	int pos = in_cercleCorr.y_top * imageWidthX3 + in_cercleCorr.x_left * 3 + GREEN;
	int conditionY = (in_cercleCorr.y_bottom + 1) * imageWidthX3;
	int conditionX = in_cercleCorr.y_top * imageWidthX3 + (in_cercleCorr.x_right + 1) * 3;

	int index = (in_cercleCorr.y_top / in_n) * (imageWidthX3 / in_n) + 3*(in_cercleCorr.x_left/in_n) , lastIndex;
	int i = 0, j = 0;
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
	isDecimatedRequired = true;
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
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		cerr << "Erreur: Vous devez specifier seulement l'image a charger" << endl;
		return EXIT_FAILURE;
	}

	char cCurrentPath[FILENAME_MAX];

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		return errno;
	}

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	//string folder = "asservissement_actif";
	//string images[] = { "image_0.rgb", "image_21.rgb", "image_53.rgb", "image_85.rgb", "image_122.rgb", "image_155.rgb", "image_186.rgb", "image_219.rgb", "image_252.rgb", "image_285.rgb", "image_319.rgb", "image_352.rgb", "image_386.rgb", "image_419.rgb", "image_451.rgb", "image_485.rgb", "image_519.rgb", "image_552.rgb", "image_585.rgb", "image_619.rgb", "image_652.rgb", "image_685.rgb", "image_718.rgb", "image_751.rgb", "image_786.rgb", "image_818.rgb", "image_851.rgb", "image_885.rgb", "image_919.rgb", "image_952.rgb", "image_985.rgb", "image_1018.rgb", "image_1052.rgb", "image_1085.rgb", "image_1118.rgb", "image_1152.rgb", "image_1185.rgb", "image_1219.rgb", "image_1252.rgb", "image_1285.rgb", "image_1318.rgb", "image_1352.rgb", "image_1385.rgb", "image_1418.rgb", "image_1452.rgb", "image_1485.rgb", "image_1518.rgb", "image_1552.rgb", "image_1585.rgb", "image_1618.rgb", "image_1651.rgb", "image_1685.rgb", "image_1718.rgb", "image_1752.rgb", "image_1785.rgb", "image_1818.rgb", "image_1852.rgb", "image_1885.rgb", "image_1918.rgb", "image_1952.rgb", "image_1985.rgb", "image_2019.rgb", "image_2052.rgb", "image_2085.rgb", "image_2118.rgb", "image_2152.rgb", "image_2185.rgb", "image_2218.rgb", "image_2252.rgb", "image_2285.rgb", "image_2318.rgb", "image_2352.rgb", "image_2385.rgb", "image_2418.rgb", "image_2452.rgb", "image_2485.rgb", "image_2518.rgb", "image_2551.rgb", "image_2585.rgb", "image_2619.rgb", "image_2652.rgb", "image_2685.rgb", "image_2718.rgb", "image_2752.rgb", "image_2785.rgb", "image_2819.rgb", "image_2852.rgb", "image_2885.rgb", "image_2918.rgb", "image_2952.rgb", "image_2985.rgb", "image_3019.rgb", "image_3052.rgb", "image_3085.rgb", "image_3119.rgb", "image_3152.rgb", "image_3185.rgb", "image_3218.rgb", "image_3252.rgb", "image_3285.rgb", "image_3318.rgb", "image_3351.rgb", "image_3385.rgb", "image_3418.rgb", "image_3452.rgb", "image_3485.rgb", "image_3518.rgb", "image_3552.rgb", "image_3585.rgb", "image_3618.rgb", "image_3651.rgb", "image_3685.rgb", "image_3719.rgb", "image_3752.rgb", "image_3785.rgb", "image_3818.rgb", "image_3852.rgb", "image_3885.rgb", "image_3918.rgb", "image_3951.rgb", "image_3985.rgb", "image_4018.rgb", "image_4051.rgb", "image_4085.rgb", "image_4118.rgb", "image_4152.rgb", "image_4185.rgb", "image_4218.rgb", "image_4251.rgb", "image_4285.rgb", "image_4318.rgb", "image_4352.rgb", "image_4385.rgb", "image_4418.rgb", "image_4452.rgb", "image_4485.rgb", "image_4518.rgb", "image_4551.rgb", "image_4585.rgb", "image_4618.rgb", "image_4652.rgb", "image_4685.rgb", "image_4718.rgb", "image_4752.rgb", "image_4785.rgb", "image_4818.rgb", "image_4851.rgb", "image_4885.rgb", "image_4918.rgb", "image_4952.rgb", "image_4985.rgb", "image_5018.rgb", "image_5051.rgb", "image_5085.rgb", "image_5118.rgb", "image_5151.rgb", "image_5185.rgb", "image_5218.rgb", "image_5252.rgb", "image_5285.rgb", "image_5318.rgb", "image_5351.rgb", "image_5385.rgb", "image_5418.rgb", "image_5452.rgb", "image_5485.rgb", "image_5518.rgb", "image_5552.rgb", "image_5585.rgb", "image_5618.rgb", "image_5651.rgb", "image_5685.rgb", "image_5718.rgb", "image_5751.rgb", "image_5785.rgb", "image_5818.rgb", "image_5851.rgb", "image_5885.rgb", "image_5918.rgb", "image_5951.rgb", "image_5985.rgb", "image_6018.rgb", "image_6051.rgb", "image_6084.rgb", "image_6118.rgb", "image_6152.rgb", "image_6185.rgb", "image_6218.rgb", "image_6251.rgb", "image_6285.rgb", "image_6318.rgb" };

	//string folder = "statique_zmax_version_1";
	//string images[] = { "image_0.rgb", "image_20.rgb", "image_53.rgb", "image_86.rgb", "image_121.rgb", "image_156.rgb", "image_186.rgb", "image_219.rgb", "image_252.rgb", "image_285.rgb", "image_318.rgb", "image_352.rgb", "image_385.rgb", "image_419.rgb", "image_451.rgb", "image_485.rgb", "image_518.rgb", "image_552.rgb", "image_585.rgb", "image_618.rgb", "image_652.rgb", "image_685.rgb", "image_718.rgb", "image_752.rgb", "image_785.rgb", "image_818.rgb", "image_851.rgb", "image_885.rgb", "image_918.rgb", "image_952.rgb", "image_985.rgb", "image_1018.rgb", "image_1052.rgb", "image_1084.rgb", "image_1118.rgb", "image_1152.rgb", "image_1185.rgb", "image_1218.rgb", "image_1251.rgb", "image_1285.rgb", "image_1318.rgb", "image_1352.rgb", "image_1385.rgb", "image_1418.rgb", "image_1451.rgb", "image_1485.rgb", "image_1518.rgb", "image_1551.rgb", "image_1585.rgb", "image_1618.rgb", "image_1651.rgb", "image_1684.rgb", "image_1718.rgb", "image_1751.rgb", "image_1785.rgb", "image_1818.rgb", "image_1851.rgb", "image_1884.rgb", "image_1918.rgb", "image_1951.rgb", "image_1985.rgb", "image_2018.rgb", "image_2051.rgb", "image_2084.rgb", "image_2118.rgb", "image_2152.rgb", "image_2185.rgb", "image_2218.rgb", "image_2251.rgb", "image_2284.rgb", "image_2318.rgb", "image_2351.rgb", "image_2384.rgb", "image_2418.rgb", "image_2452.rgb", "image_2485.rgb", "image_2518.rgb", "image_2551.rgb", "image_2584.rgb", "image_2618.rgb", "image_2651.rgb", "image_2684.rgb", "image_2718.rgb" };

	//string folder = "statique_zmax_version_2";
	//string images[] = { "image_0.rgb", "image_20.rgb", "image_54.rgb", "image_85.rgb", "image_119.rgb", "image_156.rgb", "image_187.rgb", "image_221.rgb", "image_253.rgb", "image_287.rgb", "image_320.rgb", "image_353.rgb", "image_386.rgb", "image_420.rgb", "image_454.rgb", "image_487.rgb", "image_520.rgb", "image_553.rgb", "image_586.rgb", "image_619.rgb", "image_653.rgb", "image_686.rgb", "image_719.rgb", "image_753.rgb", "image_786.rgb", "image_820.rgb", "image_853.rgb", "image_886.rgb", "image_919.rgb", "image_953.rgb", "image_986.rgb", "image_1020.rgb", "image_1053.rgb", "image_1086.rgb", "image_1119.rgb", "image_1153.rgb", "image_1186.rgb", "image_1219.rgb", "image_1252.rgb", "image_1286.rgb", "image_1320.rgb", "image_1353.rgb", "image_1386.rgb", "image_1419.rgb", "image_1452.rgb", "image_1487.rgb", "image_1519.rgb", "image_1553.rgb", "image_1586.rgb", "image_1619.rgb", "image_1653.rgb", "image_1686.rgb", "image_1719.rgb", "image_1752.rgb", "image_1786.rgb", "image_1819.rgb", "image_1853.rgb", "image_1886.rgb", "image_1919.rgb", "image_1952.rgb", "image_1986.rgb", "image_2019.rgb", "image_2053.rgb", "image_2086.rgb", "image_2119.rgb", "image_2153.rgb", "image_2186.rgb", "image_2219.rgb", "image_2252.rgb", "image_2286.rgb", "image_2320.rgb", "image_2353.rgb", "image_2386.rgb", "image_2419.rgb", "image_2453.rgb", "image_2487.rgb" };

	//string folder = "statique_zmax_version_3";
	//string images[] = { "image_0.rgb", "image_21.rgb", "image_53.rgb", "image_85.rgb", "image_123.rgb", "image_155.rgb", "image_186.rgb", "image_219.rgb", "image_252.rgb", "image_285.rgb", "image_319.rgb", "image_352.rgb", "image_386.rgb", "image_418.rgb", "image_451.rgb", "image_485.rgb", "image_519.rgb", "image_552.rgb", "image_585.rgb", "image_619.rgb", "image_652.rgb", "image_685.rgb", "image_718.rgb", "image_752.rgb", "image_785.rgb", "image_818.rgb", "image_851.rgb", "image_885.rgb", "image_918.rgb", "image_952.rgb", "image_985.rgb", "image_1018.rgb", "image_1052.rgb", "image_1086.rgb", "image_1119.rgb", "image_1152.rgb", "image_1185.rgb", "image_1219.rgb", "image_1252.rgb", "image_1285.rgb", "image_1319.rgb", "image_1352.rgb", "image_1385.rgb", "image_1418.rgb", "image_1452.rgb", "image_1485.rgb", "image_1518.rgb", "image_1552.rgb", "image_1585.rgb", "image_1618.rgb", "image_1652.rgb", "image_1685.rgb", "image_1718.rgb", "image_1751.rgb", "image_1785.rgb", "image_1818.rgb", "image_1852.rgb", "image_1885.rgb", "image_1918.rgb", "image_1952.rgb", "image_1985.rgb", "image_2018.rgb", "image_2051.rgb", "image_2085.rgb", "image_2118.rgb", "image_2152.rgb", "image_2185.rgb", "image_2218.rgb", "image_2252.rgb", "image_2285.rgb", "image_2318.rgb", "image_2352.rgb", "image_2385.rgb", "image_2418.rgb", "image_2452.rgb" };

	//string folder = "statique_zmin_version_1";
	//string images[] = { "image_0.rgb", "image_19.rgb", "image_53.rgb", "image_88.rgb", "image_123.rgb", "image_155.rgb", "image_186.rgb", "image_218.rgb", "image_251.rgb", "image_285.rgb", "image_318.rgb", "image_351.rgb", "image_385.rgb", "image_419.rgb", "image_451.rgb", "image_485.rgb", "image_518.rgb", "image_552.rgb", "image_585.rgb", "image_618.rgb", "image_651.rgb", "image_685.rgb", "image_718.rgb", "image_751.rgb", "image_785.rgb", "image_818.rgb", "image_851.rgb", "image_884.rgb", "image_918.rgb", "image_951.rgb", "image_985.rgb", "image_1018.rgb", "image_1051.rgb", "image_1084.rgb", "image_1118.rgb", "image_1151.rgb", "image_1184.rgb", "image_1217.rgb", "image_1251.rgb", "image_1285.rgb", "image_1317.rgb", "image_1351.rgb", "image_1384.rgb", "image_1418.rgb", "image_1451.rgb", "image_1485.rgb", "image_1518.rgb", "image_1551.rgb", "image_1584.rgb", "image_1618.rgb", "image_1651.rgb", "image_1685.rgb", "image_1718.rgb", "image_1751.rgb", "image_1784.rgb", "image_1817.rgb", "image_1851.rgb", "image_1884.rgb", "image_1918.rgb", "image_1951.rgb", "image_1984.rgb", "image_2017.rgb", "image_2051.rgb", "image_2084.rgb", "image_2117.rgb", "image_2151.rgb" };

	//string folder = "statique_zmin_version_2";
	//string images[] = { "image_0.rgb", "image_21.rgb", "image_53.rgb", "image_86.rgb", "image_124.rgb", "image_154.rgb", "image_186.rgb", "image_219.rgb", "image_252.rgb", "image_285.rgb", "image_319.rgb", "image_352.rgb", "image_386.rgb", "image_421.rgb", "image_452.rgb", "image_485.rgb", "image_519.rgb", "image_552.rgb", "image_585.rgb", "image_619.rgb", "image_652.rgb", "image_685.rgb", "image_719.rgb", "image_752.rgb", "image_785.rgb", "image_819.rgb", "image_852.rgb", "image_886.rgb", "image_919.rgb", "image_952.rgb", "image_985.rgb", "image_1019.rgb", "image_1052.rgb", "image_1085.rgb", "image_1119.rgb", "image_1152.rgb", "image_1185.rgb", "image_1219.rgb", "image_1252.rgb", "image_1285.rgb", "image_1319.rgb", "image_1352.rgb", "image_1385.rgb", "image_1419.rgb", "image_1452.rgb", "image_1485.rgb", "image_1519.rgb", "image_1552.rgb", "image_1585.rgb", "image_1619.rgb", "image_1652.rgb", "image_1685.rgb", "image_1718.rgb", "image_1752.rgb", "image_1785.rgb", "image_1818.rgb", "image_1852.rgb", "image_1885.rgb", "image_1918.rgb", "image_1952.rgb", "image_1985.rgb", "image_2019.rgb", "image_2052.rgb", "image_2085.rgb", "image_2118.rgb", "image_2152.rgb", "image_2185.rgb", "image_2218.rgb", "image_2252.rgb", "image_2285.rgb", "image_2318.rgb", "image_2352.rgb", "image_2385.rgb", "image_2418.rgb", "image_2452.rgb", "image_2485.rgb" };

	//string folder = "statique_zmin_version_3";
	//string images[] = { "image_0.rgb", "image_22.rgb", "image_53.rgb", "image_86.rgb", "image_126.rgb", "image_158.rgb", "image_187.rgb", "image_220.rgb", "image_253.rgb", "image_286.rgb", "image_320.rgb", "image_353.rgb", "image_386.rgb", "image_420.rgb", "image_453.rgb", "image_486.rgb", "image_519.rgb", "image_553.rgb", "image_586.rgb", "image_619.rgb", "image_652.rgb", "image_686.rgb", "image_719.rgb", "image_752.rgb", "image_785.rgb", "image_819.rgb", "image_852.rgb", "image_885.rgb", "image_919.rgb", "image_952.rgb", "image_985.rgb", "image_1019.rgb", "image_1052.rgb", "image_1085.rgb", "image_1119.rgb", "image_1152.rgb", "image_1186.rgb", "image_1219.rgb", "image_1252.rgb", "image_1285.rgb", "image_1319.rgb", "image_1352.rgb", "image_1385.rgb", "image_1418.rgb", "image_1452.rgb", "image_1485.rgb", "image_1519.rgb", "image_1552.rgb", "image_1585.rgb", "image_1619.rgb", "image_1652.rgb", "image_1685.rgb", "image_1718.rgb", "image_1752.rgb", "image_1786.rgb", "image_1819.rgb", "image_1852.rgb", "image_1886.rgb", "image_1919.rgb", "image_1952.rgb", "image_1986.rgb", "image_2019.rgb", "image_2052.rgb", "image_2085.rgb", "image_2119.rgb", "image_2152.rgb", "image_2186.rgb", "image_2219.rgb", "image_2252.rgb", "image_2286.rgb", "image_2319.rgb", "image_2353.rgb", "image_2385.rgb" };

	//string folder = "vitesse_max_version_1";
	//string images[] = { "image_718.rgb", "image_751.rgb", "image_785.rgb", "image_818.rgb", "image_852.rgb", "image_884.rgb", "image_918.rgb", "image_951.rgb", "image_985.rgb", "image_1018.rgb", "image_1051.rgb", "image_1084.rgb", "image_1118.rgb", "image_1151.rgb", "image_1184.rgb", "image_1218.rgb" };

	string folder = "vitesse_max_version_2";
	string images[] = { "image_752.rgb", "image_786.rgb", "image_820.rgb", "image_853.rgb", "image_886.rgb", "image_919.rgb", "image_952.rgb", "image_986.rgb", "image_1019.rgb", "image_1053.rgb", "image_1086.rgb", "image_1120.rgb", "image_1152.rgb", "image_1186.rgb", "image_1219.rgb", "image_1253.rgb", "image_1286.rgb", "image_1319.rgb", "image_1353.rgb" };

	//string folder = "vitesse_max_version_3";
	//string images[] = { "image_652.rgb", "image_686.rgb", "image_720.rgb", "image_753.rgb", "image_786.rgb", "image_820.rgb", "image_853.rgb", "image_886.rgb", "image_920.rgb", "image_953.rgb", "image_987.rgb", "image_1020.rgb", "image_1053.rgb", "image_1086.rgb", "image_1120.rgb", "image_1153.rgb", "image_1186.rgb", "image_1219.rgb", "image_1253.rgb", "image_1286.rgb", "image_1319.rgb", "image_1353.rgb", "image_1386.rgb", "image_1420.rgb", "image_1453.rgb", "image_1486.rgb", "image_1519.rgb", "image_1553.rgb", "image_1586.rgb", "image_1619.rgb", "image_1653.rgb", "image_1686.rgb", "image_1720.rgb", "image_1753.rgb", "image_1786.rgb", "image_1819.rgb", "image_1853.rgb", "image_1887.rgb", "image_1919.rgb", "image_1952.rgb" };


	string path = (string)cCurrentPath + "\\..\\..\\..\\images\\" + folder + "\\raw\\";

	Point<double> posBalle;
	Point<double> vitBalle;
	DummyImageProcessingPlugin dipp;
	boost::shared_array<uint8_t> image(new uint8_t[IMAGE_SIZE]);
	
	string fullPath;

	auto start = std::chrono::high_resolution_clock::now();

	// operation to be timed ...
	
	auto finish = std::chrono::high_resolution_clock::now();

	cout << folder << endl;
	int arraySize = (sizeof(images) / sizeof(*images));
	for (int i = 0; i < arraySize; i++)
	{
		fullPath = path + images[i];
		getImage(fullPath, image);

		start = std::chrono::high_resolution_clock::now();

		dipp.OnImage(image, IMAGE_WIDTH, IMAGE_HEIGHT, posBalle.x, posBalle.y);
		dipp.OnBallPosition(posBalle.x, posBalle.y, vitBalle.x, vitBalle.y);

		finish = std::chrono::high_resolution_clock::now();

		cout << i << ".\t" << images[i] << "\t P: (" << posBalle.x << ", " << posBalle.y << ")\tV: (" 
			 << vitBalle.x << ", " << vitBalle.y << ")\tTemps : " 
			 << chrono::duration_cast<chrono::nanoseconds>(finish - start).count() / 1000000.0 << "ms" << endl;
	}
	//boost::shared_array<uint8_t> myImage(new uint8_t[4*4*3]);
	//boost::shared_array<uint8_t> myImageOut(new uint8_t[2*2*3]);
	/*
	boost::shared_array<uint8_t> myImageOut(new uint8_t[25*3]);

	for (int j = 0; j < FAST_CORR_COEF; j++)
	{
		for (int i = 0; i < FAST_CORR_COEF * 3; i++)
		{
			myImageOut[i + j * FAST_CORR_COEF * 3] = 0;
		}
	}

	uint8_t TABLEAU[27*27*3];
	for (int j = 0; j < 27; j++)
	{
		for (int i = 0; i < 27 * 3; i++)
		{
			if (i % 3 == 0 || i % 3 == 2)
			{
				TABLEAU[i + j * 27*3] = 0;
			}
			if (i % 3 == 1)
			{
				TABLEAU[i + j * 27 * 3] = IMAGE_BILLE_VERTE_INVERSE[i/3 + j * 27];
			}
			cout << int(TABLEAU[i + j * 27 * 3]) << ":";
		}
		cout << endl << endl;;
	}

	dipp.decimation(TABLEAU, Point<int>(27, 27), Coords(0, 24, 0, 24), FAST_CORR_COEF, myImageOut);

	for (int j = 0; j < FAST_CORR_COEF; j++)
	{
		for (int i = 0; i < FAST_CORR_COEF * 3; i++)
		{
			cout << int(myImageOut[i + j * FAST_CORR_COEF * 3]) << " ";
		}
		cout << endl;
	}
	
	/*
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
	*/

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