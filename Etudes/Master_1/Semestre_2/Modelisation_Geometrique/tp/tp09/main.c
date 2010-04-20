#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "point3D.h"
#include "arete.h"
#include "face.h"
#include "cube.h"

using namespace std;

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#endif


extern "C"
{
#include <ml.h>
#include <he.h>
}



cube creeCube(float i, float j, float k, float pas);
void dessineQuadrillage();
void initEspace();
double potentialFunction(mlVec3 _coord);
void reductionCube();
void calculeSurface();
void polygonizeSurface();
void displaySurface();



int winX = 800;
int winY = 600;
double eloignement = 20.0;

int mouseClick[] = {0, 0, 0};

int modeRendu = GL_FILL;

hePtrMesh mesh;	
int currVert;
bool bEdgesDisplay = 0;
bool bPolyDisplay = 0;


/* Resolution de la grille de calcul */
int gridResolution = 32;

/* Graine d'initialisation de la surface implicite */
mlVec3 seed;
mlVec3 seedSphere2;

/* Rayon de la sphere implicite */
double radius = 0.5;
double radiusSphere2 = 0.5;

vector<cube> lstCube;
vector<cube> lstCubePolygon;
vector<cube> lstCubeMove;
vector<cube> lstCubePolygonMove;

bool afficheCube = true;
bool affichePolygonSphere1 = true;
bool affichePolygonSphere2 = true;

void setColor(int num){
	switch (num){
		case 0:glColor3f(0.0, 1.0, 1.0);break;
		case 1:glColor3f(1.0, 1.0, 0.0);break;
		case 2:glColor3f(1.0, 0.0, 0.0);break;
		case 3:glColor3f(0.0, 1.0, 0.0);break;	
		case 4:glColor3f(0.3, 0.5, 1.0);break;
		case 5:glColor3f(1.0, 1.0, 0.0);break;
		case 6:glColor3f(1.0, 0.0, 1.0);break;
		case 7:glColor3f(0.0, 1.0, 1.0);break;
		case 8:glColor3f(1.0, 1.0, 1.0);break;
	}
}


void displayGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
		glMultMatrixd(mlTbGetRotation());
		//Dessine la matrice de cubes
		if(afficheCube)
			dessineQuadrillage();
		displaySurface();
			
	glPopMatrix();
	
	
	
	glutSwapBuffers();
}


void reshapeGL(int _w, int _h)
{
	winX = _w;
	winY = _h;
   
	glViewport(0, 0, winX, winY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
   
	gluPerspective(60.0, (GLfloat)_w/(GLfloat)_h, 0.1, 128.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   
	mlTbInit(_w, _h);
   
	glutPostRedisplay();
}


void mouseGL(int _button, int _state, int _x, int _y)
{
	switch(_button)
	{
		case GLUT_LEFT_BUTTON:
			mouseClick[0] = (_state == GLUT_DOWN) ? 1 : 0;
				
			if(mouseClick[0] == 1)
				mlTbClick(_x, _y);
			else
				mlTbRelease(_x, _y);
			break;
		
		case GLUT_MIDDLE_BUTTON:
			mouseClick[1] = (_state == GLUT_DOWN) ? 1 : 0;
			break;
		
		case GLUT_RIGHT_BUTTON:
			mouseClick[2] = (_state == GLUT_DOWN) ? 1 : 0;
			break;
	}
	
	glutPostRedisplay();
}


void keyboardGL(unsigned char _k, int _x, int _y)
{
	if(_k == 27 || _k == 'q' || _k == 'Q')
		exit(0);
	switch(_k){
		case 'a':
			reductionCube();
		break;
		
		case 'z':
			polygonizeSurface();
		break;
		
		case 'c':
			afficheCube = !afficheCube;
		break;
		
		case 'p':
			affichePolygonSphere1 = !affichePolygonSphere1;
		break;
		
		case 'm':
			affichePolygonSphere2 = !affichePolygonSphere2;
		break;
		
		case 'x':{
			for(int i = 0; i < (int)lstCube.size(); i++)
				lstCube.at(i).lstPolyCube.clear();
			
			for(int i = 0; i < (int)lstCubeMove.size(); i++){
				for(int j = 0; j < (int)lstCubeMove.at(i).FaceCube.size(); j++){
					for(int k = 0; k < (int)lstCubeMove.at(i).FaceCube.at(j).lstArete.size(); k++){
						lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).p1.x +=0.05;
						lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).p2.x +=0.05;  
					}
				}
				lstCubeMove.at(i).lstPolyCube.clear();
			}
			mlVec3_Set(seedSphere2, seedSphere2[0]+0.05, seedSphere2[1], seedSphere2[2]);
			calculeSurface();
			polygonizeSurface();
		}
		break;
		
		case 'w':{
			for(int i = 0; i < (int)lstCube.size(); i++)
				lstCube.at(i).lstPolyCube.clear();
				
			for(int i = 0; i < (int)lstCubeMove.size(); i++){
				for(int j = 0; j < (int)lstCubeMove.at(i).FaceCube.size(); j++){
					for(int k = 0; k < (int)lstCubeMove.at(i).FaceCube.at(j).lstArete.size(); k++){
						lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).p1.x -=0.05;
						lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).p2.x -=0.05;  
					}
				}
				lstCubeMove.at(i).lstPolyCube.clear();
			}
			mlVec3_Set(seedSphere2, seedSphere2[0]-0.05, seedSphere2[1], seedSphere2[2]);
			calculeSurface();
			polygonizeSurface();
		}
		break;
		
		
	}

	glutPostRedisplay();
}


