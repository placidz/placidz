#include "Polygone.h"

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

void Polygone::tracerTrous(int _modeAffichage){
	for (int i = 0; i < (int)TROUS.size(); i++)
	{
		glColor3f(1.0, 0.0, 1.0);
		TROUS.at(i).tracerSommets();
		glColor3f(0.0, 1.0, 0.0);
		TROUS.at(i).tracerAretes(_modeAffichage);
	}
}

vector<Point2D> Polygone::estIntersecte(Polygone poly)
{
	int asc, asc2;
	int nbPtsAjoutes = 0;
	vector<Point2D> res;
	vector<Point2D> totalIntersection;
	vector<Point2D> interTmp;
	
	NPTS.clear();
	
	for(int j = 0; j < (int)PTS.size(); j++)
	{
		if(j == ((int)PTS.size()-1))
			asc2 = 0;
		else asc2 = j+1;

		NPTS.push_back(PTS.at(j));
			
		for(int i = 0; i < (int)poly.PTS.size(); i++)
		{
			if(i == ((int)poly.PTS.size()-1))
				asc = 0;
			else asc = i+1;

			Point2D ca(PTS.at(j).x - poly.PTS.at(i).x, 		PTS.at(j).y - poly.PTS.at(i).y);
			Point2D cb(PTS.at(asc2).x - poly.PTS.at(i).x, 	PTS.at(asc2).y - poly.PTS.at(i).y);
			Point2D da(PTS.at(j).x - poly.PTS.at(asc).x, 	PTS.at(j).y - poly.PTS.at(asc).y);
			Point2D db(PTS.at(asc2).x - poly.PTS.at(asc).x, PTS.at(asc2).y - poly.PTS.at(asc).y);
			if(	((ca.x * cb.y - ca.y * cb.x) < 0 && (da.x * db.y - da.y * db.x) > 0) || 
				((ca.x * cb.y - ca.y * cb.x) > 0 && (da.x * db.y - da.y * db.x) < 0))
			{
				
				if( ((-ca.x * -da.y) - (-da.x * -ca.y) < 0 && (-cb.x * -db.y) - (-db.x * -cb.y) > 0) ||
					((-ca.x * -da.y) - (-da.x * -ca.y) > 0 && (-cb.x * -db.y) - (-db.x * -cb.y) < 0))
				{
					int x1 = poly.PTS.at(i).x;
					int y1 = poly.PTS.at(i).y;
					int x2 = poly.PTS.at(asc).x;
					int y2 = poly.PTS.at(asc).y;
					int x3 = PTS.at(j).x;
					int y3 = PTS.at(j).y;
					int x4 = PTS.at(asc2).x;
					int y4 = PTS.at(asc2).y;
					
					float supx = (x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4);
					float sub = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
					float supy = (x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4);
				
					Point2D I( (int)(supx/sub) , (int)(supy/sub));
					res.push_back(I);
					totalIntersection.push_back(I);
				}			
			}
			//Recupere toutes les intersections sur 1 seul segment, et les rajoute dans l'ordre dans NPTS
			if(i == (int)poly.PTS.size()-1){
				if(!totalIntersection.empty()){
					interTmp = reorganisePoints(PTS.at(j), totalIntersection);
					totalIntersection.clear();					
					NPTS.insert(NPTS.end(),interTmp.begin(),interTmp.end());					
				}
			}
		}
	}
	return res;
}

vector<Point2D> Polygone::intersection(Polygone poly)
 {
	vector<Point2D> tmp = vector<Point2D>();
	tmp = estIntersecte(poly);
	cout<<"nbPtsIntersect :"<<tmp.size()<<endl;
	/*for (int i = 0; i < (int)tmp.size(); i++)
	{
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_POINTS);
			glVertex2i(tmp.at(i).x, tmp.at(i).y);
		glEnd();
	}*/
	return tmp;	
 }

vector<Point2D> Polygone::reorganisePoints(Point2D a, vector<Point2D> vec){
	vector<float> vecDistances;
	vector<Point2D> res;
	for(int i = 0; i < (int)vec.size(); i++){
		vecDistances.push_back(distance(a, vec.at(i)));//Rempli le tableau de distances
	}
	
	std::sort(vecDistances.begin(), vecDistances.end());//Mets le tableau des distances dans l'ordre croissant
	
	cout<<endl;
	for(int i = 0; i < (int)vecDistances.size(); i++){
		for(int j = 0; j < (int)vec.size(); j++){
			if(distance(a, vec.at(j)) == vecDistances.at(i))
				res.push_back(vec.at(j));//On recale les points du plus proche au plus eloigne, par rapport a A
		}
	}
	return res;
}

float Polygone::distance(Point2D a, Point2D b){
	return (float)sqrt( pow((double)(b.x - a.x), 2) + pow((double)(b.y - a.y), 2) );
}

