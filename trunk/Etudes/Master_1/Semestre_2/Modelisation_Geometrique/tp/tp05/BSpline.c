#include "BSpline.h" 

int fact(int _i)
{
	if (_i > 1) return (_i*fact(_i-1));
    else return (1);
}


double computeRiesenfeldCoeff(int _i, int _m, double _t)
{
	int k;
	double somme = 0;
	for (k = 0; k <= _m-_i; k++)
	{
		somme += pow(-1, k) * (pow(_t+_m-_i-k, _m)/(fact(k)*fact(_m-k+1)));
	}
	return ((_m+1)*somme);
}


void computeBSplinePoint(mlVec3 _ctrl[MAX], int _m, int _k, double _t, mlVec3 _q)
{
	int i;
	float x = 0, y = 0, z = 0;
	for (i = 0; i <= _m; i++)
	{
		double riesenfeldCoeff = computeRiesenfeldCoeff(i, _m, _t);
		x += riesenfeldCoeff * _ctrl[i+_k][0];
		y += riesenfeldCoeff * _ctrl[i+_k][1];
		z += riesenfeldCoeff * _ctrl[i+_k][2];
	}
	mlVec3_Set(_q, x, y, z);
}


void computeBSplineCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP, int _m)
{
	int i, k, sz;
	sz = _nCtrl - _m;
	for(k = 0; k < sz; k++)
	{
		for(i = k*(nBP+1); i < ((_nBP+1)*(k+1)); i++)
		{
			computeBSplinePoint(_ctrl, _m, k, i/(double)(_nBP*(k+1)), _bP[i]);
		}
	}
}


void drawBSplineCurve(mlVec3 _bP[MAX2], int _nBP)
{
	int i;
	int k;
	k = nCtrl - degreBS;
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 0.0, 1.0);

	for (i = 0; i < (_nBP+1)*k; i++)
	{
		glVertex3f(_bP[i][0], winY-_bP[i][1], _bP[i][2]);
	}
	glEnd();
}
