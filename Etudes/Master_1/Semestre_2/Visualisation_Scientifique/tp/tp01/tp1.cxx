 
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cmath> // sqrt
#include <utility> // pair
#include <vector>
#include <iostream>
#include <sstream>

#include "OutilsPGM.h"

using namespace std;


Image Im;
GLubyte *I1 = NULL;
int tailleGomme = 1;
int clicGaucheEnfonce = 0;
int windowWidth = 400;
int windowHeight = 300;
//vector<pair<int,int> > points;
//vector<pair<int,int> >::iterator curPosInPoints = points.begin ();
int curPosInPoints = 1;
int curPosDragging = -1;
int bDispIdPoint = 0;
GLenum curDispMode = GL_LINE_LOOP; //GL_POINTS;
int curInsertMode = 0;

int newx=0, newy=0, lastx=0, lasty=0;
bool saisiOk;


struct polygone
{
	vector<pair<int, int> > env;
	vector<vector<pair<int, int> > > trous;
};

polygone polyg;

int getClosestPoint ( vector<pair<int,int> > points, int x, int y, float precision )
{
	int res = -1;
	int sz = points.size ();
	float dist = 10000.0;
	float newdist = dist;
	for ( int i = 0; i < sz; ++i )
	{
		newdist = sqrt ( ( points[i].first - x ) * ( points[i].first - x ) + ( points[i].second - y ) * ( points[i].second - y ) );
		if ( newdist < dist )
		{
			dist = newdist;
			res = i;
		}
	}
	if ( dist < precision )
		return res;
	return -1;
} // getClosestPoint ()

void ecrireChaine ( char *s, int x, int y )
{
    unsigned int i,sz;
    glRasterPos2i ( x, y );
    sz = strlen ( s );
    for ( i = 0; i < sz; ++i )
        glutBitmapCharacter ( GLUT_BITMAP_HELVETICA_18, s[i] );
} // ecrireChaine ()


bool estDedansMinot( polygone _p, int _mx, int _my)
{
	int intersect = 0, x, y, sz2, sz = _p.env.size(), pos1, pos2, xi, yi;
	double xx;
	
	for(int i = 0; i<sz; i++)
	{
		x = _p.env[i].first;
		y = _p.env[i].second;
		if ( i == sz-1 )
		{
			xi = _p.env[0].first;
			yi = _p.env[0].second;
		}
		else
		{
			xi = _p.env[i+1].first;
			yi = _p.env[i+1].second;
		}
				
		// 0 si gh
		// 1 si dh
		// 2 si gb
		// 3 si db
		
		if ( (y != yi) || ((y == yi) && _my != y) )
		//if( x != xi && xi != _mx && x != _mx )
		{	
			if ( x < _mx )
			{ // on est à gauche
				if ( y < _my )
					pos1 = 0;
				else
					pos1 = 2;
			}
			else
			{ // on est à droite
				if ( y < _my )
					pos1 = 1;
				else
					pos1 = 3;
			}
			
			if ( xi < _mx )
			{ // on est à gauche
				if ( yi < _my )
					pos2 = 0;
				else
					pos2 = 2;
			}
			else
			{ // on est à droite
				if ( yi < _my )
					pos2 = 1;
				else
					pos2 = 3;
			}
			
			
			if ( (pos1 == 0 && pos2 == 3) || (pos2 == 0 && pos1 == 3)
				|| (pos1 == 1 && pos2 == 2) || (pos2 == 1 && pos1 == 2) )
			{				
				// la formule n'est pas la bonne
				//xx = (double)((double)( x - xi ) / ( y - yi )) * _my;
				xx=(double)(_my - y)*((double)(xi-x)/(yi-y))+x;
				if ( xx >= _mx )
				{
					++intersect;
					/*printf ( "diagonale\n" );
					printf ( "sommet 1 : %d; x %d; y %d\n", i, x, y );
					printf ( "sommet 2 : %d; x %d; y %d\n", i+1, xi, yi );*/
				}
			}
			//else if ( x > _mx && xi > _mx && ( y < _my && yi > _my ) || ( y > _my && yi < _my ) )
			//else if ( (pos1 == hd && pos2 == bd) || ( pos1 == bd && pos2 == hd) )
			else if ( (pos1 == 1 && pos2 == 3) || ( pos1 == 3 && pos2 == 1) )
			{
				
				++intersect;
				/*printf ( "verticale\n" );
				printf ( "sommet 1 : %d; x %d; y %d\n", i, x, y );
				printf ( "sommet 2 : %d; x %d; y %d\n", i+1, xi, yi );*/
			}
		}
		
	}
	sz = _p.trous.size();	
	for(int i = 0; i< sz; i++)
	{
		
		sz2 = _p.trous[i].size();
		
		for(int j = 0; j<sz2; j++)
		{
			x = _p.trous[i][j].first;
			y = _p.trous[i][j].second;
			if ( j == sz2-1 )
			{
				xi = _p.trous[i][0].first;
				yi = _p.trous[i][0].second;
			}
			else
			{
				xi = _p.trous[i][j+1].first;
				yi = _p.trous[i][j+1].second;
			}
			
					
			// 0 si gh
			// 1 si dh
			// 2 si gb
			// 3 si db
			if ( (y != yi) || ((y == yi) && _my != y) )
			{					
				if ( x < _mx )
				{ // on est à gauche
					if ( y < _my )
						pos1 = 0;
					else
						pos1 = 2;
				}
				else
				{ // on est à droite
					if ( y < _my )
						pos1 = 1;
					else
						pos1 = 3;
				}
				
				if ( xi < _mx )
				{ // on est à gauche
					if ( yi < _my )
						pos2 = 0;
					else
						pos2 = 2;
				}
				else
				{ // on est à droite
					if ( yi < _my )
						pos2 = 1;
					else
						pos2 = 3;
				}

				
				
				if ( (pos1 == 0 && pos2 == 3) || (pos2 == 0 && pos1 == 3)
					|| (pos1 == 1 && pos2 == 2) || (pos2 == 1 && pos1 == 2) )
				{
					
					
					xx=(double)(_my - y)*((double)(xi-x)/(yi-y))+x;
					if ( xx >= _mx )
					{
						++intersect;
						/*printf ( "diagonale trous %i\n", i );
						printf ( "sommet 1 : %d; x %d; y %d\n", j, x, y );
						printf ( "sommet 2 : %d; x %d; y %d\n", j+1, xi, yi );*/
					}
				}
				else if ( (pos1 == 1 && pos2 == 3) || ( pos1 == 3 && pos2 == 1) )
				{
					++intersect;
					/*printf ( "verticale trous %i \n", i );
					printf ( "sommet 1 : %d; x %d; y %d\n", j, x, y );
					printf ( "sommet 2 : %d; x %d; y %d\n", j+1, xi, yi );*/
				}
			}
		}
	}
	//printf ( "intersect : %d\n", intersect );
	
	return (intersect%2 != 0) ? true : false;
}

