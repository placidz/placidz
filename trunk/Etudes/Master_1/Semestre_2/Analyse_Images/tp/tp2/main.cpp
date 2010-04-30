#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <limits.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <string>
#include "OutilsPGM.h"

#define PI 3.14159265

using namespace std;

typedef struct
{
    double Reel;
    double Imag;
} Complexe;


int f1, f2, f3;
Image Im1, Im2, Im3;		// Images originales
GLubyte* I1=NULL, *I2=NULL, *I3=NULL;		// Image à afficher

Complexe tCompX[100000];
Complexe tCompXY[100000];


double Min(double a, double b)
{
    return ((a < b) ? a:b);
}

double Max(double a, double b)
{
    return ((a > b) ? a:b);
}

void ExpansionDynamique2(ImageD *in, Image *out)
{
    CreerImage(out, in->width, in->height);
    double val;
    double xmin = 999999, xmax = -999999;
    for (int i = 0; i < in->size; i++)
    {
	val = in->data[i];
	if (val < xmin) xmin = val;
	if (val > xmax) xmax = val;
    }
    printf("min: %f  - max: %f\n", xmin, xmax);
    for (int i = 0; i < out->size; i++)
    {
	val = in->data[i];
	out->data[i] = (short)(Max(0, Min(255, (val-xmin)*255/(xmax-xmin))));
    }
}

void BasculeImage(const Image *image, GLubyte *I)
{
    int i,j;
    for(i=0;i<image->height;i++)
	for(j=0;j<image->width;j++)
	    I[(image->height-1-i)*image->width+j]=(GLubyte) image->data[i*image->width+j];
    return;
}

double getModule(double a, double b)
{
    double res = sqrt(pow(a, 2) + pow(b, 2) + 1);
    return (log(res));
}

Complexe multComplexes(Complexe a, Complexe b)
{
    Complexe c;
    c.Reel = a.Reel * b.Reel - a.Imag * b.Imag;
    c.Imag = a.Reel * b.Imag + a.Imag * b.Reel;
    return c;
}

void TransformationFourier(Image *in, Image *out)
{
    ImageD tmpOut;
    CreerImage(out, in->width, in->height);
    CreerImageD(&tmpOut, in->width, in->height);

    Complexe comp;
    double N = (double)in->width;
    for(int i = 0; i < in->height; i++)
    {
	for(int k = 0; k < in->width; k++)
	{
	    comp.Reel = 0.0;
	    comp.Imag = 0.0;
	    for(int n = 0; n < in->width; n++)
	    {
		double val = ValMiror(in, n, i);
		double ang = (double)(-2*M_PI*k*((double)n/N));
		comp.Reel += val * cos(ang);
		comp.Imag += val * sin(ang);
	    }
	    comp.Reel = (1/N) * comp.Reel;
	    comp.Imag = (1/N) * comp.Imag;

	    tCompX[i*in->width+k].Reel = comp.Reel;
	    tCompX[i*in->width+k].Imag = comp.Imag;
	}
    }

    N = (double)in->height;
    for(int i = 0; i < in->width; i++)
    {
	for(int k = 0; k < in->height; k++)
	{
	    comp.Reel = 0.0;
	    comp.Imag = 0.0;
	    for(int n = 0; n < in->height; n++)
	    {
		Complexe c, tmp;
		double ang = (double)(-2*M_PI*k*((double)n/N));
		c.Reel = cos(ang);
		c.Imag = sin(ang);
		tmp = multComplexes(tCompX[n*in->width+i], c);
		comp.Reel += tmp.Reel;
		comp.Imag += tmp.Imag;
	    }
	    comp.Reel = (1/N) * comp.Reel;
	    comp.Imag = (1/N) * comp.Imag;

	    tCompXY[k*in->width+i].Reel = comp.Reel;
	    tCompXY[k*in->width+i].Imag = comp.Imag;
	}
    }

    for (int i = 0; i < in->size; i++)
    {
	double a = tCompXY[i].Reel;
	double b = tCompXY[i].Imag;
	tmpOut.data[i] = getModule(a, b);
    }
    ExpansionDynamique2(&tmpOut, out);
}

void ReplacerFrequences(Image *in, Image *out)
{
    // Quart 1
    for (int i = 0; i < in->width/2; i++)
	for (int j = 0; j < in->height/2; j++)
	    ModifierPixel(out, i + in->width/2, j + in->height/2, ValMiror(in, i, j));

    // Quart 2
    for (int i = in->width/2; i < in->width; i++)
	for (int j = 0; j < in->height/2; j++)
	    ModifierPixel(out, i - in->width/2, j + in->height/2, ValMiror(in, i, j));;

    // Quart 3
    for (int i = 0; i < in->width/2; i++)
	for (int j = in->height/2; j < in->height; j++)
	    ModifierPixel(out, i + in->width/2, j - in->height/2, ValMiror(in, i, j));;

    // Quart 4
    for (int i = in->width/2; i < in->width; i++)
	for (int j = in->height/2; j < in->height; j++)
	    ModifierPixel(out, i - in->width/2, j - in->height/2, ValMiror(in, i, j));
}

