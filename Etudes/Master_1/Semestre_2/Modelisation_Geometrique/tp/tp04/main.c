#include <stdlib.h>
#include <stdio.h>
#include <ml.h>


#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#endif


int winX = 800;
int winY = 600;
int modeRendu = GL_FILL;

int modeColoration = GL_SMOOTH;
float camX=0.0, camY=0.0, camZ=10.0;
float tx=0.0, ty=0.0, tz=0.0;

int bModeProjection = 1;
int bAffPoints = 0;
int bAffRectangle = 1;
int bAffBox = 0;

void drawRepere()
{
	glLineWidth(2.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
	glEnd();
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
	glEnd();
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 1.0);
	glEnd();
	glLineWidth(1.0);
}

void drawRectangle()
{
	glPolygonMode(GL_FRONT, modeRendu);
	glShadeModel(modeColoration);
	glBegin(GL_POLYGON);
		glColor3f(0.0, 0.0, 0.0);
		
		glVertex3f(2.0, 1.0, 0.0);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-2.0, 1.0, 0.0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(-2.0, -1.0, 0.0);
		
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(2.0, -1.0, 0.0);
	glEnd();
}

void drawPoints()
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
		glVertex3f(-2.0, 1.0, 0.0);
		glVertex3f(2.0, 1.0, 0.0);
		glVertex3f(2.0, -1.0, 0.0);
		glVertex3f(-2.0, -1.0, 0.0);
	glEnd();
	glPointSize(1.0);
}

void drawTriangle()
{
	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(tx, ty, tz);
	glBegin(GL_TRIANGLES);
		glVertex3f(5.0, 0.0, 0.0);
		glVertex3f(7.0, 0.0, 0.0);
		glVertex3f(6.0, 2.0, 0.0);
	glEnd();
	glPopMatrix();
}

void drawBox(float _size)
{
	float s = _size;
	glPolygonMode(GL_BACK, GL_LINE);
	/* Face avant */
	glBegin(GL_QUADS);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(s, -s, s);
		glVertex3f(s, s, s);
		glVertex3f(-s, s, s);
		glVertex3f(-s, -s, s);
	glEnd();
	/* Face arrière */
	glBegin(GL_QUADS);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(-s, s, -s);
		glVertex3f(s, s, -s);
		glVertex3f(s, -s, -s);
		glVertex3f(-s, -s, -s);
	glEnd();
	/* Face basse */
	glBegin(GL_QUADS);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(-s, -s, s);
		glVertex3f(-s, -s, -s);
		glVertex3f(s, -s, -s);
		glVertex3f(s, -s, s);
	glEnd();
	/* Face droite */
	glBegin(GL_QUADS);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(s, -s, s);
		glVertex3f(s, -s, -s);
		glVertex3f(s, s, -s);
		glVertex3f(s, s, s);
	glEnd();
	/* Face gauche */
	glBegin(GL_QUADS);
		glColor3f(0.0, 1.0, 1.0);
		glVertex3f(-s, -s, s);
		glVertex3f(-s, s, s);
		glVertex3f(-s, s, -s);
		glVertex3f(-s, -s, -s);
	glEnd();
}

void displayGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if (bModeProjection)
	{
		gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}
	glMultMatrixd(mlTbGetRotation());
	
	if (bAffPoints) drawPoints();
	if (bAffRectangle) drawRectangle();
	if (bAffBox) drawBox(2);
	drawTriangle();
	drawRepere();
	glutSwapBuffers();
}


