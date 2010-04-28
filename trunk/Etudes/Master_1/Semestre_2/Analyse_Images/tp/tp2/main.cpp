#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "OutilsPGM.h"

#define PI 3.14159265


typedef struct
{
	double Reel;
	double Imag;
} Complexe;


int f1, f2;
Image Im1, Im2, Im3;		// Images originales
GLubyte* I1=NULL, *I2=NULL;		// Image à afficher


Complexe *tComp;


void ModifierPixel(Image *d, int x, int y, short valeur)
{
	if(x>=0 && y>=0 && x<d->width && y<d->height)
		d->data[y*d->width+x]=valeur;
}

void CopierImage(Image *s, Image *d)
{
	int xi, yi;
	for (yi = 0; yi < s->height; yi++)
		for (xi = 0; xi < s->width; xi++)
			ModifierPixel(d, xi, yi, s->data[yi*s->width+xi]);
}

void BasculeImage(const Image *image, GLubyte *I)
{
	int i,j;
	for(i=0;i<image->height;i++)
		for(j=0;j<image->width;j++)
			I[(image->height-1-i)*image->width+j]=(GLubyte) image->data[i*image->width+j];
	return;
}

short ValMiror(Image *d, int x, int y)
{
	if(x < 0) x = -x - 1;
	if(x >= d->width) x = d->width - (x - d->width) - 1;
	if(y < 0) y = -y - 1;
	if(y >= d->height) y = d->height - (y - d->height ) - 1;
	  
	return d->data[y*d->width+x];
}

double getModule(double a, double b)
{
	return (sqrt(pow(a, 2) + pow(b, 2)));
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
	//int tab[8] = { 3, 5, 2, 3, 6, 7, 1, 0 };
	//tComp[in->width][in->height];
	Complexe comp;
	//Complexe **tComp;

	Complexe tComp[in->width][in->height];
	// Allocation de l'espace mémoire nécessaire lorsque tu connais la taille
	//tComp =(Complexe **)malloc(sizeof(Complexe *)*in->width);

	/*for (int i = 0; i < in->height; i++)
		tComp[i]=(Complexe *)malloc(sizeof(Complexe)*in->height);*/

	//val32=tableau[2][1]; // valeur pour la 3eme ligne, 2eme colonne

	double T = in->height;

	/*for (int k = 0; k < 8; k++)
	{
		comp.Reel = 0;
		comp.Imag = 0;
		for (int i = 0; i < 8; i++)
		{
			comp.Reel += tab[i] * cos(2*PI*i*k/T);
			comp.Imag += tab[i] * sin(2*PI*i*k/T);
		}
		printf("Reel: %f - Imag: %f\n", comp.Reel/8, comp.Imag/8); 
	}*/

	for (int y = 0; y < in->height; y++)
	{
		for (int x = 0; x < in->width; x++)
		{
			comp.Reel = 0;
			comp.Imag = 0;
			for (int i = 0; i < in->width; i++)
			{
				comp.Reel += (ValMiror(in, x, y) * cos(-2*PI*i*x/T));
				comp.Imag += (ValMiror(in, x, y) * sin(-2*PI*i*x/T));
			}
			printf("Reel: %f - Imag: %f\n", comp.Reel, comp.Imag);
			tComp[x][y].Reel = comp.Reel;
			tComp[x][y].Imag = comp.Imag;
		}
	}

	T = in->width;

	for (int x = 0; x < in->width; x++)
	{
		for (int y = 0; y < in->height; y++)
		{
			comp.Reel = 0;
			comp.Imag = 0;
			for (int i = 0; i < in->height; i++)
			{
				Complexe c;
				c.Reel = cos(-2*PI*i*x/T);
				c.Imag = sin(-2*PI*i*x/T);
				c = multComplexes(tComp[x][y], c);
				comp.Reel += c.Reel;
				comp.Imag += c.Imag;
				/*comp.Reel += (tComp[x][y].Reel * cos(2*PI*i*x/T));
				comp.Imag += (tComp[x][y].Imag * sin(2*PI*i*x/T));*/
			}
			ModifierPixel(out, x, y, getModule(comp.Reel, comp.Imag));
		}
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

    a = (short)(maxi2-mini2)/(maxi1-mini1);
    b = (short)(-maxi2*mini1 + maxi1*mini2)/(maxi1-mini1);
    out->size = in->size;
    for (i = 0; i < out->size; i++)
    {
	out->data[i] = (short)(a*in->data[i] + b);
    }
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
			TransformationFourier(&Im1, &Im2);
			ExpansionDynamique(&Im1, &Im2, &Im3);
			BasculeImage(&Im3, I2);
			glutSetWindow(f2);
			glutPostRedisplay();
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
	glutInit(&argc,argv);
	
	if (LireImage("carre_bis.pgm",&Im1)==-1) return -1;
	
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

	/* Transformation de l'image donnée en image à afficher */
	BasculeImage(&Im1, I1);
	BasculeImage(&Im1, I2);
	
    /*--------------------------Fenêtre ----------------------------*/
	glutInitWindowSize(Im1.width,Im1.height);
	glutInitWindowPosition(200,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f1 = glutCreateWindow("TP4");
	glutDisplayFunc(affichage);
	glutReshapeFunc(redim);
	glutMouseFunc(souris);
	CreerMenu();
	initGL();

	/*--------------------------Fenêtre ----------------------------*/
	glutInitWindowSize(Im1.width,Im1.height);
	glutInitWindowPosition(220+Im1.width,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f2 = glutCreateWindow("TP4");
	glutDisplayFunc(affichage2);
	glutReshapeFunc(redim);
	glutMouseFunc(souris);
	CreerMenu();
	initGL();

	glutMainLoop();
	return 0;
} 
