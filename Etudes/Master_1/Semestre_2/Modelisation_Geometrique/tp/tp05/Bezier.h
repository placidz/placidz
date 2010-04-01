#ifndef BEZIER_H
#define BEZIER_H
extern "C"
{
#include <ml.h>
}

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#include "defines.h"

class Bezier
{
	public:
	mlVec3 ctrl[MAX];
	int nCtrl;
	mlVec3 bP[MAX2];
	int nBP;
	mlVec3 castel[MAX][MAX];
	int tConstruction;
	int winY;
	
	Bezier();
	~Bezier();
	Bezier(int _winY);
	
	double fact(int _i);
	double computeBernsteinCoeff(int _n, int _i, double _t);
	void computeBezierPoint(mlVec3 _ctrl[MAX], int _nCtrl, double _t, mlVec3 _q);
	void computeBezierCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP);
	void drawBezierCurve(mlVec3 _bP[MAX2], int _nBP);
	void computeDeCasteljau(mlVec3 _ctrl[MAX], int _nCtrl, double _t, mlVec3 _castel[MAX][MAX]);
	void drawConstruction(mlVec3 _castel[MAX][MAX], int _n);
	void computeDeCasteljauCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP);
	void drawDeCasteljauCurve(mlVec3 _bP[MAX2], int _nBP);	
};
#endif