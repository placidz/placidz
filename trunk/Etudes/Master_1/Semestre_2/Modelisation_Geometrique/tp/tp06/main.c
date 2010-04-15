#include <stdlib.h>
#include <stdio.h>
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
#include <ml.h>
#include <he.h>
}


int winX = 800;
int winY = 600;

int mouseClick[] = {0, 0, 0};

int modeRendu = GL_FILL;

hePtrMesh mesh;
int meshing = 0;
int openedMesh = 3;

int idCurrVertex = 0;
int idCurrEdge = 0;
int idCurrFace = 0;

int trackMode = 0;
bool bVertEdgesDisplay = 0;
bool bVertPolysDisplay = 0;

bool bEdgePolysDisplay = 0;

bool bFaceEdgesDisplay = 0;
bool bFacePolysDisplay = 0;
bool bFacePlanDisplay = 0;

bool bDetectBords = 0;

mlVec3 zero = {0, 0, 0};


bool mlVec3_IsEqual(mlVec3 _in1, mlVec3 _in2)
{
	if (fabs(_in1[0] - _in2[0]) < 0.000001 || fabs(_in1[1] - _in2[1]) < 0.000001 || fabs(_in1[2] - _in2[2]) < 0.000001) return true;
	return false;
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
	  glColor3f(0.8, 0.8, 0.8);
    }
    glPolygonMode(GL_FRONT_AND_BACK, modeRendu);

    hePtrListItem it, it2, it3;
    heVert * currVertex = NULL;
    heEdge * currEdge = NULL;
    heFace * currFace = NULL;

    int max = 0;
    int min = 0;

    for(int i = 0; i < _mesh->nFaces; ++i)
    {
	  if (_mesh->faces[i]->id == idCurrFace) currFace = _mesh->faces[i];
	  glBegin(GL_POLYGON);
	  for(it = _mesh->faces[i]->edges.head; it != NULL; it = it->next)
	  {
		if (((hePtrEdge)it->data)->id == idCurrEdge)
		{
		    currEdge = ((hePtrEdge)it->data);
		}
		if (((hePtrEdge)it->data)->tail->id == idCurrVertex)
		{
		    currVertex = ((hePtrEdge)it->data)->tail;
		}
		glNormal3dv(((hePtrEdge)it->data)->tail->normal);
		glVertex3dv(((hePtrEdge)it->data)->tail->pos);

		max++;
	  }
	  glEnd();
	  if (max > meshing) meshing = max;
	  max = 0;
    }

    for (int i = 0; i < _mesh->nEdges; i++)
    {
	  for (it = _mesh->edges[i]->faces.head; it != NULL; it = it->next)
	  {
		min++;
	  }
	  for (it = _mesh->edges[i]->twin->faces.head; it != NULL; it = it->next)
	  {
		min++;
	  }
	  if (bDetectBords && min == 1)
	  {
		glDisable(GL_LIGHTING);
		glLineWidth(3.0);
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_LINES);
		    glVertex3dv(_mesh->edges[i]->tail->pos);
		    glVertex3dv(_mesh->edges[i]->head->pos);
		glEnd();
		glLineWidth(1.0);
		glEnable(GL_LIGHTING);
	  }
	  if (min < openedMesh) openedMesh = min;
	  min = 0;
    }

    if (trackMode == 0 && currVertex != NULL) // PARCOURS DE SOMMET
    {
	  glDisable(GL_LIGHTING);
	  glPointSize(5.0);
	  glColor3f(1.0, 0.0, 0.0);

	  glBegin(GL_POINTS);
		glVertex3dv(currVertex->pos);
	  glEnd();

	  glPointSize(1.0);

	  if (bVertEdgesDisplay) // Afficher les arêtes liées au point parcouru
	  {
		glLineWidth(3.0);
		glColor3f(1.0, 0.0, 0.0);
		for (it = currVertex->edges.head; it != NULL; it = it->next)
		{
		    glBegin(GL_LINES);
			  glVertex3dv(currVertex->pos);
			  glVertex3dv(((hePtrEdge)it->data)->head->pos);
		    glEnd();
		}
		glLineWidth(1.0);
	  }
	  if (bVertPolysDisplay) // Afficher les polygones liés au point parcouru
	  {
		glColor3f(1.0, 0.0, 0.0);
		for (it = currVertex->edges.head; it != NULL; it = it->next)
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
	  glEnable(GL_LIGHTING);
    }
    else if (trackMode == 1 && currEdge != NULL) // PARCOURS D'ARETE
    {
	  glDisable(GL_LIGHTING);
	  glLineWidth(3.0);
	  glColor3f(0.7, 0.0, 0.0);
	  glBegin(GL_LINES);
		glVertex3dv(currEdge->tail->pos);
		glVertex3dv(currEdge->head->pos);
	  glEnd();
	  glLineWidth(1.0);

	  if (bEdgePolysDisplay) // Affiche les polygones voisins de l'arête parcourue
	  {
		for (it = currEdge->faces.head; it != NULL; it = it->next)
		{
		    glColor3f(0.7, 0.0, 0.0);
		    glBegin(GL_POLYGON);
		    for (it2 = ((hePtrFace)it->data)->edges.head; it2 != NULL; it2 = it2->next)
		    {
			  glVertex3dv(((hePtrEdge)it2->data)->head->pos);
		    }
		    glEnd();
		}
		for (it = currEdge->twin->faces.head; it != NULL; it = it->next)
		{
		    glColor3f(0.7, 0.0, 0.0);
		    glBegin(GL_POLYGON);
		    for (it2 = ((hePtrFace)it->data)->edges.head; it2 != NULL; it2 = it2->next)
		    {
			  glVertex3dv(((hePtrEdge)it2->data)->head->pos);
		    }
		    glEnd();
		}
	  }
	  glEnable(GL_LIGHTING);
    }
    else if (trackMode == 2 && currFace != NULL) // PARCOURS DE POLYGONE
    {
	  glDisable(GL_LIGHTING);
	  glColor3f(1.0, 0.0, 0.5);
	  glBegin(GL_POLYGON);
	  for (it = currFace->edges.head; it != NULL; it = it->next)
	  {
		glVertex3dv(((hePtrEdge)it->data)->head->pos);
	  }
	  glEnd();

	  if (bFaceEdgesDisplay) // Les bords du polygone parcouru changent de couleur
	  {
		glLineWidth(3.0);
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_LINE_LOOP);
		for (it = currFace->edges.head; it != NULL; it = it->next)
		{
		    glVertex3dv(((hePtrEdge)it->data)->head->pos);
		}
		glEnd();
		glLineWidth(1.0);
	  }

	  if (bFacePolysDisplay) // Faire apparaitre tous les polygones voisins de celui parcouru
	  {
		for (it = currFace->edges.head; it != NULL; it = it->next)
		{
		    for (it2 = ((hePtrEdge)it->data)->twin->faces.head; it2 != NULL; it2 = it2->next)
		    {
			  glColor3f(1.0, 0.0, 1.0);
			  glBegin(GL_POLYGON);
			  for (it3 = ((hePtrFace)it2->data)->edges.head; it3 != NULL; it3 = it3->next)
			  {
				glVertex3dv(((hePtrEdge)it3->data)->head->pos);
			  }
			  glEnd();
		    }
		}
	  }

	  if (bFacePlanDisplay)
	  {
		mlVec3 plan[3];
		int i = 0;
		for (it = currFace->edges.head; it != NULL && i < 3; it = it->next)
		{
		    mlVec3_Copy(plan[i++], ((hePtrEdge)it->data)->head->pos);
		}
		mlVec3 v1, v2, rv1, rv2, d;
		
		mlVec3_SubVec(v1, plan[0], plan[1]);
		mlVec3_SubVec(v2, plan[2], plan[1]);

		mlVec3_AddVec(d, plan[0], v2); // placer le point d

		mlVec3_Scale(v1, v1, 5); // coef d'agrandissement du plan
		mlVec3_Scale(v2, v2, 5);
		mlVec3_Neg(rv1, v1); // inverse des vecteurs
		mlVec3_Neg(rv2, v2);

		mlVec3_AddVec(plan[0], plan[0], rv1); // positionnement des points avec effet du scale
		mlVec3_AddVec(plan[0], plan[0], rv2);
		mlVec3_AddVec(plan[1], plan[1], rv1);
		mlVec3_AddVec(plan[1], plan[1], v2);
		mlVec3_AddVec(plan[2], plan[2], v1);
		mlVec3_AddVec(plan[2], plan[2], v2);
		mlVec3_AddVec(d, d, v1);
		mlVec3_AddVec(d, d, rv2);

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
		    glVertex3dv(plan[0]);
		    glVertex3dv(plan[1]);
		    glVertex3dv(plan[2]);
		    glVertex3dv(d);
		glEnd();
	  }
	  glEnable(GL_LIGHTING);
    }
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
    case '1':
	  modeRendu = GL_FILL;
	  break;
    case '2':
	  modeRendu = GL_LINE;
	  break;
    case '3':
	  modeRendu = GL_POINT;
	  break;

    case 'i':
	  printf("--- INFORMATIONS ---\n");
	  printf("Nombre de sommets: %d\n", mesh->nVerts);
	  printf("Nombre d'arêtes: %d\n", mesh->nEdges);
	  printf("Nombre de faces: %d\n", mesh->nFaces);
	  if (meshing == 3) printf("Maillage triangulé\n");
	  else if (meshing == 4) printf("Maillage quadrangulé\n");
	  else printf("Maillage quelconque (%d)\n", meshing);
	  if (openedMesh == 1) printf("Maillage ouvert (%d)\n", openedMesh);
	  else printf("Maillage fermé (%d)\n", openedMesh);
	  break;

    case 'm':
	  trackMode++;
	  trackMode = trackMode % 3;
	  break;

	  // Tracking par vertex
    case 'a':
	  bVertEdgesDisplay = !bVertEdgesDisplay;
	  break;

    case 'p':
	  bVertPolysDisplay = !bVertPolysDisplay;
	  break;

	  // Tracking par arête
    case 'P':
	  bEdgePolysDisplay = !bEdgePolysDisplay;
	  break;

	  // Tracking par face
    case 'b':
	  bFaceEdgesDisplay = !bFaceEdgesDisplay;
	  break;

    case 'n':
	  bFacePolysDisplay = !bFacePolysDisplay;
	  break;

    case 't':
	  bFacePlanDisplay = !bFacePlanDisplay;
	  break;

    case 'x':
	  bDetectBords = !bDetectBords;

    case 43:
	  if (trackMode == 0 && idCurrVertex < mesh->nVerts) idCurrVertex++;
	  if (trackMode == 1 && idCurrEdge < mesh->nEdges) idCurrEdge++;
	  if (trackMode == 2 && idCurrFace < mesh->nFaces) idCurrFace++;
	  break;
    case 45:
	  if (trackMode == 0 && idCurrVertex > 1) idCurrVertex--;
	  if (trackMode == 1 && idCurrEdge > 1) idCurrEdge--;
	  if (trackMode == 2 && idCurrFace > 1) idCurrFace--;
	  break;
    }

    glutPostRedisplay();
}


void keyboardSpecialGL(int _k, int _x, int _y)
{
    switch(_k)
    {
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
    idCurrVertex = 0;

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
