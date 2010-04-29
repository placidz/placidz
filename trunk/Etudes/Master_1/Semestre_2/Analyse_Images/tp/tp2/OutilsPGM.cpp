#include <stdio.h>
#include <stdlib.h>
#include "OutilsPGM.h"

int CreerImage(Image *image, int largeur, int hauteur)
{
    image->width=largeur;
    image->height=hauteur;
    image->size=largeur*hauteur;
    image->data=(Pixel*)malloc(image->size * sizeof(Pixel));
    if (image->data == NULL)
    {
	printf ("ERREUR malloc (%d)\n", image->size * sizeof(Pixel));
	return -1;
    }
    return 0;
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

int LireImage(char *nom, Image *image)
{
    FILE *f;
    char s[MAXLIGNE];
    int width, height, maxgrey, n, k;
    unsigned char ligne[2048];

    /* Ouverture du fichier */
    f = fopen (nom, "rb");
    if (f == NULL)
    {
	printf ("ERREUR fopen(%s)\n", nom);
	return -1;
    }

    /* Lecture MagicValue
         * On evite fscanf pour eviter buffer overflow */
    fgets (s, MAXLIGNE, f);
    if (s[0] != 'P' || s[1] != '5')
    {
	printf ("ERREUR MagicValue <> P5\n");
	fclose (f);
	return -1;
    }

    /* Lecture width height
         * On saute les ligne commen�ant par un '#' */
    do fgets (s, MAXLIGNE, f);
    while (s[0] == '#');
    sscanf (s, "%d %d", &width, &height);

    /* Lecture maxgrey
         * On ne tient pas compte de maxgrey, qui est presque toujours 255.
         * On saute les ligne commen�ant par un '#' */
    do fgets (s, MAXLIGNE, f);
    while (s[0] == '#');
    sscanf (s, "%d", &maxgrey);

    /* Allocation Image */
    if (CreerImage (image, width, height) < 0)
    {
	fclose (f);
	return -1;
    }

    /* Lecture image */
    for (n=0;n<height;n++)
    {
	if (fread(ligne,sizeof(unsigned char)*width,1,f)!=1)
	    return -1; /* Erreur de lecture */
	for (k=0;k<width;k++)
	    image->data[n*width+k]=(Pixel)ligne[k];
    }

    fclose (f);
    return 0;
}

int EcrireImage(char *nom, Image *image)
{
    FILE *f;
    int n,k;
    unsigned char ligne[2048];

    /* Ouverture du fichier */
    f = fopen (nom, "wb");
    if (f == NULL)
    {
	printf ("ERREUR fopen(%s)\n", nom);
	return -1;
    }

    /* Ecriture du header */
    fprintf (f, "P5\n");
    fprintf (f, "# width height\n");
    fprintf (f, "%d %d\n", image->width, image->height);
    fprintf (f, "255\n");

    /* Ecriture du tableau */
    for (n=0;n<image->height;n++)
    {
	for (k=0;k<image->width;k++)
	    ligne[k]=(unsigned char)image->data[n*image->width+k];
	if (fwrite (ligne, sizeof(unsigned char)*image->width,1,f)!=1)
	{
	    printf ("ERREUR fwrite()\n");
	    fclose (f);
	    return -1;
	}
    }

    fclose (f);
    return 0;
}

void LibererImage(Image *i)
{
    if (i->data != NULL) free(i->data);
    i->data = NULL;
}

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
    float a, b;

    a = (double)(maxi2-mini2)/(double)(maxi1-mini1);
    b = (double)(-maxi2*mini1 + maxi1*mini2)/(double)(maxi1-mini1);
    out->size = in->size;
    for (i = 0; i < out->size; i++)
    {
	out->data[i] = (short)(a*in->data[i] + b);
    }
}


void InitImg(Image& in)
{
	for(int i=0;i<in.height;i++){
		for(int j=0;j<in.width;j++){
			in.data[i*in.width+j] = 0;
		}
	}
}


void initHisto(Histo * _histo,int taille)
{
	int i;
	_histo->taille = taille;
	_histo->t = new double[taille];
	for (i = 0; i< taille; i++)
	{
		_histo->t[i] = 0;
	}
}


void calculHisto (Image * im1, Histo * _histo)
{
	int i,j;
	int max, classeMax;
	for (i = 0; i < im1->width; i++)
		for (j = 0; j < im1->height; j++)
		{
			if(_histo->taille == 2048)
				_histo->t[im1->data[j*im1->width+i] +1024] ++;
			else
				_histo->t[im1->data[j*im1->width+i]]++;
		}
	max = (int)_histo->t[0];
	classeMax = 0;
	for (i = 1; i< _histo->taille; i++)
	{
		if (_histo->t[i] > max)
		{
			max = (int)_histo->t[i];
			classeMax = i;
		}
	}
	_histo->max = max;
	_histo->classeMax = classeMax;
	_histo->nb_pixels = im1->size;
}

void expDynamique(Image *in)
{
	double aMin, aMax;
	int i, j;

	Histo histo;
	initHisto(&histo,2048);
	calculHisto(in, &histo);

	aMin = histo.t[0];
	// on parcours l'histo du début vers la fin, la derniere valeur non nulle est la plus haute
	for (i = 0; i < NB_NV_GRIS_SOBEL; i++)
	{
		if (histo.t[i] != 0)
		{
			aMin = i-1024;
			break;
		}
	}

	// on parcours l'histo de la fin vers le debut,  la derniere valeur non nulle est la plus basse
	for (i = NB_NV_GRIS_SOBEL-1; i >= 0;  i--)
	{
		if (histo.t[i] != 0)
		{
			aMax = i-1024;
			break;
		}
	}

	for (i = 0; i < in->width; i++){
		for (j = 0; j < in->height ; j++){
		    in->data[i+j*in->width] = (short)(255*((in->data[i+j*in->width] -aMin)/(aMax - aMin)));
		}
	}
}