/*
void Polygone::unionPoly(Polygone p1, Polygone p2){
	vector<Point2D> res;
	vector<int> indiceP1;
	vector<int> indiceP2;
	vector<int> testPos;
	bool enonce = true;
	bool ptIntersection = false;//Pour savoir si on est sur un point d'intersection avec l'autre polygone.
	bool parcoursPoly1 = true;
	bool premierParcours = false;
	bool finParcours = false;
	bool poly1Termine = false;
	bool calculePosP1 = false, calculePosP2 = false;
	Point2D lastPtInter1, lastPtInter2(-1, -1);
	int sensPoly2 = 0, lastValPoly2, posPoly1 = 0, posPoly2 = -1, finPoly2 = 0;
	int debut = -1, fin = -1, cpt;

	p1.intersection(p2);
	p2.intersection(p1);


	for(int i = 0; i < (int)p1.NPTS.size(); i++){
		indiceP1.push_back(0);
	}

	for(int i = 0; i < (int)p2.NPTS.size(); i++){
		indiceP2.push_back(0);
	}
	cout<<endl;

	while(!finParcours){
		if(parcoursPoly1 && posPoly1 != (int)p1.NPTS.size()){	//Si on parcours le poly 1
			cout<<"Parcours du polygone 1 : "<<posPoly1<<" contre : "<<(int)p1.NPTS.size()<<endl;
			ptIntersection = false;

			if(!premierParcours){
				//On marque chaque points du poly1 situe a l'interieur du poly 2
				for(int i = 0; i < (int)p1.NPTS.size(); i++){
					if(p2.estInterieur(p1.NPTS.at(i).x, p1.NPTS.at(i).y)){
						indiceP1.at(i) = 1;
					}
				}

				cout<<"Affichage des marqueurs sur P1 : "<<endl;
			for(int i = 0; i < (int)indiceP1.size(); i++){
					cout<<indiceP1.at(i)<<" - ";
				}
			cout<<endl;
				//Si on a parcouru tous les points du polygone et que le dernier est egalement a l'interieur de p2
				//Donc p1 est inclu integralement dans p2 -> le resultat de l'union est p2.
				int cpt2 = 0;
				for(int i = 0; i < (int)p1.NPTS.size(); i++){
					if(p2.estInterieur(p1.NPTS.at(i).x, p1.NPTS.at(i).y)){
						cpt2++;	
					}
					if(cpt2 == (int)p1.NPTS.size()-1){
						res = p2.PTS;
						finParcours = true;
						parcoursPoly1 = false;
						posPoly1 = (int)p1.NPTS.size();
					}
				}
				/*
				if(p1.estInterieur(p2.NPTS.at(posPoly2).x, p2.NPTS.at(posPoly2).y) && posPoly2 < (int)p2.NPTS.size()){
					res = p1.PTS;
					finParcours = true;
					parcoursPoly1 = false;
					posPoly1 = (int)p1.NPTS.size()-1;
				}
				* /
				premierParcours = true;
			}

			if(posPoly1 != 0 && posPoly1 != ((int)p1.NPTS.size()) && calculePosP1){
				pointSuivant(p1, p2, lastPtInter2, posPoly1);
				indiceP1.at(posPoly1-1) = 1;
				calculePosP1 = false;
				if(posPoly1 == p1.NPTS.size())
					posPoly1--;
				cout<<"Nouvelle position de PosPoly1 calculee = "<<posPoly1<<endl;
			}

			while(parcoursPoly1 && posPoly1 != (int)p1.NPTS.size()){//Tant qu'on ne rencontre pas de point d'intersection
				testPos.push_back(posPoly1);
				cpt = 0;
				for(int j = 0; j < (int)p1.PTS.size(); j++){//Verifie si le point a l'indice posPoly1 existe dans la liste PTS
					if(p1.NPTS.at(posPoly1).estEgal(p1.PTS.at(j))){
						cpt++;
					}
					if(cpt == 0 && j == (int)p1.PTS.size()-1){
						ptIntersection = true;
					}
				}

				if(!p2.estInterieur(p1.NPTS.at(posPoly1).x, p1.NPTS.at(posPoly1).y) && !ptIntersection){//Au cas ou, on verifie qu'il n'est pas a l'interieur du 2e poly
					res.push_back(p1.NPTS.at(posPoly1));
					indiceP1.at(posPoly1) = 1;
					cout<<"Sur P1 : On ajoute 1 a l'indice i = "<<posPoly1<<endl<<endl;
					if(posPoly1 == (int)p1.NPTS.size()-1){
						parcoursPoly1 = false;
						cout<<"Dernier passage sur P1 effectue"<<endl;
					}
					posPoly1++;
					
					
					
				}

				//Si on est sur un point d'intersection
				if(ptIntersection){
					cout<<"While Parcours Poly 1 : Test 4"<<endl;
					res.push_back(p1.NPTS.at(posPoly1));
					lastPtInter1.x = p1.NPTS.at(posPoly1).x;
					lastPtInter1.y = p1.NPTS.at(posPoly1).y;
					parcoursPoly1 = false;
					calculePosP2 = true;
					ptIntersection = false;
					indiceP1.at(posPoly1) = 1;
					cout<<"Sur P1 : On ajoute 1 a l'indice i = "<<posPoly1<<endl;
					if(posPoly1 == (int)p1.NPTS.size()-1){
						posPoly1++;
						cout<<"Dernier passage sur P1 effectue"<<endl;
					}
					
				}

				//Si tous les point sont indices a 1, c'est qu'on a parcouru tous les points de chaque polygone
				int decompte = 0;
				for(int i = 0; i < (int)indiceP1.size(); i++){
					if(indiceP1.at(i) == 0)
						decompte++;
				}
				if(decompte == 0)
					finParcours = true;

				//Marque les points qui ont ete parcourus, ou qui ne servent pas
			cout<<"Affichage des marqueurs sur P1 : "<<endl;
			for(int i = 0; i < (int)indiceP1.size(); i++){
					cout<<indiceP1.at(i)<<" - ";
				}
			cout<<endl;

			cout<<"Affiche les valeurs de POSPOLY1 qui ont ete prises en compte : "<<endl;
			for(int i = 0; i < (int)testPos.size(); i++){
				cout<<testPos.at(i)<<" - ";
			}
			cout<<endl<<endl;
			}//Fin While
		}//Fin parcours du Poly1



		//Si on parcours le poly 2
		if(!parcoursPoly1 && posPoly1 != (int)p1.NPTS.size()){
			cout<<"Parcours du poly 2!"<<endl;

			//Si posPoly2 a deja recu une valeur; on est deja passe sur le poly2
			if(lastPtInter2.estEgal(Point2D(-1, -1))){
				sensPoly2 = pointSuivant(p2, p1, lastPtInter1, finPoly2);
				if(sensPoly2 > 0)
					cout<<"SENS POSITIF"<<endl;
				else
					cout<<"SENS NEGATIF"<<endl;
				//Determine l'indice du tableau a partir duquel on commence a parcourir le poly2
				for(int j = 0; j < (int)p2.NPTS.size(); j++){//Verifie si le point a l'indice posPoly2 existe dans la liste PTS
					if(p2.NPTS.at(j).estEgal(lastPtInter1)){
						finPoly2 = j;
						indiceP2.at(j) = 1;
						cout<<"POLY 2 : On commence a partir de l'indice : "<<finPoly2<<endl;
					}
				}
			}
			
			lastPtInter2.x = lastPtInter1.x;
			lastPtInter2.y = lastPtInter1.y;

			while(!parcoursPoly1  /*posPoly2 != finPoly2* /){//Tant qu'on ne rencontre pas de point d'intersection, on est sur poly2
				cpt = 0;
				cout<<"On passe dans le parcours du POLY 2"<<endl;
				if(calculePosP2){
					pointSuivant(p2, p1, lastPtInter1, posPoly2);
					calculePosP2 = false;
					cout<<"Test de POSPOLY2 : "<<posPoly2<<endl;
				}
				cout<<"POLY 2 : Le point pris en compte est : "<<posPoly2<<endl<<endl;
				for(int j = 0; j < (int)p2.PTS.size(); j++){//Verifie si le point a l'indice posPoly2 existe dans la liste PTS
					if(p2.NPTS.at(posPoly2).estEgal(p2.PTS.at(j))){
						cpt++;
					}
					if(cpt == 0 && j == (int)p2.PTS.size()-1)
						ptIntersection = true;
					if(ptIntersection == true && j == (int)p2.PTS.size()-1)
						cout<<"Le point "<<posPoly2<<"est un point d'intersection"<<endl;
					if(ptIntersection == false && j == (int)p2.PTS.size()-1) 
						cout<<"Le point "<<posPoly2<<"n'est pas un point d'intersection"<<endl;
				}

				if(!p1.estInterieur(p2.NPTS.at(posPoly2).x, p2.NPTS.at(posPoly2).y) && !ptIntersection){//Au cas ou, on verifie qu'il n'est pas a l'interieur du 2e poly
					cout<<"J'AJOUTE LE POINT DANS P2"<<endl;
					res.push_back(p2.NPTS.at(posPoly2));
					indiceP2.at(posPoly2) = 1;
					if(sensPoly2 > 0){//Si on est dans le sens positif
						if(posPoly2 == (int)p2.NPTS.size()-1){//Si on est sur le dernier point du tableau
							posPoly2 = 0;//On repart du debut
							cout<<"Sens Positif, on est au bout de la liste -> on remet posPoly2 a 0"<<endl<<endl;
						}
						else{ posPoly2++;//Sinon on incremente normalement
							cout<<"Sens Positif, on incremente simplement"<<endl<<endl;
						}
					}
					else{//si on est dans le sens negatif
						if(posPoly2 == 0){//On est a la toute premiere case du tableau
							posPoly2 = (int)p2.NPTS.size()-1;//On va a la derniere case
							cout<<"Sens Negatif, on est au debut de la liste -> on remet posPoly2 a la fin de la liste"<<endl<<endl;
						}
						else{ posPoly2--;//Sinon on decremente normalement
						cout<<"Sens Negatif, on decremente simplement"<<endl<<endl;
						}

					}
					cout<<"LA NOUVELLE VALEUR DE POSPOLY2 EST : "<<posPoly2<<endl;
				}

				//Si on est sur un point d'intersection
				if(ptIntersection){
					cout<<"J'AJOUTE LE POINT  D'INTERSECTION DANS P2"<<endl;
					res.push_back(p2.NPTS.at(posPoly2));
					lastPtInter2.x = p2.NPTS.at(posPoly2).x;
					lastPtInter2.y = p2.NPTS.at(posPoly2).y;
					parcoursPoly1 = true;
					ptIntersection = false;
					calculePosP1 = true;
					indiceP2.at(posPoly2) = 1;
				}

				//Marque les points qui ont ete parcourus, ou qui ne servent pas
			cout<<"Affichage des marqueurs sur P2 : "<<endl;
			for(int i = 0; i < (int)indiceP2.size(); i++){
					cout<<indiceP2.at(i)<<" - ";
			}
			cout<<endl;

				//Si tous les point sont indices a 1, c'est qu'on a parcouru tous les points de chaque polygone
				int decompte = 0;
				for(int i = 0; i < (int)indiceP1.size(); i++){
					if(indiceP1.at(i) == 0)
						decompte++;
				}
				if(decompte == 0)
					finParcours = true;
			}//Fin While
		}//Fin parcours du poly2
	}//Fin Parcours des Polygones 1 & 2
	if(res.at(res.size()-3).estEgal(res.at(res.size()-1)))
		res.pop_back();
	PTS = res;
	cout<<endl<<endl<<endl<<"Voila enfin le resultat tant attendu !!!"<<endl;
		for(int i = 0; i < (int)PTS.size(); i++){
			cout<<PTS.at(i).x<<" - ";
		}
		cout<<endl<<"C'est fini :)"<<endl;
}
*/

