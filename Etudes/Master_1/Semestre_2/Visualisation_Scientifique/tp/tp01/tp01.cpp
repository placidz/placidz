#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glut.h>

#include "Polygone.h"

using namespace std;

typedef unsigned int uint;

#define wWidth 800
#define wHeight 600

#define PI 3.14159265


int f1, f2, f3;
int lastX, lastY;
int indPoly = -1;

vector<Polygone> lPoly;
Polygone poly;

int ModeAffichage = GL_LINE_LOOP;
bool AffichageCoords = 0;
bool bModeCreation = 1;
bool bModeTrous = 0;


void affichage (void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 1.0);
	poly.tracerAretes(ModeAffichage);
	poly.tracerSommets();
	for (int i = 0; i < lPoly.size(); i++)
	{
		glColor3f(0.0, 0.0, 1.0);
		lPoly.at(i).tracerAretes(ModeAffichage);
		lPoly.at(i).tracerSommets();
	}
	//if (AffichageCoords) AfficherCoordonnees();
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

		case '1':
			ModeAffichage = GL_POINTS;
		break;
		
		case '2':
			ModeAffichage = GL_LINES;
		break;
		
		case '3':
			ModeAffichage = GL_LINE_LOOP;
		break;
		
		case '4':
			ModeAffichage = GL_LINE_STRIP;
		break;
		
		case 'e':
			lPoly.clear();
			poly.vider();
			bModeCreation = 1;
		break;
		
		case 'm':
			bModeCreation = !bModeCreation;
			if (bModeCreation)
			{
				printf(">> MODE CREATION <<\n");
			}
			else printf(">> MODE DEPLACEMENT <<\n");
		break;
		
		case 'p':
			if (poly.PTS.size() > 2)
			{
				poly.bCreationPolygone = 0;
				lPoly.push_back(poly);
				poly.vider();
				cout<<"Polygone terminé"<<endl;
				cout<<"Nombre de polygons : "<<lPoly.size()<<endl;
			}
		break;
		
		case 't':
			
		break;

		case 'c':
			// Afficher les coordonnees des points
			AffichageCoords = !AffichageCoords;
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
				if (bModeCreation)
				{
					poly.ajouterSommet(x, y);
				}
				else
				{
					cout<<"---------------------"<<endl;
					for (int i = 0; i < lPoly.size(); i++)
					{
						if (lPoly.at(i).estInterieur(x, y))
						{
							printf("ON EST DEDANS MINOOOT :B\n");
							lPoly.at(i).bEnMouvement = 1;
						}
						else printf("ON EST DEHORS :B\n");
					}
					cout<<"---------------------"<<endl;
					lastX = x;
					lastY = y;
				}
			}
			if (state == GLUT_UP)
			{
				for (int i = 0; i < lPoly.size(); i++)
				{
					lPoly.at(i).bEnMouvement = 0;
				}
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
	for (int i = 0; i < lPoly.size(); i++)
	{
		if (!bModeCreation && lPoly.at(i).bEnMouvement && lPoly.at(i).estInterieur(_x, wHeight-_y))
		{
			int vx, vy;
			vx = _x - lastX;
			vy = wHeight -_y - lastY;
			lPoly.at(i).deplacer(vx, vy);
		}
	}
    lastX = _x;
	lastY = wHeight-_y;
	
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
	f1 = glutCreateWindow("[TP1] Visualisation Scientifique - Un point dans un polygone");
	glutDisplayFunc(affichage);
	glutReshapeFunc(redim);
	glutKeyboardFunc(clavier);
	glutMouseFunc(souris);
	glutMotionFunc(motionGL);
	glutPassiveMotionFunc(passiveMotionGL);
	
	glClearColor(0.8, 0.8, 0.8, 1.0);
	glPointSize(5);
	
	glutMainLoop();

	return 0;
}
