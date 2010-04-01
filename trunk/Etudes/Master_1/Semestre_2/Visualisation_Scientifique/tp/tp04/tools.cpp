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

int roundUpToInt(float _val)
{
    _val += 0.5;
    return (floor(_val));
}

void setPixel(int _x, int _y)
{
    glBegin(GL_POINTS);
	  glVertex2i(_x, _y);
    glEnd();
}

void drawBresenhamLine(Point2D p1, Point2D p2)
{
    glColor3f(0.0, 1.0, 0.0);
    int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
    int dx, dy;

    if((dx = x2 - x1) != 0)
    {
	  if (dx > 0)
	  {
		if ((dy = y2 - y1) != 0)
		{
		    if (dy > 0)
		    {
			  if (dx >= dy)
			  {
				int e;
				dx = (e = dx) * 2;
				dy *= 2;
				while (1)
				{
				    setPixel(x1, y1);
				    if (x1++ == x2) break;
				    if ((e = e - dy) < 0)
				    {
					  y1++;
					  e += dx;
				    }
				}
			  }
			  else
			  {
				int e;
				dy = (e = dy) * 2;
				dx *= 2;
				while (1)
				{
				    setPixel(x1, y1);
				    if (y1++ == y2) break;
				    if ((e = e - dx) < 0)
				    {
					  x1++;
					  e += dy;
				    }
				}
			  }
		    }
		    else
		    {
			  if (dx >= -dy)
			  {
				int e;
				dx = (e = dx) * 2;
				dy *= 2;
				while (1)
				{
				    setPixel(x1, y1);
				    if (x1++ == x2) break;
				    if ((e = e + dy) < 0)
				    {
					  y1--;
					  e += dx;
				    }
				}
			  }
			  else
			  {
				int e;
				dy = (e = dy) * 2;
				dx *= 2;
				while (1)
				{
				    setPixel(x1, y1);
				    if (y1-- == y2) break;
				    if ((e = e + dx) > 0)
				    {
					  x1++;
					  e += dy;
				    }
				}
			  }
		    }
		}
		else
		{
		    do
		    {
			  setPixel(x1, y1);
		    } while (x1++ != x2);
		}
	  }
	  else
	  {
		if ((dy = y2 - y1) != 0)
		{
		    if (dy > 0)
		    {
			  if (-dx >= dy)
			  {
				int e;
				dx = (e = dx) *2;
				dy *= 2;
				while (1)
				{
				    setPixel(x1, y1);
				    if (x1-- == x2) break;
				    if ((e = e + dy) >= 0)
				    {
					  y1++;
					  e+= dx;
				    }
				}
			  }
			  else
			  {
				int e;
				dy = (e = dy) * 2;
				dx *= 2;
				while (1)
				{
				    setPixel(x1, y1);
				    if (y1++ == y2) break;
				    if ((e = e + dx) <= 0)
				    {
					  x1--;
					  e += dy;
				    }
				}
			  }
		    }
		    else
		    {
			  if (dx <= dy)
			  {
				int e;
				dx = (e = dx) * 2;
				dy *= 2;
				while (1)
				{
				    setPixel(x1, y1);
				    if (x1-- == x2) break;
				    if ((e = e - dy) >= 0)
				    {
					  y1--;
					  e += dx;
				    }
				}
			  }
			  else
			  {
				int e;
				dy = (e = dy) * 2;
				dx *= 2;
				while (1)
				{
				    setPixel(x1, y1);
				    if (y1-- == y2) break;
				    if ((e = e - dx) >= 0)
				    {
					  x1--;
					  e += dy;
				    }
				}
			  }
		    }
		}
		else
		{
		    do
		    {
			  setPixel(x1, y1);
		    } while (x1-- != x2);
		}
	  }
    }
    else
    {
	  if ((dy = y2 - y1) != 0)
	  {
		if (dy > 0)
		{
		    do
		    {
			  setPixel(x1, y1);
		    } while (y1++ != y2);
		}
		else
		{
		    do
		    {
			  setPixel(x1, y1);
		    } while (y1-- != y2);
		}
	  }
    }
}
