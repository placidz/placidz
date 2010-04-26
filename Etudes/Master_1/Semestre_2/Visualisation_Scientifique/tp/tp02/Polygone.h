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

using namespace std;

typedef struct
{
    int **t;
    int size;
} Tab;

class Polygone
{
public:
	vector<Point2D> PTS;
	vector<Polygone> TROUS;
	
	vector<Point2D> NPTS;
	
	int PointInsertion;
	bool bCreationPolygone;
	bool bEnMouvement;

	Polygone(void);
	~Polygone(void);
	
	void initialisation(void);
	void ajouterSommet(int _x, int _y);
	int trouverSommet(int _x, int _y);
	void changerPointInsertion(int _x, int _y);
	void supprimerSommet(int _x, int _y);
	bool estInterieur(int _x, int _y);
	void deplacer(int _x, int _y);
	vector<Point2D> estIntersecte(Polygone poly);
	vector<Point2D> intersection(Polygone poly);
	vector<Point2D> reorganisePoints(Point2D a, vector<Point2D> vec);
	float distance(Point2D a, Point2D b);
	bool compare2Pts(Point2D a, Point2D b);
	void unionPoly(Polygone p1, Polygone p2);
	bool pointAppartientAListe(Point2D pt, vector<Point2D>lstPts);

	int determineSensIntersection(int pos, Polygone p1, vector<int> mark);
	int determineIndicePtSuivant(int pos, int sensParcours, int tailleListe);
	void UNIONPoly(Polygone p1, Polygone p2);
	vector<vector<Point2D> > UNIONTrou(Polygone p1, Polygone p2);
	void INTERSECTIONPoly(Polygone p1, Polygone p2);
	vector<vector<Point2D> > INTERTrou(Polygone p1, Polygone p2);
	void DIFFERENCEPoly(Polygone p1, Polygone p2);
	Point2D parcoursPolygone(Point2D ptAnalyse, Polygone p1, Polygone p2, vector<int>& lstMark, vector<Point2D>& listePts, string typeOp);
	void determinePolyTrou(vector<vector<Point2D> > liste);
	void unionRecupereTrous(vector<Polygone> trouP1, vector<Polygone>trouP2);
	
	Point2D getVertex(int _i);
    Point2D getHoleVertex(int _nt, int _i);
    int getIndexVertex(int _i);
    int getIndexHoleVertex(int _nt, int _i);
    
    void colorer(float _r, float _g, float _b, float _a);
    void colorer(float _rgba[]);

	void vider(void);

	// Méthodes de rendu
	void tracerSommets(void);
	void tracerAretes(int _modeAffichage);
	void tracerTrous(int _modeAffichage);
};
#endif
