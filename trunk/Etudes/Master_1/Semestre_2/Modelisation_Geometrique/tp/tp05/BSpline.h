#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <ml.h>

int fact(int _i);
double computeRiesenfeldCoeff(int _i, int _m, double _t);
void computeBSplinePoint(mlVec3 _ctrl[MAX], int _m, int _k, double _t, mlVec3 _q);
void computeBSplineCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP, int _m);
void drawBSplineCurve(mlVec3 _bP[MAX2], int _nBP);
