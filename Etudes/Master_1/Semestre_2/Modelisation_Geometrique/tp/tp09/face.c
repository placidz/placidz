#include "face.h"
face::face()
{
	lstArete.clear();
}

face::face(arete _a1, arete _a2, arete _a3, arete _a4)
{
	lstArete.push_back(_a1);
	lstArete.push_back(_a2);
	lstArete.push_back(_a3);
	lstArete.push_back(_a4);
}

void face::initialiseMarkeur(){
	this->markSegment.clear();
	for(int i = 0; i < 4; i++)
		this->markSegment.push_back(false);
}

void face::setFace(vector<arete> lst)
{
	for(int i = 0; i < (int)lst.size(); i++){
		lstArete.push_back(lst.at(i));
	}
}

void face::setFace(arete _a1, arete _a2, arete _a3, arete _a4){
	lstArete.push_back(_a1);
	lstArete.push_back(_a2);
	lstArete.push_back(_a3);
	lstArete.push_back(_a4);
	
	lstFace.push_back((*this));	
}

bool face::isParcoursSegmentOver(){
	int tmp = 0;
	bool res = false;
	for(int i = 0; i < (int)markSegment.size(); i++){
		if(markSegment.at(i) == true)
			tmp++;
	}
	if(tmp == (int)markSegment.size())
		res = true;
		
	return res;
}







