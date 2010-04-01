#include "Point2D.h"
#include <algorithm>
#include <math.h>
#include <GL/glut.h>

Point2D computeIntersection(Point2D a, Point2D b, Point2D c, Point2D d);
void sortByAscOrder(int *t, int size);
void sortByDescOrder(int *t, int size);
void displayText(float x, float y, float z, void* font, const char* s);
int roundUpToInt(float _val);
void setPixel(int _x, int _y);
void drawBresenhamLine(Point2D p1, Point2D p2);