void keyboardSpecialGL(int _k, int _x, int _y)
{
	glutPostRedisplay();
}


void motionGL(int _x, int _y)
{
	if(mouseClick[0] == 1)
			mlTbMotion(_x, _y);
   
	glutPostRedisplay();
}


void passiveMotionGL(int _x, int _y)
{
	glutPostRedisplay();
}


void initGL()
{
	GLfloat mat_specular   [4] = { 0.18, 0.18, 0.18, 0.18 };
	GLfloat mat_shininess  [ ] = { 128 };
	GLfloat global_ambient [ ] = { 0.02, 0.02, 0.05, 0.05 };
	GLfloat light0_ambient [ ] = { 0, 0, 0, 0 };
	GLfloat light0_diffuse [ ] = { 0.85, 0.85, 0.8, 0.85 };
	GLfloat light0_specular[ ] = { 0.85, 0.85, 0.15, 0.85 };

	glClearColor(0.8, 0.8, 0.7, 1.0);
	
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	initEspace();
}


int main(int _argc, char ** _argv)
{
	int posX, posY;
	mlVec3_Set(seed, 0, 0, 0);
	mlVec3_Set(seedSphere2, 1, 0, 0);
	
   
	glutInit(&_argc, _argv);
   
	
	posX = (glutGet(GLUT_SCREEN_WIDTH ) - winX) / 2;
	posY = (glutGet(GLUT_SCREEN_HEIGHT) - winY) / 2;
	
	glutInitWindowSize(winX, winY);
	glutInitWindowPosition(posX, posY);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("[TP09] Modelisation Geometrique - Surfaces implicites");
	
	glutDisplayFunc(displayGL);
	glutReshapeFunc(reshapeGL);
	
	glutMouseFunc(mouseGL);
	glutKeyboardFunc(keyboardGL);
	glutSpecialFunc(keyboardSpecialGL);
	
	glutMotionFunc(motionGL);
	glutPassiveMotionFunc(passiveMotionGL);
		
	initGL();
	glutMainLoop();

	return 0;
}