bool estDedansForTrou( polygone _p, int _mx, int _my)
{
	int intersect = 0, x, y, sz2, sz = _p.env.size(), pos1, pos2, xi, yi;
	double xx;
	
	for(int i = 0; i<sz; i++)
	{
		x = _p.env[i].first;
		y = _p.env[i].second;
		if ( i == sz-1 )
		{
			xi = _p.env[0].first;
			yi = _p.env[0].second;
		}
		else
		{
			xi = _p.env[i+1].first;
			yi = _p.env[i+1].second;
		}
				
		// 0 si gh
		// 1 si dh
		// 2 si gb
		// 3 si db
		
		if ( (y != yi) || ((y == yi) && _my != y) )
		//if( x != xi && xi != _mx && x != _mx )
		{	
			if ( x < _mx )
			{ // on est à gauche
				if ( y < _my )
					pos1 = 0;
				else
					pos1 = 2;
			}
			else
			{ // on est à droite
				if ( y < _my )
					pos1 = 1;
				else
					pos1 = 3;
			}
			
			if ( xi < _mx )
			{ // on est à gauche
				if ( yi < _my )
					pos2 = 0;
				else
					pos2 = 2;
			}
			else
			{ // on est à droite
				if ( yi < _my )
					pos2 = 1;
				else
					pos2 = 3;
			}
			
			
			if ( (pos1 == 0 && pos2 == 3) || (pos2 == 0 && pos1 == 3)
				|| (pos1 == 1 && pos2 == 2) || (pos2 == 1 && pos1 == 2) )
			{				
				// la formule n'est pas la bonne
				//xx = (double)((double)( x - xi ) / ( y - yi )) * _my;
				xx=(double)(_my - y)*((double)(xi-x)/(yi-y))+x;
				if ( xx >= _mx )
				{
					++intersect;
					/*printf ( "diagonale\n" );
					printf ( "sommet 1 : %d; x %d; y %d\n", i, x, y );
					printf ( "sommet 2 : %d; x %d; y %d\n", i+1, xi, yi );*/
				}
			}
			//else if ( x > _mx && xi > _mx && ( y < _my && yi > _my ) || ( y > _my && yi < _my ) )
			//else if ( (pos1 == hd && pos2 == bd) || ( pos1 == bd && pos2 == hd) )
			else if ( (pos1 == 1 && pos2 == 3) || ( pos1 == 3 && pos2 == 1) )
			{
				
				++intersect;
				/*printf ( "verticale\n" );
				printf ( "sommet 1 : %d; x %d; y %d\n", i, x, y );
				printf ( "sommet 2 : %d; x %d; y %d\n", i+1, xi, yi );*/
			}
		}
		
	}
	
	//printf ( "intersect : %d\n", intersect );
	
	return (intersect%2 != 0) ? true : false;
}

