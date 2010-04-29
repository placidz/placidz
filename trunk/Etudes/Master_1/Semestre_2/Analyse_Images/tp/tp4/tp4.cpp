#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.14159265
#define MAX 100

int winX = 800, winY = 600;

typedef struct 
{
	int x, y, t;
} Point;

typedef struct
{
	float x, y, z, t;
} Point3D;

Point3D p1, p2, p3;
Point tab[MAX];
int nbPts = 0;

float angleX = 0.0, angleY = 0.0, angleZ = 0.0;

int d = 130;

void initGL(void)
{
     glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

void PointProjete(Point3D &p)
{
	p.x = (d*p.x)/(d+p.z);
	p.y = (d*p.y)/(d+p.z);
	p.z = 0.0;
	p.t = 1.0;
}

Point3D BarycentreTriangle(Point3D _p1, Point3D _p2, Point3D _p3)
{
	Point3D tmp;
	tmp.x = (_p1.x + _p2.x + _p3.x) / 3;
	tmp.y = (_p1.y + _p2.y + _p3.y) / 3;
	tmp.z = (_p1.z + _p2.z + _p3.z) / 3;
	
	return tmp;
}

void Translation3D(Point3D &p, float x, float y, float z)
{
	p.x += x;
	p.y += y;
	p.z += z;
}

void RotationX(Point3D &p, float angle)
{
	Point3D tmp = p;
	p.x = tmp.x;
	p.y = tmp.y*cos(angle*PI/180) - (tmp.z*sin(angle*PI/180));
	p.z = tmp.y*sin(angle*PI/180) + tmp.z*cos(angle*PI/180);
}

void RotationY(Point3D &p, float angle)
{
	Point3D tmp = p;
	p.x = tmp.x*cos(angle*PI/180) + tmp.z*sin(angle*PI/180);
	p.y = tmp.y;
	p.z = (-tmp.x*sin(angle*PI/180)) + tmp.z*cos(angle*PI/180);
}

void RotationZ(Point3D &p, float angle)
{
	Point3D tmp = p;
	p.x = tmp.x*cos(angle*PI/180) - (tmp.y*sin(angle*PI/180));
	p.y = tmp.x*sin(angle*PI/180) + tmp.y*cos(angle*PI/180);
	p.z = tmp.z;
}

void drawTriangle(void)
{
	Point3D pb;
	Point3D p1p,p2p,p3p;
	p1p.x = p1.x;
	p1p.y = p1.y;
	p1p.z = p1.z;
	p2p.x = p2.x;
	p2p.y = p2.y;
	p2p.z = p2.z;
	p3p.x = p3.x;
	p3p.y = p3.y;
	p3p.z = p3.z;
	//Translation
	pb = BarycentreTriangle(p1, p2, p3);
	//printf("x: %f , y: %f , z: %f", p.x, p.y, p.z);
	Translation3D(p1p, -pb.x, -pb.y, -pb.z);
	Translation3D(p2p, -pb.x, -pb.y, -pb.z);
	Translation3D(p3p, -pb.x, -pb.y, -pb.z);
	//RotationX
	RotationX(p1p, angleX);
	RotationX(p2p, angleX);
	RotationX(p3p, angleX);
	//RotationY
	RotationY(p1p, angleY);
	RotationY(p2p, angleY);
	RotationY(p3p, angleY);
	//RotationZ
	RotationZ(p1p, angleZ);
	RotationZ(p2p, angleZ);
	RotationZ(p3p, angleZ);
	//Tranlation-1
	Translation3D(p1p, pb.x, pb.y, pb.z);
	Translation3D(p2p, pb.x, pb.y, pb.z);
	Translation3D(p3p, pb.x, pb.y, pb.z);
	//Projection
	PointProjete(p1p);
	PointProjete(p2p);
	PointProjete(p3p);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(p1p.x, p1p.y);
		glColor3f(0.0, 1.0, 0.0);
		glVertex2f(p2p.x, p2p.y);
		glColor3f(0.0, 0.0, 1.0);
		glVertex2f(p3p.x, p3p.y);
	glEnd();
	glPointSize(5.0);
	/*PointProjete(pb);
	glBegin(GL_POINTS);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(pb.x, pb.y);
	glEnd();*/
	printf("angleX: %f, angleY: %f, angleZ: %f\n", angleX, angleY, angleZ);
}

void drawPoints(void)
{
	glColor3f(0.0, 0.0, 0.0);
	for (int i = 0; i < nbPts; i++)
	{
		glBegin(GL_POINTS);
		tab[i].t = 1;
		glVertex3i(tab[i].x/tab[i].t, (-1* tab[i].y)/tab[i].t, tab[i].t);
		glEnd();
	}
}

void drawRepere(void)
{
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES); // ligne x
		glVertex2i(-winX/2, 0);
		glVertex2i(winX/2, 0);
	glEnd();
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES); // ligne y
		glVertex2i(0, -winY/2);
		glVertex2i(0, winY/2);
	glEnd();
}