cube creeCube(float i, float j, float k, float pas){
	face fa;
	vector<face> lstFa;
	cube res;
	//Face Avant
	fa.setFace( arete( point3D(i, j, k), point3D(i, j+pas, k) ), 
				arete( point3D(i, j+pas, k), point3D(i+pas, j+pas, k) ), 
				arete( point3D(i+pas, j+pas, k), point3D(i+pas, j, k)), 
				arete( point3D(i+pas, j, k), point3D(i, j, k)) );			
	lstFa.push_back(fa);
	fa.lstFace.clear();
	fa.lstArete.clear();
	
	//Face Arriere
	fa.setFace( arete( point3D(i+pas, j, k+pas), point3D(i+pas, j+pas, k+pas) ), 
				arete( point3D(i+pas, j+pas, k+pas), point3D(i, j+pas, k+pas) ), 
				arete( point3D(i, j+pas, k+pas), point3D(i, j, k+pas)), 
				arete( point3D(i, j, k+pas), point3D(i+pas, j, k+pas)) );			
	lstFa.push_back(fa);
	fa.lstFace.clear();
	fa.lstArete.clear();
	
	//Face Gauche
	fa.setFace( arete( point3D(i, j, k+pas), point3D(i, j+pas, k+pas) ), 
				arete( point3D(i, j+pas, k+pas), point3D(i, j+pas, k) ), 
				arete( point3D(i, j+pas, k), point3D(i, j, k)), 
				arete( point3D(i, j, k), point3D(i, j, k+pas)) );			
	lstFa.push_back(fa);
	fa.lstFace.clear();
	fa.lstArete.clear();
	
	//Face Droite
	fa.setFace( arete( point3D(i+pas, j, k), point3D(i+pas, j+pas, k) ), 
				arete( point3D(i+pas, j+pas, k), point3D(i+pas, j+pas, k+pas) ), 
				arete( point3D(i+pas, j+pas, k+pas), point3D(i+pas, j, k+pas) ), 
				arete( point3D(i+pas, j, k+pas), point3D(i+pas, j, k) ) );			
	lstFa.push_back(fa);
	fa.lstFace.clear();
	fa.lstArete.clear();
	
	//Face Haut
	fa.setFace( arete( point3D(i, j+pas, k), point3D(i, j+pas, k+pas) ), 
				arete( point3D(i, j+pas, k+pas), point3D(i+pas, j+pas, k+pas) ), 
				arete( point3D(i+pas, j+pas, k+pas), point3D(i+pas, j+pas, k) ), 
				arete( point3D(i+pas, j+pas, k), point3D(i, j+pas, k) ) );			
	lstFa.push_back(fa);
	fa.lstFace.clear();
	fa.lstArete.clear();
	
	//Face Bas
	fa.setFace( arete( point3D(i, j, k+pas), point3D(i, j, k) ), 
				arete( point3D(i, j, k), point3D(i+pas, j, k) ), 
				arete( point3D(i+pas, j, k), point3D(i+pas, j, k+pas) ), 
				arete( point3D(i+pas, j, k+pas), point3D(i, j, k+pas) ) );			
	lstFa.push_back(fa);
	fa.lstFace.clear();
	fa.lstArete.clear();
	
	res.setCube(lstFa);
	lstFa.clear();
	fa.lstArete.clear();
	return res;
}


void initEspace(){	
	float pas = 1/(float)gridResolution;
	
	for(float i=-0.5; i<0.5; i+=pas){
		for(float j=-0.5; j<0.5; j+=pas){
			for(float k=-0.5; k<0.5; k+=pas){
				lstCube.push_back( creeCube(i, j, k, pas) );
			}
		}
	}
	
	for(float i=0.5; i<1.5; i+=pas){
		for(float j=-0.5; j<0.5; j+=pas){
			for(float k=-0.5; k<0.5; k+=pas){
				lstCubeMove.push_back( creeCube(i, j, k, pas) );
			}
		}
	}
	
}

void dessineQuadrillage(){	
	glDisable(GL_LIGHTING);
	for(int i = 0; i < (int)lstCube.size(); i++){
		for(int j =0; j < (int)lstCube.at(i).FaceCube.size(); j++){
			for(int k =0; k < (int)lstCube.at(i).FaceCube.at(j).lstArete.size(); k++){
				setColor(6);
				glBegin(GL_LINES);
					glVertex3f(lstCube.at(i).FaceCube.at(j).lstArete.at(k).p1.x, lstCube.at(i).FaceCube.at(j).lstArete.at(k).p1.y, lstCube.at(i).FaceCube.at(j).lstArete.at(k).p1.z);
					glVertex3f(lstCube.at(i).FaceCube.at(j).lstArete.at(k).p2.x, lstCube.at(i).FaceCube.at(j).lstArete.at(k).p2.y, lstCube.at(i).FaceCube.at(j).lstArete.at(k).p2.z);
				glEnd();
			}
		}
	}
	
	for(int i = 0; i < (int)lstCubeMove.size(); i++){
		for(int j =0; j < (int)lstCubeMove.at(i).FaceCube.size(); j++){
			for(int k =0; k < (int)lstCubeMove.at(i).FaceCube.at(j).lstArete.size(); k++){
				setColor(5);
				glBegin(GL_LINES);
					glVertex3f(lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).p1.x, lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).p1.y, lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).p1.z);
					glVertex3f(lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).p2.x, lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).p2.y, lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).p2.z);
				glEnd();
			}
		}
	}
	glEnable(GL_LIGHTING);
}