void whySoSerious()
{

}


void exo3 ( void )
{
	glRasterPos2i(0,0);
	glPointSize ( 3.0 );
	
	// aretes de l'enveloppe
	glBegin ( curDispMode );
	glColor3f ( 0.0, 0.0, 0.0 );
	int sz = polyg.env.size ();
	ecrireChaine ( "aaaaaaaa", 100, 100 );
	for ( int i = 0; i < sz; ++i )
	{
		//cerr << i << '\n';
		glVertex2i ( polyg.env[i].first, windowHeight - polyg.env[i].second );
		if ( bDispIdPoint == 1 )
		{
			cerr << "bDispIdPoint == 1" << endl;
			stringstream ss ( stringstream::in );
			ss << i;
			ecrireChaine ( "aa", polyg.env[i].first, windowHeight - polyg.env[i].second );
		}
	}
	glEnd ();
	
	// dessin aretes des trous
	sz = polyg.trous.size ();
	int sz2;
	for ( int i = 0; i < sz; ++i )
	{
		// aretes
		glPointSize ( 3.0 );
		glBegin ( curDispMode );
		glColor3f ( 0.0, 0.0, 0.0 );
		sz2 = polyg.trous[i].size ();
		for ( int j = 0; j < sz2; ++j )
		{
			glVertex2i ( polyg.trous[i][j].first, windowHeight - polyg.trous[i][j].second );
		}
		glEnd ();
		
		// points
		glPointSize ( 5.0 );
		glColor3f ( 0.0, 1.0, 0.0 );
		glBegin ( GL_POINTS );
		for ( int j = 0; j < sz2; ++j )
		{
			glVertex2i ( polyg.trous[i][j].first, windowHeight - polyg.trous[i][j].second );
		}
		glEnd ();
	}
	
	// points de l'enveloppe
	sz = polyg.env.size ();
	glPointSize ( 5.0 );
	glBegin ( GL_POINTS );
	glColor3f ( 0.0, 0.0, 1.0 );
	for ( int i = 0; i < sz; ++i )
	{
		glVertex2i ( polyg.env[i].first, windowHeight - polyg.env[i].second );
	}
	glEnd ();
	
	// dessin du point selectionné
	glPointSize ( 5.0 );
	glBegin ( GL_POINTS );
	glColor3f ( 1.0, 0.0, 0.0 );
	try
	{
		glVertex2i ( polyg.env.at(curPosInPoints).first, windowHeight - polyg.env.at(curPosInPoints).second );
	} catch ( ... ){}
	glEnd ();
} // exo3 ()

void affichage ( void )
{
	glClear ( GL_COLOR_BUFFER_BIT );
	exo3 ();
	glutSwapBuffers ();
} // affichage ()

void redim ( int width, int height )
{
	windowHeight = height;
	windowWidth = width;
    glViewport ( 0, 0, width, height );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity ();
    gluOrtho2D ( 0, width, 0, height );
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity ();
} // redim ()

void clavier(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'e':
            polyg.env = vector<pair<int,int> > ();
            polyg.trous = vector<vector<pair<int, int> > > ();
            curInsertMode = 0;
        break;
        
        case 'i': // passer en mode insertion de l'envelope du polyg
			curInsertMode = 0;
        break;
        
        case 't': // passer en mode insertion de trous
			curInsertMode = 1;
			polyg.trous.push_back ( vector<pair<int,int> > () );
        break;
        
        case 'p': // passer en mode positionnement (déplacement)
			curInsertMode = 2;
        break;
        
        case 'j': // Super Secret Backdoor Weapon of Ultimate Deathstiny
			whySoSerious();
        break;
            
        case 'c':
            bDispIdPoint = (bDispIdPoint == 1) ? 0 : 1;
        break;
		
		case '1':
			curDispMode = GL_POINTS;
		break;
		
		case '2':
			curDispMode = GL_LINES;
		break;
		
		case '3':
			curDispMode = GL_LINE_LOOP;
		break;
		
		case '4':
			curDispMode = GL_LINE_STRIP;
		break;
        
        case 27: // echap
            exit(0); break;
        default :
            printf("tagadatsointsoin\n");        
    }
    glutPostRedisplay();
} // clavier ()