void affichage(void)
{
	glClearColor(0.8, 0.8, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//drawRepere();
	drawPoints();
	drawTriangle();
	glutSwapBuffers();
}

void redim(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0, width, 0, height);
	gluOrtho2D(-winX/2, winX/2, -winY/2, winY/2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ChoixMenuPrincipal(int value)
{
	switch (value)
	{
		case 11 :
			exit(0); /* On quitte */
		break;
	} 
	glutPostRedisplay();
}

void AjouterPoint(int x, int y)
{
	Point p;
	p.x = x;
	p.y = y;
	p.t = 1;
	tab[nbPts] = p;
	nbPts++;
	printf("nbPts: %d\n", nbPts);
}

void souris (int button, int state, int x, int y)
{
	x = x-winX/2;
	y = y-winY/2;
	switch(button)
	{
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) 
			{
				printf("x: %d - y: %d\n", x, y);
				AjouterPoint(x, y);
			}
		break;
	}
	glutPostRedisplay();
}

void clavier(unsigned char _k, int _x, int _y)
{
	double vitesse = 5.0;
	if(_k == 27 || _k == 'q' || _k == 'Q')
		exit(0);
	switch(_k)
	{
		case 'x':
			angleX+=vitesse;
		break;
		case 'w':
			angleX-=vitesse;
		break;
		
		case 'y':
			angleY+=vitesse;
		break;
		case 't':
			angleY-=vitesse;
		break;
		
		case 'z':
			angleZ+=vitesse;
		break;
		case 'a':
			angleZ-=vitesse;
		break;
	}

	glutPostRedisplay();
}

void CreerMenu(void)
{
	glutCreateMenu(ChoixMenuPrincipal);

	glutAddMenuEntry("Translation", 1);
	glutAddMenuEntry("RotationO", 2);
	glutAddMenuEntry("Homothetie", 3);
	glutAddMenuEntry("SymetrieX", 4);
	glutAddMenuEntry("SymetrieO", 5);
	glutAddMenuEntry("HomothetieC", 6);
	glutAddMenuEntry("Quitter", 11);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	
    /*--------------------------Fenêtre ----------------------------*/
	glutInitWindowSize(winX, winY);
	glutInitWindowPosition(200,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("TP04 - Projection");
	glutDisplayFunc(affichage);
	glutReshapeFunc(redim);
	glutMouseFunc(souris);
	glutKeyboardFunc(clavier);
	CreerMenu();
	initGL();

	glPointSize(5.0);
	
	p1.x = 80;
	p1.y = -20;
	p1.z = 10;

	p2.x = -80;
	p2.y = -20;
	p2.z = 10;

	p3.x = 0;
	p3.y = 90;
	p3.z = 10;

	glutMainLoop();
	return 0;
} 
