#include "Bezier.h"

Bezier::Bezier()
{
}

Bezier::Bezier(int _winY)
{
	nCtrl = 0;
	winY = _winY;
}

Bezier::~Bezier()
{
}

double Bezier::fact(int _i)
{
     if (_i > 1) return (_i*fact(_i-1));
     else return (1);
}


double Bezier::computeBernsteinCoeff(int _n, int _i, double _t)
{
	return ((fact(_n) / (fact(_i)*fact(_n-_i)))*((double)pow((double)_t, _i)*(double)pow((double)1-_t, _n-_i)));
}


void Bezier::computeBezierPoint(mlVec3 _ctrl[MAX], int _nCtrl, double _t, mlVec3 _q)
{
	int i, j;
	float x = 0, y = 0, z = 0;
	double coeffBernstein = 0;
	for(i = 0; i < _nCtrl; i++)
	{
		coeffBernstein = computeBernsteinCoeff(_nCtrl-1, i, _t);
		x += (_ctrl[i][0] * coeffBernstein);
		y += (_ctrl[i][1] * coeffBernstein);
		z += (_ctrl[i][2] * coeffBernstein);
	}
	mlVec3_Set(_q, x, y, z);
}


void Bezier::computeBezierCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP)
{
	int i;
	for(i = 0; i < _nBP+1; i++)
	{
		computeBezierPoint(_ctrl, _nCtrl, i/(double)_nBP, _bP[i]);
	}
}


void Bezier::drawBezierCurve(mlVec3 _bP[MAX2], int _nBP)
{
	int i;
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 0.0, 1.0);
	for (i = 0; i < _nBP+1; i++)
	{
		glVertex3f(_bP[i][0], winY-_bP[i][1], _bP[i][2]);
	}
	glEnd();
}


void Bezier::computeDeCasteljau(mlVec3 _ctrl[MAX], int _nCtrl, double _t, mlVec3 _castel[MAX][MAX])
{
	int i, j;
	float x, y;
	for (i = 0; i < _nCtrl; i++)
		mlVec3_Set(_castel[i][0], _ctrl[i][0], _ctrl[i][1], _ctrl[i][2]);

	for (j = 1; j < _nCtrl; j++)
	{
		for (i = 0; i < _nCtrl-j; i++)
		{
			x = _t*_castel[i+1][j-1][0] + (1-_t)*_castel[i][j-1][0];
			y = _t*_castel[i+1][j-1][1] + (1-_t)*_castel[i][j-1][1];
			mlVec3_Set(_castel[i][j], x, y, 0);
		}
	}
}


void Bezier::drawConstruction(mlVec3 _castel[MAX][MAX], int _n)
{
	int i, j;
	int cpt =0;
	computeDeCasteljau(ctrl, nCtrl, tConstruction, _castel);
	glColor3f(0.4, 0.4, 0.4);
	for (j = 0; j < _n-1; j++)
	{
		for (i = 0; i < _n-j-1; i++)
		{
			glBegin(GL_LINE_STRIP);
				glVertex2f(_castel[i][j][0], winY-_castel[i][j][1]);
				glVertex2f(_castel[i+1][j][0], winY-_castel[i+1][j][1]);
			glEnd();
		}
	}
	glColor3f(0.5, 0.5, 0.5);
	glPointSize(4.0);
	for (j = 1; j < _n; j++)
	{
		for (i = 0; i < _n-j; i++)
		{
			glBegin(GL_POINTS);
				glVertex2f(_castel[i][j][0], winY-_castel[i][j][1]);;
			glEnd();
		}
	}
	glPointSize(5.0);
}


void Bezier::computeDeCasteljauCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP)
{
	int i;
	float x, y;
	for(i = 0; i < _nBP+1; i++)
	{
		computeDeCasteljau(_ctrl, _nCtrl, i/(double)_nBP, castel);
		x = castel[0][_nCtrl-1][0];
		y = castel[0][_nCtrl-1][1];
		mlVec3_Set(_bP[i], x, y, 0);
	}
}


void Bezier::drawDeCasteljauCurve(mlVec3 _bP[MAX2], int _nBP)
{
	int i;
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0, 0.0, 0.0);
	for (i = 0; i < _nBP+1; i++)
	{
		glVertex3f(_bP[i][0], winY-_bP[i][1], _bP[i][2]);
	}
	glEnd();
} 