//Determine le sens dans lequel le polygone sera parcouru, dans le cas d'une Intersection
int Polygone::determineSensIntersection(int pos, Polygone p1, vector<int> mark){
	int res = 0, tmp;

	//Si c'est un point normal, on parcours dans le sens normal, car seul le 1er point du 1er poly entre dans ce cas
	if(!p1.NPTS.at(pos).estPtIntersection(p1.PTS))
		res = 1;
	//Sinon : le point suivant ne doit pas etre marque mais a le droit d'etre un point d'intersection
	else{
		//On regarde dabord sur l'element +1
		if(pos <= (int)p1.NPTS.size()-2){
			if(mark.at(pos+1) != 1 && !p1.NPTS.at(pos+1).estPtIntersection(p1.PTS))
				res = 1;
			else{
				tmp = pos-1;
				if(pos == 0) 
					tmp = (int)p1.NPTS.size()-1;
				if(mark.at(tmp) != 1 && !p1.NPTS.at(tmp).estPtIntersection(p1.PTS))
					res = -1;
			} 
		}
		else{//si pos == p1.NPTS.size()-1 -> on est sur le dernier element de la liste
			if(mark.at(0) != 1 && !p1.NPTS.at(0).estPtIntersection(p1.PTS))//l'element pos+1 devient le premier element de la liste
				res = 1;
			else{
				tmp = pos-1;
				if(mark.at(tmp) != 1 && !p1.NPTS.at(tmp).estPtIntersection(p1.PTS))
					res = -1;
			}
		}
	}
	if(res == 0){//Pas de point normal non marque, cette fois, on regarde sur les points d'intersection
		//On regarde dabord sur l'element +1
		if(pos <= (int)p1.NPTS.size()-2){
			if(mark.at(pos+1) != 1)
				res = 1;
			else
				res = -1;
			} 
		else{//si pos == p1.NPTS.size()-1 -> on est sur le dernier element de la liste
			if(mark.at(0) != 1)//l'element pos+1 devient le premier element de la liste
				res = 1;
			else{
					res = -1;
			}
		}
	}
	return res;
}