void souris ( int button, int state, int x, int y )
{

	float precision;
	int id;
	
	switch ( curInsertMode )
	{
		case 0: // mode insertion des pts de l'envelope
			precision = 15.0;
			id = getClosestPoint ( polyg.env, x, y, precision );
			switch ( button )
			{
				case GLUT_LEFT_BUTTON:
					if ( state == GLUT_DOWN && curPosDragging == -1 )
					{
						cerr << "curPosDragging == -1" << endl;
						if ( id == -1 )
						{
							cerr << "id == -1" << endl;
							if ( curPosInPoints >= polyg.env.size () )
								polyg.env.push_back ( pair<int,int> ( x, y ) );
							else
								polyg.env.insert ( polyg.env.begin () + curPosInPoints + 1, pair<int,int> ( x, y ) );
							//curPosInPoints = points.size () - 1;
							++curPosInPoints;
						}
						else
						{ // saisir le point
							curPosDragging = id;
							try
							{
								polyg.env.at ( curPosDragging ).first = x;
								polyg.env.at ( curPosDragging ).second = y;
							} catch ( ... ) {}
						}
					}
					else if ( state == GLUT_UP && curPosDragging != -1 )
					{ // déposer le point s'il existe
						try
						{
							polyg.env.at ( curPosDragging ).first = x;
							polyg.env.at ( curPosDragging ).second = y;
						} catch ( ... ) {}
						curPosDragging = -1;
					}
				break;
				case GLUT_MIDDLE_BUTTON:
					if ( state == GLUT_UP && id != -1 )
						curPosInPoints = id;
				break;
				case GLUT_RIGHT_BUTTON:
					if ( state == GLUT_UP && id != -1 )
					{
						polyg.env.erase ( polyg.env.begin () + id );
						if ( curPosInPoints >= id )
							--curPosInPoints;
					}
				break;
			} // switch
		break;
		
		case 1: // mode insertion des pts d'un nouveau trou
			if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			{
				if (estDedansForTrou(polyg, x,y))
				{
					polyg.trous.back ().push_back ( pair<int,int> ( x, y ) );	
				}
				else
				{
					printf("Dans le polygone MINOT, dans le POLYGONE !\n");
				}
			}
			
		break;
		
		case 2:
			if ( state == GLUT_DOWN && curPosDragging == -1 )
			{
				if ( id != -1 )
				{ //saisir le polygone													
					curPosDragging = id;
					if(estDedansMinot(polyg, x,y) && saisiOk)
					{
						lastx = x;
						lasty = y;								
					}							
					else 
						saisiOk = false;
				}
			}
			else if ( state == GLUT_UP && curPosDragging != -1 )
			{ // déposer le point s'il existe
				if(estDedansMinot(polyg, x,y))
				{
					for(int i = 0; i< polyg.env.size(); i++)
					{
						polyg.env[i].first += (newx-lastx);
						polyg.env[i].second += (newy-lasty);
					}
				}
				saisiOk = true;
				curPosDragging = -1;
			}
		break;
	} // switch
	glutPostRedisplay ();
} // souris ()

void sourisMvt ( int x, int y )
{
    if ( curPosDragging != -1 )
    {    	
		try
		{
			if(curInsertMode == 2 && estDedansMinot(polyg, x,y) && saisiOk)
			{
				newx = x;
				newy = y;
				for(int i = 0; i< polyg.env.size(); i++)
				{
					polyg.env[i].first += (newx-lastx);
					polyg.env[i].second += (newy-lasty);
				}
				for(int i = 0; i< polyg.trous.size(); i++)
				{						
					for(int j = 0; j<polyg.trous[i].size(); j++)
					{
						polyg.trous[i][j].first += (newx-lastx);
						polyg.trous[i][j].second += (newy-lasty);
					}
				}
				lastx = newx;
				lasty = newy;
			}
			else
			{
				polyg.env.at ( curPosDragging ).first = x;
				polyg.env.at ( curPosDragging ).second = y;
			}
		} catch ( ... ) {}
		glutPostRedisplay ();		
    }
} // sourisMvt ()

int main ( int argc, char **argv )
{
	glutInit ( &argc, argv );
	glutInitWindowSize ( windowWidth, windowHeight );
	glutInitWindowPosition ( 100, 100 );
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );
	glutCreateWindow ( "tp2exo3" );
	glutDisplayFunc ( affichage );
	glutReshapeFunc ( redim );
	glutKeyboardFunc ( clavier );
	glutMouseFunc ( souris );
	glutMotionFunc( sourisMvt );
	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
	glEnable ( GL_LINE_SMOOTH );
	glPointSize ( 3.0 );
	glClearColor ( 1.0, 1.0, 1.0, 1.0 );
	glutMainLoop ();
	return 0;
} // main ()
