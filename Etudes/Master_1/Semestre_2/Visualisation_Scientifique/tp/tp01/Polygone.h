#ifndef POLYGONE_H
#define POLYGONE_H

#include <vector>
#include <math.h>
#include <GL/glut.h>
#include "Point2D.h"

using namespace std;

class Polygone
{
public:
	std::vector<Point2D> PTS;
	std::vector<vector<Point2D> > TROUS;
	int PointInsertion;

	Polygone(void);
	~Polygone(void);

	void ajouterSommet(int _x, int _y);
	int trouverSommet(int _x, int _y);
	void changerPointInsertion(int _x, int _y);
	void supprimerSommet(int _x, int _y);
	bool estInterieur(int _x, int _y);

	void tracerSommets(void);
	void tracerAretes(int _modeAffichage);
};
#endif