bool Polygone::pointAppartientAListe(Point2D pt, vector<Point2D>lstPts){
	bool res = false;
	for(int i = 0; i < (int)lstPts.size(); i++){
		if(lstPts.at(i) == pt)
			res = true;
	}
	return res;
}

int Polygone::determineIndicePtSuivant(int pos, int sensParcours, int tailleListe){
	if(sensParcours == 1){//sens Positif
		if(pos < (tailleListe-1))
			pos++;
		else
			pos = 0;
	}
	else{//sens negatif
		if(pos != 0)
			pos--;
		else
			pos = tailleListe-1;
	}
	return pos;
}


void Polygone::UNIONPoly(Polygone p1, Polygone p2){
	//verifier qu'un poly ne contient pas l'autre entierement
	Point2D ptSuivant;
	bool finirParcours = false;
	vector<int>marqueurP1;
	vector<int>marqueurP2;
	vector<Point2D> res;
	vector<vector<Point2D>> lstRes;
	vector<vector<Point2D>> lstTmp;

	bool mark1 = false, mark2 = false;
	int cptMark, test = 0;

	//Avant de chercher l'union entre les deux polygones, on determine l'union entre les trous et les polygones
	for(int i = 0; i < (int)p1.TROUS.size(); i++){
		
		lstTmp = UNIONTrou(p1.TROUS.at(i), p2);
		for(int j = 0; j < (int)lstTmp.size(); j++){
			lstRes.push_back(lstTmp.at(j));
		}
		lstTmp.clear();
	}

	for(int i = 0; i < (int)p2.TROUS.size(); i++){
		
		lstTmp = UNIONTrou(p2.TROUS.at(i), p1);
		for(int j = 0; j < (int)lstTmp.size(); j++){
			lstRes.push_back(lstTmp.at(j));
		}
		lstTmp.clear();
	}

	p1.intersection(p2);
	p2.intersection(p1);
	//Initialise les marqueurs & marque les points qui sont deja a l'interieur du polygone oppose
	for(int i = 0; i < (int)p1.NPTS.size(); i++){
		if(p2.estInterieur(p1.NPTS.at(i).x, p1.NPTS.at(i).y) && !p1.NPTS.at(i).estPtIntersection(p1.PTS))
			marqueurP1.push_back(1);
		else
			marqueurP1.push_back(0);
	}

	for(int i = 0; i < (int)p2.NPTS.size(); i++){
		if(p1.estInterieur(p2.NPTS.at(i).x, p2.NPTS.at(i).y) && !p2.NPTS.at(i).estPtIntersection(p2.PTS))
			marqueurP2.push_back(1);
		else
			marqueurP2.push_back(0);
	}

	for(int i = 0; i < (int)p1.NPTS.size(); i++){
		if(p2.estInterieur(p1.NPTS.at(i).x, p1.NPTS.at(i).y))
			test++;
	}
	if(test == (int)p1.NPTS.size()){
		lstRes.push_back(p2.NPTS);
		finirParcours = true;
	}
	else if(test == 0){
		lstRes.push_back(p1.NPTS);
		finirParcours = true;
	}

	//On initialise ptSuivant avec la toute premiere valeur du poly1
	ptSuivant = p1.PTS.at(0);

	//On parcours tour a tour chaque polygone
	while(!finirParcours){
		/*
			si le point existe deja a l'element 0 : on a fait le tour exterieur du polygone
			on se met donc a chercher les trous
		*/
		if(ptSuivant.trouvePosDansListe(res) == 0)
			ptSuivant.Set(-1, -1);//dans "parcoursPolygone -> si on a (-1, -1), on verifie la liste et on traite ou pas"
			//cree un nouveau vector a passer en parametre, ajouter ce vector dans une liste de polygones
			//le polygone qui englobe les autres est le polygone principal (PTS), les autres sont des trous (TROUS)
		if(!mark1){
			if(ptSuivant == Point2D(-1, -1)){
				if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
				lstRes.push_back(res);
				res.clear();
			}
			
			
			ptSuivant = parcoursPolygone(ptSuivant, p1, p2, marqueurP1, res, "union");
		}

		if(!mark2){
			if(ptSuivant == Point2D(-1, -1)){
				if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
				lstRes.push_back(res);
				res.clear();
			}

			ptSuivant = parcoursPolygone(ptSuivant, p2, p1, marqueurP2, res, "union");
			cout<<endl;
		}

		/*Condition de sortie mark = true si tous les points ont ete parcourus*/
		cptMark = 0;
		for(int i = 0; i < (int)marqueurP1.size(); i++)
			if(marqueurP1.at(i) == 0)
				cptMark++;
		if(cptMark == 0){
			mark1 = true;
		}
		cptMark = 0;
		for(int i = 0; i < (int)marqueurP2.size(); i++)
			if(marqueurP2.at(i) == 0)
				cptMark++;
		if(cptMark == 0){
			mark2 = true;
		}
		if(mark1 && mark2){
			finirParcours = true;
			if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
			lstRes.push_back(res);
		}

	}
	

	//On a termine de parcourir les polygones -> on stocke le resultat.
	determinePolyTrou(lstRes);
	//unionRecupereTrous(p1.TROUS, p2.TROUS);
}



