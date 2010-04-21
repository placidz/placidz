#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <limits.h>

#include "Bezier.h"
#include "BSpline.h"
#include "tools.h"


#include "defines.h"

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
#include <he.h>
#include <ml.h>
}

using namespace std;

string loadMode;
string filename;

int winX = 800;
int winY = 600;

GLuint idBaseDL = 0;

Image texture;
GLubyte* pTexture = NULL;
GLuint idTexture;

int size_u = 0, size_v = 0;

float zoom = 10.0;
int vueOrtho = 15;

mlVec3 PtsUV[MAX][MAX];
int nBP = 16;
mlVec3 bP[MAX2];

int degreBS = 3;

bool bRenduBezier = 1;
int drawMode = 2;

Point3D barycentre;
double barx, bary, barz;

int AltCtrlShift_State;
int movePoint = 0;
int du = -1, dv = -1;

bool view3D = 0;


int trouverPoint(mlVec3 _ctrl[MAX][MAX], int _size_u, int _size_v, double _cx, double _cy, double _cz, double _ratioX, double _ratioY, int * _du, int * _dv)
{
    int plan;
    int found = 0;
    double e = 0.02;
    double eX = e * _ratioX;
    double eY = e * _ratioY;
    printf("eX: %f, eY: %f\n", eX, eY);
    //printf("cx: %f, cy: %f, cz: %f\n", _cx, _cy, _cz);
    if (_cz == INT_MAX) plan = 1;
    if (_cy == INT_MAX) plan = 2;
    if (_cx == INT_MAX) plan = 3;
    double px, py, pz;
    for (int j = 0; j < _size_v; j++)
    {
	  for(int i = 0; i < _size_u; i++)
	  {
		px = _ctrl[i][j][0];
		py = _ctrl[i][j][1];
		pz = _ctrl[i][j][2];
		//printf("px: %f, py: %f, pz: %f\n", px, py, pz);
		switch(plan)
		{
		case 1: // Plan XY
		    if ((_cx >= (px-eX) && _cx <= (px+eX)) && (_cy >= (py-eY) && _cy <= (py+eY)))
		    {
			  printf("Plan XY\n");
			  *_du = i;
			  *_dv = j;
			  found = 1;
		    }
		    break;

		case 2: // Plan XZ
		    if ((_cx >= (px-eX) && _cx <= (px+eX)) && (_cz >= (pz-eY) && _cz <= (pz+eY)))
		    {
			  printf("Plan XZ\n");
			  *_du = i;
			  *_dv = j;
			  found = 2;
		    }
		    break;

		case 3: // Plan YZ
		    if ((_cz >= (pz-eX) && _cz <= (pz+eX)) && (_cy >= (py-eY) && _cy <= (py+eY)))
		    {
			  printf("Plan YZ\n");
			  *_du = i;
			  *_dv = j;
			  found = 3;
		    }
		    break;
		}
	  }
    }
    return found;
}

void drawControlPointsSurface(mlVec3 _ctrl[MAX][MAX], int _size_u, int _size_v)
{
    glDisable(GL_LIGHTING);
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (int u = 0; u < _size_u; u++)
	  for (int v = 0; v < _size_v; v++)
	  {
	  if (du == u && dv == v) glColor3f(1.0, 1.0, 0.0);
	  else glColor3f(1.0, 0.0, 0.0);
	  glVertex3dv(_ctrl[u][v]);
    }
    glEnd();
    glPointSize(1.0);
    glEnable(GL_LIGHTING);
}

void draw3DViewOnly()
{
    // VIEWPORT 0 : ECRAN ENTIER
    glViewport(0, 0, winX, winY);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0, 1.0, 0.1, 1000.0);

    gluLookAt(0.0, 0.0, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    glMultMatrixd(mlTbGetRotation());
    glCallList(idBaseDL);
    if (bRenduBezier)
    {
	  cout<<"Surface de Bezier"<<endl;
	  glBindTexture(GL_TEXTURE_2D, idTexture);
	  //glDisable(GL_LIGHTING);
	  glEnable(GL_TEXTURE_2D);
	  drawBezierSurface(PtsUV, size_u, size_v, bP, nBP, drawMode);
	  glDisable(GL_TEXTURE_2D);
	  //glEnable(GL_LIGHTING);
    }
    else if (size_u > degreBS && size_v > degreBS)
    {
	  cout<<"Surface BSpline"<<endl;
	  drawBSplineSurface(PtsUV, size_u, size_v, bP, nBP, degreBS, degreBS, drawMode);
    }
    else std::cout<<"Le degré utilisé pour la surface BSpline est égal ou supérieur au nombre de points de contrôle sur U et/ou V"<<std::endl;
    drawControlPointsSurface(PtsUV, size_u, size_v);
    glPopMatrix();
}

