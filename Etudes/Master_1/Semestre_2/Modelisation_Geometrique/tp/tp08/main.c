#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>

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
#include "../../gmtools/he/inc/he.h"
#include "../../gmtools/he/inc/he/he_define.h"
    //#include <he.h>
}

typedef struct
{
    hePtrEdge edge;
    double length;
} Arete;


int winX = 800;
int winY = 600;

int mouseClick[] = {0, 0, 0};

hePtrMesh mesh;
hePtrEdge currEdge;
int idCurrEdge = 0;

std::vector<Arete> lstAretes;
double epsilon;


double getMin(double _a, double _b)
{
    if (_a <= _b) return _a;
    else return _b;
}

double getEdgeLength(hePtrEdge _edge)
{
    double Ax, Ay, Az, Bx, By, Bz, length;
    Ax = _edge->tail->pos[0];
    Ay = _edge->tail->pos[1];
    Az = _edge->tail->pos[2];
    Bx = _edge->head->pos[0];
    By = _edge->head->pos[1];
    Bz = _edge->head->pos[2];
    length = pow(Bx-Ax, 2) + pow(By-Ay, 2) + pow(Bz-Az, 2);
    length = sqrt(length);
    return length;
}

void sortByAscOrder(int *t, int size)
{
    bool en_desordre = true;
    for (int i = 0; i < size && en_desordre; ++i)
    {
	  en_desordre = false;
	  for (int j = 1; j < size - i; ++j)
		if (t[j-1] > t[j])
		{
		std::swap(t[j], t[j-1]);
		en_desordre = true;
	  }
    }
}

void deleteFaceFromMesh(hePtrMesh _mesh, hePtrFace _face)
{
    bool found = 0;
    for (int i=0; i<_mesh->nFaces-1; i++)
    {
	  if (_mesh->faces[i]->id == _face->id)
		found = 1;
	  if (found)
		_mesh->faces[i] = _mesh->faces[i+1];
    }
    _mesh->nFaces--;
}

void deleteEdgeFromMesh(hePtrMesh _mesh, hePtrEdge _edge)
{
    bool found = 0;
    for (int i = 0; i < _mesh->nEdges-1; i++)
    {
	  if (_mesh->edges[i]->id == _edge->id)
		found = 1;
	  if (found)
		_mesh->edges[i] = _mesh->edges[i+1];
    }
    _mesh->nEdges--;
}

void deleteVertexFromMesh(hePtrMesh _mesh, hePtrVert _vert)
{
    bool found = 0;
    for (int i=0; i<_mesh->nVerts-1; i++)
    {
	  if (_mesh->verts[i]->id == _vert->id)
		found = 1;
	  if (found)
		_mesh->verts[i] = _mesh->verts[i+1];
    }
    _mesh->nVerts--;
}

void updateEdgeList()
{
    lstAretes.clear();
    Arete arete;
    arete.edge = NULL;
    arete.length = 0.0;
    for (int i = 0; i < mesh->nEdges; i++)
    {
	  arete.edge = mesh->edges[i];
	  arete.length = getEdgeLength(arete.edge);
	  lstAretes.push_back(arete);
    }
}


bool isBorderEdge(heEdge * _edge)
{
    int min = 0;
    hePtrListItem it;
    for (it = _edge->faces.head; it != NULL; it = it->next)
	  min++;
    for (it = _edge->twin->faces.head; it != NULL; it = it->next)
	  min++;

    if (min == 1) return 1;
    return 0;
}


bool mlVec3_IsEqual(mlVec3 _in1, mlVec3 _in2)
{
    if (fabs(_in1[0] - _in2[0]) < 0.000001 || fabs(_in1[1] - _in2[1]) < 0.000001 || fabs(_in1[2] - _in2[2]) < 0.000001)
	  return true;
    return false;
}