Point2D Polygone::parcoursPolygone(Point2D ptAnalyse, Polygone p1, Polygone p2, vector<int>& lstMark, vector<Point2D>& listePts, string typeOp){
	int pos = 0, sensDeParcours = 0, cptMark = 0;
	Point2D res, PtSuivant;
	bool ptIntersectionRencontre = false;//Condition de sortie du While
	

	pos = ptAnalyse.trouvePosDansListe(p1.NPTS);


	cout<<"Valeur de pos : "<<pos<<endl;
	//Dans le cas du debut de parcours, on verife qu'on ne commence pas sur un point deja marqué
	if(pos == -1){
		pos = 0;
		while(lstMark.at(pos) == 1)
			pos++;
	}
	
	
	//Verifie qu'on ne vient pas tout juste de terminer le polygone
	for(int i = 0; i < (int)lstMark.size(); i++)
		if(lstMark.at(i) == 0)
			cptMark++;
	if(cptMark == 0){
		ptIntersectionRencontre = true;
	}


	if(listePts.size() != 0 && !ptIntersectionRencontre)			//Verifie qu'on ajoute pas 2 fois le meme point
		if(p1.NPTS.at(pos) != listePts.at((int)listePts.size()-1))
			listePts.push_back(p1.NPTS.at(pos));
	if(typeOp == "union")
		if(listePts.size() == 0 && lstMark.at(pos) == 0){
			listePts.push_back(p1.NPTS.at(pos));
		}

	if(typeOp == "intersection")
		if(listePts.empty() && lstMark.at(pos) == 0)
			listePts.push_back(p1.NPTS.at(pos));

	if(!ptIntersectionRencontre)
		sensDeParcours = determineSensIntersection(pos, p1, lstMark);

	cout<<"Sens = "<<sensDeParcours<<endl<<endl;
	lstMark.at(pos) = 1;				//Le point est valide, on le marque

	while(!ptIntersectionRencontre){
		pos = determineIndicePtSuivant(pos, sensDeParcours, (int)p1.NPTS.size());//Trouve le point suivant a chaque tour de boucle
		cout<<"Indice du point en cours : "<<pos<<endl;

		//si ce n'est pas un point d'intersection, on l'ajoute et on le marque, et on continue la boucle
		if(!p1.NPTS.at(pos).estPtIntersection(p1.PTS)){
			cout<<"N'est pas un pt d'intersection"<<endl;
			if(/*pointAppartientAListe(p1.NPTS.at(pos), listePts) ||*/ lstMark.at(pos) == 1){
				ptIntersectionRencontre = true;
				res.Set(-1, -1);
				cout<<"POINT DEJA ENREGISTRE -> FIN DES CALCULS"<<endl;
			}
			if(lstMark.at(pos) == 0)
				listePts.push_back(p1.NPTS.at(pos));
			lstMark.at(pos) = 1;
			
			
		}		
		//sinon on l'ajoute, on le marque, et on sort de la boucle
		else{
			if(lstMark.at(pos) == 1){
				res.Set(-1, -1);
				cout<<"POINT INTERSECTION DEJA ENREGISTRE -> FIN DES CALCULS"<<endl;
			}
			else{
				if(lstMark.at(pos) == 0)
					listePts.push_back(p1.NPTS.at(pos));

				lstMark.at(pos) = 1;
				res = p1.NPTS.at(pos);
				cout<<"Est un pt d'intersection : Changement de poly"<<endl;
			}
			ptIntersectionRencontre = true;
		}
	}
	return res;
}

//Determine quels polygones sont a l'interieur des autres -> permet de recuperer le poly et ses trous
void Polygone::determinePolyTrou(vector<vector<Point2D>> liste){
	vector<Point2D> ptsPoly;
	Polygone futurTrou;
	int cpt, indicePoly = 0;

	PTS = liste.at(0);
	if((int)liste.size() > 1){
		for(int i = 1; i < (int)liste.size(); i++){
			cpt = 0;
			for(int j = 0; j < (int)liste.at(i).size(); j++){
				if(this->estInterieur(liste.at(i).at(j).x, liste.at(i).at(j).y)){
					cpt++;
				}
				if(cpt == 0 && j == (int)liste.at(i).size()-1){
					PTS.clear();
					PTS = liste.at(i);
					indicePoly = i;
				}
			}
		}
	}
	
	//ajoute les trous
	for(int i = 0; i < (int)liste.size(); i++){
		if(i != indicePoly){
			futurTrou.PTS = liste.at(i);
			this->TROUS.push_back(futurTrou);
		}
	}
}

void Polygone::unionRecupereTrous(vector<Polygone> trouP1, vector<Polygone>trouP2){
	for(int i = 0; i < (int)trouP1.size(); i++)
		this->TROUS.push_back(trouP1.at(i));

	for(int i = 0; i < (int)trouP2.size(); i++)
		this->TROUS.push_back(trouP2.at(i));
}

