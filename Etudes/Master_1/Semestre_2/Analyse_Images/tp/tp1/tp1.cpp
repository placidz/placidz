#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glut.h>

using namespace std;

#define wWidth 400
#define wHeight 300

#define PI 3.14159265
#define NB_PTS_MAX 100

typedef struct 
{
	int x;
	int y;
} Point;

typedef struct
{
	float theta;
	float rho;
} ptHough;

int f1, f2, f3;

vector<Point> PTS;
ptHough ptH;

bool bDrawLine = 0;

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

void OneToMany()
{
	printf("/*** OneToMany() ***/\n");
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < (int)PTS.size(); i++)
		for (float a = -90; a < 90; a+= 0.1)
		{
			ptHough ph;
			ph.theta = a;
			ph.rho = PTS.at(i).x*cos(ph.theta*PI/180) + PTS.at(i).y*sin(ph.theta*PI/180);
			//printf("theta: %f  -  rho: %f\n", ph.theta, ph.rho);
			glBegin(GL_POINTS);
				glVertex2f(ph.theta+100, ph.rho+150);
			glEnd();
		}
	printf("/*** OneToMany() - FIN ***/\n");
}

void ManyToOne()
{
	printf("/*** ManyToOne() ***/\n");
	glColor3f(1.0, 1.0, 1.0);
	Point p1, p2;
	double a, b, c;
	for (int j = 0; j < (int)PTS.size()-1; j++)
	{
		p1.x = PTS.at(j).x;
		p1.y = PTS.at(j).y;
		for (int i = j+1; i < (int)PTS.size(); i++)
		{
			p2.x = PTS.at(i).x;
			p2.y = PTS.at(i).y;
			ptHough ph;
			a = p1.x * p2.y - p2.x * p1.y;
			b = p2.x * p2.x - 2 * p2.x * p1.x + p1.x * p1.x + p2.y * p2.y - 2 * p2.y * p1.y + p1.y * p1.y;
			c = pow((double)p1.y-p2.y,2);
			ph.rho=-a/((p1.y-p2.y)*sqrt(b/c));
			ph.theta = -atan((double)(PTS.at(i).x - PTS.at(j).x)/(PTS.at(i).y - PTS.at(j).y));
			printf("theta: %f  -  rho: %f\n", ph.theta*180/PI, ph.rho);
			glBegin(GL_POINTS);
				glVertex2f((ph.theta*180/PI)+100, ph.rho+150);
			glEnd();
		}
	}
	printf("/*** ManyToOne() - FIN ***/\n");
}

void AfficherPoints(void)
{
	printf("/*** AfficherPoints() ***/\n");
	glPointSize(5);
	glColor3f(0.0, 0.0, 1.0);
	for (int i = 0; i < (int)PTS.size(); i++)
	{
		//printf("x: %d  -  y: %d\n", PTS.at(i).x, PTS.at(i).y);
		glBegin(GL_POINTS);
			glVertex2i(PTS.at(i).x, PTS.at(i).y);
		glEnd();
	}
	printf("/*** AfficherPoints() - FIN ***/\n");
}

void drawLine()
{
	Point a, b;
	a.x = 0;
	a.y = ptH.rho / sin(ptH.theta);
	
	b.x = wWidth;
	b.y = (ptH.rho - (b.x*cos(ptH.theta))) / sin(ptH.theta);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex2f(a.x, a.y);
	glVertex2f(b.x, b.y);
	glEnd();
}

void affichage (void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (bDrawLine) drawLine();
	AfficherPoints();
	glFlush();
}

void affichage2 (void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	OneToMany();
	ManyToOne();
	glFlush();
}

void affichage3 (void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	ManyToOne();
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
		
		case 'e':
			PTS.clear();
		break;
	}
	glutPostRedisplay();
	glutSetWindow(f2);
	glutPostRedisplay();
	glutSetWindow(f3);
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
				Point p;
				p.x = x; 
				p.y = y;
				PTS.push_back(p);
			}
		break;

		case GLUT_MIDDLE_BUTTON:

		break;

		case GLUT_RIGHT_BUTTON:

		break;
	}
	glutPostRedisplay();
	glutSetWindow(f2);
	glutPostRedisplay();
	glutSetWindow(f3);
	glutPostRedisplay();
}

void souris3(int button, int state, int x, int y)
{
	//y = wHeight - y;
	switch(button)
	{
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
			{
				ptH.theta = (x-100)*PI/180; 
				ptH.rho = y - 150;
				bDrawLine = 1;
				//glVertex2f((ph.theta*180/PI)+100, ph.rho+150);
			}
		break;

		case GLUT_MIDDLE_BUTTON:

		break;

		case GLUT_RIGHT_BUTTON:

		break;
	}
	glutSetWindow(f1);
	glutPostRedisplay();
	glutSetWindow(f2);
	glutPostRedisplay();
	glutSetWindow(f3);
	glutPostRedisplay();
}


int main (int argc, char **argv)
{
	glutInit(&argc, argv);
	
	/*** Fenetre 1 ***/
	glutInitWindowSize(wWidth, wHeight);
	glutInitWindowPosition(200, 100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f1 = glutCreateWindow("[TP1] Analyse d'Images - Transformation de Hough");
	glutDisplayFunc(affichage);
	glutReshapeFunc(redim);
	glutKeyboardFunc(clavier);
	glutMouseFunc(souris);
	
	/*** Fenetre 2 ***/
	glutInitWindowSize(wWidth/2, wHeight);
	glutInitWindowPosition(220+wWidth, 100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f2 = glutCreateWindow("One to Many");
	glutDisplayFunc(affichage2);
	glutReshapeFunc(redim);
	glutKeyboardFunc(clavier);
	glutMouseFunc(souris3);
	
	/*** Fenetre 3 ***/
	glutInitWindowSize(wWidth/2, wHeight);
	glutInitWindowPosition((int)(240+wWidth*1.5), 100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f3 = glutCreateWindow("Many to One");
	glutDisplayFunc(affichage3);
	glutReshapeFunc(redim);
	glutKeyboardFunc(clavier);
	glutMouseFunc(souris3);

	glutMainLoop();

	return 0;
}
