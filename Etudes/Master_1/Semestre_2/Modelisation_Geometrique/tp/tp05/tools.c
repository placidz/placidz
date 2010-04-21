#include "tools.h"

using namespace std;


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

int LireImage(const char *nom, Image *image)
{
    FILE *f;
    char s[MAXLIGNE];
    char * r;
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
    r = fgets (s, MAXLIGNE, f);
    if (s[0] != 'P' || s[1] != '5')
    {
	  printf ("ERREUR MagicValue <> P5\n");
	  fclose (f);
	  return -1;
    }

    /* Lecture width height
		     * On saute les ligne commenant par un '#' */
    do r=fgets (s, MAXLIGNE, f);
    while (s[0] == '#');
    sscanf (s, "%d %d", &width, &height);

    /* Lecture maxgrey
		     * On ne tient pas compte de maxgrey, qui est presque toujours 255.
		     * On saute les ligne commenant par un '#' */
    do r=fgets (s, MAXLIGNE, f);
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

void BasculeImage(const Image *image, GLubyte *I)
{
	   int i,j;
	   for(i=0;i<image->height;i++)
	     for(j=0;j<image->width;j++)
		     I[(image->height-1-i)*image->width+j]=(GLubyte) image->data[i*image->width+j];
	   return;
}


Point2D computeIntersection(Point2D a, Point2D b, Point2D c, Point2D d)
{
    int x1 = a.x;
    int y1 = a.y;
    int x2 = b.x;
    int y2 = b.y;
    int x3 = c.x;
    int y3 = c.y;
    int x4 = d.x;
    int y4 = d.y;

    float supx = (x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4);
    float sub = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
    float supy = (x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4);

    Point2D I( (int)(supx/sub) , (int)(supy/sub));
    return I;
}

void sortByAscOrder(int *t, int size)
{
    bool en_desordre = true;
    for (int i = 0; i < size && en_desordre; ++i)
    {
	  en_desordre = false;
	  for (int j = 1; j < size - i; ++j)
		if (t[j-1] > t[j])
		{
		std::swap(t[j], t[j-1]);
		en_desordre = true;
	  }
    }
}

void sortByDescOrder(int *t, int size)
{
    bool en_desordre = true;
    for (int i = 0; i < size && en_desordre; ++i)
    {
	  en_desordre = false;
	  for (int j = 1; j < size - i; ++j)
		if (t[j-1] < t[j])
		{
		std::swap(t[j], t[j-1]);
		en_desordre = true;
	  }
    }
}

void displayText(float x, float y, float z, void* font, const char* s)
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glRasterPos2f(x, y);
    while(*s)
    {
	  glutBitmapCharacter(font, *s);
	  s++;
    }
}

vector<Point3D> loadFilePTS(string _filename)
{
    printf("Lecture du fichier '%s'.\n", _filename.c_str());
    int nbPtsCtrl;
    vector<Point3D> res;
    double x, y, z;
    FILE* fichier = NULL;
    fichier = fopen(_filename.c_str(), "r");
    if (fichier != NULL)
    {
	  printf("  Le fichier a bien été ouvert.\n");
	  fscanf(fichier, "# nb sommets = %d\n", &nbPtsCtrl);
	  while (fscanf(fichier, "v %lf %lf %lf\n", &x, &y, &z) != EOF)
	  {
		Point3D tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.z = z;
		res.push_back(tmp);
	  }
    }
    fclose(fichier);
    printf("Fin de lecture de fichier.\n\n");
    return res;
}

vector<vector<Point3D> > loadFilePTS2(string _filename, int * _u, int * _v)
{
    printf("Lecture du fichier '%s'.\n", _filename.c_str());
    int u, v;
    vector<vector<Point3D> > vecUV;
    vector<Point3D> vecU;
    double x, y, z;
    FILE* fichier = NULL;
    fichier = fopen(_filename.c_str(), "r");
    if (fichier != NULL)
    {
	  printf("  Le fichier a bien été ouvert.\n");
	  fscanf(fichier, "# nb sommets u = %d\n", &u);
	  fscanf(fichier, "# nb sommets v = %d\n", &v);
	  *_u = u;
	  *_v = v;
	  printf("  u = %d , v = %d\n", *_u, *_v);
	  while ((int)vecUV.size() < v)
	  {
		while ((int)vecU.size() < u)
		{
		    fscanf(fichier, "v %lf %lf %lf\n", &x, &y, &z);
		    Point3D tmp;
		    tmp.x = x;
		    tmp.y = y;
		    tmp.z = z;
		    vecU.push_back(tmp);
		}
		vecUV.push_back(vecU);
		vecU.clear();
	  }
    }
    fclose(fichier);
    printf("Fin de lecture de fichier.\n\n");
    return vecUV;
}

void saveFilePTS2(string _filename, mlVec3 _ctrl[MAX][MAX], int _size_u, int _size_v)
{
    FILE* fichier = NULL;
    fichier = fopen(_filename.c_str(), "w");
    if (fichier)
    {
	  fprintf(fichier, "# nb sommets u = %d\n", _size_u);
	  fprintf(fichier, "# nb sommets v = %d\n", _size_v);
	  for (int j = 0; j < _size_v; j++)
	  {
		for (int i = 0; i < _size_u; i++)
		{
		    fprintf(fichier, "v %f %f %f\n", _ctrl[i][j][0], _ctrl[i][j][1], _ctrl[i][j][2]);
		}
	  }
    }
    fclose(fichier);
}

std::vector<std::vector<Point3D> > loadFilePGMtoPTS2(Image _image, int * _u, int * _v)
{
    *_u = 16;
    *_v = 16;
    vector<vector<Point3D> > vecUV;
    vector<Point3D> vecU;
    for (int y = 0; y < _image.height; y+=16)
    {
	  for (int x = 0; x < _image.width; x+=16)
	  {
		Point3D p;
		p.x = x;
		p.y = _image.data[x+y*_image.width];
		p.z = y;
		vecU.push_back(p);
	  }
	  vecUV.push_back(vecU);
	  vecU.clear();
    }
    return vecUV;
}