double potentialFunction(mlVec3 _coord)
{
	/* fonction de calcul de l'equation de potentiel */
	double d, res;
	mlVec3 tmp;
	mlVec3_SubVec( tmp, _coord, seed); 
	d = mlVec3_Norm(tmp);
	
	if(0 <= d && d < radius/(float)3){
		res = (1-3*(pow(d/radius, 2)));
	}
	else if(radius/(float)3 <= d && d < radius){
		res = (3/2*(pow(1 - d/radius, 2)));
	}
	else{
		res = 0;
	}
	return res;
}
double potentialFunctionSphere2(mlVec3 _coord)
{
	/* fonction de calcul de l'equation de potentiel */
	double d, res;
	mlVec3 tmp;
	mlVec3_SubVec( tmp, _coord, seedSphere2); 
	d = mlVec3_Norm(tmp);
	
	if(0 <= d && d < radiusSphere2/(float)3){
		res = (1-3*(pow(d/radiusSphere2, 2)));
	}
	else if(radiusSphere2/(float)3 <= d && d < radiusSphere2){
		res = (3/2*(pow(1 - d/radiusSphere2, 2)));
	}
	else{
		res = 0;
	}
	return res;
}

void reductionCube(){
	mlVec3 Ptmp;
	double val, valTest;
	int verif = 0;
	
	for(int i = 0; i < (int)lstCube.size(); i++){
		for(int j = 0; j < (int)lstCube.at(i).FaceCube.size(); j++){
			for(int k = 0; k < (int)lstCube.at(i).FaceCube.at(j).lstArete.size(); k++){
				lstCube.at(i).FaceCube.at(j).lstArete.at(k).p1.pt3DtoMlVec(Ptmp);
				val = potentialFunction(Ptmp);
				if(val > radius){
					lstCube.at(i).FaceCube.at(j).lstArete.at(k).isPtInter = 0;
					lstCube.at(i).FaceCube.at(j).markSommet.push_back(false);
				}
				
				else{
					lstCube.at(i).FaceCube.at(j).lstArete.at(k).isPtInter = 1;
					lstCube.at(i).FaceCube.at(j).markSommet.push_back(true);
				}
				 
			}
		}
	}	
	
	//Meme traitement pour la seconde sphere
	for(int i = 0; i < (int)lstCubeMove.size(); i++){
		for(int j = 0; j < (int)lstCubeMove.at(i).FaceCube.size(); j++){
			for(int k = 0; k < (int)lstCubeMove.at(i).FaceCube.at(j).lstArete.size(); k++){
				lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).p1.pt3DtoMlVec(Ptmp);
				val = potentialFunctionSphere2(Ptmp);
				if(val > radiusSphere2){
					lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).isPtInter = 0;
					lstCubeMove.at(i).FaceCube.at(j).markSommet.push_back(false);
				}
				
				else{
					lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).isPtInter = 1;
					lstCubeMove.at(i).FaceCube.at(j).markSommet.push_back(true);
				}
				
				/*
				//regarde par rapport a la sphere 1
				if(lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).isPtInter == true){ //si la valeur n'a pas bouge, on est interesse de savoir si le sommet appartient a la seconde sphere{
					val = potentialFunction(Ptmp);
					if(val > radius){
						lstCube.at(i).FaceCube.at(j).lstArete.at(k).isPtInter = false;
						lstCube.at(i).FaceCube.at(j).markSommet.push_back(false);
					}
					
					else{
						lstCube.at(i).FaceCube.at(j).lstArete.at(k).isPtInter = true;
						lstCube.at(i).FaceCube.at(j).markSommet.push_back(true);
					}
					
				}
				*/ 		
			}	
		}
	}
	
	//Supprime les cubes entierement a l'exterieur/interieur
	for(int i = 0; i < (int)lstCube.size(); i++){
		for(int j = 0; j < (int)lstCube.at(i).FaceCube.size(); j++){
			for(int k = 0; k < (int)lstCube.at(i).FaceCube.at(j).markSommet.size(); k++){
				if(lstCube.at(i).FaceCube.at(j).markSommet.at(k) == true)
					verif++;
			}	
		}
		//Si les 8 sommets du cubes sont dans le meme etat, on sauvegarde la cube
		if(verif != 0 && verif != 24){
			lstCubePolygon.push_back(lstCube.at(i));
			
		}
		verif= 0;
	}
	
	for(int i = 0; i < (int)lstCubeMove.size(); i++){
		for(int j = 0; j < (int)lstCubeMove.at(i).FaceCube.size(); j++){
			for(int k = 0; k < (int)lstCubeMove.at(i).FaceCube.at(j).markSommet.size(); k++){
				if(lstCubeMove.at(i).FaceCube.at(j).markSommet.at(k) == true)
					verif++;
			}	
		}
		//Si les 8 sommets du cubes sont dans le meme etat, on sauvegarde la cube
		if(verif != 0 && verif != 24){
			lstCubePolygonMove.push_back(lstCubeMove.at(i));
			
		}
		verif= 0;
	}
	
	lstCube.clear();
	lstCubeMove.clear();
	lstCube = lstCubePolygon;
	lstCubeMove = lstCubePolygonMove;
	
	
	
}

