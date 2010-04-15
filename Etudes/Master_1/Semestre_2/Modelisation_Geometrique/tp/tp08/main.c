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
#include <he.h>
}


int winX = 800;
int winY = 600;

int mouseClick[] = {0, 0, 0};

hePtrMesh mesh;

double epsilon;


double getMin(double _a, double _b)
{
    if (_a <= _b) return _a;
    else return _b;
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
    hePtrListItem it, it2, it3;


    for (it = _edge->faces.head; it != NULL; it = it->next) // parcours des faces liées à l'arête (2 faces théoriques)
    {
	  for (it2 = ((hePtrFace)it->data)->edges.head; it2 != NULL; it2 = it2->next) // pour chaque face, on parcourt les arêtes liées
	  {
		mlVec3_Copy(v3, ((hePtrEdge)it->data)->tail->pos);
		if (!mlVec3_IsEqual(v1, v3) && !mlVec3_IsEqual(v2, v3))
		{
		    mlVec3_SubVec(tmp, v3, v1);
		    w = mlVec3_Dot(_d, tmp);
		    if (w >= 0) pos = 1;
		    else neg = 1;
		}
	  }
    }
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
    double res = (1 - _d) * normE;

    return res;
}


void edgeCollapse(/* ... */)
{
    /* fonction qui realise la fusion de deux points d'une arete (edge collapse) */

    for (int i = 0; i < mesh->nVerts; i++)
    {

    }
}


void meshDisplay(heMesh * _mesh)
{
    hePtrListItem it;
    for(int i = 0; i < _mesh->nFaces; ++i)
    {
	  glBegin(GL_POLYGON);
	  for(it = _mesh->faces[i]->edges.head; it != NULL; it = it->next)
	  {
		glNormal3dv(((hePtrEdge)it->data)->tail->normal);
		glVertex3dv(((hePtrEdge)it->data)->tail->pos);
	  }
	  glEnd();
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
