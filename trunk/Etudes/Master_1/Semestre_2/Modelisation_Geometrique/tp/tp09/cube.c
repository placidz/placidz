#include "cube.h"

cube::cube(){
	FaceCube.clear();
}

cube::cube(vector<face> lst){
	FaceCube = lst;
}

void cube::setCube(vector<face> lst){
	FaceCube = lst;
}

void cube::polygonise(){
	int numFace = 0;
	int numSegmentCourant = 0;
	int numSegmentNext;
	int test;
	bool finParcours = false;
	for(int i = 0; i < 6; i++)
		markFace.push_back(0);
	for(int i = 0; i < (int)FaceCube.size(); i++){
		FaceCube.at(i).initialiseMarkeur();
	}
		
		while(!finParcours){
			for(int j = 0;  j < (int)FaceCube.size(); j++){
				for(int i = 0;  i < (int)FaceCube.at(j).lstArete.size(); i++){
						if(!(int)FaceCube.at(j).lstArete.at(i).isPtInter){
							test++;
						}
						
				}
			}
			test = 0;
			if(markFace.at(numFace) != 1){
				numSegmentNext = getNextSommet(numSegmentCourant);//On a le sommet actuel, et le suivant
				//Si le point actuel et le suivant sont l'un a l'interieur et l'autre a l'exterieur
				if( FaceCube.at(numFace).lstArete.at(numSegmentCourant).isPtInter != FaceCube.at(numFace).lstArete.at(numSegmentNext).isPtInter){
					lstPolyCube.push_back(FaceCube.at(numFace).lstArete.at(numSegmentCourant).getCentre());//recupere le point au centre du segment
					//On marque la face pour ne plus revenir dessus
					markFace.at(numFace) = 1;
					//Et on change de face, et on recupere l'indice de la nouvelle arete!
					numFace = getNextFace(numFace, FaceCube.at(numFace).lstArete.at(numSegmentCourant));
					numSegmentCourant = getIndiceArete(FaceCube.at(numFace), FaceCube.at(numFace).lstArete.at(numSegmentCourant));
					numSegmentNext = getNextSommet(numSegmentCourant);
				}
				FaceCube.at(numFace).markSegment.at(numSegmentCourant) = true;
				}
				else{
				for(int i = 0; i < (int)markFace.size(); i++)
					if(markFace.at(i) != 1)
						numFace = i;
						//finParcours = true;
				}
				//Si on a parcouru tous les segments sans rien trouver, on passe sur une autre face.
				if(FaceCube.at(numFace).isParcoursSegmentOver()){
					markFace.at(numFace) = 1;
					for(int i = 0; i < (int)markFace.size(); i++)
						if(markFace.at(i) != 1)
							numFace = i;			
				}			
			
			
			//numSegmentNext = getNextSommet();
			/*cout<<endl<<"Segment courant = "<<numSegmentCourant;
			cout<<endl<<"Segment suivant = "<<numSegmentNext<<endl;*/
			//On prepare le tour de boucle suivant
			numSegmentCourant = numSegmentNext;
			//condition de sortie
			if(isParcoursFacesOver())
				finParcours = true;
		}
	markFace.clear();
	/*
	for(int i = 0;  i < (int)lstPolyCube.size(); i++){
		cout<<lstPolyCube.at(i)<<endl;
	}
	* */
}

//Retourne l'indice de la face suivante, a partir de la derniere arete
int cube::getNextFace(int faceCourante, arete ar){
	int res = 0;
	for(int i = 0; i < (int)FaceCube.size(); i++){
		if(markFace.at(i) != 1){//Pour ne pas retomber sur face deja faite^^
			for(int j = 0; j < (int)FaceCube.at(i).lstArete.size(); j++){
				if(ar.estComplement(FaceCube.at(i).lstArete.at(j))){
					res = i;
				}
			}
		}
	}
	return res;
}

//Retourne l'indice de l'arete sur la face consideree, utiliser lorsqu'on change de face
int cube::getIndiceArete(face f, arete ar){
	int res = -1;
	for(int i = 0; i < (int)f.lstArete.size(); i++)
		if(ar.estEgale(f.lstArete.at(i)))
			res = i;
	if(res == -1)
		cout<<"Mauvaise Face :("<<endl;
	return res;
}

//Retourne le sommet qui suit le sommet en cours
int cube::getNextSommet(int SommetCourant){
	int res = 0;
	if(SommetCourant < 3){
		res = SommetCourant+1;
	}
	else if(SommetCourant >= 3){
		res = 0;
	}
	return res;
}

bool cube::isParcoursFacesOver(){
	int tmp = 0;
	bool res = false;
	for(int i = 0; i < (int)markFace.size(); i++){
		if(markFace.at(i) == 1)
			tmp++;
	}
	cout<<endl;
	if(tmp == (int)markFace.size())
		res = true;
		
	return res;
}

void cube::initSommet(){
	for(int i = 0; i < (int)FaceCube.size(); i++){
		for(int j = 0; j < (int)FaceCube.at(i).lstArete.size(); j++){
			FaceCube.at(i).lstArete.at(j).isPtInter = -1;
		}
	}
}


cube& cube::operator=(const cube &op){	
	FaceCube = op.FaceCube; return *this;	
}
