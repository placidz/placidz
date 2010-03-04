 #include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#endif

#define MAX 100

extern "C" {
	#include <ml.h>		
}

void computeHermiteCoeff(mlVec3 _p1, mlVec3 _pA, mlVec3 _p2, mlVec3 _pB);
void drawCurve(mlVec4 _cx, mlVec4 _cy, mlVec4 _cz, int _nPoints);

int winX = 800;
int winY = 600;

int MouseX;
int MouseY;

mlVec3 PTS[MAX];
mlVec3 VEC[MAX];

mlVec4 cx, cy, cz;
mlVec3 P1, P2, R1, R2, V1, V2;
mlMat4 Mh;
int nbPts = 0;
int nbVec = 0;
int bCreationVecteur = 0;
int bDrawTangents = 1;



void computeHermiteCoeff(mlVec3 _p1, mlVec3 _pA, mlVec3 _p2, mlVec3 _pB)
{
	mlVec3 v1, v2;
	mlVec4 Ghx, Ghy, Ghz;
	
	mlVec3_SubVec(v1, _pA, _p1);
	mlVec3_SubVec(v2, _pB, _p2);
	
	mlVec4_Set(Ghx, _p1[0], _p2[0], v1[0], v2[0]);
	mlVec4_Set(Ghy, _p1[1], _p2[1], v1[1], v2[1]);
	mlVec4_Set(Ghz, _p1[2], _p2[2], v1[2], v2[2]);
	
	mlVec4_MultMat(cx, Ghx, Mh);
	mlVec4_MultMat(cy, Ghy, Mh);
	mlVec4_MultMat(cz, Ghz, Mh);
	
	drawCurve(cx, cy, cz, 100);
}

void drawCurve(mlVec4 _cx, mlVec4 _cy, mlVec4 _cz, int _nPoints)
{
	float x, y, z, t, t2, t3;
	float pas = (1.0 / (_nPoints+2));
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (t = 0; t <= 1; t+= pas)
	{
		t2 = t*t;
		t3 = t2*t;
		/*printf("t = %f \n", t);*/
		x = (_cx[0]*t3 + _cx[1]*t2 + _cx[2]*t + _cx[3]);
		y = (_cy[0]*t3 + _cy[1]*t2 + _cy[2]*t + _cy[3]);
		glVertex2i(x, winY-y);
		
	}
	glEnd();
}


void drawContinuousCurve()
{
	int i;
	for (i = 1; i < nbVec; i++)
	{
		computeHermiteCoeff(PTS[i-1], VEC[i-1], PTS[i], VEC[i]);
	}
}

void drawPoints()
{
	int i;
	if (nbPts > 0)
	{
		glPointSize(5);
		glColor3f(1.0, 0.0, 0.0);
		for (i = 0; i < nbPts; i++)
		{
			glBegin(GL_POINTS);
				glVertex3f(PTS[i][0], winY-PTS[i][1], PTS[i][2]);
			glEnd();
		}
		glColor3f(0.0, 0.0, 1.0);
		for (i = 0; i < nbVec; i++)
		{
			glBegin(GL_POINTS);
				glVertex3f(VEC[i][0], winY-VEC[i][1], VEC[i][2]);
			glEnd();
		}
	}
}

void drawDynamicConstruction(int x, int y)
{
	if (bCreationVecteur)
	{
		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_LINES);
			glVertex2i(PTS[nbVec][0], winY - PTS[nbVec][1]);
			glVertex2i(x, y);
		glEnd();
		glBegin(GL_POINTS);
			glVertex2i(x, y);
		glEnd();
	}
	else
	{
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POINTS);
			glVertex2i(x, y);
		glEnd();
	}
}

void drawTangents()
{
	int i;
	for (i = 0; i < nbVec; i++)
	{
		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_LINES);
			glVertex2i(PTS[i][0], winY - PTS[i][1]);
			glVertex2i(VEC[i][0], winY - VEC[i][1]);
		glEnd();
	}
}

void displayGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawContinuousCurve();
	drawDynamicConstruction(MouseX, winY - MouseY);
	if (bDrawTangents) drawTangents();
	drawPoints();
	glutSwapBuffers();
}


void reshapeGL(int _w, int _h)
{
	winX = _w;
	winY = _h;
	
	glViewport(0, 0, winX, winY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluOrtho2D(0.0, winX, 0.0, winY);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glutPostRedisplay();
}

void createFirstSegment(int _x, int _y)
{
	if (!bCreationVecteur)
	{
		mlVec3_Set(PTS[nbPts], _x, _y, 0);
		nbPts++;
		if (nbPts != 0 && nbPts%2 == 0) bCreationVecteur = 1;
	}
	else
	{
		mlVec3_Set(VEC[nbVec], _x, _y, 0);
		nbVec++;
		if (nbVec%2 == 0) bCreationVecteur = 0;
	}
}

void createNextSegment(int _x, int _y)
{
	if (!bCreationVecteur)
	{
		mlVec3_Set(PTS[nbPts], _x, _y, 0);
		nbPts++;
		bCreationVecteur = 1;
	}
	else
	{
		mlVec3_Set(VEC[nbVec], _x, _y, 0);
		nbVec++;
		bCreationVecteur = 0;
	}
}


void mouseGL(int _button, int _state, int _x, int _y)
{
	int i;
	/*_y = winY - _y;*/
	if(_button == GLUT_LEFT && _state == GLUT_DOWN)
	{
		if (nbVec<2) createFirstSegment(_x, _y);
		else createNextSegment(_x, _y);
	}
	glutPostRedisplay();
}


void keyboardGL(unsigned char _k, int _x, int _y)
{
	if(_k == 27 || _k == 'q' || _k == 'Q')
		exit(0);
		
	switch (_k)
	{
		case 'e':
			nbPts = 0;
			nbVec = 0;
			bCreationVecteur = 0;
		break;
		
		case 't':
			bDrawTangents = !bDrawTangents;
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
	glutPostRedisplay();
}


void passiveMotionGL(int _x, int _y)
{
	MouseX = _x;
	MouseY = _y;

	glutPostRedisplay();
}


void initGL()
{
	glClearColor(0.8, 0.8, 0.7, 1.0);
}


int main(int _argc, char ** _argv)
{
	int posX, posY;
	
	glutInit(&_argc, _argv);	
	
	posX = (glutGet(GLUT_SCREEN_WIDTH ) - winX) / 2;
	posY = (glutGet(GLUT_SCREEN_HEIGHT) - winY) / 2;
	
	glutInitWindowSize(winX, winY);
	glutInitWindowPosition(posX, posY);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("[TP01] Modelisation Geometrique - Courbes hermitiennes cubiques");
	
	glutDisplayFunc(displayGL);
	glutReshapeFunc(reshapeGL);
	
	glutMouseFunc(mouseGL);
	glutKeyboardFunc(keyboardGL);
	glutSpecialFunc(keyboardSpecialGL);
	
	glutMotionFunc(motionGL);
	glutPassiveMotionFunc(passiveMotionGL);
		
	initGL();
	mlMat4_Set(Mh,	2.0, -3.0, 0.0, 1.0, 
		   	-2.0, 3.0, 0.0, 0.0, 
		   	1.0, -2.0, 1.0, 0.0, 
		   	1.0, -1.0, 0.0, 0.0);
		   	
	glutMainLoop();

	return 0;
}
