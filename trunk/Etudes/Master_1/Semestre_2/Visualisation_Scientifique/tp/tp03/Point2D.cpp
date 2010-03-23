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

Point2D& Point2D::operator=(const Point2D& p)
{
	this->x = p.x;
	this->y = p.y;
	return *this;
}
