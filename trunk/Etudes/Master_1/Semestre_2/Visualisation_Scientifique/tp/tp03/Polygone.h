#ifndef POLYGONE_H
#define POLYGONE_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <limits.h>
#include <algorithm>
//#include "glut.h"
#include <GL/glut.h>
#include "Point2D.h"
#include "tools.h"

typedef struct
{
    int **t;
    int size;
} Tab;

class Polygone
{
public:
    std::vector<Point2D> PTS;
    std::vector<Polygone> TROUS;

    int PointInsertion;
    bool bCreationPolygone;
    bool bEnMouvement;
    bool bColoration;

    Polygone(void);
    ~Polygone(void);

    void initialisation(void);
    void ajouterSommet(int _x, int _y);
    int trouverSommet(int _x, int _y);
    void changerPointInsertion(int _x, int _y);
    void supprimerSommet(int _x, int _y);

    Point2D getVertex(int _i);
    Point2D getHoleVertex(int _nt, int _i);
    int getIndexVertex(int _i);
    int getIndexHoleVertex(int _nt, int _i);

    bool estInterieur(int _x, int _y);
    void deplacer(int _x, int _y);
    void colorer(float _r, float _g, float _b, float _a);
    void colorer(float _rgba[]);

    void vider(void);

    // Méthodes de rendu
    void tracerSommets(void);
    void tracerAretes(int _modeAffichage);
    void tracerTrous(int _modeAffichage);
    void afficherCoords();
};
#endif
