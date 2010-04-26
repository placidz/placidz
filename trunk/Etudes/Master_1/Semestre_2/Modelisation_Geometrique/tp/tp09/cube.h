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
	
	point3D potActuel;
	
	cube(); // (0,0,0) par defaut
	cube(vector<face> lst);
	
	void initSommet();
	void setCube(vector<face> lst);
	void polygonise(mlVec3 seed, double radius);
	int getNextFace(int faceCourante, arete ar);
	int getIndiceArete(face f, arete ar);
	int getNextSommet(int SommetCourant);
	bool isParcoursFacesOver();
	double potentialFunction(mlVec3 _coord, mlVec3 seed, double radius);
	point3D getBarycentre(mlVec3 seed, double radius, arete a1, arete a2);
	
	cube&     operator=(const cube &op);
};
#endif
