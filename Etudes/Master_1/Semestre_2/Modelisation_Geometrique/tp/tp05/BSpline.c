#include "BSpline.h"

double fact(int _n)
{
    double f = 1;
    while (_n > 1) f *= _n--;
    return f;
}

double computeRiesenfeldCoeff(int _i, int _m, double _t)
{
    int k;
    double somme = 0;
    for (k = 0; k <= (_m-_i); k++)
    {
	  somme += pow(-1, k) * (pow(_t+_m-_i-k, _m)/(double)(fact(k)*fact(_m-k+1)));
    }
    return ((_m+1)*somme);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////
// SURFACES BSPLINES
///////////////////////////////////

// ctrl : tableau des points de controle
// mu : degré selon u
// mv : degré selon V
// ku : décalage selon U
// kv : décalage selon V
// u : temps selon U
// v : temps selon V
void computeBSplinePointSurface(mlVec3 _ctrl[MAX][MAX], int _mu, int _mv, int _ku, int _kv, double _u, double _v, mlVec3 _q)
{
    double coefRu = 0;
    double coefRv = 0;
    mlVec3 tmp;
    mlVec3_Zero(_q);
    for (int j = 0; j <= _mv; j++)
    {
	  coefRv = computeRiesenfeldCoeff(j, _mv, _v);
	  for (int i = 0; i <= _mu; i++)
	  {
		coefRu = computeRiesenfeldCoeff(i, _mu, _u);
		mlVec3_Scale(tmp, _ctrl[i+_ku][j+_kv], coefRu);
		mlVec3_Scale(tmp, tmp, coefRv);
		mlVec3_AddVec(_q, _q, tmp);
	  }
    }
}

// size_u : nombre de U
// size_v : nombre de V
void computeBSplineSurface(mlVec3 _ctrl[MAX][MAX], int _size_u, int _size_v, mlVec3 _bP[MAX2], int _nBP, int _mu, int _mv)
{
    double u, v;
    int nu = _size_u - _mu;
    int nv = _size_v - _mv;
    int n;
    for (int kv = 0; kv < nv; kv++)
    {
	  for (int ku = 0; ku < nu; ku++)
	  {
		n = ku*((_nBP+1)*(_nBP+1)) + kv*(nu*((_nBP+1)*(_nBP+1)));
		for (int j = 0; j < (_nBP+1); j++)
		{
		    v = (double) j / _nBP;
		    for (int i = 0; i < (_nBP+1); i++)
		    {
			  u = (double) i / _nBP;
			  computeBSplinePointSurface(_ctrl, _mu, _mv, ku, kv, u, v, _bP[i + j*(_nBP+1) + n]);
		    }
		}
	  }
    }
}


void drawBSplineSurface(mlVec3 _ctrl[MAX][MAX], int _size_u, int _size_v, mlVec3 _bP[MAX2], int _nBP, int _mu, int _mv)
{
    glDisable(GL_LIGHTING);
    computeBSplineSurface(_ctrl, _size_u, _size_v, _bP, _nBP, _mu, _mv);
    int nu = _size_u - _mu;
    int nv = _size_v - _mv;
    int n;
    glLineWidth(2.0);
    for (int kv = 0; kv < nv; kv++)
    {
	  for (int ku = 0; ku < nu; ku++)
	  {
		n = ku*((_nBP+1)*(_nBP+1)) + kv*(nu*((_nBP+1)*(_nBP+1)));
		for (int j = 0; j < _nBP; j++)
		{
		    for (int i = 0; i < _nBP; i++)
		    {
			  glColor3f(0.0, 1.0, 0.0);
			  glBegin(GL_QUADS);
				glVertex3dv(_bP[i + j*(_nBP+1) + n]);
				glVertex3dv(_bP[i + (j+1)*(_nBP+1) + n]);
				glVertex3dv(_bP[(i+1) + (j+1)*(_nBP+1) + n]);
				glVertex3dv(_bP[(i+1) + j*(_nBP+1) + n]);
			  glEnd();

			  glColor3f(0.0, 0.0, 0.0);
			  glBegin(GL_LINE_LOOP);
				glVertex3dv(_bP[i + j*(_nBP+1) + n]);
				glVertex3dv(_bP[i + (j+1)*(_nBP+1) + n]);
				glVertex3dv(_bP[(i+1) + (j+1)*(_nBP+1) + n]);
				glVertex3dv(_bP[(i+1) + j*(_nBP+1) + n]);
			  glEnd();
		    }
		}
	  }
    }

    glLineWidth(1.0);
    glEnable(GL_LIGHTING);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////
// COURBES BSPLINES
//////////////////////////////////////////

// ctrl : tableau des points de controle
// m : degré de la BS
// k : décalage
// t : temps
void computeBSplinePoint(mlVec3 _ctrl[MAX], int _m, int _k, double _t, mlVec3 _q)
{
    double coefR = 0;
    mlVec3 tmp;
    mlVec3_Zero(_q);
    for (int i = 0; i <= _m; i++)
    {
	  coefR = computeRiesenfeldCoeff(i, _m, _t);
	  mlVec3_Scale(tmp, _ctrl[i+_k], coefR);
	  mlVec3_AddVec(_q, _q, tmp);
    }
}


void computeBSplineCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP, int _m)
{
    int i, k, sz;
    sz = _nCtrl - _m;
    for(k = 0; k < sz; k++)
    {
	  for(i = k*(_nBP+1); i < ((_nBP+1)*(k+1)); i++)
	  {
		computeBSplinePoint(_ctrl, _m, k, i/(double)(_nBP*(k+1)), _bP[i]);
	  }
    }
}


void drawBSplineCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP, int _m)
{
    int i;
    int k;
    computeBSplineCurve(_ctrl, _nCtrl, _bP, _nBP, _m);
    k = _nCtrl - _m;
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 1.0);

    for (i = 0; i < (_nBP+1)*k; i++)
    {
	  glVertex3dv(_bP[i]);
    }
    glEnd();
}
