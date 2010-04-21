#ifndef FACE_H
#define FACE_H

#include "arete.h"
#include <vector>
class face
{  
	
public:

	vector<arete> lstArete;
	vector<face> lstFace;
	vector<bool> markSegment;
	vector<bool> markSommet;//vrai ou faux selon si le sommet est a l'interieur ou a l'exterieur de l'objet
	
	face(); // (0,0,0) par defaut
	face(arete _a1, arete _a2, arete _a3, arete _a4);
	
	void initialiseMarkeur();
	void setFace(vector<arete> lst);
	void setFace(arete _a1, arete _a2, arete _a3, arete _a4);
	bool isParcoursSegmentOver();
};
#endif
