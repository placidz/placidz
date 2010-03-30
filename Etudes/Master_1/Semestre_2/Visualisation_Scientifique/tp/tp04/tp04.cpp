#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
//#include "glut.h"
#include <GL/glut.h>

#include "Point2D.h"
#include "Polygone.h"

using namespace std;

#define wWidth 800
#define wHeight 600

#define PI 3.14159265

float bg_color[] = {0.8, 0.8, 0.8, 1.0};

int f1, f2, f3;
int mouseX, mouseY;


void affichage (void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glutSwapBuffers();
}

void redim (int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void clavier (unsigned char key, int x, int y)
{
    switch (key)
    {
	case 27:
	exit(0);
	break;

    }
    glutPostRedisplay();
}

void souris (int button, int state, int x, int y)
{
    y = wHeight - y;

    switch(button)
    {
	case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {

        }
        if (state == GLUT_UP)
        {

        }
        break;

	case GLUT_MIDDLE_BUTTON:
        if (state == GLUT_DOWN)
        {
        }
        break;

	case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
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

int main (int argc, char **argv)
{
    glutInit(&argc, argv);

    /*** Fenetre 1 ***/
    glutInitWindowSize(wWidth, wHeight);
    glutInitWindowPosition(200, 100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    f1 = glutCreateWindow("[TP4] Visualisation Scientifique - Trace de segments : Algorithme naif et Bresenham");
    glutDisplayFunc(affichage);
    glutReshapeFunc(redim);
    glutKeyboardFunc(clavier);
    glutMouseFunc(souris);
    glutMotionFunc(motionGL);
    glutPassiveMotionFunc(passiveMotionGL);

    glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);
    //glPointSize(5);

    glutMainLoop();

    return 0;
}