void drawFourViews()
{
    // VIEWPORT 0 : ECRAN ENTIER
    glViewport(0, 0, winX, winY);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, winX, 0, winY);
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(winX/2, 0);
    glVertex2f(winX/2, winY);
    glVertex2f(0, winY/2);
    glVertex2f(winX, winY/2);
    glEnd();
    glPopMatrix();

    // VIEWPORT 1 : HAUT-GAUCHE
    // Vue 3D
    glViewport(0, winY/2, winX/2, winY/2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0, 1.0, 0.1, 1000.0);

    gluLookAt(0.0, 0.0, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    glMultMatrixd(mlTbGetRotation());
    glCallList(idBaseDL);
    if (bRenduBezier)
    {
	  cout<<"Surface de Bezier"<<endl;
	  glBindTexture(GL_TEXTURE_2D, idTexture);
	  //glDisable(GL_LIGHTING);
	  glEnable(GL_TEXTURE_2D);
	  drawBezierSurface(PtsUV, size_u, size_v, bP, nBP, drawMode);
	  glDisable(GL_TEXTURE_2D);
	  //glEnable(GL_LIGHTING);
    }
    else if (size_u > degreBS && size_v > degreBS)
    {
	  cout<<"Surface BSpline"<<endl;
	  drawBSplineSurface(PtsUV, size_u, size_v, bP, nBP, degreBS, degreBS, drawMode);
    }
    else std::cout<<"Le degré utilisé pour la surface BSpline est égal ou supérieur au nombre de points de contrôle sur U et/ou V"<<std::endl;
    drawControlPointsSurface(PtsUV, size_u, size_v);
    glPopMatrix();

    // VIEWPORT 2 : HAUT-DROITE
    // Plan XY
    glViewport(winX/2, winY/2, winX/2, winY/2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-vueOrtho, vueOrtho, -vueOrtho, vueOrtho, -100, 100);

    glPushMatrix();
    glCallList(idBaseDL);
    drawControlPointsSurface(PtsUV, size_u, size_v);
    glPopMatrix();

    // VIEWPORT 3 : BAS-GAUCHE
    // Plan XZ
    glViewport(0, 0, winX/2, winY/2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-vueOrtho, vueOrtho, -vueOrtho, vueOrtho, -100, 100);
    glRotated(-90, 1, 0, 0); /* on tourne de -90° autour de l'axe x, sens antihoraire */

    glPushMatrix();
    glCallList(idBaseDL);
    drawControlPointsSurface(PtsUV, size_u, size_v);
    glPopMatrix();

    // VIEWPORT 4 : BAS-DROITE
    // Plan XZ
    glViewport(winX/2, 0, winX/2, winY/2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-vueOrtho, vueOrtho, -vueOrtho, vueOrtho, -100, 100);
    glRotated(90, 0, 1, 0); /* on tourne de -90° autour de l'axe x, sens antihoraire */
}

void displayGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (view3D) draw3DViewOnly();
    else drawFourViews();

    glPushMatrix();
    glCallList(idBaseDL);
    if (!view3D) drawControlPointsSurface(PtsUV, size_u, size_v);
    glPopMatrix();

    glutSwapBuffers();
}


void reshapeGL(int _w, int _h)
{
    winX = _w;
    winY = _h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-winX/2, winX/2, -winY/2, winY/2, -winY, winY/2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutPostRedisplay();
}


