#include <stdlib.h>
#include <stdio.h>

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
int currVert;
bool bEdgesDisplay = 0;
bool bPolyDisplay = 0;


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
		glEnd();
	}
	glPointSize(5.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
		glVertex3dv(v->normal);
		glVertex3dv(v->pos);
	glEnd();
	if (bEdgesDisplay)
	{
		glLineWidth(3.0);
		glColor3f(1.0, 0.0, 0.0);
		if (v != NULL)
		{
			for (it = v->edges.head; it != NULL; it = it->next)
			{
				glBegin(GL_LINES);
					/*glNormal3dv(v->normal);*/
					glVertex3dv(v->pos);
					/*glNormal3dv(((hePtrEdge)it->data)->head->normal);*/
					glVertex3dv(((hePtrEdge)it->data)->head->pos);
				glEnd();
			}
		}
	}
	glLineWidth(1.0);
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
		
		case 'a':
			bEdgesDisplay = !bEdgesDisplay;
		break;
		
		case 'p':
			bPolyDisplay = !bPolyDisplay;
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