void getEdgeVector(mlVec3 _e, heEdge * _edge)
{
    /* _e = vecteur directeur de l'arete _edge */

    mlVec3 v1, v2;
    mlVec3_Copy(v1, _edge->tail->pos);
    mlVec3_Copy(v2, _edge->head->pos);
    mlVec3_SubVec(_e, v2, v1);
}


// Entrées
// _n : vecteur normal au sommet courant
// _e : vecteur de l'arête courante
// Sortie
// _d : vecteur normé orthogonal aux 2 vecteurs précédents
void getPerpendicularVector(mlVec3 _d, mlVec3 _n, mlVec3 _e)
{
    /* _d = vecteur normalise perpendiculaire a _n et _e */

    mlVec3 tmp;
    mlVec3_Cross(tmp, _n, _e);
    mlVec3_Normalize(_d, tmp);
}


int isInterior(mlVec3 _d, heEdge * _edge)
{
    /* retourne 1 si d pointe vers l'interieur du maillage, 0 sinon */

    double w;
    mlVec3 v1, v2, v3, tmp;
    mlVec3_Copy(v1, _edge->tail->pos);
    mlVec3_Copy(v2, _edge->head->pos);

    bool pos = 0, neg = 0;
    hePtrListItem it;
    hePtrEdge twin = _edge->twin;
    hePtrFace face1, face2;

    face1 = ((hePtrFace)_edge->faces.head->data);
    face2 = ((hePtrFace)twin->faces.head->data);



    for (it = face1->edges.head; it != NULL; it = it->next)
    {
	  mlVec3_Copy(tmp, ((hePtrEdge)it->data)->tail->pos);
	  if (!mlVec3_IsEqual(tmp, v1) && !mlVec3_IsEqual(tmp, v2))
		mlVec3_Copy(v3, tmp);
    }
    mlVec3_SubVec(tmp, v3, v1);
    double scalaire = mlVec3_Dot(_d, tmp);
    if (scalaire >= 0) pos = 1;
    else neg = 1;
    
    for (it = face2->edges.head; it != NULL; it = it->next)
    {
	  mlVec3_Copy(tmp, ((hePtrEdge)it->data)->tail->pos);
	  if (!mlVec3_IsEqual(tmp, v1) && !mlVec3_IsEqual(tmp, v2))
		mlVec3_Copy(v3, tmp);
    }
    mlVec3_SubVec(tmp, v3, v1);
    scalaire = mlVec3_Dot(_d, tmp);
    if (scalaire >= 0) pos = 1;
    else neg = 1;

    if (pos && neg) return 1;
    return 0;
}


double computeCost(double _d, heEdge * _edge)
{
    /* fonction de cout */

    mlVec3 v1, v2, E;
    mlVec3_Copy(v1, _edge->tail->pos);
    mlVec3_Copy(v2, _edge->head->pos);
    mlVec3_SubVec(E, v2, v1);

    double normE = mlVec3_Norm(E);
    double cost = (1 - _d) * normE;

    return cost;
}


