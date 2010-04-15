#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#ifdef __APPLE__
        #include <OpenGL/gl.h>
        #include <OpenGL/glu.h>
        #include <GLUT/glut.h>
#else
        #include <GL/gl.h>
        #include <GL/glu.h>
        #include <GL/glut.h>
#endif

extern "C"
{
#include <ml.h>
#include <he.h>
}

typedef struct{
	float x;
	float y;
	float z;
}Point3D;

int winX = 800;
int winY = 600;

int mouseClick[] = {0, 0, 0};

int modeRendu = GL_FILL;

hePtrMesh mesh;
int currVert;
bool bEdgesDisplay = 0;
bool bPolyDisplay = 0;

//vecteur pour enregistrer les donnees des coposantes connexes dans un fichier .obj
vector<Point3D> lstVertex;
vector<int> lstFaceTmp;
vector<vector<int> > lstFace;
int indiceDebut = 0, indiceFin = 0; 
bool enregistrer = false;


void ecrireObj(string nomFichier){
    FILE* fichier = NULL;
    fichier = fopen(nomFichier.c_str(), "w");
    if (fichier)
    {
		for(int i = 0; i < (int)lstVertex.size(); i++)
			fprintf(fichier, "v %f %f %f\n", lstVertex.at(i).x, lstVertex.at(i).y, lstVertex.at(i).z);
			
		for(int i = 0; i < (int)lstFace.size(); i++){
			
			for(int j = 0; j < (int)lstFace.at(i).size(); j++){
				if(j == 0 && lstFace.at(i).at(j) <= (int)lstVertex.size())
					fprintf(fichier, "f ");
				fprintf(fichier, "%d ", lstFace.at(i).at(j));
			}
			fprintf(fichier, "\n");
		}
    }
    fclose(fichier);
}

int getIndiceVertex(Point3D pt, vector<Point3D>liste){
	int res = -1;
	
	for(int i = 0; i < (int)liste.size(); i++){
			if(pt.x == liste.at(i).x && pt.y == liste.at(i).y && pt.z == liste.at(i).z)
				res = i;
	}
	return res;
}

bool isOkRajouteFace(vector<int>lstFace, vector<vector<int> >liste){
	bool res = true;
	int test = 0;
	
	for(int i = 0; i < (int)liste.size(); i++){
		for(int j = 0; j < (int)lstFace.size(); j++){
			if((int)liste.at(i).size() == (int)lstFace.size())
				if(liste.at(i).at(j) == lstFace.at(j))
					test++;
		}
		if(test == (int)lstFace.size())
			res = false;
		test = 0;
	}	
	return res;
}

void parcoursEnregistrement(hePtrEdge arete, vector<int> &markFace){
	hePtrListItem it, it2;
	Point3D Vetmp;
	vector<int> indicesVertex;//possede l'indice des vertex deja enregistres, et -1 si le vertex n'es pas enregistre
	
	for (it = arete->faces.head; it != NULL; it = it->next){
		//Si la face n'a pas ete marquee, on trace le polygone, et on appelle la fonction reccursivement pour chaque arrete de la face
		if(markFace.at( ((hePtrFace)it->data)->id  )  == 0){
			
			for (it2 = ((hePtrFace)it->data)->edges.head; it2 != NULL; it2 = it2->next){
				Vetmp.x = ((hePtrEdge)it2->data)->head->pos[0];
				Vetmp.y = ((hePtrEdge)it2->data)->head->pos[1];
				Vetmp.z = ((hePtrEdge)it2->data)->head->pos[2];
				indicesVertex.push_back(getIndiceVertex(Vetmp, lstVertex));
				if(indicesVertex.at( (int)indicesVertex.size()-1 ) == -1)
					lstVertex.push_back(Vetmp);
			}
						
			for(int i = 0; i < (int)indicesVertex.size(); i++){
				if(indicesVertex.at(i) != -1)
					lstFaceTmp.push_back( indicesVertex.at(i)+1 );
				
				else{
					lstFaceTmp.push_back( indiceDebut+1 );
					indiceDebut++;
				}
			}
			indiceDebut = lstVertex.size();
			if(isOkRajouteFace(lstFaceTmp, lstFace))
				lstFace.push_back(lstFaceTmp);
			lstFaceTmp.clear();
			indicesVertex.clear();
			//markFace.at( ((hePtrFace)it->data)->id  ) = 1;	
		}
	}	
	
	for (it = arete->twin->faces.head; it != NULL; it = it->next){
		//Si la face n'a pas ete marquee, on trace le polygone, et on appelle la fonction reccursivement pour chaque arrete de la face
		if(markFace.at( ((hePtrFace)it->data)->id  )  == 0){
			
			for (it2 = ((hePtrFace)it->data)->edges.head; it2 != NULL; it2 = it2->next){
				Vetmp.x = ((hePtrEdge)it2->data)->head->pos[0];
				Vetmp.y = ((hePtrEdge)it2->data)->head->pos[1];
				Vetmp.z = ((hePtrEdge)it2->data)->head->pos[2];
				indicesVertex.push_back(getIndiceVertex(Vetmp, lstVertex));
				if(indicesVertex.at( (int)indicesVertex.size()-1 ) == -1)
					lstVertex.push_back(Vetmp);
			}
						
			for(int i = 0; i < (int)indicesVertex.size(); i++){
				if(indicesVertex.at(i) != -1)
					lstFaceTmp.push_back( indicesVertex.at(i)+1 );
				
				else{
					lstFaceTmp.push_back( indiceDebut+1 );
					indiceDebut++;
				}
			}
			indiceDebut = lstVertex.size();
			if(isOkRajouteFace(lstFaceTmp, lstFace))
				lstFace.push_back(lstFaceTmp);
			lstFaceTmp.clear();
			indicesVertex.clear();
			
			markFace.at( ((hePtrFace)it->data)->id  ) = 1;
			for (it2 = ((hePtrFace)it->data)->edges.head; it2 != NULL; it2 = it2->next){
				parcoursEnregistrement(((hePtrEdge)it2->data), markFace);
			}
				
		}
	}	
}

