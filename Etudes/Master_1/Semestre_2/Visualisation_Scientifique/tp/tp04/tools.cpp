#include "tools.h"

Point2D computeIntersection(Point2D a, Point2D b, Point2D c, Point2D d)
{
	int x1 = a.x;
	int y1 = a.y;
	int x2 = b.x;
	int y2 = b.y;
	int x3 = c.x;
	int y3 = c.y;
	int x4 = d.x;
	int y4 = d.y;
	
	float supx = (x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4);
	float sub = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
	float supy = (x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4);

	Point2D I( (int)(supx/sub) , (int)(supy/sub));
	return I;
}

void sortByAscOrder(int *t, int size)
{
	bool en_desordre = true;
	for (int i = 0; i < size && en_desordre; ++i)
	{
		en_desordre = false;
		for (int j = 1; j < size - i; ++j)
			if (t[j-1] > t[j])
			{
				std::swap(t[j], t[j-1]);
				en_desordre = true;
 			}
	}
}

void sortByDescOrder(int *t, int size)
{
	bool en_desordre = true;
	for (int i = 0; i < size && en_desordre; ++i)
	{
		en_desordre = false;
		for (int j = 1; j < size - i; ++j)
			if (t[j-1] < t[j])
			{
				std::swap(t[j], t[j-1]);
				en_desordre = true;
 			}
	}
}

void displayText(float x, float y, float z, void* font, const char* s)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glRasterPos2f(x, y);
	while(*s)
	{
		glutBitmapCharacter(font, *s);
		s++;
	}
}
