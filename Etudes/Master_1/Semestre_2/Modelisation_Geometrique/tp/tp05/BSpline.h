#ifndef BSPLINE_H
#define BSPLINE_H
extern "C"
{
#include <ml.h>
}

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#include "defines.h"
#include "tools.h"


void drawBSplineSurface(mlVec3 _ctrl[MAX][MAX], int _size_u, int _size_v, mlVec3 _bP[MAX2], int _nBP, int _mu, int _mv, int _mode, std::string _loadMode);

void drawBSplineCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP, int _m);


#endif
