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

Point2D Polygone::getVertex(int _i)
{
    return (PTS[getIndexVertex(_i)]);
}

int Polygone::getIndexVertex(int _i)
{
    int ns = PTS.size();
    if (_i == -1) return (ns-1);
    if (_i == ns) return (0);
    return (_i);
}

Point2D Polygone::getHoleVertex(int _nt, int _i)
{
    return (TROUS[_nt].PTS[getIndexVertex(_i)]);
}

int Polygone::getIndexHoleVertex(int _nt, int _i)
{
    int ns = TROUS[_nt].PTS.size();
    if (_i == -1) return (ns-1);
    if (_i == ns) return (0);
    return (_i);
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
}

void Polygone::colorer(float _r, float _g, float _b, float _a)
{
    printf("*************************************************************************\n");
    printf("************************* Fonction COLORER ******************************\n");
    printf("*************************************************************************\n");

    // On recopie les sommets du polygone dans un tableau à 2 dimensions, en séparant les X et les Y
    // S : tableau de sommets
    // ns : nombre de sommets
    int ns = PTS.size();
    int nbTrous = TROUS.size();
    int S[2][ns];
    for (int i=0; i < ns; i++)
    {
	   S[0][i] = PTS[i].x;
	   S[1][i] = PTS[i].y;
    }

    // TS : vecteur de tableau 2D [2][nbSommetsTrou]
    vector<Tab> TS;

    int nbSommetsTrou;
    for (int ti=0; ti<nbTrous; ti++)
    {
	   nbSommetsTrou = TROUS[ti].PTS.size();
	   Tab tab;
	   // tab.t[2][nbSommetsTrou]
	   tab.size = nbSommetsTrou;
	   tab.t = new int * [2];
	   std::fill_n( tab.t, 2, static_cast<int*>( 0 ) );
	   // dimension 2 : les tableaux d'entiers
	   for (int dim_allouee = 0; dim_allouee < 2; ++dim_allouee)
	   {
		  tab.t[dim_allouee] = new int[nbSommetsTrou];
	   }
	   for (int i=0; i<nbSommetsTrou; i++)
	   {
		  tab.t[0][i] = TROUS[ti].PTS[i].x;
		  tab.t[1][i] = TROUS[ti].PTS[i].y;
	   }
	   TS.push_back(tab);
	   delete [] tab.t;
    }

    vector<int>Y;
    for (int i=0; i<ns; i++)
	   Y.push_back(S[1][i]);
    for (int ti=0; ti<nbTrous; ti++)
    {
	   int nbSommetsTrou = TROUS[ti].PTS.size();
	   for (int i=0; i<nbSommetsTrou; i++)
		  Y.push_back(TROUS[ti].PTS[i].y);
    }

    sort(Y.begin(), Y.end()); // trie dans l'ordre croissant
    reverse(Y.begin(), Y.end()); // inverse l'ordre

    for (int i=0; i<Y.size(); i++)
	   printf("%d ", Y[i]);
    printf("\n");

    // On épure la liste des ordonnées afin de virer les doublons
    vector<int> bo;
    int prev, curr;
    prev = Y[0];
    for (int i=1; i<ns; i++)
    {
	   curr = Y[i];
	   if (prev != curr)
	   {
		  bo.push_back(prev);
	   }
	   prev = curr;
    }
    bo.push_back(prev);

    // bornes : tableau des bornes de niveaux
    int nbniv = bo.size();
    int bornes[nbniv];
    for (int i=0; i<nbniv; i++)
	   bornes[i] = bo[i];

    // On active pour chaque niveau, les arêtes qui sont concernées par le niveau courant
    // BAA[ns][nbniv] : tableau à 2 dimensions nombre d'arêtes par nombre de niveaux
    int bornesup, borneinf;
    int BAA[ns][nbniv];
    int sup, inf;
    Point2D tail, head;
    bool bSup, bInf;
    for (int i=0; i<ns; i++)
    {
	   bSup = 0;
	   bInf= 0;
	   for (int ni=0; ni<nbniv-1; ni++)
	   {
		  BAA[i][ni] = 0;
		  bornesup = bornes[ni];
		  borneinf = bornes[ni+1];
		  tail = getVertex(i);
		  head = getVertex(i+1);
		  sup = inf = -1;
		  if (tail.y > head.y)
		  {
			 sup = tail.y;
			 inf = head.y;
		  }
		  else
		  {
			 if (tail.y < head.y)
			 {
				sup = head.y;
				inf = tail.y;
			 }
		  }
		  if (!bSup && sup == bornesup)
		  {
			 bSup = 1;
		  }
		  if (bSup)
		  {
			 BAA[i][ni] = 1;
			 if (!bInf && inf == borneinf)
			 {
				bInf = 1;
			 }
			 if (bInf) bSup = 0;
		  }
	   }
    }

    vector<Tab> BTAA;

    for (int ti=0; ti<nbTrous; ti++)
    {
	   nbSommetsTrou = TROUS[ti].PTS.size();
	   Tab tab;
	   tab.size = nbSommetsTrou;
	   tab.t = new int * [ nbSommetsTrou ];
	   std::fill_n( tab.t, nbSommetsTrou, static_cast<int*>( 0 ) );
	   // dimension 2 : les tableaux d'entiers
	   for (int dim_allouee = 0; dim_allouee < nbSommetsTrou; ++dim_allouee)
	   {
		  tab.t[ dim_allouee ] = new int[ nbniv ];
	   }
	   for (int i=0; i<nbSommetsTrou; i++)
	   {
		  bSup = 0;
		  bInf= 0;
		  for (int ni=0; ni<nbniv-1; ni++)
		  {
			 tab.t[i][ni] = 0;
			 bornesup = bornes[ni];
			 borneinf = bornes[ni+1];
			 tail = getHoleVertex(ti, i);
			 head = getHoleVertex(ti, i+1);
			 sup = inf = -1;
			 if (tail.y > head.y)
			 {
				sup = tail.y;
				inf = head.y;
			 }
			 else
			 {
				if (tail.y < head.y)
				{
				    sup = head.y;
				    inf = tail.y;
				}
			 }
			 if (!bSup && sup == bornesup)
			 {
				bSup = 1;
			 }
			 if (bSup)
			 {
				tab.t[i][ni] = 1;
				if (!bInf && inf == borneinf)
				{
				    bInf = 1;
				}
				if (bInf) bSup = 0;
			 }
		  }
	   }
	   BTAA.push_back(tab);
    }

    // On parcourt chaque niveau, on regarde les arêtes actives, on calcule les intersections et on dessine
    int AAX[ns];
    int naa;
    // Pour chaque niveau...
    for (int ni=0; ni<nbniv-1; ni++)
    {
	   bornesup = bornes[ni];
	   borneinf = bornes[ni+1];
	   // ...on parcourt le niveau courant...
	   for (int l=bornesup; l>=borneinf; l--)
	   {
		  naa = 0;
		  // ...on regarde quelle arête est concernée...
		  for (int i=0; i<ns; i++)
		  {
			 // ...si l'arête est concernée, on calcule l'intersection...
			 if (BAA[i][ni])
			 {
				Point2D I, I1, I2;
				Point2D dg(INT_MIN, l);
				Point2D dd(INT_MAX, l);
				Point2D pa, pb, pc;

				pa.Set(S[0][i], S[1][i]);

				if (i < ns-1) pb.Set(S[0][i+1], S[1][i+1]);
				else if (i == ns-1) pb.Set(S[0][0], S[1][0]);

				if (pa.y == pb.y)
				{
				    AAX[naa++] = pa.x;
				    AAX[naa++] = pb.x;
				}
				else
				{
				    if (pa.y == l) I.x = pa.x;
				    else if (pb.y == l) I.x = pb.x;
				    else I = computeIntersection(pa, pb, dg, dd);
				    AAX[naa++] = I.x;
				}
			 }
		  }

		  for (int ti=0; ti<nbTrous; ti++)
		  {
			 nbSommetsTrou = BTAA[ti].size;
			 for (int i=0; i<nbSommetsTrou; i++)
			 {
				// ...si l'arête est concernée, on calcule l'intersection...
				if (BTAA[ti].t[i][ni])
				{
				    Point2D I, I1, I2;
				    Point2D dg(INT_MIN, l);
				    Point2D dd(INT_MAX, l);
				    Point2D pa, pb, pc;

				    pa.Set(TS[ti].t[0][i], TS[ti].t[1][i]);

				    if (i < TS[ti].size-1) pb.Set(TS[ti].t[0][i+1], TS[ti].t[1][i+1]);
				    else if (i == TS[ti].size-1) pb.Set(TS[ti].t[0][0], TS[ti].t[1][0]);

				    if (pa.y == pb.y)
				    {
					   AAX[naa++] = pa.x;
					   AAX[naa++] = pb.x;
				    }
				    else
				    {
					   if (pa.y == l) I.x = pa.x;
					   else if (pb.y == l) I.x = pb.x;
					   else I = computeIntersection(pa, pb, dg, dd);
					   AAX[naa++] = I.x;
				    }
				}
			 }
		  }


		  // ...on trie le tableau des X intersectés dans l'ordre croissant...
		  sortByAscOrder(AAX, naa);
		  // ...et enfin on dessine la ligne de remplissage.
		  for (int i=0; i<=naa-2; i+=2)
		  {
                         glColor4f(_r, _g, _b, _a);
			 glBegin(GL_LINES);
				glVertex2i(AAX[i], l);
				glVertex2i(AAX[i+1], l);
			 glEnd();
		  }
	   }
    }
}

