#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

#include "Bezier.h"
#include "BSpline.h"
#include "tools.h"


#include "defines.h"

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
    #include <ml.h>
}

using namespace std;

string filename;
GLuint idBaseDL = 0;

int winX = 800;
int winY = 600;

int size_u = 0, size_v = 0;

mlVec3** PtsUV;
int nBP = 20;
mlVec3 bP[MAX2];

bool bRenduBezier = 0;


void displayGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

	  glMultMatrixd(mlTbGetRotation());
	  glCallList(idBaseDL);
	  if (bRenduBezier) drawBezierSurface(PtsUV, size_u, size_v, bP, nBP);
	  else drawBSplineSurface(PtsUV, size_u, size_v, bP, nBP, 3, 3);

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
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glutPostRedisplay();
}


void mouseGL(int _button, int _state, int _x, int _y)
{
    switch (_button)
    {
    case GLUT_LEFT_BUTTON:
	  if (_state==GLUT_DOWN)
	  {
		mlTbClick(_x,_y);
	  }
	  if (_state==GLUT_UP)
	  {
		/*mlTbRelease(_x,_y);*/
	  }
	  break ;
    case GLUT_RIGHT_BUTTON:
	  break;
    default:
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
    case 'n':
	  nBP--;
	  printf("nBP: %d\n", nBP);
	  break;
    case 'N':
	  nBP++;
	  printf("nBP: %d\n", nBP);
	  break;
    case 'b':
	  bRenduBezier = !bRenduBezier;
    }
    glutPostRedisplay();
}


void keyboardSpecialGL(int _k, int _x, int _y)
{
    glutPostRedisplay();
}


void motionGL(int _x, int _y)
{
    mlTbMotion(_x,_y);
    glutPostRedisplay();
}


void passiveMotionGL(int _x, int _y)
{
    glutPostRedisplay();
}

void drawRepere()
{
	 glDisable(GL_LIGHTING);
	glLineWidth(2.0);
	// Axe X
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
	glEnd();
	// Axe Y
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
	glEnd();
	// Axe Z
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 1.0);
	glEnd();
	glLineWidth(1.0);
	glEnable(GL_LIGHTING);
}

GLuint createBaseDL()
{
    GLuint id;
    id = glGenLists(1);
    if (!id) return id;

    glNewList(id, GL_COMPILE);
    drawRepere();
    glEndList();

    return id;
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

    mlTbInit(winX,winY);
    idBaseDL = createBaseDL();
}


int main(int _argc, char ** _argv)
{
    printf("\n[TP05] Modelisation Geometrique - Surfaces a poles\n\n");
    int posX, posY;

    glutInit(&_argc, _argv);

    filename = _argv[1];

    vector<vector<Point3D> > vPts2;
    vPts2 = loadFilePTS2(filename, &size_u, &size_v);

    PtsUV = T_Array2D<mlVec3>(size_u, size_v);

    for (int v=0; v<size_v; v++)
    {
	  for (int u=0; u<size_u; u++)
	  {
		printf("x: %f, y: %f, z: %f \n", vPts2[v][u].x, vPts2[v][u].y, vPts2[v][u].z);
		mlVec3_Set(PtsUV[u][v], vPts2[v][u].x, vPts2[v][u].y, vPts2[v][u].z);
	  }
    }

    vPts2.clear();

    posX = (glutGet(GLUT_SCREEN_WIDTH ) - winX) / 2;
    posY = (glutGet(GLUT_SCREEN_HEIGHT) - winY) / 2;

    glutInitWindowSize(winX, winY);
    glutInitWindowPosition(posX, posY);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("[TP05] Modelisation Geometrique - Surfaces a poles");

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
