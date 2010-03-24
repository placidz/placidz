#ifndef POINT2D_H
#define POINT2D_H

class Point2D
{
public:
	int x;
	int y;

	Point2D(void);
	Point2D(int _x, int _y);
	~Point2D(void);
	
	void Set(int _x, int _y);
	
	Point2D& operator=(const Point2D&);
};
#endif
