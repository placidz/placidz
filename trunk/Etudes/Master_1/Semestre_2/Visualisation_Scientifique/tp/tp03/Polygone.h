#ifndef POLYGONE_H
#define POLYGONE_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <limits.h>
#include <algorithm>
//#include "glut.h"
#include <GL/glut.h>
#include "Point2D.h"
#include "tools.h"

class Polygone
{
public:
	std::vector<Point2D> PTS;
	std::vector<Polygone> TROUS;
	
	int PointInsertion;
	bool bCreationPolygone;
	bool bEnMouvement;
	bool bColoration;

	Polygone(void);
	~Polygone(void);
	
	void initialisation(void);
	void ajouterSommet(int _x, int _y);
	int trouverSommet(int _x, int _y);
	void changerPointInsertion(int _x, int _y);
	void supprimerSommet(int _x, int _y);
	bool estInterieur(int _x, int _y);
	void deplacer(int _x, int _y);
	void colorer(float _r, float _g, float _b);
	
	void vider(void);

	// Méthodes de rendu
	void tracerSommets(void);
	void tracerAretes(int _modeAffichage);
	void tracerTrous(int _modeAffichage);
};
#endif
