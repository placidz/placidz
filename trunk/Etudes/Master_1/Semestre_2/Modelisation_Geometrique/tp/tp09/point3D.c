#include "point3D.h"

point3D::point3D()
{	x=0; y=0; z=0;	}

point3D::point3D(double X,double Y,double Z)
{	x=X; y=Y; z=Z;	}

void point3D::pt3DtoMlVec(mlVec3 &vec3){
	mlVec3_Set(vec3, this->x, this->y, this->z);
}

double point3D::distance(mlVec3 vec3){
	double tmp;
	tmp = sqrt( pow(x+vec3[0], 2) + pow(y+vec3[1], 2) + pow(z+vec3[2], 2) );
	return tmp;
}





bool point3D::operator==(const point3D &op)const
{	return(x==op.x && y==op.y && z==op.z);	}

point3D& point3D::operator=(const point3D &op)
{	x=op.x; y=op.y; z=op.z; return *this;	}


point3D point3D::operator+(const point3D &op)const
{	return(point3D(x+op.x,y+op.y,z+op.z));	}

point3D point3D::operator-(const point3D &op)const
{	return(point3D(x-op.x,y-op.y,z-op.z));	}

point3D& point3D::operator*=(const double op)
{	x*=op; y*=op; z*=op; return *this;	}

point3D point3D::operator*(const double op)const
{	return(point3D(x*op,y*op,z*op));	}

point3D& point3D::operator/=(const double op)
{	x/=op; y/=op; z/=op; return *this;	}

point3D point3D::operator/(const double op)const
{	return(point3D(x/op,y/op,z/op));	}


void point3D :: Permutation(point3D *A, point3D *B)
{ point3D PEch; 
  PEch = (*A); 
  (*A) = (*B);
  (*B) = PEch;
}

ostream&  operator<<(ostream& p, point3D op)
{
	p << "(" << op.x <<", " << op.y << "," << op.z  << ")";
	return(p);
}

