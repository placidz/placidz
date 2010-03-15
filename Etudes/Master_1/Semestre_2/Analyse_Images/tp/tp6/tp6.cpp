#include <stdio.h>
#include <stdlib.h>
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

Filtre flt_moyen;
Filtre flt_median;
Filtre flt_nagao;

int f1, f2;
Image Im1, Im2;		// Images originales
GLubyte *I1=NULL, *I2=NULL;		// Image à afficher


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

double Distance(ImageD *im1, ImageD *im2)
{
	int i, j;
	double somme = 0;
	for (i=0; i<im1->width; i++)
		for (j=0; j<im1->height; j++)
		{
			somme += pow((im1->data[i+im1->height*j] - im2->data[i+im2->height*j]), 2);
		}
	return (sqrt(somme));
}

void Laplacien(ImageD *im1, ImageD *im2)
{
	int i, j;
	for (i=0; i<im1->width; i++)
		for (j=0; j<im1->height; j++)
		{
			im2->data[i+j*im1->height] = im1->data[(i-1)+j*im1->height] 
										+ im1->data[(i+1)+j*im1->height] 
										+ im1->data[(i)+(j-1)*im1->height] 
										+ im1->data[(i)+(j+1)*im1->height] 
										- 4*im1->data[(i)+(j)*im1->height];
		}
}

void ExpansionDynamique(Image *ori, ImageD *in, Image *out)
{
	int i, j;
	int width = in->width, height = in->height;
	int maxi1 = -10000000, mini1 = 10000000;
	int maxi2 = -10000000, mini2 = 10000000;
	for (j=0; j< height; j++)
		for (i = 0; i < width; i++)
		{
			if (ValMiror(ori, i, j) < mini2) mini2 = ValMiror(ori, i, j);
			if (ValMiror(ori, i, j) > maxi2) maxi2 = ValMiror(ori, i, j);
		}
	for (j=0; j< height; j++)
		for (i = 0; i < width; i++)
		{
			if (in->data[j*height+i] < mini1) mini1 = (int)in->data[j*height+i];
			if (in->data[j*height+i] > maxi1) maxi1 = (int)in->data[j*height+i];
		}
	short a, b;
	
	a = (maxi2-mini2)/(maxi1-mini1);
	b = (-maxi2*mini1 + maxi1*mini2)/(maxi1-mini1);
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
	ImageD temp;
	U1.width = U2.width = lap.width = width;
	U1.height = U2.height = lap.height = height;
	U1.size = U2.size = lap.size = size;
	
	U1.data = new double[size];
	U2.data = new double[size];
	lap.data = new double[size];
	temp.data = new double[size];
	
	
	
	for (j=0; j<height; j++)
		for (i = 0; i < width; i++)
		{
			U1.data[j*height+i] = (double)(ValMiror(dep, i, j));
		}
	while (dist > 0.05)
	{
		Laplacien(&U1, &lap);
		for (i=0; i<width; i++)
			for (j=0; j<height; j++)
			{
				U2.data[i+j*height] = U1.data[i+j*height] + dt * ((double)ValMiror(dep, i, j) - U1.data[j+i*height] + lam * lap.data[i+j*height]);
			}
		dist = Distance(&U1, &U2);
		for (j=0; j<height; j++)
			for (i = 0; i < width; i++)
			{
				U1.data[j*height+i] = (double)(U2.data[j*height+i]);
			}
	}
	for (j=0; j<height; j++)
		for (i = 0; i < width; i++)
		{
			temp.data[j*height+i] = (short)(U2.data[j*height+i]);
		}
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
			Tikhonov(&Im1, &Im2, 0.2, 2);
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
	
	flt_moyen.taille = 3;
	flt_moyen.coef[flt_moyen.taille*flt_moyen.taille];
	
	//flt_moyen.coef = malloc(sizeof(double)*flt_moyen.taille*flt_moyen.taille);
	//flt_median.coef = malloc(sizeof(double)*flt_moyen.taille*flt_moyen.taille);
	
	for (int i = 0; i < 9; i++)
	{
		flt_moyen.coef[i] = 1.0 / 9;
		printf("%f \n", flt_moyen.coef[i]);
	}
	
	if (LireImage("lena.pgm",&Im1)==-1) return -1;
	
	CreerImage(&Im2, Im1.width,Im1.height);
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
	CreerMenu();
	initGL();
	
	glutInitWindowSize(Im1.width,Im1.height);
	glutInitWindowPosition(620,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f2 = glutCreateWindow("Filtree");
	glutDisplayFunc(affichage2);
	glutReshapeFunc(redim);
	glutMouseFunc(souris);
	CreerMenu();
	initGL();

	glutMainLoop();
	return 0;
} 