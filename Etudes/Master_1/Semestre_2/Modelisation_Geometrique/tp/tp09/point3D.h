#ifndef POINT3D_H
#define POINT3D_H


#include <iostream>
#include <math.h>
using namespace std ;

extern "C"
{
	#include <ml.h>
}

# define M_PI		3.14159265358979323846


/******************* point3D ******************/

class	point3D{   

public:

	double      x;
	double      y;
	double      z;


	point3D();                                        // (0,0,0) par defaut
    point3D(double X,double Y,double Z);
    
    void pt3DtoMlVec(mlVec3 &vec3);
    double distance(mlVec3 vec3);
    
	bool         operator==(const point3D &op)const;
	point3D&     operator=(const point3D &op);
	point3D      operator+(const point3D &op)const;
	point3D      operator-(const point3D &op)const;
	point3D&     operator*=(const double op);          //produit par un scalaire
	point3D      operator*(const double op)const;                 //idem
	point3D&     operator/=(const double op);         //division par un scalaire
	point3D      operator/(const double op)const;                 //idem
	void		Permutation(point3D *A, point3D *B);	     // Permutation de deux points
	friend ostream&  operator<<(ostream& p, point3D op);
};
#endif

