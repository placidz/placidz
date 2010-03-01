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


#define MAX		20
#define MAX2	100


int winX	  = 800;
int winY	  = 600;

int indPt = -1;
int bMovePoint = 0;

int affBezierCurve = 0;
int affDeCasteljauCurve = 0;
int affConstructionDeCasteljau = 0;
int creaPt = 0;
float tConstruction = 0.3;

mlVec3 ctrl[MAX]; /* tableau des points de contrôle */
int nCtrl = 0; /* nombre de points de contrôle */

mlVec3 bP[MAX2]; /* tableau des points de la courbe */
int nBP = 5; /* nombre de points de la courbe */
mlVec3 bPdC[MAX2];
int nBPdC = 5;

int TP[MAX][MAX];
mlVec3 castel[MAX][MAX];

void computeTriangleDePascal(void)
{
	int i, n;
	for (n = 0; n < MAX; n++)
		for (i = 0; i < MAX; i++)
		{
			if (i == 0) TP[n][i] = 1;
			else if (i == n) TP[n][i] = 1;
			else TP[n][i] = 0;
		}

	for (n = 0; n < MAX; n++)
		for (i = 0; i < MAX; i++)
		{
			if (i < n && i > 0)
			{
				TP[n][i] = TP[n-1][i-1] + TP[n-1][i];
			}
		}
}


double fact(int _i)
{
     if (_i > 1) return (_i*fact(_i-1));
     else return (1);
}


double computeBernsteinCoeff(int _n, int _i, double _t)
{
	return ((fact(_n) / (fact(_i)*fact(_n-_i)))*((double)pow((double)_t, _i)*(double)pow((double)1-_t, _n-_i)));
}


void computeBezierPoint(mlVec3 _ctrl[MAX], int _nCtrl, double _t, mlVec3 _q)
{
	int i, j;
	float x = 0, y = 0, z = 0;
	double coeffBernstein = 0;
	for(i = 0; i < _nCtrl; i++)
	{
		coeffBernstein = computeBernsteinCoeff(_nCtrl-1, i, _t);
		x += (_ctrl[i][0] * coeffBernstein);
		y += (_ctrl[i][1] * coeffBernstein);
		z += (_ctrl[i][2] * coeffBernstein);
	}
	mlVec3_Set(_q, x, y, z);
}


void computeBezierCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP)
{
	int i;
	for(i = 0; i < _nBP+1; i++)
	{
		computeBezierPoint(_ctrl, _nCtrl, i/(double)_nBP, _bP[i]);
	}
}


void drawBezierCurve(mlVec3 _bP[MAX2], int _nBP)
{
	int i;
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 0.0, 1.0);
	for (i = 0; i < _nBP+1; i++)
	{
		glVertex3f(_bP[i][0], winY-_bP[i][1], _bP[i][2]);
	}
	glEnd();
}


void computeDeCasteljau(mlVec3 _ctrl[MAX], int _nCtrl, double _t, mlVec3 _castel[MAX][MAX])
{
	int i, j;
	float x, y;
	for (i = 0; i < _nCtrl; i++)
		mlVec3_Set(_castel[i][0], _ctrl[i][0], _ctrl[i][1], _ctrl[i][2]);

	for (j = 1; j < _nCtrl; j++)
	{
		for (i = 0; i < _nCtrl-j; i++)
		{
			x = _t*_castel[i+1][j-1][0] + (1-_t)*_castel[i][j-1][0];
			y = _t*_castel[i+1][j-1][1] + (1-_t)*_castel[i][j-1][1];
			mlVec3_Set(_castel[i][j], x, y, 0);
		}
	}
}


