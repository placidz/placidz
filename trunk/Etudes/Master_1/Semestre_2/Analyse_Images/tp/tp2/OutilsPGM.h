#ifndef OUTILSPGM_H
#define OUTILSPGM_H

#include <math.h>

#define MAXLIGNE 256
#define NB_NV_GRIS_SOBEL 2048

typedef short Pixel;

typedef struct
{
    int width, height, size;
    Pixel *data;
} Image;

typedef struct
{
    int width, height, size;
    double *data;
} ImageD;

typedef struct {
	unsigned long nb_pixels;
	double max;
	int classeMax;
	int taille;
	double *t;

}Histo;


typedef struct {
	int taille;
	float *coef;
} FiltreLineaire;

int CreerImage(Image *image, int largeur, int hauteur);
int CreerImageD(ImageD *image, int largeur, int hauteur);
int LireImage(char *nom, Image *image);
int EcrireImage(char *nom, Image *image);
void LibererImage(Image *i);

short ValMiror(Image *d, int x, int y);
double ValMirorD(ImageD *d, int x, int y);
void ModifierPixel(Image *d, int x, int y, short valeur);
void ModifierPixelD(ImageD *d, int x, int y, double valeur);
void CopierImage(Image *s, Image *d);
void CopierImageD(ImageD *s, ImageD *d);
void CopierImageStoD(Image *s, ImageD *d);
void CopierImageDtoS(ImageD *s, Image *d);
double Distance(ImageD *im1, ImageD *im2);
void Laplacien(ImageD * in, ImageD * out);
void ExpansionDynamique(Image *ori, Image *in, Image *out);
void expDynamique(Image *in);

 #endif
