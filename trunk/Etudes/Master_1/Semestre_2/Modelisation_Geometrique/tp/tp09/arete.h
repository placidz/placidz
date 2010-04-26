#ifndef ARETE_H
#define ARETE_H

#include <iostream>
#include "point3D.h"

using namespace std ;

class arete
{  
	
public:

	point3D p1;
	point3D p2;
	int isPtInter;
	
	int numSphere;
	double potentiel;
	
	arete(point3D pt); // (0,0,0) par defaut
	arete(point3D _p1, point3D _p2);
	void setArete(point3D _p1, point3D _p2);
	bool estEgale(arete ar);
	bool estComplement(arete ar);
	point3D getCentre();
};
#endif