void drawConstruction(mlVec3 _castel[MAX][MAX], int _n)
{
	int i, j;
	int cpt =0;
	computeDeCasteljau(ctrl, nCtrl, tConstruction, _castel);
	glColor3f(0.4, 0.4, 0.4);
	for (j = 0; j < _n-1; j++)
	{
		for (i = 0; i < _n-j-1; i++)
		{
			glBegin(GL_LINE_STRIP);
				glVertex2f(_castel[i][j][0], winY-_castel[i][j][1]);
				glVertex2f(_castel[i+1][j][0], winY-_castel[i+1][j][1]);
			glEnd();
		}
	}
	glColor3f(0.5, 0.5, 0.5);
	glPointSize(4.0);
	for (j = 1; j < _n; j++)
	{
		for (i = 0; i < _n-j; i++)
		{
			glBegin(GL_POINTS);
				glVertex2f(_castel[i][j][0], winY-_castel[i][j][1]);;
			glEnd();
		}
	}
	glPointSize(5.0);
}

void computeDeCasteljauCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP)
{
	int i;
	float x, y;
	for(i = 0; i < _nBP+1; i++)
	{
		computeDeCasteljau(_ctrl, _nCtrl, i/(double)_nBP, castel);
		x = castel[0][_nCtrl-1][0];
		y = castel[0][_nCtrl-1][1];
		mlVec3_Set(_bP[i], x, y, 0);
	}
}

void drawDeCasteljauCurve(mlVec3 _bP[MAX2], int _nBP)
{
	int i;
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0, 0.0, 0.0);
	for (i = 0; i < _nBP+1; i++)
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

int trouverPoint(int _x, int _y)
{
    int i, ind = -1;
	int Px, Py;
	for (i = 0; i < nCtrl && ind == -1; i++)
	{
		Px = ctrl[i][0];
		Py = ctrl[i][1];
		if (Px >= _x-10 && Px <= _x+10 && Py >= _y-10 && Py <= _y+10)
			ind = i;
	}
	return ind;
}


void displayGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (affBezierCurve)
	{
		computeBezierCurve(ctrl, nCtrl, bP, nBP);
		drawBezierCurve(bP, nBP);
	}
	if (affDeCasteljauCurve)
	{
		computeDeCasteljauCurve(ctrl, nCtrl, bPdC, nBPdC);
		drawDeCasteljauCurve(bPdC, nBPdC);
		if (affConstructionDeCasteljau) drawConstruction(castel, nCtrl);
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
	/*_y = winY - _y;*/
	switch (_button)
	{
		case GLUT_LEFT:
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
                if (bMovePoint) bMovePoint = 0;
            }

		break;
	}
	glutPostRedisplay();
}


void keyboardGL(unsigned char _k, int _x, int _y)
{
	switch(_k)
	{
		case 27:
			exit(0);
		break;

		case 'n':
			nBP--;
		break;

		case 'N':
			nBP++;
		break;

		case 'm':
			nBPdC--;
		break;

		case 'M':
			nBPdC++;
		break;

		case 'o':
			if (tConstruction > 0.1) tConstruction-=0.1;
		break;

		case 'O':
			if (tConstruction < 1.0) tConstruction+=0.1;
		break;

		case 'e':
			nCtrl = 0;
		break;

		case 'b':
			nBP = 5;
			affBezierCurve = !affBezierCurve;
		break;

		case 'c':
			nBP = 5;
			affDeCasteljauCurve = !affDeCasteljauCurve;
		break;

		case 'a':
			if (affDeCasteljauCurve) affConstructionDeCasteljau = !affConstructionDeCasteljau;
		break;

		case 't':
			computeTriangleDePascal();
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
	glutCreateWindow("[TP02] Modelisation Geometrique - Courbes de Bezier");

	glutDisplayFunc(displayGL);
	glutReshapeFunc(reshapeGL);

	glutMouseFunc(mouseGL);
	glutKeyboardFunc(keyboardGL);
	glutSpecialFunc(keyboardSpecialGL);

	glutMotionFunc(motionGL);
	glutPassiveMotionFunc(passiveMotionGL);

	initGL();
	glPointSize(5.0);

	/*computeTriangleDePascal();*/
	glutMainLoop();

	return 0;
}