/*void ExpansionDynamique(Image *ori, Image *in, Image *out)
{
    int i, j;
    int width = in->width, height = in->height;
    double maxi1 = -10000000, mini1 = 10000000;
    double maxi2 = -10000000, mini2 = 10000000;
    for (j=0; j< height; j++)
	for (i = 0; i < width; i++)
	{
	if (ValMiror(ori, i, j) < mini2) mini2 = ValMiror(ori, i, j);
	if (ValMiror(ori, i, j) > maxi2) maxi2 = ValMiror(ori, i, j);
    }
    for (j=0; j< height; j++)
	for (i = 0; i < width; i++)
	{
	if (ValMiror(in, i, j) < mini1) mini1 = ValMiror(in, i, j);
	if (ValMiror(in, i, j) > maxi1) maxi1 = ValMiror(in, i, j);
    }
    short a, b;

    a = (double)(maxi2-mini2)/(maxi1-mini1);
    b = (double)(-maxi2*mini1 + maxi1*mini2)/(maxi1-mini1);
    out->size = in->size;
    for (i = 0; i < out->size; i++)
    {
	out->data[i] = (short)(a*in->data[i] + b);
    }
}*/

void initGL(void)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

void affichage(void)
{
    glRasterPos2i(0,0);
    glDrawPixels(Im1.width, Im1.height, GL_LUMINANCE, GL_UNSIGNED_BYTE, I1);
    glFlush();
}

void affichage2(void)
{
    glRasterPos2i(0,0);
    glDrawPixels(Im1.width, Im1.height, GL_LUMINANCE, GL_UNSIGNED_BYTE, I2);
    glFlush();
}

void affichage3(void)
{
    glRasterPos2i(0,0);
    glDrawPixels(Im3.width, Im3.height, GL_LUMINANCE, GL_UNSIGNED_BYTE, I3);
    glFlush();
}

void redim(int width,int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,width,0,height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void ChoixMenuPrincipal(int value)
{
    switch (value)
    {
    case 1:
	TransformationFourier(&Im1, &Im2);
	BasculeImage(&Im2, I2);
	ReplacerFrequences(&Im2, &Im3);
	BasculeImage(&Im3, I3);
	break;

    case 10 :
	EcrireImage("nomImage.pgm", &Im2);
	break;

    case 11 :
	LibererImage(&Im1);
	free(I1);
	exit(0); /* On quitte */
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
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
	break;
    }
}

void CreerMenu(void)
{
    glutCreateMenu(ChoixMenuPrincipal);

    glutAddMenuEntry("LOL", 1);
    glutAddMenuEntry("Quitter", 11);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc,char **argv)
{
    glutInit(&argc, argv);

    //std::string filename = argv[0];
    char* filename = argv[1];
    printf("Fichier : '%s'\n", filename);

    if (LireImage(filename, &Im1) == -1) return -1;
    //if (LireImage("fourier1.pgm", &Im1) == -1) return -1;

    CreerImage(&Im2, Im1.width,Im1.height);
    CreerImage(&Im3, Im1.width,Im1.height);
    //CopierImage(&Im, &Im2);

    /* Allocation de la memoire des images à afficher */
    if ( (I1=(GLubyte *) malloc(sizeof(GLubyte)*Im1.width*Im1.height)) == NULL)
    {
	printf("Impossible d'allouer I1\n");
	return -1;
    }
    if ( (I2=(GLubyte *) malloc(sizeof(GLubyte)*Im1.width*Im1.height)) == NULL)
    {
	printf("Impossible d'allouer I2\n");
	return -1;
    }

    if ( (I3=(GLubyte *) malloc(sizeof(GLubyte)*Im1.width*Im1.height)) == NULL)
    {
	printf("Impossible d'allouer I3\n");
	return -1;
    }

    /* Transformation de l'image donnée en image à afficher */
    BasculeImage(&Im1, I1);
    BasculeImage(&Im1, I2);
    BasculeImage(&Im1, I3);

    /*--------------------------Fenêtre ----------------------------*/
    glutInitWindowSize(Im1.width,Im1.height);
    glutInitWindowPosition(200,100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    f1 = glutCreateWindow("Originale");
    glutDisplayFunc(affichage);
    glutReshapeFunc(redim);
    glutMouseFunc(souris);
    CreerMenu();
    initGL();

    /*--------------------------Fenêtre ----------------------------*/
    glutInitWindowSize(Im1.width,Im1.height);
    glutInitWindowPosition(200 + (20+Im1.width)*1,100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    f2 = glutCreateWindow("TF Discrete");
    glutDisplayFunc(affichage2);
    glutReshapeFunc(redim);
    glutMouseFunc(souris);
    CreerMenu();
    initGL();

    /*--------------------------Fenêtre ----------------------------*/
    glutInitWindowSize(Im1.width,Im1.height);
    glutInitWindowPosition(200 + (20+Im1.width)*2,100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    f3 = glutCreateWindow("Apres replacement des frequences");
    glutDisplayFunc(affichage3);
    glutReshapeFunc(redim);
    glutMouseFunc(souris);
    CreerMenu();
    initGL();

    glutMainLoop();
    return 0;
}
