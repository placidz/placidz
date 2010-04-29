#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include "OutilsPGM.h"

typedef struct
{
    int taille;
    double coef[];
} Filtre;

typedef struct
{
    int width, height, size;
    double *data;
} ImageD;

int f1, f2;
Image Im1, Im2;		// Images originales
GLubyte *I1=NULL, *I2=NULL;		// Image à afficher

double delta = 0.2;
double lambda = 1.1;


short ValMiror(Image *d, int x, int y)
{
    if(x < 0) x = -x - 1;
    if(x >= d->width) x = d->width - (x - d->width) - 1;
    if(y < 0) y = -y - 1;
    if(y >= d->height) y = d->height - (y - d->height ) - 1;

    return d->data[x + y*d->width];
}

double ValMirorD(ImageD *d, int x, int y)
{
    if(x < 0) x = -x - 1;
    if(x >= d->width) x = d->width - (x - d->width) - 1;
    if(y < 0) y = -y - 1;
    if(y >= d->height) y = d->height - (y - d->height ) - 1;

    return d->data[x + y*d->width];
}

void ModifierPixel(Image *d, int x, int y, short valeur)
{
    if(x>=0 && y>=0 && x<d->width && y<d->height)
	d->data[x + y*d->width]=valeur;
}

void ModifierPixelD(ImageD *d, int x, int y, double valeur)
{
    if(x>=0 && y>=0 && x<d->width && y<d->height)
	d->data[x + y*d->width]=valeur;
}

void CopierImage(Image *s, Image *d)
{
    int x, y;
    for (y = 0; y < s->height; y++)
	for (x = 0; x < s->width; x++)
	    ModifierPixel(d, x, y, ValMiror(s, x, y));
}

void CopierImageD(ImageD *s, ImageD *d)
{
    int x, y;
    for (y = 0; y < s->height; y++)
	for (x = 0; x < s->width; x++)
	    ModifierPixelD(d, x, y, ValMirorD(s, x, y));
}

void CopierImageStoD(Image *s, ImageD *d)
{
    int x, y;
    for (y = 0; y < s->height; y++)
	for (x = 0; x < s->width; x++)
	    ModifierPixelD(d, x, y, (double)ValMiror(s, x, y));
}

void CopierImageDtoS(ImageD *s, Image *d)
{
    int x, y;
    for (y = 0; y < s->height; y++)
	for (x = 0; x < s->width; x++)
	    ModifierPixel(d, x, y, (short)ValMirorD(s, x, y));
}

void BasculeImage(const Image *image, GLubyte *I)
{
    int i,j;
    for(i=0;i<image->height;i++)
	for(j=0;j<image->width;j++)
	    I[(image->height-1-i)*image->width+j]=(GLubyte) image->data[i*image->width+j];
    return;
}

int CreerImageD(ImageD *image, int largeur, int hauteur)
{
    image->width=largeur;
    image->height=hauteur;
    image->size=largeur*hauteur;
    image->data=(double*)malloc(image->size * sizeof(double));
    if (image->data == NULL)
    {
	printf ("ERREUR malloc (%d)\n", image->size * sizeof(double));
	return -1;
    }
    return 0;
}



double Distance(ImageD *im1, ImageD *im2)
{
    int i, j;
    double somme = 0;
    for (i=0; i<im1->width; i++)
	for (j=0; j<im1->height; j++)
	    somme += pow(ValMirorD(im1, i, j) - ValMirorD(im2, i, j), 2.);

    return (sqrt(somme)/((float)(im1->size)));
}

void Laplacien(ImageD * in, ImageD * out)
{
    int i, j;
    for (i=0; i<in->width; i++)
	for (j=0; j<in->height; j++)
	{
	    double val = ValMirorD(in, i-1, j) + ValMirorD(in, i+1, j) + ValMirorD(in, i, j-1)  + ValMirorD(in, i, j+1) - 4*ValMirorD(in, i, j);
	    ModifierPixelD(out, i, j, val);
	}
}

void ExpansionDynamique(Image *ori, Image *in, Image *out)
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
}

void Tikhonov(Image *dep, Image *res, double dt, double lam)
{
    int i,j;
    double dist = 100;
    int width = dep->width, height = dep->height;
    int size = dep->size;

    ImageD U1, U2, lap;
    CreerImageD(&U1, width, height);
    CreerImageD(&U2, width, height);
    CreerImageD(&lap, width, height);
    Image temp;
    CreerImage(&temp, width, height);

    std::cout<<"U1.size:"<<U1.size<<"   size:"<<size<<std::endl;

    CopierImageStoD(dep, &U1);
    while (dist > 0.05)
    {
	Laplacien(&U1, &lap);
	for (i=0; i<width; i++)
	    for (j=0; j<height; j++)
	    {
		double val;
		val = ValMirorD(&U1, i, j) + dt * ((double)ValMiror(dep, i, j) - ValMirorD(&U1, i, j) + lam * ValMirorD(&lap, i, j));
		ModifierPixelD(&U2, i, j, val);
	    }
	dist = Distance(&U1, &U2);
	printf("distance: %f\n", dist);
	CopierImageD(&U2, &U1);
    }
    CopierImageDtoS(&U1, &temp);
    ExpansionDynamique(dep, &temp, res);
}

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
	Tikhonov(&Im1, &Im2, delta, lambda);
	BasculeImage(&Im2, I2);
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
}

void souris (int button, int state, int x, int y)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
	break;
    }
}

void clavier (unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'd':
	delta -= 0.01;
	printf("dt: %f\n", delta);
	break;

	case 'f':
	delta += 0.01;
	printf("dt: %f\n", delta);
	break;

	case 'l':
	lambda -= 0.1;
	printf("lambda: %f\n", lambda);
	break;

	case 'm':
	lambda += 0.1;
	printf("lambda: %f\n", lambda);
	break;
	
		
	}
}

void CreerMenu(void)
{
    int sousMenu;

    glutCreateMenu(ChoixMenuPrincipal);

    glutAddMenuEntry("Tikhonov", 1);
    glutAddMenuEntry("EcrireImage", 10);
    glutAddMenuEntry("Quitter", 11);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc,char **argv)
{
    glutInit(&argc,argv);

    if (LireImage("lena.pgm", &Im1)==-1) return -1;

    CreerImage(&Im2, Im1.width, Im1.height);
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

    /* Transformation de l'image donnée en image à afficher */
    BasculeImage(&Im1, I1);
    BasculeImage(&Im1, I2);

    /*--------------------------Fenêtre ----------------------------*/
    glutInitWindowSize(Im1.width,Im1.height);
    glutInitWindowPosition(200,100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    f1 = glutCreateWindow("Originale");
    glutDisplayFunc(affichage);
    glutReshapeFunc(redim);
    glutMouseFunc(souris);
    glutKeyboardFunc(clavier);
    CreerMenu();
    initGL();

    glutInitWindowSize(Im1.width,Im1.height);
    glutInitWindowPosition(620,100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    f2 = glutCreateWindow("Filtree");
    glutDisplayFunc(affichage2);
    glutReshapeFunc(redim);
    glutMouseFunc(souris);
    glutKeyboardFunc(clavier);
    CreerMenu();
    initGL();

    glutMainLoop();
    return 0;
} 
