#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#endif

#include <ml.h>


#define MAX	50
#define MAX2	1000

mlVec3 ctrl		[MAX];
mlVec3 bP  		[MAX2];

int nCtrl = 0; /* nombre de points de contrôle */
int nBP = 5; /* nombre de points de la courbe */
int degreBS = 3;

int affBSpline = 0;
int bMovePoint = 0;

int winX	  = 800;
int winY	  = 600;
int mouseX;
int mouseY;

int indPt = -1;
int mouseX, mouseY;

int fact(int _i)
{
	if (_i > 1) return (_i*fact(_i-1));
    else return (1);
}


double computeRiesenfeldCoeff(int _i, int _m, double _t)
{
	int k;
	double somme = 0;
	for (k = 0; k <= _m-_i; k++)
	{
		somme += pow(-1, k) * (pow(_t+_m-_i-k, _m)/(fact(k)*fact(_m-k+1)));
	}
	return ((_m+1)*somme);
}


void computeBSplinePoint(mlVec3 _ctrl[MAX], int _m, int _k, double _t, mlVec3 _q)
{
	int i;
	float x = 0, y = 0, z = 0;
	for (i = 0; i <= _m; i++)
	{
		double riesenfeldCoeff = computeRiesenfeldCoeff(i, _m, _t);
		x += riesenfeldCoeff * _ctrl[i+_k][0];
		y += riesenfeldCoeff * _ctrl[i+_k][1];
		z += riesenfeldCoeff * _ctrl[i+_k][2];
	}
	mlVec3_Set(_q, x, y, z);
}


void computeBSplineCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP, int _m)
{
	int i, k, sz;
	sz = _nCtrl - _m;
	for(k = 0; k < sz; k++)
	{
		for(i = k*(nBP+1); i < ((_nBP+1)*(k+1)); i++)
		{
			computeBSplinePoint(_ctrl, _m, k, i/(double)(_nBP*(k+1)), _bP[i]);
		}
	}
}


void drawBSplineCurve(mlVec3 _bP[MAX2], int _nBP)
{
	int i;
	int k;
	k = nCtrl - degreBS;
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 0.0, 1.0);

	for (i = 0; i < (_nBP+1)*k; i++)
	{
		glVertex3f(_bP[i][0], winY-_bP[i][1], _bP[i][2]);
	}
	glEnd();
}


void drawPoints(void)
{
	int i;
	for (i = 0; i < nCtrl; i++)
	{
		glBegin(GL_POINTS);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(ctrl[i][0], winY-ctrl[i][1]);
		glEnd();
	}
}

int trouverPoint(int x, int y)
{
	int i, ind = -1;
	int Px, Py;
	for (i = 0; i < nCtrl && ind == -1; i++)
	{
		Px = ctrl[i][0];
		Py = ctrl[i][1];
		
		if (Px >= x-10 && Px <= x+10 && Py >= y-10 && Py <= y+10)
			ind = i;
	}
	return ind;
}

void displayGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (affBSpline && nCtrl > degreBS)
	{
		computeBSplineCurve(ctrl, nCtrl, bP, nBP, degreBS);
		drawBSplineCurve(bP, nBP);
	}
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


void mouseGL(int _button, int _state, int _x, int _y)
{
	switch (_button)
	{
		case GLUT_LEFT_BUTTON:
		if (_state == GLUT_DOWN)
		{
			indPt = trouverPoint(_x, _y);
			if (indPt != -1)
			{
				bMovePoint = 1;
			}
			else
			{
				if (nCtrl < MAX)
				{
					mlVec3_Set(ctrl[nCtrl], _x, _y, 0);
					nCtrl++;
				}
			}
		}
		if (_state == GLUT_UP) 
		{
			bMovePoint = 0;
			indPt = -1;
		}
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
		break;

		case 'N':
			nBP++;
		break;
		
		case 'd':
			degreBS--;
		break;

		case 'D':
			degreBS++;
		break;

		case 'e':
			nCtrl = 0;
		break;
		
		case 'l':
		break;

		case 'b':
			affBSpline = !affBSpline;
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
	if (bMovePoint)
    {
        ctrl[indPt][0] = _x;
        ctrl[indPt][1] = _y;
    }

	glutPostRedisplay();
}


void passiveMotionGL(int _x, int _y)
{
	mouseX = _x;
	mouseY = _y;
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
	glutCreateWindow("[TP03] Modelisation Geometrique - Courbes BSplines");

	glutDisplayFunc(displayGL);
	glutReshapeFunc(reshapeGL);

	glutMouseFunc(mouseGL);
	glutKeyboardFunc(keyboardGL);
	glutSpecialFunc(keyboardSpecialGL);

	glutMotionFunc(motionGL);
	glutPassiveMotionFunc(passiveMotionGL);

	glPointSize(5.0);

	initGL();
	glutMainLoop();

	return 0;
}
