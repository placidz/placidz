#include "arete.h"

arete::arete(point3D pt)
{
	p1 = pt;
	p2 = pt;
	isPtInter = -1;
}

arete::arete(point3D _p1, point3D _p2)
{
	p1 = _p1;
	p2 = _p2;
	isPtInter = -1;
}

void arete::setArete(point3D _p1, point3D _p2){
	p1 = _p1;
	p2 = _p2;
}

bool arete::estEgale(arete ar){
	bool res = false;
	if(this->p1 == ar.p1 && this->p2 == ar.p2){
		res = true;
	}
	return res;
}

bool arete::estComplement(arete ar){
	bool res = false;
	if(this->p1 == ar.p2 && this->p2 == ar.p1){
		res = true;
	}
	return res;
}

point3D arete::getCentre(){
	point3D res((p1.x+p2.x)/2, (p1.y+p2.y)/2, (p1.z+p2.z)/2);
	return res;
}
