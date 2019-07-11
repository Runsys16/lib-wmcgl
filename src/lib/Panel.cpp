#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Panel.h"


//#define DEBUG
//#define DEBUG_CONST

#ifdef DEBUG_WM
#	define DEBUG
#endif

#ifdef DEBUG
#	define DEBUG_CONST
#endif



using namespace std;

Panel::Panel()	{
	#ifdef DEBUG_CONST
	cout << "Constructeur Panel ..." << endl;
	#endif
	init();
}



void Panel::init()	{
	#ifdef DEBUG_CONST
	cout << " Panel::init() ..." << endl;
	#endif
	
	visible			= true;
	canMove			= true;
	mouseVisible	= true;

	x_raw			= 0;
	y_raw			= 0;
	dx_raw			= 0;
	dy_raw			= 0;
	
	parent = NULL;
	
	ID = -1;
	
	displayGL_cb        = NULL;
	click_left_cb       = NULL;
	release_left_cb     = NULL;
	click_right_cb      = NULL;
    release_right_cb    = NULL;

	
	bHaveMove = false;
	bScissor = false;
	bFantome = false;
}





void Panel::add( Panel* p)	{
	#ifdef DEBUG_CONST
	cout << " Panel::add() ..." << endl;
	#endif
	childs.push_back( p );
	
	p->setParent( this );
}

void Panel::sup( Panel* p)	{
	int nb = childs.size();
	for ( int i=0; i<nb; i++ )	{
		if ( childs[i] == p )	{
			childs.erase( childs.begin()+i );
		}
	}
}



void Panel::clickLeft( int x, int y)
{
    if (click_left_cb)      (*click_left_cb)(Screen2x(x), Screen2y(y));
}



void Panel::releaseLeft( int x, int y)
{
    if (release_left_cb)      (*release_left_cb)(x,y);
}



void Panel::clickRight( int x, int y)
{
    if (click_right_cb)      (*click_right_cb)(x,y);
}



void Panel::releaseRight( int x, int y)
{
    if (release_right_cb)      (*release_right_cb)(x,y);
}



Panel* Panel::isMouseOver(int xm, int ym)	{
	#ifdef DEBUG
	cout << "Panel::isMouseOver()" << x_raw <<", "<< y_raw <<", "<< dx_raw <<", "<< dy_raw << endl;
	#endif

	if ( !visible )			return NULL;
	if ( bFantome )			return NULL;

	int nb = childs.size();
	//cout << "Panel::isMouseOver() nb = " << nb << endl;
	for( int i=0; i<nb; i++ )	{
		Panel * p = childs[i]->isMouseOver(xm, ym);
		if ( p )		return p;
	}

	if ( x_raw <= xm && xm <= (x_raw+dx_raw) && y_raw <= ym && ym <= (y_raw+dy_raw) )		return this;
	else																					return NULL;
}

/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void Panel::clickUp(int xm, int ym)	{
	if ( parent )		parent->clickUp( x, y );
	return;
	int nb = childs.size();
	
	for( int i=0; i<nb; i++ )	{
		childs[i]->clickUp( x, y );
	}
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void Panel::clickDown(int xm, int ym)	{
	if ( parent )		parent->clickDown( x, y );
	return;
	int nb = childs.size();
	for( int i=0; i<nb; i++ )	{
		childs[i]->clickDown( x, y );
	}
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void Panel::updatePos()	{
	//cout << "Panel::updatePos() ..." << endl;
	x_raw = x;
	y_raw = y;
	Panel* current = parent;
	//cout << "Panel::updatePos() ..."<<"ID="<< ID <<"x_raw="<< x_raw <<" y_raw="<< y_raw << endl;
	while( current != NULL )	{
		//cout << " 0-Panel::updatePos() ..."<<"ID="<< parent->getID() <<"x_raw="<< x_raw <<" y_raw="<< y_raw << endl;
		x_raw += current->getPosX();
		y_raw += current->getPosY();
		//cout << " 1-Panel::updatePos() ..."<<"ID="<< current->getID() <<"x_raw="<< x_raw <<" y_raw="<< y_raw << endl;
		current = current->getParent();
	}

	dx_raw = dx;
	dy_raw = dy;

	int nb = childs.size();
	for( int i=0; i<nb; i++ )	{
		childs[i]->updatePos();
	}
}



/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void Panel::idle(float elapsedtime)	{
	int nb = childs.size();
	for( int i=0; i<nb; i++ )	{
		childs[i]->idle( elapsedtime );
	}
}



void Panel::displayGL()	{


	int nb = childs.size();
	
	for( int i=0; i<nb; i++ )	{
		childs[i]->displayGL();
	}

	if (displayGL_cb)   (*displayGL_cb)();
}





void Panel::deleteChilds()	{
	//_ResourceManager& res = _ResourceManager::getInstance();
    if ( childs.size() == 0 )           return;

    childs.clear();
    /*    
	int nb = childs.size();
	for ( int i=nb; i>0; i++ )	{
			childs.erase( childs.begin()+i-1 );
	}
	*/
}




