#ifndef TOOLS_H
#define TOOLS_H

#include "Point2D.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <string>
#include <GL/glut.h>

#include "defines.h"

extern "C"
{
#include <ml.h>
}

typedef struct
{
    double x, y, z;
} Point3D;


Point2D computeIntersection(Point2D a, Point2D b, Point2D c, Point2D d);
void sortByAscOrder(int *t, int size);
void sortByDescOrder(int *t, int size);
void displayText(float x, float y, float z, void* font, const char* s);
std::vector<Point3D> loadFilePTS(std::string _filename);
std::vector<std::vector<Point3D> > loadFilePTS2(std::string _filename, int * _u, int * _v);
void saveFilePTS2(std::string _filename, mlVec3 _ctrl[MAX][MAX], int _size_u, int _size_v);

std::vector<std::vector<Point3D> > loadFilePGMtoPTS2(std::string _filename, int * _u, int * _v);


template <class T>
T ** T_Array2D(int _dim1, int _dim2)
{
    T ** t = NULL;
    int dim_allouee = 0;
    try
    {
	  // dimension 1 : tableau de _dim1 pointeurs vers des tableaux de T
	  t = new T * [ _dim1 ];
	  // dimension 2 : les tableaux de T
	  for ( dim_allouee = 0; dim_allouee < _dim1; ++dim_allouee)
	  {
		t[ dim_allouee ] = new T[ _dim2 ];
	  }
    }
    catch ( const std::bad_alloc & ) // erreur d'allocation
    {
	  // désallouer tout ce qui a été alloué avec succès
	  for ( int i = 0; i < dim_allouee; ++i )
	  {
		delete [] t[ i ];
	  }
	  delete [] t;
    }
    return t;
}


#endif