void edgeCollapse(hePtrMesh _mesh, hePtrEdge _edge)
{
    /* fonction qui realise la fusion de deux points d'une arete (edge collapse) */

    printf("Phase 1 starts...\n");

    hePtrVert v1 = _edge->tail;
    hePtrVert v2 = _edge->head;
    hePtrVert v3, v4;
    mlVec3 middle;
    double x = (v1->pos[0] + v2->pos[0]) / 2.0;
    double y = (v1->pos[1] + v2->pos[1]) / 2.0;
    double z = (v1->pos[2] + v2->pos[2]) / 2.0;
    mlVec3_Set(middle, x, y, z);

    hePtrFace face1 = NULL, face2 = NULL, mface1 = NULL, mface2 = NULL, face3 = NULL;
    hePtrEdge edge1 = NULL, dedge1 = NULL, medge1 = NULL, bedge1 = NULL, edge2 = NULL, dedge2 = NULL, medge2 = NULL, bedge2 = NULL;

    hePtrListItem it;

    face1 = (hePtrFace)_edge->faces.head->data; // On récupère la face liée à l'arête passée en paramètre
    for (it = face1->edges.head; it != NULL; it = it->next) // On cherche l'arête à sauvegarder de la face (ci-dessus) qui va être supprimée
    {
	  if (((hePtrEdge)it->data)->tail->id == v2->id)
	  {
		edge1 = (hePtrEdge)it->data; // On trouve l'arête
		v3 = edge1->head; // On déduit le sommet qu'il faudra mettre à jour
	  }
    }
    for (it = v3->edges.head; it != NULL; it = it->next) // On cherche l'arête à supprimer
    {
	  if (((hePtrEdge)it->data)->head->id == v1->id)
	  {
		dedge1 = (hePtrEdge)it->data;
	  }
    }
    mface1 = (hePtrFace)dedge1->twin->faces.head->data; // On récupère la face qui va être refaite
    for (it = mface1->edges.head; it != NULL; it = it->next) // On cherche l'arête qui va être raccordée
    {
	  if (((hePtrEdge)it->data)->head->id == v1->id)
	  {
		medge1 = (hePtrEdge)it->data;
		v4 = medge1->tail;
	  }
	  if (((hePtrEdge)it->data)->tail->id == v3->id)
	  {
		bedge1 = (hePtrEdge)it->data;
	  }
    }
    face3 = (hePtrFace)medge1->twin->faces.head->data; // On récupère la face qui va être allongée

    printf("Phase 1 middle\n");

    heList_Del_Item(&_edge->faces, face1); // On supprime toute trace de la face à supprimer dans les listes de faces des arêtes de la face
    heList_Del_Item(&edge1->faces, face1);
    heList_Del_Item(&dedge1->faces, face1);
    //heList * e1 = &face1->edges;
    //heList_Free(&e1); // On détruit la liste d'arêtes de la face à supprimer

    deleteFaceFromMesh(_mesh, face1); // On supprime la face dans la liste du mesh

    heList_Del_Item(&v1->edges, dedge1->twin);
    heList_Del_Item(&v3->edges, dedge1);

    heList_Del_Item(&mface1->edges, dedge1);
    //heList_Del_Item(&v1->edges, dedge1);

    deleteEdgeFromMesh(_mesh, dedge1->twin);
    deleteEdgeFromMesh(_mesh, dedge1);

    // Redéfinition de la mface1
    heList_Clear(&mface1->edges);
    heList_Push_Back (&mface1->edges, bedge1);
    heList_Push_Back (&mface1->edges, medge1);
    heList_Push_Back (&mface1->edges, edge1);

    medge1->head = v2; // Liaison avec v2

    printf("Phase 1 done !\n");

    /************************************************************************************************/

    printf("Phase 2 starts...\n");

    face2 = (hePtrFace)_edge->twin->faces.head->data; // On récupère la face liée à l'arête passée en paramètre
    for (it = face2->edges.head; it != NULL; it = it->next) // On cherche l'arête à sauvegarder de la face (ci-dessus) qui va être supprimée
    {
	  if (((hePtrEdge)it->data)->head->id == v2->id)
	  {
		edge2 = (hePtrEdge)it->data; // On trouve l'arête
		v3 = edge2->tail; // On déduit le sommet qu'il faudra mettre à jour
	  }
    }
    for (it = v1->edges.head; it != NULL; it = it->next) // On cherche l'arête à supprimer
    {
	  if (((hePtrEdge)it->data)->head->id == v3->id)
	  {
		dedge2 = (hePtrEdge)it->data;
	  }
    }
    if (dedge2 != NULL) printf("Un truc dans dedge2\n");
    else printf("dedge2 est NULL\n");

    hePtrEdge twdedge2 = dedge2->twin;
    if (twdedge2 != NULL) printf("Un truc dans twdedge2\n");
    else printf("twdedge2 est NULL\n");
    mface2 = (hePtrFace)twdedge2->faces.head->data; // On récupère la face qui va être refaite
    for (it = mface2->edges.head; it != NULL; it = it->next) // On cherche l'arête qui va être raccordée
    {
	  if (((hePtrEdge)it->data)->tail->id == v1->id)
	  {
		medge2 = (hePtrEdge)it->data;
		v4 = medge2->head;
	  }
	  if (((hePtrEdge)it->data)->head->id == v3->id)
	  {
		bedge2 = (hePtrEdge)it->data;
	  }
    }
    //face3 = (hePtrFace)medge1->twin->faces.head->data; // On récupère la face qui va être allongée

    printf("Phase 2 - middle\n");

    heList_Del_Item(&_edge->faces, face2); // On supprime toute trace de la face à supprimer dans les listes de faces des arêtes de la face
    heList_Del_Item(&edge2->faces, face2);
    heList_Del_Item(&dedge2->faces, face2);
    //heList * e2 = &face2->edges;
    //heList_Free(&e2); // On détruit la liste d'arêtes de la face à supprimer

    deleteFaceFromMesh(_mesh, face2); // On supprime la face dans la liste du mesh

    heList_Del_Item(&v1->edges, dedge2);
    heList_Del_Item(&v3->edges, dedge2->twin);

    heList_Del_Item(&mface2->edges, dedge2);
    //heList_Del_Item(&v1->edges, dedge1);

    deleteEdgeFromMesh(_mesh, dedge2->twin);
    deleteEdgeFromMesh(_mesh, dedge2);

    // Redéfinition de la mface1
    heList_Clear(&mface2->edges);
    heList_Push_Back(&mface2->edges, bedge2);
    heList_Push_Back(&mface2->edges, edge2);
    heList_Push_Back(&mface2->edges, medge2);

    medge2->tail = v2; // Liaison avec v2

    printf("Phase 2 done !\n");

    for (it = face3->edges.head; it != NULL; it = it->next)
    {
	  if (((hePtrEdge)it->data)->tail->id == v1->id)
		((hePtrEdge)it->data)->tail = v2;
	  if (((hePtrEdge)it->data)->head->id == v1->id)
		((hePtrEdge)it->data)->head = v2;
    }

    heList_Del_Item(&v2->edges, _edge->twin);

    heList_Push_Back(&v2->edges, medge2);
    heList_Push_Back(&v2->edges, medge1->twin);

    //heList * e = &v1->edges;
    //heList_Free(&e);

    deleteVertexFromMesh(_mesh, v1);

    mlVec3_Copy(v2->pos, middle);

    
}


