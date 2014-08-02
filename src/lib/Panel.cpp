#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Panel.h"




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
	visible			= true;
	canMove			= true;
	mouseVisible	= true;

	x_raw			= 0;
	y_raw			= 0;
	dx_raw			= 0;
	dy_raw			= 0;
	
	ID = -1;
}



void Panel::init()	{
	#ifdef DEBUG_CONST
	cout << " Panel::init() ..." << endl;
	#endif
}





void Panel::add( Panel* p)	{
	childs.push_back( p );
	
	p->setParent( this );
}



bool Panel::isMouseOver(int xm, int ym)	{
	#ifdef DEBUG
	cout << "Panel::isMouseOver()" << x_raw <<", "<< y_raw <<", "<< dx_raw <<", "<< dy_raw << endl;
	#endif
	if ( !visible )			return false;

	if ( x_raw <= xm && xm <= (x_raw+dx_raw) && y_raw <= ym && ym <= (y_raw+dy_raw) )		return true;
	else																					return false;
}




void Panel::updatePos()	{
	//cout << "Panel::updatePos() ..." << endl;
	x_raw = x;
	y_raw = y;
	Panel* current = parent;
	while( current != NULL )	{
		x_raw += current->getX();
		y_raw += current->getY();
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

	
}












