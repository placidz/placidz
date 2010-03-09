#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <GL/glut.h>

#include "Polygone.h"

using namespace std;

typedef unsigned int uint;

#define wWidth 800
#define wHeight 600

#define PI 3.14159265
#define NB_PTS_MAX 100

typedef struct 
{
	int x;
	int y;
} Point;

Polygone poly;

int f1, f2, f3;

int ModeAffichage = GL_LINE_LOOP;

int ModeFonctionnement = 0;

bool ModeCreationPolygon = 1;
bool AffichageCoords = 0;
bool CreationPoint = 0;

//vector<Point> PTS;
vector<vector<Point> > TROUS;

//int PtInsertion = -1;
int NbPolygons = 0;

void AfficherTexte(float x, float y, float z, void* font, const char* s)
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

/*void AfficherCoordonnees(void)
{
	if (!PTS.empty())
	{
		for (int i = 0; i < (int)PTS.size(); i++)
		{
			glColor3f(0.0, 0.0, 0.0);
			char coords[30] = "";
			sprintf(coords,"[x: %d ; y: %d]", PTS[i].x,PTS[i].y);
			AfficherTexte(PTS[i].x, wHeight - PTS[i].y+10, 0.0, GLUT_BITMAP_TIMES_ROMAN_10, coords);
		}
	}
}*/

void affichage (void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	poly.tracerAretes(ModeAffichage);
	poly.tracerSommets();
	//if (AffichageCoords) AfficherCoordonnees();
	glFlush();
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
			poly.PTS.clear();
			poly.PointInsertion = -1;
		break;
		
		case 'p':
			ModeCreationPolygon = !ModeCreationPolygon;
			//NbPolygons++;
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
				if (ModeCreationPolygon)
				{
					if (!CreationPoint)
					{
						if (NbPolygons == 0) NbPolygons++;
						CreationPoint = 1;
						poly.ajouterSommet(x, y);
					}
				}
				else 
				{
					if (poly.estInterieur(x, y)) printf("ON EST DEDANS MINOOOOOOT\n");
					else printf("ET NOOOOOOON DEHORS MINOOOOOT\n");
				}
			}
			if (state == GLUT_UP)
			{
				if (CreationPoint) CreationPoint = 0;
			}
		break;

		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN)
			{
				poly.changerPointInsertion(x, y);
			}
		break;

		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN)
			{
				poly.supprimerSommet(x, y);
			}
		break;
	}
	glutPostRedisplay();
}

int main (int argc, char **argv)
{
	glutInit(&argc, argv);
	
	/*** Fenetre 1 ***/
	glutInitWindowSize(wWidth, wHeight);
	glutInitWindowPosition(200, 100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f1 = glutCreateWindow("[TP1] Visualisation Scientifique - Un point dans un polygone");
	glutDisplayFunc(affichage);
	glutReshapeFunc(redim);
	glutKeyboardFunc(clavier);
	glutMouseFunc(souris);
	
	glutMainLoop();

	return 0;
}