void Polygone::INTERSECTIONPoly(Polygone p1, Polygone p2){
	//verifier qu'un poly ne contient pas l'autre entierement
	Point2D ptSuivant;
	bool finirParcours = false;
	vector<int>marqueurP1;
	vector<int>marqueurP2;
	vector<Point2D> res;
	vector<vector<Point2D>> lstRes;

	bool mark1 = false, mark2 = false;
	int cptMark, test = 0;
	int cptPtSuivant = 0;

	p1.intersection(p2);
	p2.intersection(p1);
	//Initialise les marqueurs & marque les points qui sont deja a l'exterieur du polygone oppose
	for(int i = 0; i < (int)p1.NPTS.size(); i++){
		if(!p2.estInterieur(p1.NPTS.at(i).x, p1.NPTS.at(i).y) && !p1.NPTS.at(i).estPtIntersection(p1.PTS))
			marqueurP1.push_back(1);
		else
			marqueurP1.push_back(0);
	}

	for(int i = 0; i < (int)p2.NPTS.size(); i++){
		if(!p1.estInterieur(p2.NPTS.at(i).x, p2.NPTS.at(i).y) && !p2.NPTS.at(i).estPtIntersection(p2.PTS))
			marqueurP2.push_back(1);
		else
			marqueurP2.push_back(0);
	}

	cout<<"Affichage des marqueurs sur P1 : "<<endl;
		for(int i = 0; i < (int)marqueurP1.size(); i++){
				cout<<marqueurP1.at(i)<<" - ";
		}
		cout<<endl;

	//La variable test est la pour savoir si un des deux poly est entierement a l'interieur de l'autre
	for(int i = 0; i < (int)p1.NPTS.size(); i++){
		if(p2.estInterieur(p1.NPTS.at(i).x, p1.NPTS.at(i).y))
			test++;
	}
	if(test == (int)p1.NPTS.size()){
		lstRes.push_back(p1.NPTS);
		finirParcours = true;
	}
	else if(test == 0){
		lstRes.push_back(p2.NPTS);
		finirParcours = true;
	}

	//On initialise ptSuivant avec la toute premiere valeur du poly1
	while(marqueurP1.at(cptPtSuivant) != 0)
		cptPtSuivant++;
	if(cptPtSuivant > 0)
		cptPtSuivant--;
	ptSuivant = p1.PTS.at(cptPtSuivant);

	//On parcours tour a tour chaque polygone
	while(!finirParcours){
		/*
			si le point existe deja a l'element 0 : on a fait le tour exterieur du polygone
			on se met donc a chercher les trous
		*/
		if(ptSuivant.trouvePosDansListe(res) == 0)
			ptSuivant.Set(-1, -1);//dans "parcoursPolygone -> si on a (-1, -1), on verifie la liste et on traite ou pas"
			//cree un nouveau vector a passer en parametre, ajouter ce vector dans une liste de polygones
			//le polygone qui englobe les autres est le polygone principal (PTS), les autres sont des trous (TROUS)
		if(!mark1){
			if(ptSuivant == Point2D(-1, -1)){
				if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
				lstRes.push_back(res);
				res.clear();
				cout<<"OMG on a plusieurs poly :o"<<endl;
			}
			
			
			cout<<"Poly 1 :"<<endl;
			ptSuivant = parcoursPolygone(ptSuivant, p1, p2, marqueurP1, res, "intersection");
			cout<<endl;
		}
		//Marque les points qui ont ete parcourus, ou qui ne servent pas
		cout<<"Affichage des marqueurs sur P1 : "<<endl;
		for(int i = 0; i < (int)marqueurP1.size(); i++){
				cout<<marqueurP1.at(i)<<" - ";
		}
		cout<<endl;

		if(!mark2){
			if(ptSuivant == Point2D(-1, -1)){
				if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
				lstRes.push_back(res);
				res.clear();
				cout<<"OMG on a plusieurs poly :o"<<endl;
			}

			cout<<"Poly 2 :"<<endl;
			ptSuivant = parcoursPolygone(ptSuivant, p2, p1, marqueurP2, res, "intersection");
			cout<<endl;
		}

		//Marque les points qui ont ete parcourus, ou qui ne servent pas
		cout<<"Affichage des marqueurs sur P2 : "<<endl;
		for(int i = 0; i < (int)marqueurP2.size(); i++){
				cout<<marqueurP2.at(i)<<" - ";
		}
		cout<<endl;

		/*Condition de sortie mark = true si tous les points ont ete parcourus*/
		cptMark = 0;
		for(int i = 0; i < (int)marqueurP1.size(); i++)
			if(marqueurP1.at(i) == 0)
				cptMark++;
		if(cptMark == 0){
			mark1 = true;
			cout<<"LE POLY 1 EST COMPLET ! "<<endl;
		}
		cptMark = 0;
		for(int i = 0; i < (int)marqueurP2.size(); i++)
			if(marqueurP2.at(i) == 0)
				cptMark++;
		if(cptMark == 0){
			mark2 = true;
			cout<<"LE POLY 2 EST COMPLET ! "<<endl;
		}
		if(mark1 && mark2){
			finirParcours = true;
			if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
			lstRes.push_back(res);
		}

	}
	//Faire fonction ou envoyer les trous de p1, puis p2 : meme principe que la difference
	
	
	//On a termine de parcourir les polygones -> on stocke le resultat.
	
	determinePolyTrou(lstRes);
	//unionRecupereTrous(p1.TROUS, p2.TROUS);
}


//On envoie en parametre le trou du poly1 et le poly2 afin de determiner la nouvelle forme du trou en question
vector<vector<Point2D>> Polygone::UNIONTrou(Polygone p1, Polygone p2){
	//verifier qu'un poly ne contient pas l'autre entierement
	Point2D ptSuivant;
	bool finirParcours = false;
	vector<int>marqueurP1;
	vector<int>marqueurP2;
	vector<Point2D> res;
	vector<vector<Point2D>> lstRes;

	bool mark1 = false, mark2 = false;
	int cptMark, test = 0, cptPtSuivant = 0;

	p1.intersection(p2);
	p2.intersection(p1);
	//Initialise les marqueurs & marque les points de p1 qui sont deja a l'interieur du polygone oppose
	for(int i = 0; i < (int)p1.NPTS.size(); i++){
		if(p2.estInterieur(p1.NPTS.at(i).x, p1.NPTS.at(i).y) && !p1.NPTS.at(i).estPtIntersection(p1.PTS))
			marqueurP1.push_back(1);
		else
			marqueurP1.push_back(0);
	}


	//Initialise les marqueurs & marque les points de p2 qui sont deja a l'exterieur du polygone oppose
	for(int i = 0; i < (int)p2.NPTS.size(); i++){
		if(!p1.estInterieur(p2.NPTS.at(i).x, p2.NPTS.at(i).y) && !p2.NPTS.at(i).estPtIntersection(p2.PTS))
			marqueurP2.push_back(1);
		else
			marqueurP2.push_back(0);
	}

	for(int i = 0; i < (int)p1.NPTS.size(); i++){
		if(p2.estInterieur(p1.NPTS.at(i).x, p1.NPTS.at(i).y))
			test++;
	}

	if(test == (int)p1.NPTS.size()){
		finirParcours = true;
	}
	else if(test == 0){
		lstRes.push_back(p1.NPTS);
		finirParcours = true;
	}

	if(!finirParcours){
		//On initialise ptSuivant avec la toute premiere valeur du poly1
		while(marqueurP1.at(cptPtSuivant) != 0)
			cptPtSuivant++;
		if(cptPtSuivant > 0)
			cptPtSuivant--;
		ptSuivant = p1.PTS.at(cptPtSuivant);
	}

	//On parcours tour a tour chaque polygone
	while(!finirParcours){
		/*
			si le point existe deja a l'element 0 : on a fait le tour exterieur du polygone
			on se met donc a chercher les trous
		*/
		if(ptSuivant.trouvePosDansListe(res) == 0)
			ptSuivant.Set(-1, -1);//dans "parcoursPolygone -> si on a (-1, -1), on verifie la liste et on traite ou pas"
			//cree un nouveau vector a passer en parametre, ajouter ce vector dans une liste de polygones
			//le polygone qui englobe les autres est le polygone principal (PTS), les autres sont des trous (TROUS)
		if(!mark1){
			if(ptSuivant == Point2D(-1, -1)){
				if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
				lstRes.push_back(res);
				res.clear();
				cout<<"OMG on a plusieurs poly :o"<<endl;
			}
			
			
			cout<<"Poly 1 :"<<endl;
			ptSuivant = parcoursPolygone(ptSuivant, p1, p2, marqueurP1, res, "union");
			cout<<endl;
		}
		//Marque les points qui ont ete parcourus, ou qui ne servent pas
		cout<<"Affichage des marqueurs sur P1 : "<<endl;
		for(int i = 0; i < (int)marqueurP1.size(); i++){
				cout<<marqueurP1.at(i)<<" - ";
		}
		cout<<endl;

		if(!mark2){
			if(ptSuivant == Point2D(-1, -1)){
				if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
				lstRes.push_back(res);
				res.clear();
				cout<<"OMG on a plusieurs poly :o"<<endl;
			}

			cout<<"Poly 2 :"<<endl;
			ptSuivant = parcoursPolygone(ptSuivant, p2, p1, marqueurP2, res, "union");
			cout<<endl;
		}

		//Marque les points qui ont ete parcourus, ou qui ne servent pas
		cout<<"Affichage des marqueurs sur P2 : "<<endl;
		for(int i = 0; i < (int)marqueurP2.size(); i++){
				cout<<marqueurP2.at(i)<<" - ";
		}
		cout<<endl;

		/*Condition de sortie mark = true si tous les points ont ete parcourus*/
		cptMark = 0;
		for(int i = 0; i < (int)marqueurP1.size(); i++)
			if(marqueurP1.at(i) == 0)
				cptMark++;
		if(cptMark == 0){
			mark1 = true;
			cout<<"LE POLY 1 EST COMPLET ! "<<endl;
		}
		cptMark = 0;
		for(int i = 0; i < (int)marqueurP2.size(); i++)
			if(marqueurP2.at(i) == 0)
				cptMark++;
		if(cptMark == 0){
			mark2 = true;
			cout<<"LE POLY 2 EST COMPLET ! "<<endl;
		}
		if(mark1 && mark2){
			finirParcours = true;
			if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
			lstRes.push_back(res);
		}

	}
	return lstRes;
}

