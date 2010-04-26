#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
//#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Point2D.h"
#include "Polygone.h"

using namespace std;


// DEFINES

#define wWidth 800
#define wHeight 600
#define PI 3.14159265
#define MAX_POINTS 24


// DECLARATIONS VARIABLES

GLuint idBaseDL = 0;
float bg_color[] = {0.8, 0.8, 0.8, 1.0};

int f1, f2, f3;
int mouseX, mouseY;

vector<Point2D> ListePoints;
bool bAjoutPoint = 0;
bool bTraceNaif = 1;
bool bTraceBresenham = 0;


// CORPS DES FONCTIONS



void tracer_naif(Point2D p1, Point2D p2)
{
    int x1, y1, x2, y2;
    if (p1.x < p2.x)
    {
	  x1 = p1.x;
	  y1 = p1.y;
	  x2 = p2.x;
	  y2 = p2.y;
    }
    else
    {
	  x1 = p2.x;
	  y1 = p2.y;
	  x2 = p1.x;
	  y2 = p1.y;
    }

    int dx = x2 - x1;
    int dy = y2 - y1;
    glColor3f(0.0, 0.0, 1.0);
    for (int x=x1; x < x2; x++)
    {
	  int y = y1 + dy * (x - x1) / dx;
	  setPixel(x, y);
    }
    glEnd();
}

void tracer_naif2(Point2D p1, Point2D p2)
{
	float ar = (p2.y - p1.y)/(p2.x - p1.x);
	int x = p1.x;
	int y = p1.y;
	setPixel(x, y);
	while (x < p2.x)
	{
		x++;
		float yr = yr + ar;
		y = (int)(yr);
		setPixel(x, y);
	}
}

void drawPoints(void)
{
    glPointSize(3.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (int i=0; i<(int)ListePoints.size(); i++)
    {
	  glVertex2i(ListePoints[i].x, ListePoints[i].y);
    }
    glEnd();
    glPointSize(1.0);
}

void drawBase(void)
{
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    // Axe X
    glVertex2i(0, 0);
    glVertex2i(100, 0);
    // Axe Y
    glVertex2i(0, 0);
    glVertex2i(0, 100);
    glEnd();
}

void displayGL(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glCallList(idBaseDL);
    if (!ListePoints.empty())
    {
	  for (int i=0; i<(int)ListePoints.size()-1; i+=2)
	  {
		if (bTraceNaif) tracer_naif2(ListePoints[i], ListePoints[i+1]);
		if (bTraceBresenham) drawBresenhamLine(ListePoints[i], ListePoints[i+1]);
	  }
	  drawPoints();
    }

    glutSwapBuffers();
}

void reshapeGL(int _width, int _height)
{
    glViewport(0, 0, _width, _height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-_width/2, _width/2, -_height/2, _height/2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboardGL(unsigned char _key, int _x, int _y)
{
    switch (_key)
    {
    case 27:
	  exit(0);
	  break;

    case 'e':
	  ListePoints.clear();
	  break;

    case 'n':
	  bTraceNaif = !bTraceNaif;
	  break;

    case 'b':
	  bTraceBresenham = !bTraceBresenham;
	  break;

    }
    glutPostRedisplay();
}

void mouseGL(int _button, int _state, int _x, int _y)
{
    _x -= wWidth/2;
    _y = wHeight/2 - _y;

    switch(_button)
    {
    case GLUT_LEFT_BUTTON:
	  if (_state == GLUT_DOWN)
        {
		if(!bAjoutPoint && ListePoints.size() < MAX_POINTS)
		{
		    ListePoints.push_back(Point2D(_x, _y));
		    bAjoutPoint = 1;
		}

        }
	  if (_state == GLUT_UP)
        {
		bAjoutPoint = 0;
        }
        break;

	case GLUT_MIDDLE_BUTTON:
	  if (_state == GLUT_DOWN)
        {
        }
        break;

	case GLUT_RIGHT_BUTTON:
	  if (_state == GLUT_DOWN)
        {
        }
        break;
    }
    glutPostRedisplay();
}


void motionGL(int _x, int _y)
{
    mouseX = _x;
    mouseY = wHeight-_y;

    glutPostRedisplay();
}


void passiveMotionGL(int _x, int _y)
{
    glutPostRedisplay();
}

GLuint createBaseDL()
{
    GLuint id;
    id = glGenLists(1);
    if (!id) return id;

    glNewList(id, GL_COMPILE);
    drawBase();
    glEndList();

    return id;
}

void initGL(void)
{
    glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);

    idBaseDL = createBaseDL();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    /*** Fenetre 1 ***/
    glutInitWindowSize(wWidth, wHeight);
    glutInitWindowPosition(200, 100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    f1 = glutCreateWindow("[TP4] Visualisation Scientifique - Trace de segments : Algorithme naif et Bresenham");
    glutDisplayFunc(displayGL);
    glutReshapeFunc(reshapeGL);
    glutKeyboardFunc(keyboardGL);
    glutMouseFunc(mouseGL);
    glutMotionFunc(motionGL);
    glutPassiveMotionFunc(passiveMotionGL);

    initGL();

    glutMainLoop();

    return 0;
}