void parcoursAretes(hePtrEdge arete, vector<int> &markFace){
	hePtrListItem it, it2;
	int Indicetmp;
	Point3D Vetmp;
	vector<int> indicesVertex;//possede l'indice des vertex deja enregistres, et -1 si le vertex n'es pas enregistre
	
	for (it = arete->twin->faces.head; it != NULL; it = it->next){
		//Si la face n'a pas ete marquee, on trace le polygone, et on appelle la fonction reccursivement pour chaque arrete de la face
		if(markFace.at( ((hePtrFace)it->data)->id  )  == 0){
			
			glBegin(GL_POLYGON);
			for (it2 = ((hePtrFace)it->data)->edges.head; it2 != NULL; it2 = it2->next){
				glNormal3dv(((hePtrEdge)it2->data)->head->normal);
				glVertex3dv(((hePtrEdge)it2->data)->head->pos);
			}
			glEnd();
			markFace.at( ((hePtrFace)it->data)->id  ) = 1;
			for (it2 = ((hePtrFace)it->data)->edges.head; it2 != NULL; it2 = it2->next){
				parcoursAretes(((hePtrEdge)it2->data), markFace);
			}
				
		}
	}
}

void setColor(int num){
	switch (num){
		case 1:glColor3f(1.0, 1.0, 0.0);break;
		case 2:glColor3f(1.0, 0.0, 0.0);break;
		case 3:glColor3f(0.0, 1.0, 0.0);break;	
		case 4:glColor3f(0.3, 0.5, 1.0);break;
		case 5:glColor3f(1.0, 1.0, 0.0);break;
		case 6:glColor3f(1.0, 0.0, 1.0);break;
		case 7:glColor3f(0.0, 1.0, 1.0);break;
		case 8:glColor3f(1.0, 1.0, 1.0);break;
	}
}

void SauvegardeConnectedComponents(heMesh * _mesh)
{
        int nCC = 0;
        bool nvleComposante = false;
        hePtrListItem it, it2;
        heVert * v = NULL;
        bool fstPassage;
        vector<int> marqueFace(_mesh->nFaces, 0);//initialise le tableau qui servira a marquer les faces
        Point3D Vetmp;
		vector<int> indicesVertex;
        
        string nomFichier = "ComposanteN";
        string nomTmp;
        
        
        //Parcours de toutes les faces du mesh - Traitement des composantes connexes.
        for(int i = 0; i < _mesh->nFaces; ++i)
        {
			fstPassage = true;
			for(it = _mesh->faces[i]->edges.head; it != NULL; it = it->next)
			{
				
				//Si la face n'est pas encore tracee, on lance la fonction
				if(marqueFace.at(_mesh->faces[i]->id) == 0){
					//cout<<"Valeur de la face numero : "<< _mesh->faces[i]->id <<" = "<<marqueFace.at(_mesh->faces[i]->id)<<endl;
					if(fstPassage == true){
						if(nCC != 0){
							ecrireObj(nomTmp);
							lstVertex.clear();
							lstFace.clear();
						}
						std::ostringstream oss;
						nCC++;
						oss << nCC;
						fstPassage = false;
						setColor(nCC);//attribue une couleur selon la valeur de nCC
						nomTmp =  nomFichier + oss.str() + ".obj";//Genere un nom de fichier pour chaque composante
						cout<<nomTmp<<endl;
					}
					//Parcours et marque toutes les faces ratachees a la face en cours
					parcoursEnregistrement((hePtrEdge)it->data, marqueFace);	
				}
			}
			 
        }
		ecrireObj(nomTmp);
}

