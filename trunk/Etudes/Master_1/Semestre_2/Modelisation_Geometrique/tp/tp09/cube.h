#ifndef CUBE_H
#define CUBE_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "face.h"

using namespace std;
class cube
{  
	
public:
	//ordre des faces du cube : avant arriere gauche droite haut bas L+R Croix Rond Triangle Start
	vector<face> FaceCube;
	vector<int> markFace;
	vector<point3D> lstPolyCube;
	
	cube(); // (0,0,0) par defaut
	cube(vector<face> lst);
	
	void initSommet();
	void setCube(vector<face> lst);
	void polygonise();
	int getNextFace(int faceCourante, arete ar);
	int getIndiceArete(face f, arete ar);
	int getNextSommet(int SommetCourant);
	bool isParcoursFacesOver();
	
	cube&     operator=(const cube &op);
};
#endif