void mouseGL(int _button, int _state, int _x, int _y)
{
    double unitBaseX = winX / 4; // 200
    double unitBaseY = winY / 4; // 150
    double unitScaleX = unitBaseX / vueOrtho;
    double unitScaleY = unitBaseY / vueOrtho;

    switch (_button)
    {
    case GLUT_LEFT_BUTTON:
	  if (_state==GLUT_DOWN)
	  {
		AltCtrlShift_State = glutGetModifiers();
		if (AltCtrlShift_State == GLUT_ACTIVE_CTRL) mlTbClick(_x,_y);

		//Connaître dans quel coin de l'écran on clique
		if (movePoint == 0 && _x > winX/2 && _y < winY/2) // Coin haut droit
		{
		    printf("CLIC GAUCHE DANS PLAN HAUT-DROIT\n");
		    _y = winY/2 - _y;
		    int oriX, oriY;
		    oriX = 3 * winX/4;
		    oriY = winY/4;
		    double x, y;
		    x = _x - oriX;
		    y = _y - oriY;
		    // Scale par rapport au zoom
		    x = x / unitScaleX;
		    y = y / unitScaleY;
		    movePoint = trouverPoint(PtsUV, size_u, size_v, x, y, (double)INT_MAX, vueOrtho, vueOrtho, &du, &dv);
		}
		else if (movePoint == 0 && _x < winX/2 && _y > winY/2) // Coin bas gauche
		{
		    printf("CLIC GAUCHE DANS PLAN BAS-GAUCHE\n");
		    _y = 3*winY/2 - _y;
		    int oriX, oriY;
		    oriX = winX/4;
		    oriY = 3 * winY/4;
		    double x, y;
		    x = _x - oriX;
		    y = _y - oriY;
		    // Scale par rapport au zoom
		    x = x / unitScaleX;
		    y = y / unitScaleY;
		    movePoint = trouverPoint(PtsUV, size_u, size_v, x, (double)INT_MAX, y, vueOrtho, vueOrtho, &du, &dv);
		}
		else if (movePoint == 0 && _x > winX/2 && _y > winY/2) // Coin bas droit
		{
		    printf("CLIC GAUCHE DANS PLAN BAS-DROIT\n");
		    _y = 3*winY/2 - _y;
		    int oriX, oriY;
		    oriX = 3 * winX/4;
		    oriY = 3 * winY/4;
		    double x, y;
		    x = _x - oriX;
		    y = _y - oriY;
		    // Scale par rapport au zoom
		    x = x / unitScaleX;
		    y = y / unitScaleY;
		    movePoint = trouverPoint(PtsUV, size_u, size_v, (double)INT_MAX, y, x, vueOrtho, vueOrtho, &du, &dv);
		}
	  }
	  if (_state==GLUT_UP)
	  {
		movePoint = 0;
		du = -1;
		dv = -1;
		/*mlTbRelease(_x,_y);*/
	  }
	  break ;
    case GLUT_RIGHT_BUTTON:
	  break;
    default:
	  break;
    }
    glutPostRedisplay();
}


