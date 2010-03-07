#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#ifndef BEZIER_H
#define BEZIER_H

#include <ml.h>

#define MAX	20
#define MAX2 100

class Bezier
{
	public:
	int nCtrl;
	mlVec3 ctrl[MAX];
	int winY;
	
	Bezier();
	Bezier(int _winY);
	~Bezier();
	
	double fact(int _i);
	double computeBernsteinCoeff(int _n, int _i, double _t);
	void computeBezierPoint(mlVec3 _ctrl[MAX], int _nCtrl, double _t, mlVec3 _q);
	void computeBezierCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP);
	void drawBezierCurve(mlVec3 _bP[MAX2], int _nBP);
	void computeDeCasteljau(mlVec3 _ctrl[MAX], int _nCtrl, double _t, mlVec3 _castel[MAX][MAX]);
	void drawConstruction(mlVec3 _castel[MAX][MAX], int _n);
	void computeDeCasteljauCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP);
	void drawDeCasteljauCurve(mlVec3 _bP[MAX2], int _nBP);	
}
#endif