void Polygone::DIFFERENCEPoly(Polygone p1, Polygone p2){
	//verifier qu'un poly ne contient pas l'autre entierement
	Point2D ptSuivant;
	bool finirParcours = false;
	vector<int>marqueurP1;
	vector<int>marqueurP2;
	vector<Point2D> res;
	vector<vector<Point2D>> lstRes;
	vector<vector<Point2D>> lstTmp;

	bool mark1 = false, mark2 = false;
	int cptMark, test = 0, cptPtSuivant = 0;
cout<<"Parcours des trous du POLY 1"<<endl;
	//Avant de chercher l'union entre les deux polygones, on determine l'union entre les trous et les polygones
	for(int i = 0; i < (int)p1.TROUS.size(); i++){
		
		lstTmp = UNIONTrou(p1.TROUS.at(i), p2);
		for(int j = 0; j < (int)lstTmp.size(); j++){
			lstRes.push_back(lstTmp.at(j));
		}
		lstTmp.clear();
	}
	cout<<endl<<endl<<endl;
cout<<"Parcours des trous du POLY 2"<<endl;
	for(int i = 0; i < (int)p2.TROUS.size(); i++){
		
		lstTmp = UNIONTrou(p2.TROUS.at(i), p1);
		for(int j = 0; j < (int)lstTmp.size(); j++){
			lstRes.push_back(lstTmp.at(j));
		}
		lstTmp.clear();
	}
cout<<endl<<endl<<endl;
	p1.intersection(p2);
	p2.intersection(p1);

	//Initialise les marqueurs & marque les points qui sont deja a l'interieur du polygone oppose
	for(int i = 0; i < (int)p1.NPTS.size(); i++){
		if(p2.estInterieur(p1.NPTS.at(i).x, p1.NPTS.at(i).y) && !p1.NPTS.at(i).estPtIntersection(p1.PTS))
			marqueurP1.push_back(1);
		else
			marqueurP1.push_back(0);
	}

	for(int i = 0; i < (int)p2.NPTS.size(); i++){
		if(!p1.estInterieur(p2.NPTS.at(i).x, p2.NPTS.at(i).y) && !p2.NPTS.at(i).estPtIntersection(p2.PTS))
			marqueurP2.push_back(1);
		else
			marqueurP2.push_back(0);
	}

	for(int i = 0; i < (int)p1.NPTS.size(); i++){
		if(p2.estInterieur(p1.NPTS.at(i).x, p1.NPTS.at(i).y))
			test++;
	}
	if(test == (int)p1.NPTS.size()){
		lstRes.push_back(p2.NPTS);
		finirParcours = true;
	}
	else if(test == 0){
		TROUS.push_back(p2);
		finirParcours = true;
	}

	//On initialise ptSuivant avec la toute premiere valeur du poly1
	//ptSuivant = p1.PTS.at(0);

	while(marqueurP1.at(cptPtSuivant) != 0)
		cptPtSuivant++;
	if(cptPtSuivant > 0)
		cptPtSuivant--;
	ptSuivant = p1.PTS.at(cptPtSuivant);

	//On parcours tour a tour chaque polygone
	while(!finirParcours){
		/*
			si le point existe deja a l'element 0 : on a fait le tour exterieur du polygone
			on se met donc a chercher les trous
		*/
		if(ptSuivant.trouvePosDansListe(res) == 0)
			ptSuivant.Set(-1, -1);//dans "parcoursPolygone -> si on a (-1, -1), on verifie la liste et on traite ou pas"
			//cree un nouveau vector a passer en parametre, ajouter ce vector dans une liste de polygones
			//le polygone qui englobe les autres est le polygone principal (PTS), les autres sont des trous (TROUS)
		if(!mark1){
			if(ptSuivant == Point2D(-1, -1)){
				if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
				lstRes.push_back(res);
				res.clear();
			}
			
			
			ptSuivant = parcoursPolygone(ptSuivant, p1, p2, marqueurP1, res, "union");
		}

		if(!mark2){
			if(ptSuivant == Point2D(-1, -1)){
				if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
				lstRes.push_back(res);
				res.clear();
			}

			ptSuivant = parcoursPolygone(ptSuivant, p2, p1, marqueurP2, res, "union");
			cout<<endl;
		}

		/*Condition de sortie mark = true si tous les points ont ete parcourus*/
		cptMark = 0;
		for(int i = 0; i < (int)marqueurP1.size(); i++)
			if(marqueurP1.at(i) == 0)
				cptMark++;
		if(cptMark == 0){
			mark1 = true;
		}
		cptMark = 0;
		for(int i = 0; i < (int)marqueurP2.size(); i++)
			if(marqueurP2.at(i) == 0)
				cptMark++;
		if(cptMark == 0){
			mark2 = true;
		}
		if(mark1 && mark2){
			finirParcours = true;
			if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
			lstRes.push_back(res);
		}

	}
	
	//int test2 = (int)lstRes.size()-3;
	//On a termine de parcourir les polygones -> on stocke le resultat.
	/*for(int i = 0; i < test; i++){
		cout<<"On envoie le poly numero : "<<i<<endl;	
		retireDoublonFromPoly(lstRes.at(i), lstRes, i);

	}
	*/
	determinePolyTrou(lstRes);
	//unionRecupereTrous(p1.TROUS, p2.TROUS);
}




