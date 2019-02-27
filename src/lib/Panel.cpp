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
	
	displayGL_cb = NULL;
	
	bHaveMove = false;
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



Panel* Panel::isMouseOver(int xm, int ym)	{
	#ifdef DEBUG
	cout << "Panel::isMouseOver()" << x_raw <<", "<< y_raw <<", "<< dx_raw <<", "<< dy_raw << endl;
	#endif
	if ( !visible )			return NULL;

	int nb = childs.size();
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



void Panel::displayGL()	{


	int nb = childs.size();
	
	for( int i=0; i<nb; i++ )	{
		childs[i]->displayGL();
	}

	if (displayGL_cb)   (*displayGL_cb)();
}






