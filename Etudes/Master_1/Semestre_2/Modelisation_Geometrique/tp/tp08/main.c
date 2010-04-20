#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>


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

using namespace std;

typedef struct
{
    hePtrEdge ptrEdge;
    double length;
} EdgeLength;


int winX = 800;
int winY = 600;

int mouseClick[] = {0, 0, 0};

hePtrMesh mesh, simpleMesh;
hePtrEdge currEdge;
int idCurrEdge = 0;

bool OriginalDisplay = 1;

std::vector<EdgeLength> lstAretes;

double epsilon = 0.0001;
double average = 0.0;


double getMin(double _a, double _b)
{
    if (_a <= _b) return _a;
    else return _b;
}

bool sortLength(EdgeLength _a, EdgeLength _b)
{
    return (_a.length < _b.length);
}


void getEdgeMiddle(hePtrEdge _edge, mlVec3 _middle)
{
    mlVec3_Set(_middle, ((_edge->tail->pos[0] + _edge->head->pos[0]) / 2.0), ((_edge->tail->pos[1] + _edge->head->pos[1]) / 2.0), ((_edge->tail->pos[2] + _edge->head->pos[2]) / 2.0));
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

void deleteFaceFromMesh(hePtrMesh _mesh, hePtrFace _face)
{
    bool found = 0;
    for (int i = 0; i < _mesh->nFaces-1; i++)
    {
	  if (_mesh->faces[i] == _face) found = 1;
	  if (found) _mesh->faces[i] = _mesh->faces[i+1];
    }
    _mesh->nFaces--;
    _mesh->faces[_mesh->nFaces] = NULL;
}

void deleteEdgeFromMesh(hePtrMesh _mesh, hePtrEdge _edge)
{
    bool found = 0;
    for(int i = 0; i < _mesh->nEdges-1; i++)
    {
	  if (lstAretes[i].ptrEdge == _edge) lstAretes.erase(lstAretes.begin()+i);
	  if (_mesh->edges[i] == _edge) found = 1;
	  if (found)
	  {
		_mesh->edges[i] = _mesh->edges[i+1];
	  }
    }
    _mesh->nEdges--;
    _mesh->edges[_mesh->nEdges] = NULL;
}

void deleteVertexFromMesh(hePtrMesh _mesh, hePtrVert _vert)
{
    bool found = 0;
    for(int i = 0; i < _mesh->nVerts-1; i++)
    {
	  if (_mesh->verts[i] == _vert) found = 1;
	  if (found)
	  {
		_mesh->verts[i] = _mesh->verts[i+1];
	  }
    }
    _mesh->nVerts--;
    _mesh->verts[mesh->nVerts] = NULL;
}

void updateEdgeList(hePtrMesh _mesh)
{
    lstAretes.clear();
    EdgeLength e;
    for (int i = 0; i < _mesh->nEdges; i++)
    {
	  e.ptrEdge = _mesh->edges[i];
	  e.length = getEdgeLength(e.ptrEdge);
	  lstAretes.push_back(e);
    }
    sort(lstAretes.begin(), lstAretes.end(), sortLength);
}


bool isBorderEdge(hePtrEdge _edge)
{
    hePtrVert vTail = _edge->tail;
    hePtrVert vHead = _edge->head;
    hePtrListItem it;
    int sz1 = 0, sz2 = 0;

    sz1 = heList_Size(&_edge->faces);
    sz2 = heList_Size(&_edge->twin->faces);

    if (sz1 == 0 || sz2 == 0) return true;
    else
    {
	  hePtrEdge edge, twin;
	  for (it = vTail->edges.head; it != NULL; it = it->next)
	  {
		edge = ((hePtrEdge)it->data);
		twin = ((hePtrEdge)it->data)->twin;
		sz1 = heList_Size(&edge->faces);
		sz2 = heList_Size(&twin->faces);
		if (sz1 == 0 || sz2 == 0) return true;
	  }

	  for (it = vHead->edges.head; it != NULL; it = it->next)
	  {
		edge = ((hePtrEdge)it->data);
		twin = ((hePtrEdge)it->data)->twin;
		sz1 = heList_Size(&edge->faces);
		sz2 = heList_Size(&twin->faces);
		if (sz1 == 0 || sz2 == 0) return true;
	  }
    }
    return false;
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
    //printf("Contraction d'arête en cours...\n");

    hePtrVert vTail = _edge->tail;
    hePtrVert vHead = _edge->head;
    hePtrVert vFar = NULL;
    hePtrEdge edgeH = NULL, edgeT = NULL;

    hePtrFace faceA = NULL, faceB = NULL;
    faceA = ((hePtrFace)_edge->faces.head->data);
    faceB = ((hePtrFace)_edge->twin->faces.head->data);

    if (faceA == NULL || faceB == NULL) printf("Une face est NULLE\n");

    mlVec3 middle;
    getEdgeMiddle(_edge, middle);

    hePtrListItem it;

    // On parcourt les arêtes de la face A qui va être supprimée
    for (it = faceA->edges.head; it != NULL; it = it->next)
    {
	  if (((hePtrEdge)it->data)->tail == vHead)
	  {
		edgeH = ((hePtrEdge)it->data); // On récupère l'arête côté vHead
		vFar = edgeH->head; // Ainsi que le 3e sommet de la face
	  }
	  if (((hePtrEdge)it->data)->head == vTail)
		edgeT = ((hePtrEdge)it->data); // On récupère l'arête côté vTail
    }
    heList_Del_Item(&vFar->edges, edgeT); // On supprime l'arête reliée à vTail de la liste d'arêtes de vFar car elle va être supprimée
    heList_Del_Item(&vHead->edges, edgeH); // On supprime l'arête reliée à vHead de la liste d'arêtes de vHead ---
    edgeH->twin->twin = edgeT->twin; // On rattache les twins des arêtes qui vont être supprimées
    edgeT->twin->twin = edgeH->twin; // ---

    for (it = faceA->edges.head; it != NULL; it = it->next)
    {
	  heList_Del_Item(&faceA->edges, ((hePtrEdge)it->data));
	  if (((hePtrEdge)it->data) != _edge)
	  {
		deleteEdgeFromMesh(_mesh, ((hePtrEdge)it->data));
	  }
    }
    deleteFaceFromMesh(_mesh, faceA); // On supprime la faceA du mesh

    for (it = faceB->edges.head; it != NULL; it = it->next)
    {
	  if (((hePtrEdge)it->data)->tail == vTail)
	  {
		edgeT = ((hePtrEdge)it->data);
		vFar = edgeT->head;
	  }
	  if (((hePtrEdge)it->data)->head == vHead)
		edgeH = ((hePtrEdge)it->data);
    }
    edgeT->twin->head = vHead;
    heList_Del_Item(&vFar->edges, edgeH);
    heList_Del_Item(&vTail->edges, edgeT);
    edgeH->twin->twin = edgeT->twin; // On rattache les twins des arêtes qui vont être supprimées
    edgeT->twin->twin = edgeH->twin;

    for (it = faceB->edges.head; it != NULL; it = it->next)
    {
	  heList_Del_Item(&faceB->edges, ((hePtrEdge)it->data));
	  if (((hePtrEdge)it->data) != _edge->twin)
	  {
		deleteEdgeFromMesh(_mesh, ((hePtrEdge)it->data));
	  }
    }
    deleteFaceFromMesh(_mesh, faceB); // On supprime la faceA du mesh

    heList_Del_Item(&vHead->edges, _edge->twin);
    heList_Del_Item(&vTail->edges, _edge);
    deleteEdgeFromMesh(_mesh, _edge->twin);
    deleteEdgeFromMesh(_mesh, _edge);

    for (it = vTail->edges.head; it != NULL; it = it->next)
    {
	  ((hePtrEdge)it->data)->tail = vHead;
	  ((hePtrEdge)it->data)->twin->head = vHead;
	  heList_Push_Back(&vHead->edges, ((hePtrEdge)it->data));
    }

    deleteVertexFromMesh(_mesh, vTail);

    // On place le sommet restant au centre de l'arête supprimée
    mlVec3_Copy(vHead->pos, middle);

    //printf("Contraction faite.\n");
}


void simpleMeshSimplification(hePtrMesh _mesh)
{
    printf("AVANT:\n");
    printf("NbVerts: %d\n", _mesh->nVerts);
    printf("NbEdges: %d\n", _mesh->nEdges);
    printf("NbFaces: %d\n\n", _mesh->nFaces);
    updateEdgeList(_mesh);
    bool working = 1;
    int i = 0, n = 0;
    while(working)
    //for (int j = 0; j < 2; j++)
    {
	  i = 0;
	  working = 0;
	  while(lstAretes[i].length < epsilon && i < (int)lstAretes.size())
	  {
		if (isBorderEdge(lstAretes[i].ptrEdge) == false)
		{
		    //printf("i: %d - length: %f\n", i, lstAretes[i].length);
		    edgeCollapse(_mesh, lstAretes[i].ptrEdge);
		    working = 1;
		}
		//else printf("i: %d - Arête ignorée\n", i);
		i++;
	  }
	  if (working) updateEdgeList(_mesh);
	  n++;
	  printf("Passe %d\n", n);
	  printf("Nombre aretes contractees: %d\n", i);
    }
    printf("APRES:\n");
    printf("NbVerts: %d\n", _mesh->nVerts);
    printf("NbEdges: %d\n", _mesh->nEdges);
    printf("NbFaces: %d\n\n", _mesh->nFaces);
    printf("Nombre de passes: %d\n\n", n);
}


void meshDisplay(heMesh * _mesh)
{
    //glColor3f(0.0, 0.0, 0.0);
    int n = 0;
    hePtrListItem it;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
    /*glColor3f(1.0, 0.0, 0.0);
    glLineWidth(3.0);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glVertex3dv(currEdge->tail->pos);
    glVertex3dv(currEdge->head->pos);
    glEnd();
    glColor3f(1.0, 01.0, 0.0);
    glPointSize(5.0);
    glBegin(GL_POINTS);
    glVertex3dv(currEdge->head->pos);
    glEnd();
    glEnable(GL_LIGHTING);
    glLineWidth(1.0);*/
}


void displayGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glMultMatrixd(mlTbGetRotation());
    if (OriginalDisplay) meshDisplay(mesh);
    else meshDisplay(simpleMesh);
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
	  if (!isBorderEdge(currEdge)) edgeCollapse(simpleMesh, currEdge);
	  break;

    case 'p':
	  idCurrEdge++;
	  break;
    case 'm':
	  idCurrEdge--;
	  break;

    case 'i':
	  if (isBorderEdge(currEdge)) printf("ARETE DE BORD\n");
	  else printf("ARETE NORMALE\n");

    case 's':
	  printf("Simplification en cours...\n");
	  simpleMeshSimplification(simpleMesh);
	  printf("Simplification faite.\n");
	  break;

    case 'b':
	  OriginalDisplay = !OriginalDisplay;
	  if (OriginalDisplay) cout<<"Mesh original"<<endl;
	  else cout<<"Mesh simplifié"<<endl;
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

    simpleMesh = heMesh_New_Copy(mesh);

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
