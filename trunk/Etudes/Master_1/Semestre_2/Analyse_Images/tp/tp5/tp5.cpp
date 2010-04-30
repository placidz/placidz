#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include <GL/glut.h>
#include "glut.h"
#include <math.h>

#define PI 3.14159265
#define MAX 100

int winX = 800, winY = 600;

typedef struct
{
        int x, y, t;
} Point;

typedef struct
{
        float x, y, z, t;
} Point3D;

typedef struct
{
        float a, b, c, d;
} Quat;

Point3D p1, p2, p3;
Point tab[MAX];
int nbPts = 0;

Quat P, Q;


void afficherQuat(Quat q)
{
        printf("a: %f  b: %f  c: %f  d: %f\n", q.a, q.b, q.c, q.d);
}

Quat additionQuat(Quat qa, Quat qb)
{
        Quat res;
       
        res.a = qa.a + qb.a;
        res.b = qa.b + qb.b;
        res.c = qa.c + qb.c;
        res.d = qa.d + qb.d;
       
        return res;
}

Quat soustractQuat(Quat qa, Quat qb)
{
        Quat res;
       
        res.a = qa.a - qb.a;
        res.b = qa.b - qb.b;
        res.c = qa.c - qb.c;
        res.d = qa.d - qb.d;
       
        return res;
}

Quat multQuat(Quat qa, Quat qb)
{
        Quat res;
       
        res.a = qa.a*qb.a - qa.b*qb.b - qa.c*qb.c - qa.d*qb.d;
        res.b = qa.a*qb.b + qa.b*qb.a + qa.c*qb.d - qa.d*qb.c;
        res.c = qa.a*qb.c + qa.c*qb.a + qa.d*qb.d - qa.b*qb.d;
        res.d = qa.a*qb.d + qa.d*qb.a + qa.b*qb.c - qa.c*qb.b;
       
        return res;
}

void isMultQuatCommutatif(Quat qa, Quat qb)
{
        Quat tmp1, tmp2;
        tmp1 = multQuat(qa, qb);
        tmp2 = multQuat(qb, qa);
        if(     tmp1.a == tmp2.a &&
                tmp1.b == tmp2.b &&
                tmp1.c == tmp2.c &&
                tmp1.d == tmp2.d)
                printf("C'est commutatif >_<\n");
               
        else printf("C'est pas commutatif :B\n");
}

Quat conjugueQuat(Quat q)
{
        Quat res;
        res.a = q.a;
        res.b = -q.b;
        res.c = -q.c;
        res.d = -q.d;
        return res;    
}

float normeQuatCarre(Quat q)
{
        float res = 0;
        res += pow(q.a, 2);
        res += pow(q.b, 2);
        res += pow(q.c, 2);
        res += pow(q.d, 2);
        //printf("a = %f, b = %f, c = %f, d = %f", res.a, res.b, res.c, res.d);
        return res;    
}

Quat normeQuatConj(Quat q)
{
        Quat res;
        Quat conj = conjugueQuat(q);
        res = multQuat(q, conj);
        //printf("a = %f, b = %f, c = %f, d = %f", res.a, res.b, res.c, res.d);
        return res;
}


Quat getScal(Quat q)
{
        Quat res;
        res = additionQuat(q, conjugueQuat(q));
        res.a *= 0.5;
        res.b *= 0.5;
        res.c *= 0.5;
        res.d *= 0.5;
        return res;
}


Quat getVect(Quat q)
{
        Quat res;
        res = soustractQuat(q, conjugueQuat(q));
        res.a *= 0.5;
        res.b *= 0.5;
        res.c *= 0.5;
        res.d *= 0.5;
        return res;
}

Quat inverseQuat(Quat q)
{
        Quat res, conj;
        float norm;
        conj = conjugueQuat(q);
        norm = normeQuatCarre(q);
        res.a = conj.a / norm;
        res.b = conj.b / norm;
        res.c = conj.c / norm;
        res.d = conj.d / norm;
       
        return res;
}

void initGL(void)
{
     glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

void affichage(void)
{
        glClearColor(0.8, 0.8, 0.8, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glutSwapBuffers();
}

void redim(int width, int height)
{
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //gluOrtho2D(0, width, 0, height);
        gluOrtho2D(-winX/2, winX/2, -winY/2, winY/2);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}

void ChoixMenuPrincipal(int value)
{
        switch (value)
        {
                case 11 :
                        exit(0); /* On quitte */
                break;
        }
        glutPostRedisplay();
}

void AjouterPoint(int x, int y)
{
        Point p;
        p.x = x;
        p.y = y;
        p.t = 1;
        tab[nbPts] = p;
        nbPts++;
        printf("nbPts: %d\n", nbPts);
}


void souris (int button, int state, int x, int y)
{
        x = x-winX/2;
        y = y-winY/2;
        switch(button)
        {
                case GLUT_LEFT_BUTTON:
                        if (state == GLUT_DOWN)
                        {
                                printf("x: %d - y: %d\n", x, y);
                                AjouterPoint(x, y);
                        }
                break;
        }
        glutPostRedisplay();
}

void clavier(unsigned char _k, int _x, int _y)
{
        if(_k == 27 || _k == 'q' || _k == 'Q')
                exit(0);
        switch(_k)
        {
        }

        glutPostRedisplay();
}

void CreerMenu(void)
{
        glutCreateMenu(ChoixMenuPrincipal);

        glutAddMenuEntry("Translation", 1);
        glutAddMenuEntry("RotationO", 2);
        glutAddMenuEntry("Homothetie", 3);
        glutAddMenuEntry("SymetrieX", 4);
        glutAddMenuEntry("SymetrieO", 5);
        glutAddMenuEntry("HomothetieC", 6);
        glutAddMenuEntry("Quitter", 11);
       
        glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc,char **argv)
{
        glutInit(&argc,argv);
       
    /*--------------------------Fenêtre ----------------------------*/
        glutInitWindowSize(winX, winY);
        glutInitWindowPosition(200,100);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
        //glutCreateWindow("TP04");
        glutDisplayFunc(affichage);
        glutReshapeFunc(redim);
        glutMouseFunc(souris);
        glutKeyboardFunc(clavier);
        CreerMenu();
        initGL();

        glPointSize(5.0);
       
        P.a = 10;
        P.b = 12;
        P.c = 23;
        P.d = 15;

        Q.a = 6;
        Q.b = 19;
        Q.c = 31;
        Q.d = 11;
       
        afficherQuat(P);
        afficherQuat(Q);
        std::cout<<"Addition :"<<std::endl;
        afficherQuat(additionQuat(P, Q));
        std::cout<<"Multiplication :"<<std::endl;
        afficherQuat(multQuat(P, Q));
        std::cout<<"Non-commutativité :"<<std::endl;
        isMultQuatCommutatif(P, Q);
        std::cout<<"Conjugué :"<<std::endl;
        afficherQuat(conjugueQuat(P));
        std::cout<<"Norme carré :"<<std::endl;
        std::cout<<normeQuatCarre(P)<<std::endl;
        std::cout<<"Norme conjugué :"<<std::endl;
        afficherQuat(normeQuatConj(P));

        glutMainLoop();
        return 0;
}