void calculeSurface(){
	mlVec3 Ptmp;
	double val;
	for(int i = 0; i < (int)lstCube.size(); i++)
		lstCube.at(i).initSommet();
	for(int i = 0; i < (int)lstCubeMove.size(); i++)
		lstCubeMove.at(i).initSommet();	
		
	for(int i = 0; i < (int)lstCube.size(); i++){
		for(int j = 0; j < (int)lstCube.at(i).FaceCube.size(); j++){
			for(int k = 0; k < (int)lstCube.at(i).FaceCube.at(j).lstArete.size(); k++){
				lstCube.at(i).FaceCube.at(j).lstArete.at(k).p1.pt3DtoMlVec(Ptmp);
				val = potentialFunction(Ptmp);
				if(val > radius){
					lstCube.at(i).FaceCube.at(j).lstArete.at(k).isPtInter = 0;
					lstCube.at(i).FaceCube.at(j).markSommet.push_back(false);
				}
				
				else{
					val = potentialFunctionSphere2(Ptmp);
					if(val > radiusSphere2){
						lstCube.at(i).FaceCube.at(j).lstArete.at(k).isPtInter = 0;
						lstCube.at(i).FaceCube.at(j).markSommet.push_back(false);
					}
					
					else{
						lstCube.at(i).FaceCube.at(j).lstArete.at(k).isPtInter = 1;
						lstCube.at(i).FaceCube.at(j).markSommet.push_back(true);
					}
				}
			}	
		}
	}
	
	//Meme traitement pour la seconde sphere
	for(int i = 0; i < (int)lstCubeMove.size(); i++){
		for(int j = 0; j < (int)lstCubeMove.at(i).FaceCube.size(); j++){
			for(int k = 0; k < (int)lstCubeMove.at(i).FaceCube.at(j).lstArete.size(); k++){
				lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).p1.pt3DtoMlVec(Ptmp);
				val = potentialFunction(Ptmp);
				if(val > radiusSphere2){
					lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).isPtInter = 0;
					lstCubeMove.at(i).FaceCube.at(j).markSommet.push_back(false);
				}
				
				else{
					val = potentialFunctionSphere2(Ptmp);
					if(val > radius){
						lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).isPtInter = 0;
						lstCubeMove.at(i).FaceCube.at(j).markSommet.push_back(false);
					}
					
					else{
						lstCubeMove.at(i).FaceCube.at(j).lstArete.at(k).isPtInter = 1;
						lstCubeMove.at(i).FaceCube.at(j).markSommet.push_back(true);
					}
				}
			}	
		}
	}
}



void polygonizeSurface(){
	/* fonction de polygonisation de la surface implicite */
	int test;
	cout<<"Reste "<<(int)lstCube.size()<<" cubes"<<endl;
	for(int i = 0; i < (int)lstCube.size(); i++){
		lstCube.at(i).polygonise();	
	}
	
	for(int i = 0; i < (int)lstCubeMove.size(); i++){
		lstCubeMove.at(i).polygonise();	
	}
	
}

void displaySurface()
{
	/* fonction d'affichage de la surface implicite */
	glDisable(GL_LIGHTING);
	if(affichePolygonSphere1){
		glColor3f(0.0, 0.0, 1.0);
		for(int i = 0; i < (int)lstCube.size(); i++){
			glBegin(GL_LINE_LOOP);
			//glBegin(GL_POLYGON);
			for(int j = 0; j < (int)lstCube.at(i).lstPolyCube.size(); j++){
				glVertex3f(lstCube.at(i).lstPolyCube.at(j).x, lstCube.at(i).lstPolyCube.at(j).y, lstCube.at(i).lstPolyCube.at(j).z);
			}
			glEnd();
		}
	}
	
	if(affichePolygonSphere2){	
		for(int i = 0; i < (int)lstCubeMove.size(); i++){
			glBegin(GL_LINE_LOOP);
			for(int j = 0; j < (int)lstCubeMove.at(i).lstPolyCube.size(); j++){
				glVertex3f(lstCubeMove.at(i).lstPolyCube.at(j).x, lstCubeMove.at(i).lstPolyCube.at(j).y, lstCubeMove.at(i).lstPolyCube.at(j).z);
			}
			glEnd();
		}
	}	
	glEnable(GL_LIGHTING);
}


