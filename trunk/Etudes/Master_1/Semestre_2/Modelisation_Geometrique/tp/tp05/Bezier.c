#include "Bezier.h"

int tabPascal[100][100];

void computeTrianglePascal()
{
    int n = 100;
    for(int y = 0; y < n; y++)
	  for(int x = 0; x <= y; x++)
		if(x == 0 || x==y) tabPascal[x][y] = 1;
		else tabPascal[x][y] = tabPascal[x-1][y-1] + tabPascal[x][y-1];
}

double computeBernsteinCoeff(int _n, int _i, double _t)
{
    return tabPascal[_i][_n]*pow(_t,_i)*pow(1-_t,_n-_i);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////
// SURFACES DE BEZIER			    //
//////////////////////////////////////////

void computeBezierPointSurface(mlVec3 _ctrl[MAX][MAX], int _size_u, int _size_v, double _u, double _v, mlVec3 _q)
{
    double coefBu = 0;
    double coefBv = 0;
    mlVec3 tmp;
    mlVec3_Zero(_q);
    for (int j = 0; j < _size_v; j++)
    {
	  coefBv = computeBernsteinCoeff(_size_v-1, j, _v);
	  for (int i = 0; i < _size_u; i++)
	  {
		coefBu = computeBernsteinCoeff(_size_u-1, i, _u);
		mlVec3_Scale(tmp, _ctrl[i][j], coefBu);
		mlVec3_Scale(tmp, tmp, coefBv);
		mlVec3_AddVec(_q, _q, tmp);
	  }
    }
}

void computeBezierSurface(mlVec3 _ctrl[MAX][MAX], int _size_u, int _size_v, mlVec3 _bP[MAX2], int _nBP)
{
    double tu, tv;
    for (int j = 0; j < _nBP+1; j++)
    {
	  tv = j/(double)_nBP;
	  for (int i = 0; i < _nBP+1; i++)
	  {
		tu = i/(double)_nBP;
		computeBezierPointSurface(_ctrl, _size_u, _size_v, tu, tv, _bP[i + j*(_nBP+1)]);
	  }
    }
}


void drawBezierSurface(mlVec3 _ctrl[MAX][MAX], int _size_u, int _size_v, mlVec3 _bP[MAX2], int _nBP, int _mode, std::string _loadMode)
{
    glDisable(GL_LIGHTING);
    computeTrianglePascal();
    computeBezierSurface(_ctrl, _size_u, _size_v, _bP, _nBP);

    glLineWidth(1.0);
    for (int j = 0; j < _nBP; j++)
    {
	  for (int i = 0; i < _nBP; i++)
	  {
		if (_loadMode == "-img")
		{
		    glColor3f(1.0, 1.0, 1.0);
		    glBegin(GL_QUADS);
			  glTexCoord2f((double)i/(_nBP+1), (double)(_nBP+1 - j)/(_nBP+1));
			  glVertex3dv(_bP[i + j*(_nBP+1)]);
			  glTexCoord2f((double)i/(_nBP+1), (double)(_nBP+1 - j-1)/(_nBP+1));
			  glVertex3dv(_bP[i + (j+1)*(_nBP+1)]);
			  glTexCoord2f((double)(i+1)/(_nBP+1), (double)(_nBP+1 - j-1)/(_nBP+1));
			  glVertex3dv(_bP[(i+1) + (j+1)*(_nBP+1)]);
			  glTexCoord2f((double)(i+1)/(_nBP+1), (double)(_nBP+1 - j)/(_nBP+1));
			  glVertex3dv(_bP[(i+1) + j*(_nBP+1)]);
		    glEnd();
		}
		else if (_loadMode == "-pts")
		{
		    glColor3f(0.0, 0.0, 1.0);
		    glBegin(GL_QUADS);
			  glVertex3dv(_bP[i + j*(_nBP+1)]);
			  glVertex3dv(_bP[i + (j+1)*(_nBP+1)]);
			  glVertex3dv(_bP[(i+1) + (j+1)*(_nBP+1)]);
			  glVertex3dv(_bP[(i+1) + j*(_nBP+1)]);
		    glEnd();

		    if (_mode == 2)
		    {
			  glLineWidth(2.0);
			  glColor3f(0.0, 0.0, 0.0);
			  glBegin(GL_LINE_LOOP);
				glVertex3dv(_bP[i + j*(_nBP+1)]);
				glVertex3dv(_bP[i + (j+1)*(_nBP+1)]);
				glVertex3dv(_bP[(i+1) + (j+1)*(_nBP+1)]);
				glVertex3dv(_bP[(i+1) + j*(_nBP+1)]);
			  glEnd();
			  glLineWidth(1.0);
		    }
		}
	  }
    }
    glLineWidth(1.0);

    glEnable(GL_LIGHTING);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////
// COURBES DE BEZIER			    //
//////////////////////////////////////////

void computeBezierPoint(mlVec3 _ctrl[MAX], int _nCtrl, double _t, mlVec3 _q)
{
    int i, j;
    double coeffBernstein = 0;
    mlVec3 tmp;
    mlVec3_Zero(_q);
    for(i = 0; i < _nCtrl; i++)
    {
	  coeffBernstein = computeBernsteinCoeff(_nCtrl-1, i, _t);
	  mlVec3_Scale(tmp, _ctrl[i], coeffBernstein);
	  mlVec3_AddVec(_q, _q, tmp);
    }
}


void computeBezierCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP)
{
    computeTrianglePascal();
    int i;
    for(i = 0; i < _nBP+1; i++)
    {
	  computeBezierPoint(_ctrl, _nCtrl, i/(double)_nBP, _bP[i]);
    }
}


void drawBezierCurve(mlVec3 _ctrl[MAX], int _nCtrl, mlVec3 _bP[MAX2], int _nBP)
{

    computeBezierCurve(_ctrl, _nCtrl, _bP, _nBP);
    int i;
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 1.0);
    for (i = 0; i < _nBP+1; i++)
    {
	  glVertex3dv(_bP[i]);
    }
    glEnd();
}
