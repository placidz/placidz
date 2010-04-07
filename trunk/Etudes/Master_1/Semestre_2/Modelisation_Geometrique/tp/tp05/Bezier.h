#ifndef BEZIER_H
#define BEZIER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#include "defines.h"
#include "tools.h"

extern "C"
{
#include <ml.h>
}


void drawBezierSurface(mlVec3 **_ctrl, int _size_u, int _size_v, mlVec3 _bP[MAX2], int _nBP);
void drawBezierControlPointsSurface(mlVec3 **_ctrl, int _size_u, int _size_v);

void drawBezierCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP);

#endif