int countConnectedComponents(heMesh * _mesh)
{
        int nCC = 0;
        bool nvleComposante = false;
        hePtrListItem it;
        heVert * v = NULL;
        bool fstPassage;
        vector<int> marqueFace(_mesh->nFaces, 0);//initialise le tableau qui servira a marquer les faces        
        
        //Parcours de toutes les faces du mesh - Traitement des composantes connexes.
        for(int i = 0; i < _mesh->nFaces; ++i)
        {
			fstPassage = true;
			for(it = _mesh->faces[i]->edges.head; it != NULL; it = it->next)
			{
				
				//Si la face n'est pas encore tracee, on lance la fonction
				if(marqueFace.at(_mesh->faces[i]->id) == 0){
					//cout<<"Valeur de la face numero : "<< _mesh->faces[i]->id <<" = "<<marqueFace.at(_mesh->faces[i]->id)<<endl;
					if(fstPassage == true){
						nCC++;
						fstPassage = false;
						setColor(nCC);//attribue une couleur selon la valeur de nCC
					}
					//Parcours et marque toutes les faces ratachees a la face en cours
					parcoursAretes((hePtrEdge)it->data, marqueFace);
						
						
				}
			}
			 
        }
        
        cout<<"NOMBRE DE COMPOSANTES : "<<nCC<<endl;
        return nCC;
}



void meshDisplay(heMesh * _mesh)
{
        glPointSize(1.0);
        if (modeRendu == GL_POINT || modeRendu == GL_LINE)
        {
                glDisable(GL_LIGHTING);
                glColor3f(0.5, 0.5, 0.5);
        }
        else
        {
                glEnable(GL_LIGHTING);
                glColor3f(1.0, 1.0, 0.0);
        }
        glPolygonMode(GL_FRONT_AND_BACK, modeRendu);
       
        hePtrListItem it, it2, it3;
        heVert * v = NULL;
        vector<int> marqueFace(_mesh->nFaces, 0);//initialise le tableau qui servira a marquer les faces
        vector<int> marqueArete(_mesh->nEdges, 0);//initialise le tableau qui servira a marquer les faces
        //Parcours de toutes les faces du mesh - Traitement des composantes connexes.
        for(int i = 0; i < _mesh->nFaces; ++i)
        {
			
                glBegin(GL_POLYGON);
                for(it = _mesh->faces[i]->edges.head; it != NULL; it = it->next)
                {
                        if (((hePtrEdge)it->data)->tail->id == currVert)
                        {
                                v = ((hePtrEdge)it->data)->tail;
                        }
                        glNormal3dv(((hePtrEdge)it->data)->tail->normal);
                        glVertex3dv(((hePtrEdge)it->data)->tail->pos);
                }
                cout<<endl;
                glEnd();
        }
        glPointSize(5.0);
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_POINTS);
                glVertex3dv(v->normal);
                glVertex3dv(v->pos);
        glEnd();
        //Trace chaque arrete qui relie le point v a un autre point
        if (bEdgesDisplay)
        {
                glLineWidth(3.0);
                glColor3f(1.0, 0.0, 0.0);
                if (v != NULL)
                {
                        for (it = v->edges.head; it != NULL; it = it->next)
                        {
                                glBegin(GL_LINES);
                                        glVertex3dv(v->pos);
                                        glVertex3dv(((hePtrEdge)it->data)->head->pos);//Voisins de v
                                glEnd();
                        }
                }
        }
        glLineWidth(1.0);
        //Trace chaque polygone relie au sommet v
        if (bPolyDisplay)
        {
                glColor3f(1.0, 0.0, 0.0);
                if (v != NULL)
                {
                        for (it = v->edges.head; it != NULL; it = it->next)
                        {
                                for (it2 = ((hePtrEdge)it->data)->faces.head; it2 != NULL; it2 = it2->next)
                                {
                                        glBegin(GL_POLYGON);
                                        for (it3 = ((hePtrFace)it2->data)->edges.head; it3 != NULL; it3 = it3->next)
                                        {
                                                glVertex3dv(((hePtrEdge)it3->data)->head->pos);
                                        }
                                        glEnd();
                                }
                        }
                }
        }
}


void displayGL()
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        glPushMatrix();
                glMultMatrixd(mlTbGetRotation());
                //meshDisplay(mesh);
                countConnectedComponents(mesh);
        glPopMatrix();
       
        glutSwapBuffers();
}


