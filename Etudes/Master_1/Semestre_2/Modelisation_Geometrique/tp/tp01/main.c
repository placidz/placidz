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


#include <ml.h>


int winX = 800;
int winY = 600;


void drawCurve(mlVec4 _cx, mlVec4 _cy, mlVec4 _cz, int _nPoints)
{
}


void computeHermiteCoeff(mlVec3 _p1, mlVec3 _r1, mlVec3 _p4, mlVec3 _r4)
{
}


void displayGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
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
	glutPostRedisplay();
}


void keyboardGL(unsigned char _k, int _x, int _y)
{
	if(_k == 27 || _k == 'q' || _k == 'Q')
		exit(0);

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
	glutMainLoop();

	return 0;
}