//p1 correspond au trou, p2 au polygone
vector<vector<Point2D>> Polygone::INTERTrou(Polygone p1, Polygone p2){
		//verifier qu'un poly ne contient pas l'autre entierement
	Point2D ptSuivant;
	bool finirParcours = false;
	vector<int>marqueurP1;
	vector<int>marqueurP2;
	vector<Point2D> res;
	vector<vector<Point2D>> lstRes;
	vector<vector<Point2D>> lstTmp;

	bool mark1 = false, mark2 = false;
	int cptMark, dansP1 = 0, dansP2 = 0, cptPtSuivant = 0;
cout<<"Parcours des trous du POLY 1"<<endl;
	//Avant de chercher l'union entre les deux polygones, on determine l'union entre les trous et les polygones
	for(int i = 0; i < (int)p1.TROUS.size(); i++){
		
		lstTmp = UNIONTrou(p1.TROUS.at(i), p2);
		for(int j = 0; j < (int)lstTmp.size(); j++){
			lstRes.push_back(lstTmp.at(j));
		}
		lstTmp.clear();
	}
	cout<<endl<<endl<<endl;
cout<<"Parcours des trous du POLY 2"<<endl;
	for(int i = 0; i < (int)p2.TROUS.size(); i++){
		
		lstTmp = UNIONTrou(p2.TROUS.at(i), p1);
		for(int j = 0; j < (int)lstTmp.size(); j++){
			lstRes.push_back(lstTmp.at(j));
		}
		lstTmp.clear();
	}
cout<<endl<<endl<<endl;
	p1.intersection(p2);
	p2.intersection(p1);

	//Initialise les marqueurs & marque les points qui sont deja a l'interieur du polygone oppose
	for(int i = 0; i < (int)p1.NPTS.size(); i++){
		if(p2.estInterieur(p1.NPTS.at(i).x, p1.NPTS.at(i).y) && !p1.NPTS.at(i).estPtIntersection(p1.PTS))
			marqueurP1.push_back(1);
		else
			marqueurP1.push_back(0);
	}

	for(int i = 0; i < (int)p2.NPTS.size(); i++){
		if(!p1.estInterieur(p2.NPTS.at(i).x, p2.NPTS.at(i).y) && !p2.NPTS.at(i).estPtIntersection(p2.PTS))
			marqueurP2.push_back(0);
		else
			marqueurP2.push_back(1);
	}

	for(int i = 0; i < (int)p1.NPTS.size(); i++){
		if(p2.estInterieur(p1.NPTS.at(i).x, p1.NPTS.at(i).y))
			dansP2++;
	}
	for(int i = 0; i < (int)p2.NPTS.size(); i++){
		if(p1.estInterieur(p2.NPTS.at(i).x, p2.NPTS.at(i).y))
			dansP1++;
	}
	if(dansP2 == (int)p1.NPTS.size()){
		lstRes.push_back(p2.NPTS);
		finirParcours = true;
	}
	if(dansP1 == (int)p2.NPTS.size()){
		finirParcours = true;
	}

	//On initialise ptSuivant avec la toute premiere valeur du poly1
	//ptSuivant = p1.PTS.at(0);

	while(marqueurP1.at(cptPtSuivant) != 0)
		cptPtSuivant++;
	if(cptPtSuivant > 0)
		cptPtSuivant--;
	ptSuivant = p1.PTS.at(cptPtSuivant);

	//On parcours tour a tour chaque polygone
	while(!finirParcours){
		/*
			si le point existe deja a l'element 0 : on a fait le tour exterieur du polygone
			on se met donc a chercher les trous
		*/
		if(ptSuivant.trouvePosDansListe(res) == 0)
			ptSuivant.Set(-1, -1);//dans "parcoursPolygone -> si on a (-1, -1), on verifie la liste et on traite ou pas"
			//cree un nouveau vector a passer en parametre, ajouter ce vector dans une liste de polygones
			//le polygone qui englobe les autres est le polygone principal (PTS), les autres sont des trous (TROUS)
		if(!mark1){
			if(ptSuivant == Point2D(-1, -1)){
				if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
				lstRes.push_back(res);
				res.clear();
			}
			
			
			ptSuivant = parcoursPolygone(ptSuivant, p1, p2, marqueurP1, res, "union");
		}

		if(!mark2){
			if(ptSuivant == Point2D(-1, -1)){
				if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
				lstRes.push_back(res);
				res.clear();
			}

			ptSuivant = parcoursPolygone(ptSuivant, p2, p1, marqueurP2, res, "union");
			cout<<endl;
		}

		/*Condition de sortie mark = true si tous les points ont ete parcourus*/
		cptMark = 0;
		for(int i = 0; i < (int)marqueurP1.size(); i++)
			if(marqueurP1.at(i) == 0)
				cptMark++;
		if(cptMark == 0){
			mark1 = true;
		}
		cptMark = 0;
		for(int i = 0; i < (int)marqueurP2.size(); i++)
			if(marqueurP2.at(i) == 0)
				cptMark++;
		if(cptMark == 0){
			mark2 = true;
		}
		if(mark1 && mark2){
			finirParcours = true;
			if(res.at(0) == res.at((int)res.size()-1))
					res.pop_back();
			lstRes.push_back(res);
		}

	}
	
	return lstRes;
}
