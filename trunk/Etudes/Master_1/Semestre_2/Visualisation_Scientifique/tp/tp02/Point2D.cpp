#include "Point2D.h"

Point2D::Point2D(void)
{
        x = 0;
        y = 0;
}

Point2D::Point2D(int _x, int _y)
{
        x = _x;
        y = _y;
}

Point2D::~Point2D(void)
{
}

void Point2D::Set(int _x, int _y)
{
        this->x = _x;
        this->y = _y;
}

int Point2D::trouvePosDansListe(vector<Point2D>listePt){
	int res = -1;
	for(int i = 0; i < (int)listePt.size(); i++)
		if((*this) == listePt.at(i))
			res =  i;
	return res;
}

bool Point2D::estPtIntersection(vector<Point2D>lstOrig){
	bool res = true;
	for(int i = 0; i < (int)lstOrig.size(); i++){
		if((*this) == lstOrig.at(i))
			res = false;
	}
	return res;
}


Point2D& Point2D::operator=(const Point2D& p)
{
        this->x = p.x;
        this->y = p.y;
        return *this;
}

bool Point2D::operator ==(const Point2D& p){
    return x==p.x && y==p.y;
}

bool Point2D::operator !=(const Point2D& p){
        return !(*this==p);
}

