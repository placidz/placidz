#ifndef POINT2D_H
#define POINT2D_H

#include <iostream>
#include <vector>

using namespace std;

class Point2D
{
public:
        int x;
        int y;

        Point2D(void);
        Point2D(int _x, int _y);
        ~Point2D(void);
        
        void Set(int _x, int _y);
		int trouvePosDansListe(vector<Point2D>listePt);
		bool estPtIntersection(vector<Point2D>lstOrig);
        
        Point2D& operator=(const Point2D&);
		bool operator ==(const Point2D&);
        bool operator !=(const Point2D&);

};
#endif
