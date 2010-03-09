#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <unistd.h>
extern "C"
{
#include "include/OutilsPGM.h"
}

#define VRAI 0
#define FAUX 255

typedef int tabTranscode[512];

Image Im,Im2,Im3;          	     // Images originales
GLubyte *I1=NULL,*I2=NULL;   // Images � afficher
int f1,f2;		     // ID fen�tres
int ordre=5;		     // Taille �l�ment structurant
int k = 0;


void BasculeImage(const Image *image, GLubyte *I)
{
     int i,j;
     for(i=0;i<image->height;i++)
       for(j=0;j<image->width;j++)
         I[(image->height-1-i)*image->width+j]=(GLubyte) image->data[i*image->width+j];
     return;
}

void affichage1(void)
{
    glRasterPos2i(0,0);
    glDrawPixels(Im.width,Im.height,GL_LUMINANCE,GL_UNSIGNED_BYTE,I1);
    glFlush();
}

void AffecterIm(Image *d,int x,int y,short valeur)
{
	if(x>=0 && y>=0 && x<d->width && y<d->height)
	  d->data[y*d->width+x]=valeur;
}

short ValMiror(Image *d,int x,int y)
{
	if(x<0)
	  x=abs(x);
	if(x>=d->width)
	  x=d->width-(x-d->width)-1;
	if(y<0)
	  y=abs(y);
	if(y>=d->height)
	  y=d->height-(y-d->height)-1;
	return d->data[y*d->width+x];
}


void CopieImg(Image& in, Image& out){
	for(int i=0;i<in.height;i++){
		for(int j=0;j<in.width;j++){
			out.data[i*Im.width+j] = in.data[i*Im.width+j];
		}
	}
}

void InitImg(Image& in){
	for(int i=0;i<in.height;i++){
		for(int j=0;j<in.width;j++){
			in.data[i*Im.width+j] = FAUX;
		}
	}
}

void affichage2(void)
{
    glRasterPos2i(0,0);
    glDrawPixels(Im2.width,Im2.height,GL_LUMINANCE,GL_UNSIGNED_BYTE,I2);
    glFlush();
}

void initGL(void)
{
     glPixelStorei(GL_UNPACK_ALIGNMENT,1);
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

void ChoixSousMenu(int value)
{
     ordre=value;
}

void ChoixMenuPrincipal(int value)
{
	switch (value)
	{
		case 6:
		break;
		case 10 :// EcrireImage("res.pgm",&Im2);
		break;
		case 11 : //LibererImage(&Im);
		//LibererImage(&Im2);
		free(I1);
		free(I2);
		exit(0); /* On quitte */
		break;
	}
	glutPostRedisplay();
}


void CreerMenu(void)
{
	/*int sousMenu;

	sousMenu = glutCreateMenu(ChoixSousMenu);
	glutAddMenuEntry("1",1); 
	glutAddMenuEntry("2",2);
	glutAddMenuEntry("3",3);
	glutAddMenuEntry("4",4);
	glutAddMenuEntry("5",5);*/
	
	glutCreateMenu(ChoixMenuPrincipal);
	//glutAddSubMenu("Ordre",sousMenu);
	glutAddMenuEntry("Filtrage",6);
	glutAddMenuEntry("Quitter",11);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	
	/* Lecture de "bin0.pgm"*/
	if (LireImage(argv[1],&Im)==-1) return -1;
	
	CreerImage(&Im2,Im.width,Im.height);

	/* Allocation de la memoire des images � afficher */
	if ( (I1=(GLubyte *) malloc(sizeof(GLubyte)*Im.width*Im.height)) == NULL)
	{
		printf("Impossible d'allouer I1\n");
		return -1;
	}
	if ( (I2=(GLubyte *) malloc(sizeof(GLubyte)*Im.width*Im.height)) == NULL)
	{
		printf("Impossible d'allouer I2\n");
		return -1;
	}

	BasculeImage(&Im,I1);
	BasculeImage(&Im,I2);

    	/*--------------------------Fen�tre 1----------------------------*/
	glutInitWindowSize(Im.width,Im.height);
	glutInitWindowPosition(400,200);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f1=glutCreateWindow("Originale");
	glutDisplayFunc(affichage1);
	glutReshapeFunc(redim);
	CreerMenu();
	initGL();

	/*--------------------------Fen�tre 2----------------------------*/
	glutInitWindowSize(Im2.width,Im2.height);
	glutInitWindowPosition(700,200);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f2=glutCreateWindow("Filtree");
	glutDisplayFunc(affichage2);
	glutReshapeFunc(redim);
	initGL();
	
	glutMainLoop();
	return 0;
}
