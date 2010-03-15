
#define MAXLIGNE 256

typedef short Pixel;

typedef struct
	{
	int width, height, size;
	Pixel *data;
	}Image;

int CreerImage(Image *image, int largeur, int hauteur);	// Allouer image
int LireImage(char *nom, Image *image);				// Charger et allouer image
int EcrireImage(char *nom, Image *image);				// Sauvegarder image
void LibererImage(Image *i);
