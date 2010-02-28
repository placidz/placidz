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
int affPoints = 0;
int modeColoration = GL_SMOOTH;
float camX=0.0, camY=0.0, camZ=15.0;
float tx=0.0, ty=0.0, tz=0.0;

void drawRectangle()
{
	glShadeModel(modeColoration);
	glBegin(GL_POLYGON);
	glPolygonMode(GL_FRONT, modeRendu);
		glColor3f(0.0, 0.0, 0.0);
		glVertex3f(-2.0, 1.0, 0.0);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(2.0, 1.0, 0.0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(2.0, -1.0, 0.0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(-2.0, -1.0, 0.0);
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
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(tx, ty, tz);
	glBegin(GL_TRIANGLES);
		glVertex3f(5.0, 0.0, 0.0);
		glVertex3f(7.0, 0.0, 0.0);
		glVertex3f(6.0, 2.0, 0.0);
	glEnd();
}


void displayGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	drawTriangle();
	drawRectangle();
	if (affPoints) drawPoints();
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
	
	glutPostRedisplay();
}


void mouseGL(int _button, int _state, int _x, int _y)
{
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
			affPoints = !affPoints;
		break;
		
		case 4:
			modeColoration = GL_SMOOTH;
		break;
		
		case 5:
			modeColoration = GL_FLAT;
		break;
	
		case 11 :
			exit(0); /* On quitte */
		break;
	} 
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
	glutAddMenuEntry("Quitter", 11);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void keyboardGL(unsigned char _k, int _x, int _y)
{
	if(_k == 27)
		exit(0);
	
	switch(_k)
	{
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
