#include "Polygone.h"

using namespace std;

Polygone::Polygone(void)
{
	initialisation();
}

Polygone::~Polygone(void)
{
}

void Polygone::initialisation(void)
{
	PointInsertion = -1;
	bCreationPolygone = 1;
	bEnMouvement = 0;
	bColoration = 0;
}

void Polygone::ajouterSommet(int _x, int _y)
{
	std::vector<Point2D>::iterator it;
	it = PTS.begin();

	PointInsertion++;
	PTS.insert(it+PointInsertion, Point2D(_x, _y));
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
	
	for (int i=0; i < (int)TROUS.size(); i++)
	{
		if (TROUS.at(i).estInterieur(_x, _y)) return false;
	}

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

void Polygone::deplacer(int _x, int _y)
{
	for (int i = 0; i < (int)PTS.size(); i++)
	{
		PTS.at(i).x += _x;
		PTS.at(i).y += _y;
	}

	for (int i = 0; i < (int)TROUS.size(); i++)
	{
		for(int j = 0; j < (int)TROUS.at(i).PTS.size(); j++){
			TROUS.at(i).PTS.at(j).x += _x;
			TROUS.at(i).PTS.at(j).y += _y;
		}
	}
	/*for (int i=0; i<TROUS.size(); i++)
	{
		for (int j=0; j<TROUS.at(i).size(); j++)
		{
			TROUS.at(i).at(j).x += _x;
			TROUS.at(i).at(j).y += _y;
		}
	}*/
}

void Polygone::colorer(float r, float g, float b)
{
	// S : tableau de sommets
	// ns : nombre de sommets
	int ns = PTS.size();
	int S[2][ns];
	for (int i=0; i < ns; i++)
	{
		S[0][i] = PTS[i].x;
		S[1][i] = PTS[i].y;
	}
	
	// Y : tableau des ordonnées avec l'indice de sommet correspondant
	int Y[2][ns];
	for (int i=0; i < ns; i++)
	{
		Y[0][i] = i;
		Y[1][i] = PTS[i].y;
	}
	/*for (int i=0; i<ns; i++)
		printf("ind: %d - Y: %d\n", Y[0][i], Y[1][i]);
	printf("-------------");*/
	
	// Tri de Y dans l'ordre décroissant
	bool en_desordre = true;
	for (int i = 0; i < ns && en_desordre; ++i)
	{
		en_desordre = false;
		for (int j = 1; j < ns - i; ++j)
			if (Y[1][j-1] > Y[1][j])
			{
				std::swap(Y[0][j], Y[0][j-1]);
				std::swap(Y[1][j], Y[0][j-1]);
				en_desordre = true;
 			}
	}
	/*for (int i=0; i<ns; i++)
		printf("ind: %d - Y: %d\n", Y[0][i], Y[1][i]);*/
	
	// bo : liste d'ordonnées
	vector<int> bo;
	
	// On épure la liste des ordonnées afin de virer les doublons
	int last, curr;
	last = Y[1][0];
	for (int i=1; i<ns; i++)
	{
		curr = Y[1][i];
		if (last != curr) bo.push_back(last);
		last = curr;
	}
	
	/*for (int i=0; i<bo.size(); i++)
		printf("%d\n", bo[i]);*/
		
	// bornes : tableau des bornes de niveaux
	int nbniv = bo.size();;
	int bornes[nbniv];
	for (int i=0; i<nbniv; i++)
		bornes[i] = bo[i];
	
	// AA : tableau des arêtes actives
	bool AA[ns];
	for (int i=0; i<ns; i++)
		AA[i] = 0;
	int naa = 0;
	
	// AAX : tableau de naa éléments contenant les arêtes coupées avec l'abscisse correspondante
	int AAX[2][ns];
	
	// Pour chaque borne ...
	for (int ni=0; ni<nbniv-1; ni++)
	{
		// ... on regarde toutes les arêtes ...
		naa = 0;
		for (int i=0; i<ns; i++)
		{
			// ... et on active/désactive les arêtes concernées.
			if (bornes[ni] >= Y[1][i] && Y[1][i] >= bornes[ni+1])
			{
				AA[Y[0][i]] = 1;
				//AAX[0][naa] = i;
				naa++;
			}
			else
			{
				AA[Y[0][i]] = 0;
				//naa--;
			}
		}
		int bnaa = 0;
		
		// Pour chaque valeur de l entre 2 bornes ...
		for (int l=bornes[ni]; l >= bornes[ni+1]; l--)
		{
			bnaa = 0;
			for (int i=0; i<ns; i++)
			{
				if (AA[i])
				{
					Point2D pa, pb;
					Point2D bg(INT_MIN, l);
					Point2D bd(INT_MAX, l);
					if (i < ns-1)
					{
						pa.x = S[0][i];
						pa.y = S[1][i];
						pb.x = S[0][i+1];
						pb.y = S[1][i+1];
					}
					else if (i == ns-1)
					{
						pa.x = S[0][i];
						pa.y = S[1][i];
						pb.x = S[0][0];
						pb.y = S[1][0];
					}
					Point2D I = computeIntersection(pa, pb, bg, bd);
					AAX[1][bnaa] = I.x;
					bnaa++;
				}
			}
			bool en_desordre = true;
			for (int i = 0; i < bnaa && en_desordre; ++i)
			{
				en_desordre = false;
				for (int j = 1; j < bnaa - i; ++j)
					if (AAX[1][j-1] > AAX[1][j])
					{
						std::swap(AAX[0][j], AAX[0][j-1]);
						std::swap(AAX[1][j], AAX[0][j-1]);
						en_desordre = true;
					}
			}
			for (int i=0; i<bnaa-1; i++)
			{
				glColor3f(r, g, b);
				glBegin(GL_LINES);
					glVertex2i(AAX[1][i], l);
					glVertex2i(AAX[1][i+1], l);
				glEnd();
			}
		}
	}
}

void Polygone::vider(void)
{
	PTS.clear();
	TROUS.clear();
	initialisation();
}

void Polygone::tracerSommets(void)
{
	for (int i = 0; i < (int)PTS.size(); i++)
	{
		if (PointInsertion == i)
			glColor3f(1.0, 1.0, 0.0);
		else glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POINTS);
			glVertex2i(PTS.at(i).x, PTS.at(i).y);
		glEnd();
	}
}

void Polygone::tracerAretes(int _modeAffichage)
{
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

void Polygone::tracerTrous(int _modeAffichage)
{
	for (int i = 0; i < (int)TROUS.size(); i++)
	{
		glColor3f(0.0, 1.0, 0.0);
		TROUS.at(i).tracerAretes(_modeAffichage);
		glColor3f(0.0, 0.0, 1.0);
		TROUS.at(i).tracerSommets();
	}
}