void reshapeGL(int _w, int _h)
{
        winX = _w;
        winY = _h;
       
        glViewport(0, 0, winX, winY);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
       
        gluPerspective(60.0, (GLfloat)_w/(GLfloat)_h, 0.1, 128.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
       
        mlTbInit(_w, _h);
       
        glutPostRedisplay();
}


void mouseGL(int _button, int _state, int _x, int _y)
{
        switch(_button)
        {
                case GLUT_LEFT_BUTTON:
                        mouseClick[0] = (_state == GLUT_DOWN) ? 1 : 0;
                               
                        if(mouseClick[0] == 1)
                                mlTbClick(_x, _y);
                        else
                                mlTbRelease(_x, _y);
                        break;
               
                case GLUT_MIDDLE_BUTTON:
                        mouseClick[1] = (_state == GLUT_DOWN) ? 1 : 0;
                        break;
               
                case GLUT_RIGHT_BUTTON:
                        mouseClick[2] = (_state == GLUT_DOWN) ? 1 : 0;
                        break;
        }
       
        glutPostRedisplay();
}


void keyboardGL(unsigned char _k, int _x, int _y)
{
        if(_k == 27 || _k == 'q' || _k == 'Q')
                exit(0);
               
        switch(_k)
        {
                case '1':
                        modeRendu = GL_FILL;
                break;
                case '2':
                        modeRendu = GL_LINE;
                break;
                case '3':
                        modeRendu = GL_POINT;
                break;
               
                case 'a':
                        bEdgesDisplay = !bEdgesDisplay;
                break;
               
                case 'p':
                        bPolyDisplay = !bPolyDisplay;
                break;
					
                 case 'e':
                        SauvegardeConnectedComponents(mesh);
                break;
               
                case 43:
                if (currVert < mesh->nVerts)
                        currVert++;
                break;
                case 45:
                if(currVert>0)
                        currVert--;
                break;
        }


        glutPostRedisplay();
}


void keyboardSpecialGL(int _k, int _x, int _y)
{
        switch(_k)
        {
                case '+':
                if (currVert < mesh->nVerts)
                        currVert++;
                break;
                case '-':
                if(currVert>0)
                        currVert--;
                break;
        }
        glutPostRedisplay();
}


void motionGL(int _x, int _y)
{
        if(mouseClick[0] == 1)
                mlTbMotion(_x, _y);
       
        glutPostRedisplay();
}


void passiveMotionGL(int _x, int _y)
{
        //glutPostRedisplay();
}


void initGL()
{
        GLfloat mat_specular   [4] = { 0.18, 0.18, 0.18, 0.18 };
        GLfloat mat_shininess  [ ] = { 128 };
        GLfloat global_ambient [ ] = { 0.02, 0.02, 0.05, 0.05 };
        GLfloat light0_ambient [ ] = { 0, 0, 0, 0 };
        GLfloat light0_diffuse [ ] = { 0.85, 0.85, 0.8, 0.85 };
        GLfloat light0_specular[ ] = { 0.85, 0.85, 0.15, 0.85 };

        glClearColor(0.8, 0.8, 0.7, 1.0);
       
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
       
        glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
       
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
       
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_NORMALIZE);
       
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
}


int main(int _argc, char ** _argv)
{
        int posX, posY;
        currVert = 0;
       
        glutInit(&_argc, _argv);
       
        /* Argument testing */
        if(_argc < 2)
        {
                fprintf(stderr, "usage: %s file\n", _argv[0]);
                return EXIT_FAILURE;
        }
       
        /* Mesh creation */
        mesh = heMesh_New();
       
        /* Mesh loading */
        printf("Loading... ");
        if(heMesh_Load(mesh, _argv[1]) < 0)
        {
                printf("Fail.\n");
                heMesh_Free(&mesh);
               
                return EXIT_FAILURE;
        }
        printf("Done.\n\n");
        heMesh_Normalize(mesh);
        heMesh_Compute_Normals(mesh);
       
        posX = (glutGet(GLUT_SCREEN_WIDTH ) - winX) / 2;
        posY = (glutGet(GLUT_SCREEN_HEIGHT) - winY) / 2;
       
        glutInitWindowSize(winX, winY);
        glutInitWindowPosition(posX, posY);
       
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
        glutCreateWindow("[TP06] Modelisation Geometrique - Chargeur de maillages");
       
        glutDisplayFunc(displayGL);
        glutReshapeFunc(reshapeGL);
       
        glutMouseFunc(mouseGL);
        glutKeyboardFunc(keyboardGL);
        glutSpecialFunc(keyboardSpecialGL);
       
        glutMotionFunc(motionGL);
        glutPassiveMotionFunc(passiveMotionGL);
               
        initGL();
       
        glutMainLoop();

        return 0;
}

