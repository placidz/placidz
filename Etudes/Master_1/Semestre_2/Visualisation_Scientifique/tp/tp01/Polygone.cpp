#include "Polygone.h"

Polygone::Polygone(void)
{
	PointInsertion = -1;
}

Polygone::~Polygone(void)
{
}

void Polygone::ajouterSommet(int _x, int _y)
{
	//Point2D(_x, _y);
	
	std::vector<Point2D>::iterator it;
	it = PTS.begin();

	PointInsertion++;
	PTS.insert(it+PointInsertion, Point2D(_x, _y));
	//printf("Nombre de points : %d\n", PTS.size());
}

int Polygone::trouverSommet(int _x, int _y)
{
	int ind = -1;
	int Px, Py;
	for (int i = 0; i < (int)PTS.size() && ind == -1; i++)
	{
		Px = PTS[i].x;
		Py = PTS[i].y;
		
		if (Px >= _x-10 && Px <= _x+10 && Py >= _y-10 && Py <= _y+10)
			ind = i;
	}
	return ind;
}

void Polygone::changerPointInsertion(int _x, int _y)
{
	int ind = trouverSommet(_x, _y);
	if (ind > -1)
	{
		PointInsertion = ind;
	}
}

void Polygone::supprimerSommet(int _x, int _y)
{
	int ind = trouverSommet(_x, _y);
	if (ind > -1)
	{
		PTS.erase(PTS.begin() + ind);
		if (ind <= PointInsertion) PointInsertion--;
	}
}

bool Polygone::estInterieur(int _x, int _y)
{
	int nbIntersect = 0, sz = PTS.size(), xi, yi, x, y, pos1, pos2;
	double xx;

	for (int i = 0; i < sz; i++)
	{
		x = PTS.at(i).x;
		y = PTS.at(i).y;
		if (i == sz-1)
		{
			xi = PTS.at(0).x;
			yi = PTS.at(0).y;
		}
		else
		{
			xi = PTS.at(i+1).x;
			yi = PTS.at(i+1).y;
		}

		// 0 si GH
		// 1 si DH
		// 2 si GB
		// 3 si DB

		if ((y != yi) || ((y == yi) && (_y != y)))
		{
			if (x < _x)
			{ // on est a gauche
				if (y < _y) pos1 = 0; // et en haut
				else pos1 = 2; // et en bas
			}
			else
			{ // on est a droite
				if (y < _y) pos1 = 1; // et en haut
				else pos1 = 3; // et en bas
			}

			if (xi < _x)
			{ // on est a gauche
				if (yi < _y) pos2 = 0; // et en haut
				else pos2 = 2; // et en bas
			}
			else
			{ // on est a droite
				if (yi < _y) pos2 = 1; // et en haut
				else pos2 = 3; // et en bas
			}

			if ((pos1==0 && pos2 == 3) || (pos2==0 && pos1==3) || (pos1==1 && pos2==2) || (pos2==1 && pos1==2))
			{
				xx = (double)(_y-y)*((double)(xi-x)/(yi-y))+x;
				if (xx >= _x) nbIntersect++;
			}

			if ((pos1==1 && pos2 == 3) || (pos1==3 && pos2==1))
			{
				nbIntersect++;
			}
		}
	}
	return (nbIntersect%2 != 0) ? true : false;
}

void Polygone::tracerSommets(void)
{
	for (int i = 0; i < (int)PTS.size(); i++)
	{
		if (PointInsertion == i)
			glColor3f(1.0, 1.0, 0.0);
		else glColor3f(1.0, 0.0, 0.0);
		glPointSize(5);
		glBegin(GL_POINTS);
			glVertex2i(PTS.at(i).x, PTS.at(i).y);
		glEnd();
	}
}

void Polygone::tracerAretes(int _modeAffichage)
{
	glColor3f(0.0, 0.0, 1.0);
	if (PTS.size() > 1)
	{
		glBegin(_modeAffichage);
		for (int i = 0; i < (int)PTS.size()-1; i++)
		{
			glVertex2i(PTS.at(i).x, PTS.at(i).y);
			glVertex2i(PTS.at(i+1).x,  PTS.at(i+1).y);
		}
		glEnd();
	}
}