void Polygone::colorer(float _rgba[])
{
    colorer(_rgba[0], _rgba[1], _rgba[2], _rgba[3]);
}

void Polygone::vider(void)
{
    PTS.clear();
    TROUS.clear();
    initialisation();
}

void Polygone::tracerSommets(void)
{
    glPointSize(5.0);
    for (int i = 0; i < (int)PTS.size(); i++)
    {
	   if (PointInsertion == i)
		  glColor3f(1.0, 1.0, 0.0);
	   else glColor3f(1.0, 0.0, 0.0);
	   glBegin(GL_POINTS);
	   glVertex2i(PTS.at(i).x, PTS.at(i).y);
	   glEnd();
    }
    glPointSize(1.0);
}

void Polygone::tracerAretes(int _modeAffichage)
{
    glLineWidth(2.0);
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
    glLineWidth(1.0);
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

void Polygone::afficherCoords(int _height)
{
    if (!PTS.empty())
    {
	   for (int i = 0; i < (int)PTS.size(); i++)
	   {
		  glColor3f(0.0, 0.0, 0.0);
		  char coords[30] = "";
		  sprintf(coords,"[x: %d ; y: %d]", PTS[i].x,PTS[i].y);
		  displayText(PTS[i].x, /*_height -*/ PTS[i].y+10, 0.0, GLUT_BITMAP_TIMES_ROMAN_10, coords);
	   }
    }
}