void keyboardGL(unsigned char _k, int _x, int _y)
{
    if(_k == 27 || _k == 'q' || _k == 'Q')
	  exit(0);
    switch(_k)
    {
    case 'n':
	  nBP--;
	  printf("nBP: %d\n", nBP);
	  break;
    case 'N':
	  nBP++;
	  printf("nBP: %d\n", nBP);
	  break;
    case 'b':
	  bRenduBezier = !bRenduBezier;
	  break;
    case 'r':
	  drawMode++;
	  drawMode = drawMode % 3;
	  if (drawMode == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	  else if (drawMode == 1) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	  else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	  break;

    case 'p':
	  zoom--;
	  break;
    case 'm':
	  zoom++;
	  break;

    case 'z':
	  view3D = !view3D;
	  break;

    case 'o':
	  vueOrtho--;
	  printf("VueOrtho: %d\n", vueOrtho);
	  break;
    case 'l':
	  vueOrtho++;
	  printf("VueOrtho: %d\n", vueOrtho);
	  break;

    case 's':
	  saveFilePTS2("save.pts2", PtsUV, size_u, size_v);
	  break;
    }
    glutPostRedisplay();
}


void keyboardSpecialGL(int _k, int _x, int _y)
{
    switch(_k)
    {
    case GLUT_KEY_UP:
	  printf("JOURBONN LOLOL");
	  break;
    }

    glutPostRedisplay();
}


void motionGL(int _x, int _y)
{
    double unitBaseX = winX / 4; // 200
    double unitBaseY = winY / 4; // 150
    double unitScaleX = unitBaseX / vueOrtho;
    double unitScaleY = unitBaseY / vueOrtho;

    if (AltCtrlShift_State == GLUT_ACTIVE_CTRL) mlTbMotion(_x,_y);

    //Connaître dans quel coin de l'écran on clique
    if (movePoint == 1 && _x > winX/2 && _y < winY/2) // Coin haut droit
    {
	  int ry = winY/2 - _y;
	  int oriX, oriY;
	  oriX = 3 * winX/4;
	  oriY = winY/4;
	  double x, y;
	  x = _x - oriX;
	  y = ry - oriY;
	  // Scale par rapport au zoom
	  x = x / unitScaleX;
	  y = y / unitScaleY;
	  if (movePoint)
	  {
		PtsUV[du][dv][0] = x;
		PtsUV[du][dv][1] = y;
	  }
    }
    else if (movePoint == 2 && _x < winX/2 && _y > winY/2) // Coin bas gauche
    {
	  _y = 3*winY/2 - _y;
	  int oriX, oriY;
	  oriX = winX/4;
	  oriY = 3 * winY/4;
	  double x, y;
	  x = _x - oriX;
	  y = _y - oriY;
	  // Scale par rapport au zoom
	  x = x / unitScaleX;
	  y = y / unitScaleY;
	  if (movePoint)
	  {
		PtsUV[du][dv][0] = x;
		PtsUV[du][dv][2] = y;
	  }
    }
    else if (movePoint == 3 && _x > winX/2 && _y > winY/2) // Coin bas droit
    {
	  _y = 3*winY/2 - _y;
	  int oriX, oriY;
	  oriX = 3 * winX/4;
	  oriY = 3 * winY/4;
	  double x, y;
	  x = _x - oriX;
	  y = _y - oriY;
	  // Scale par rapport au zoom
	  x = x / unitScaleX;
	  y = y / unitScaleY;
	  if (movePoint)
	  {
		PtsUV[du][dv][2] = x;
		PtsUV[du][dv][1] = y;
	  }
    }

    glutPostRedisplay();
}


void passiveMotionGL(int _x, int _y)
{
    glutPostRedisplay();
}

void drawRepere()
{
    glDisable(GL_LIGHTING);
    glLineWidth(1.0);
    // Axe X
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    glEnd();
    // Axe Y
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    glEnd();
    // Axe Z
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();
    glLineWidth(1.0);
    glEnable(GL_LIGHTING);
}

GLuint createBaseDL()
{
    GLuint id;
    id = glGenLists(1);
    if (!id) return id;

    glNewList(id, GL_COMPILE);
    drawRepere();
    glEndList();

    return id;
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

    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);

    pTexture = (GLubyte*) malloc(sizeof(GLubyte) * texture.size);
    BasculeImage(&texture, pTexture);

    glTexImage2D(GL_TEXTURE_2D,0,1,256,256,0,GL_LUMINANCE,GL_UNSIGNED_BYTE, pTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    mlTbInit(winX,winY);

    idBaseDL = createBaseDL();
}

void computeBarycentre(mlVec3** _ctrl, int _size_u, int _size_v, double * _barx, double * _bary, double * _barz)
{
    *_barx = *_bary = *_barz = 0;
    for (int v=0; v<size_v; v++)
    {
	  for (int u=0; u<size_u; u++)
	  {
		*_barx+= PtsUV[u][v][0];
		*_bary+= PtsUV[u][v][1];
		*_barz+= PtsUV[u][v][2];
	  }
    }
    *_barx /= size_u*size_v;
    *_bary /= size_u*size_v;
    *_barz /= size_u*size_v;
}

void adjustToOrigin(mlVec3 _ctrl[MAX][MAX], int _size_u, int _size_v)
{
    double barx = 0, bary = 0, barz = 0;
    for (int v=0; v<size_v; v++)
    {
	  for (int u=0; u<size_u; u++)
	  {
		barx+= PtsUV[u][v][0];
		bary+= PtsUV[u][v][1];
		barz+= PtsUV[u][v][2];
	  }
    }
    barx /= size_u*size_v;
    bary /= size_u*size_v;
    barz /= size_u*size_v;
    for (int v=0; v<size_v; v++)
    {
	  for (int u=0; u<size_u; u++)
	  {
		PtsUV[u][v][0] -= barx;
		PtsUV[u][v][1] -= bary;
		PtsUV[u][v][2] -= barz;
	  }
    }
}

int main(int _argc, char ** _argv)
{
    printf("\n[TP05] Modelisation Geometrique - Surfaces a poles\n\n");
    int posX, posY;

    glutInit(&_argc, _argv);

    loadMode = _argv[1];
    filename = _argv[2];

    vector<vector<Point3D> > vPts;

    if (loadMode == "-pts")
    {
	  vPts = loadFilePTS2(filename, &size_u, &size_v);
	  //PtsUV = T_Array2D<mlVec3>(size_u, size_v);
    }
    else if (loadMode == "-img")
    {
	  if (LireImage(filename.c_str(), &texture) == -1) return -1;
	  vPts = loadFilePGMtoPTS2(texture, &size_u, &size_v);
    }

    // Recopie dans tableau 2D
    for (int v=0; v<size_v; v++)
    {
	  for (int u=0; u<size_u; u++)
	  {
		mlVec3_Set(PtsUV[u][v], vPts[v][u].x, vPts[v][u].y, vPts[v][u].z);
	  }
    }
    vPts.clear();

    adjustToOrigin(PtsUV, size_u, size_v);

    posX = (glutGet(GLUT_SCREEN_WIDTH ) - winX) / 2;
    posY = (glutGet(GLUT_SCREEN_HEIGHT) - winY) / 2;

    glutInitWindowSize(winX, winY);
    glutInitWindowPosition(posX, posY);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("[TP05] Modelisation Geometrique - Surfaces a poles");

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