void reshapeGL(int _w, int _h)
{
	winX = _w;
	winY = _h;
	
	glViewport(0, 0, winX, winY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if (bModeProjection)
	{
		gluPerspective(60.0, (GLfloat)_w/(GLfloat)_h, 0.1, 200.0);
	}
	else
	{
		glOrtho(-10, 10, -10, 10, 0.1, 200);
	}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glutPostRedisplay();
}


void mouseGL(int _button, int _state, int _x, int _y)
{
	switch (_button)
	{
		case GLUT_LEFT_BUTTON:
			if (_state==GLUT_DOWN)
			{
				mlTbClick(_x,_y);
			}
			if (_state==GLUT_UP)
			{
				mlTbRelease(_x,_y);
			}
		break ;
		case GLUT_RIGHT_BUTTON:
		break;
		default:
		break;
	}
	glutPostRedisplay();
}

void ChoixMenuPrincipal(int value)
{
	switch (value)
	{
		case 1:
			modeRendu = GL_LINE;
		break;
		
		case 2:
			modeRendu = GL_FILL;
		break;
		
		case 3:
			bAffPoints = !bAffPoints;
		break;
		
		case 4:
			modeColoration = GL_SMOOTH;
		break;
		
		case 5:
			modeColoration = GL_FLAT;
		break;
		
		case 6:
			bModeProjection = !bModeProjection;
		break;
	
		case 11 :
			exit(0); /* On quitte */
		break;
	}
	glutPostRedisplay();
}

void CreerMenu(void)
{
	glutCreateMenu(ChoixMenuPrincipal);

	glutAddMenuEntry("Rendu - Fil de fer", 1);
	glutAddMenuEntry("Rendu - Face opaque", 2);
	glutAddMenuEntry("--------------------", -1);
	glutAddMenuEntry("Afficher les points", 3);
	glutAddMenuEntry("--------------------", -1);
	glutAddMenuEntry("Couleurs - Interpolation", 4);
	glutAddMenuEntry("Couleurs - Unique", 5);
	glutAddMenuEntry("--------------------", -1);
	glutAddMenuEntry("Mode de Projection", 6);
	glutAddMenuEntry("--------------------", -1);
	glutAddMenuEntry("Quitter", 11);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void keyboardGL(unsigned char _k, int _x, int _y)
{
	if(_k == 27)
		exit(0);
	
	switch(_k)
	{
		/* Déplacement de la caméra */
		case 'o':
			camY+= 1.0;
		break;
		case 'l':
			camY-= 1.0;
		break;
		case 'k':
			camX-= 1.0;
		break;
		case 'm':
			camX+= 1.0;
		break;
		case 'i':
			camZ+= 1.0;
		break;
		case 'j':
			camZ-= 1.0;
		break;
		
		/* Déplacement du triangle */
		case 'q':
			tx-= 1.0;
		break;
		case 'd':
			tx+= 1.0;
		break;
		case 'z':
			ty+= 1.0;
		break;
		case 's':
			ty-= 1.0;
		break;
		case 'r':
			tz+= 1.0;
		break;
		case 'f':
			tz-= 1.0;
		break;
		
		case 'b':
			bAffRectangle = !bAffRectangle;
			bAffBox = !bAffBox;
			bAffPoints = 0;
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
	mlTbMotion(_x,_y);
	glutPostRedisplay();
}


void passiveMotionGL(int _x, int _y)
{
	glutPostRedisplay();
}


void initGL()
{
	glClearColor(0.8, 0.8, 0.7, 1.0);
	glEnable(GL_DEPTH_TEST);
	mlTbInit(winX,winY);
}


int main(int _argc, char ** _argv)
{
	int posX, posY;
	
	glutInit(&_argc, _argv);	
	
	posX = (glutGet(GLUT_SCREEN_WIDTH ) - winX) / 2;
	posY = (glutGet(GLUT_SCREEN_HEIGHT) - winY) / 2;
	
	glutInitWindowSize(winX, winY);
	glutInitWindowPosition(posX, posY);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH| GLUT_DOUBLE);
	glutCreateWindow("[TP04] Modelisation Geometrique - OpenGL en 3D");
	
	glutDisplayFunc(displayGL);
	glutReshapeFunc(reshapeGL);
	
	glutMouseFunc(mouseGL);
	glutKeyboardFunc(keyboardGL);
	glutSpecialFunc(keyboardSpecialGL);
	
	glutMotionFunc(motionGL);
	glutPassiveMotionFunc(passiveMotionGL);
	
	CreerMenu();
		
	initGL();
	glutMainLoop();

	return 0;
}
