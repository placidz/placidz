#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
//#include "glut.h"
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
int indiceSegment = -1;

vector<Polygone> lPoly;
vector<int> suiteIndice;
Polygone poly;

vector<Point2D> ptsInter = vector<Point2D>();

int ModeAffichage = GL_LINE_LOOP;
int indiceTrou = -1;
bool AffichageCoords = 0;
bool bModeCreation = 1;
bool bModeTrous = false;
bool selectionPoly = false;
bool verrouDplt = true;


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

void AfficherCoordonnees(void)
{
	if (!lPoly.empty())
	{
		for (int i = 0; i < (int)lPoly.size(); i++)
		{
			if (!lPoly.at(i).PTS.empty())
			{
				for (int j = 0; j < (int)lPoly.at(i).PTS.size(); j++)
				{
					glColor3f(0.0, 0.0, 0.0);
					char coords[30] = "";
					sprintf(coords,"[x: %d ; y: %d]", lPoly.at(i).PTS.at(j).x, lPoly.at(i).PTS.at(j).y);
					AfficherTexte(lPoly.at(i).PTS.at(j).x, lPoly.at(i).PTS.at(j).y+10, 0.0, GLUT_BITMAP_TIMES_ROMAN_10, coords);
				}
			}
		}
	}
}


void affichage (void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 1.0);
	poly.tracerAretes(ModeAffichage);
	poly.tracerSommets();
	for (int i = 0; i < (int)lPoly.size(); i++)
	{
		glColor3f(0.0, 0.0, 1.0);
		lPoly.at(i).tracerAretes(ModeAffichage);
		lPoly.at(i).tracerSommets();
		lPoly.at(i).tracerTrous(ModeAffichage);
	}
	for (int i = 0; i < (int)ptsInter.size(); i++)
	{
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_POINTS);
			glVertex2i(ptsInter.at(i).x, ptsInter.at(i).y);
		glEnd();
	}
	if (AffichageCoords) AfficherCoordonnees();
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
			bModeTrous = false;
			selectionPoly = false;
			indiceTrou = -1;
		break;
		
		case 'm':
			bModeCreation = !bModeCreation;
			if (bModeCreation)
			{
				printf(">> MODE CREATION <<\n");
			}
			else printf(">> MODE DEPLACEMENT <<\n");
		break;
		
		case 'a':
			if (poly.PTS.size() > 2 && bModeTrous == false)
			{
				poly.bCreationPolygone = 0;
				lPoly.push_back(poly);
				poly.vider();
				cout<<"Polygone terminé"<<endl;
				cout<<"Nombre de polygons : "<<lPoly.size()<<endl;
			}
			else if (poly.PTS.size() > 2 && bModeTrous == true && indiceTrou >= 0){
				poly.bCreationPolygone = 0;
				lPoly.at(indiceTrou).TROUS.push_back(poly);//insere le trou dans le polygone adequat
				poly.vider();
			}
			selectionPoly = false;
		break;
		
		case 'p':// On passe en mode dessin de polygones
			bModeTrous = false; 
			indiceTrou = -1;
			bModeCreation = 1;
			selectionPoly = false;
		break;

		case 't':// On passe en mode dessin de trous
			bModeTrous = true;	
			selectionPoly = false;
		break;

		case 'c':
			// Afficher les coordonnees des points
			AffichageCoords = !AffichageCoords;
		break;

		case 'i':
			ptsInter = lPoly.at(0).intersection(lPoly.at(1));			
		break;

		case 'U':{
			Polygone ptmp;
			ptmp.UNIONPoly(lPoly.at(0), lPoly.at(1));
			lPoly.clear();
			lPoly.push_back(ptmp);
				 }
		break;

		case 'I':{
			Polygone polyTmp, pTrou;
			polyTmp.INTERSECTIONPoly(lPoly.at(0), lPoly.at(1));
			lPoly.clear();
			for(int i = 0; i < (int)polyTmp.TROUS.size(); i++){
				pTrou.PTS = polyTmp.TROUS.at(i).PTS;
				lPoly.push_back(pTrou);
			}
			polyTmp.TROUS.clear();
			lPoly.push_back(polyTmp);
				 }
		break;

		case 'D':{
			Polygone polyTmp, pTrou;
			polyTmp.DIFFERENCEPoly(lPoly.at(0), lPoly.at(1));
			lPoly.clear();
			if((int)polyTmp.TROUS.size() > 1)
			for(int i = 0; i < (int)polyTmp.TROUS.size(); i++){
				pTrou.PTS = polyTmp.TROUS.at(i).PTS;
				lPoly.push_back(pTrou);
			}
			polyTmp.TROUS.clear();
			lPoly.push_back(polyTmp);
				 }
		break;
	}
	glutPostRedisplay();	
}

void souris (int button, int state, int x, int y)
{
	int cptNbPoly = 0;
	y = wHeight - y;
	
	switch(button)
	{
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) 
			{
				if (bModeCreation)
				{
					cout<<"Indice trou = " << indiceTrou << endl;
					if (bModeTrous) cout<<"Dessin de trous active!" <<endl;
					if(bModeTrous == false){	//si on trace un polygone
						poly.ajouterSommet(x, y);
						

					}
					if (selectionPoly == false && bModeTrous == true){
						for (int i = 0; i < (int)lPoly.size(); i++)
						{
							if (lPoly.at(i).estInterieur(x, y))
							{
								indiceTrou = i;
								cptNbPoly++;
							}
						}
						if(cptNbPoly > 1){
							cout<<"Veuillez ne selectionner qu'un seul polygone! (Non mais sans deconner quoi...)"<<endl;
							indiceTrou = -1;
						}
						
						
					}

					if(indiceTrou >= 0 && cptNbPoly > 0 || selectionPoly){
						selectionPoly = true;
						if (lPoly.at(indiceTrou).estInterieur(x, y)){
							bool dansTrou = false;
							for(int i = 0; i < (int)lPoly.at(indiceTrou).TROUS.size(); i++)
								if (lPoly.at(indiceTrou).TROUS.at(i).estInterieur(x, y))
									dansTrou = true;
							if(!dansTrou)
								poly.ajouterSommet(x, y);
						}
						printf("Polygone Selectionneavec succes, le trace du trou a commence!");
					}
				}
				else
				{
						cout<<"---------------------"<<endl;
						for (int i = 0; i < (int)lPoly.size(); i++)
						{
							if (lPoly.at(i).estInterieur(x, y) && verrouDplt == true)
							{
								verrouDplt = false;
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
				for (int i = 0; i < (int)lPoly.size(); i++)
				{
					lPoly.at(i).bEnMouvement = 0;
					verrouDplt = true;
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
	for (int i = 0; i < (int)lPoly.size(); i++)
	{
		if (!bModeCreation && !verrouDplt && lPoly.at(i).bEnMouvement)
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
