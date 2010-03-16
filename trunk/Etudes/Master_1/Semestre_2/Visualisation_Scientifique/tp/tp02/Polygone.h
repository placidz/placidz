#ifndef POLYGONE_H
#define POLYGONE_H

#include <vector>
#include <iostream>
#include <math.h>
//#include "glut.h"
#include <GL/glut.h>
#include "Point2D.h"

using namespace std;

class Polygone
{
public:
	vector<Point2D> PTS;
	vector<Polygone> TROUS;
	
	vector<Point2D> NPTS;
	
	int PointInsertion;
	bool bCreationPolygone;
	bool bEnMouvement;

	Polygone(void);
	~Polygone(void);
	
	void initialisation(void);
	void ajouterSommet(int _x, int _y);
	int trouverSommet(int _x, int _y);
	void changerPointInsertion(int _x, int _y);
	void supprimerSommet(int _x, int _y);
	bool estInterieur(int _x, int _y);
	void deplacer(int _x, int _y);
	vector<Point2D> estIntersecte(Polygone poly);
	vector<Point2D> intersection(Polygone poly);
	
	void vider(void);

	// Méthodes de rendu
	void tracerSommets(void);
	void tracerAretes(int _modeAffichage);
	void tracerTrous(int _modeAffichage);
};
#endif
