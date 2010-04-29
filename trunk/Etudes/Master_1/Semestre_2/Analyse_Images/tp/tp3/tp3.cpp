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


Point tab[MAX];
int nbPts = 0;

Point Translation(Point p, int a, int b)
{
    Point r;
    r.x = p.x + a;
    r.y = p.y + b;
    return r;
}

Point RotationO(Point p, float ang)
{
    Point r;
    ang = PI * ang / 180;
    r.x = cos(ang) * p.x - sin(ang) * p.y;
    r.y = sin(ang) * p.x + cos(ang) * p.y;
    return r;
}

Point RotationC(Point p, Point c, float ang)
{
    Point r;
    r = Translation(p, -c.x, -c.y);
    r = RotationO(r, ang);
    r = Translation(r, c.x, c.y);
    return r;
}

Point Homothetie(Point p, float scale)
{
    Point r;
    r.x = p.x * scale;
    r.y = p.y * scale;
    return r;
}

Point SymetrieX(Point p)
{
    Point r;
    r.x = p.x;
    r.y = p.y * -1;
    return r;
}

Point SymetrieO(Point p)
{
    Point r;
    r.x = p.x * -1;
    r.y = p.y * -1;
    return r;
}

Point HomothetieC(Point p, Point c, float scale)
{
    Point r;
    int x, y;
    x = c.x;
    y = c.y;
    r = Translation(p, -c.x, -c.y);
    r = Homothetie(r, scale);
    r = Translation(r, c.x, c.y);
    return r;
}

void initGL(void)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glPointSize(5.0);
}

void drawPoints(void)
{
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 0; i < nbPts; i++)
    {
	glBegin(GL_POINTS);
	if (i == 0) glColor3f(1.0, 1.0, 0.0);
	else glColor3f(0.0, 0.0, 0.0);
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
    drawRepere();
    drawPoints();
    glutSwapBuffers();
}

void redim(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(-400, 400, -300, 300);
    gluOrtho2D(-winX/2, winX/2, -winY/2, winY/2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void ChoixMenuPrincipal(int value)
{
    switch (value)
    {
    case 1:
	tab[0] = Translation(tab[0], 10, -10);
	break;

    case 2:
	tab[0] = RotationO(tab[0], 90);
	break;

    case 3:
	tab[0] = Homothetie(tab[0], 3);
	break;

    case 4:
	tab[0] = SymetrieX(tab[0]);
	break;

    case 5:
	tab[0] = SymetrieO(tab[0]);
	break;

    case 6:
	tab[0] = HomothetieC(tab[0], tab[1], 3);
	break;

    case 7:
	tab[0] = RotationC(tab[0], tab[1], 90);
	break;

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
	    if (nbPts<2)
	    {
		printf("x: %d - y: %d\n", x, y);
		AjouterPoint(x, y);
	    }
	}
	break;
    }
    glutPostRedisplay();
}

void clavier(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27:
	exit(0);
	break;

    case 'e':
	nbPts = 0;
	break;
    }
    glutPostRedisplay();
}

void CreerMenu(void)
{
    glutCreateMenu(ChoixMenuPrincipal);

    glutAddMenuEntry("Translation", 1);
    glutAddMenuEntry("RotationO", 2);
    glutAddMenuEntry("RotationC", 7);
    glutAddMenuEntry("HomothetieO", 3);
    glutAddMenuEntry("HomothetieC", 6);
    glutAddMenuEntry("SymetrieX", 4);
    glutAddMenuEntry("SymetrieO", 5);

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
    glutCreateWindow("TP03 - Transformations");
    glutDisplayFunc(affichage);
    glutReshapeFunc(redim);
    glutKeyboardFunc(clavier);
    glutMouseFunc(souris);
    CreerMenu();
    initGL();

    glutMainLoop();
    return 0;
} 