void simpleMeshSimplification()
{

}


void meshDisplay(heMesh * _mesh)
{
    glColor3f(0.0, 0.0, 0.0);
    int n = 0;
    hePtrListItem it;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for(int i = 0; i < _mesh->nFaces; ++i)
    {
	  glBegin(GL_POLYGON);
	  for(it = _mesh->faces[i]->edges.head; it != NULL; it = it->next)
	  {
		if (idCurrEdge == n/*((hePtrEdge)it->data)->id*/) currEdge = ((hePtrEdge)it->data);
		glNormal3dv(((hePtrEdge)it->data)->tail->normal);
		glVertex3dv(((hePtrEdge)it->data)->tail->pos);
		n++;
	  }
	  glEnd();
    }
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(3.0);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
	  glVertex3dv(currEdge->tail->pos);
	  glVertex3dv(currEdge->head->pos);
    glEnd();
    glEnable(GL_LIGHTING);
    glLineWidth(1.0);
}


void displayGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glMultMatrixd(mlTbGetRotation());
    meshDisplay(mesh);
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
    case 't':
	  edgeCollapse(mesh, currEdge);
	  break;

    case 'p':
	  idCurrEdge++;
	  break;
    }

    glutPostRedisplay();
}


void keyboardSpecialGL(int _k, int _x, int _y)
{
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
    glutPostRedisplay();
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
    glutCreateWindow("[TP08] Modelisation Geometrique - Simplification de maillages");

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
