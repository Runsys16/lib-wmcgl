#include <stdlib.h>
#include <stdio.h>
#include <iostream>
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
#include "Panel.h"
#include "wm.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
//#define DEBUG
//#define DEBUG_CONST
#define DEBUG_DEST
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
#ifdef DEBUG_WM
#	define DEBUG
#endif

#ifdef DEBUG
#	define DEBUG_CONST
#endif
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
using namespace std;
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Panel::Panel()	{
	#ifdef DEBUG_CONST
	logf( (char*)"Constructeur Panel ... %s", sExtra.c_str() );
	#endif
	init();
}
	/*
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Panel::~Panel()	{
	#ifdef DEBUG_DEST
	logf( (char*)"Destructeur Panel ... %s, %s", sExtra.c_str()  );
	#endif
	int n = childs.size();
	if ( n> 0 )	
	{
		for ( int i=0; i<n; n++ )
		{
			delete childs[i];
			childs[i] = NULL;
		}
		childs.clear();
	}
}
	*/
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Panel::init()	{
	#ifdef DEBUG_CONST
	cout << " Panel::init() ..." << endl;
	#endif
	
	visible			= true;
	bCanMove		= true;
	mouseVisible	= true;

	x  = x_raw		= 0;
	y  = y_raw		= 0;
	dx = dx_raw		= 0;
	dy = dy_raw		= 0;
	
	parent = NULL;
	
	ID = -1;
	
	displayGL_cb        = NULL;
	click_left_cb       = NULL;
	release_left_cb     = NULL;
	click_right_cb      = NULL;
    release_right_cb    = NULL;

	panel_click_left       = NULL;
	panel_release_left     = NULL;
	panel_click_right      = NULL;
    panel_release_right    = NULL;
	
	bHaveMove		= false;
	bScissor		= false;
	bFantome		= false;
	bCapture		= false;
	bFocus			= false;
	bParentCliping	= false;
	
	sExtra = "";
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Panel::add( Panel* p)	{
	#ifdef DEBUG_CONST
	cout << " Panel::add() ..." << endl;
	#endif
	if ( p->getParent() != NULL )			{ cout << " Panel::add() parent existe ..." << endl; return; }

	childs.push_back( p );
	
	p->setParent( this );
	p->setCanMove( false );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Panel::sup( Panel* p)	{
	int nb = childs.size();
	for ( int i=0; i<nb; i++ )	{
		if ( childs[i] == p )	{
			WindowsManager& wm = WindowsManager::getInstance();
			if ( p == wm.getCapture() )		wm.supCapture(p);
			if ( p == wm.getFocus() )		wm.supFocus(p);
			childs.erase( childs.begin()+i );
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Panel::onTop()	{
	#ifdef DEBUG
	//cout << "Panel::onTop() ID="<< p->getID() <<" nb="<< childs.size() << endl;
	#endif
	if ( parent== NULL )		return;
	
	std::vector<Panel*>& parChilds = parent->getChilds();
	int nb = parChilds.size();
	int id = getID();
	
	for ( int i=0; i<nb; i++ )	{
		
		if ( parChilds[i] == this )	{
			parChilds.erase( parChilds.begin()+i );
			parChilds.push_back(this);
			break;
		}
	}

	#ifdef DEBUG
	//cout << "Panel::onTop() nb="<< childs.size() << endl;
	#endif
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Panel::onBottom()	{
	#ifdef DEBUG
	cout << "Panel::onBottom() ID="<< p->getID() <<" nb="<< childs.size() << endl;
	#endif
	if ( parent== NULL )		return;

	Panel* pC_SVG;
	Panel* pF_SVG;
    
    
	std::vector<Panel*>& parChilds = parent->getChilds();
	parent->sup(this);
	parChilds.insert( parChilds.begin(), this );
    
	#ifdef DEBUG
	cout << "Panel::onBottom() nb="<< childs.size() << endl;
	#endif
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Panel::clickLeft( int x, int y)
{
    if (click_left_cb)              (*click_left_cb)(Screen2x(x), Screen2y(y));
    if (panel_click_left)           clickLeft( Screen2x(x), Screen2y(y) );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Panel::releaseLeft( int x, int y)
{
    if (release_left_cb)      (*release_left_cb)(x,y);
    if (panel_release_left)   panel_release_left->releaseLeft( x, y );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Panel::clickRight( int x, int y)
{
    if (click_right_cb)      (*click_right_cb)(x,y);
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Panel::releaseRight( int x, int y)
{
    if (release_right_cb)      (*release_right_cb)(x,y);
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Panel* Panel::isMouseOverRaw(int xm, int ym)	{
//#define DEBUG
	#ifdef DEBUG
	WindowsManager& wm = WindowsManager::getInstance();
	wm.logf( (char*)"Panel::isMouseOver(%d,%d) \"%s\" id=%d (%d, %d, %d, %d)", xm, ym, getExtraString().c_str(), getID(),
	                 x_raw, y_raw, dx_raw, dy_raw );
	wm.log_tab(true);
	#endif
	
	if ( !visible || bFantome )
	{
	    #ifdef DEBUG
	    wm.log_tab(false);
	    #endif
        return NULL;
    }

	if ( x_raw <= xm && xm <= (x_raw+dx_raw) && y_raw <= ym && ym <= (y_raw+dy_raw) ){
	    #ifdef DEBUG
      	wm.logf( (char*)"OK-Panel::isMouseOver() = %s", getExtraString().c_str() );
        wm.log_tab(false);
	    #endif
	    return this;
	}
	else {
	    #ifdef DEBUG
       	wm.logf( (char*)"NOK-Panel::isMouseOver(%d,%d)", xm, ym );
        wm.log_tab(false);
	    #endif
	    return NULL;
	}

#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Panel* Panel::isMouseOver(int xm, int ym)	{
//#define DEBUG
	#ifdef DEBUG
	WindowsManager& wm = WindowsManager::getInstance();
	wm.logf( (char*)"Panel::isMouseOver(%d,%d) \"%s\" id=%d (%d, %d, %d, %d)", xm, ym, getExtraString().c_str(), getID(),
	                 x_raw, y_raw, dx_raw, dy_raw );
	wm.log_tab(true);
	#endif
	
	
	if ( !visible || bFantome )
	{
	    #ifdef DEBUG
	    wm.log_tab(false);
	    #endif
        return NULL;
    }


	int nb = childs.size();
	for( int i=nb-1; i>=0; i-- )	{
		Panel* p;
		p = childs[i]->isMouseOver(xm, ym);
		
		if ( p )		{
    	    #ifdef DEBUG
        	wm.logf( (char*)"OK-Panel::isMouseOver(%d,%d)", xm, ym );
		    wm.log_tab(false);
		    #endif
		    return p;
		}
	}

	if ( bParentCliping && parent!=NULL )		return parent->isMouseOverRaw(xm, ym)!=NULL ? this : NULL;

	if ( x_raw <= xm && xm <= (x_raw+dx_raw) && y_raw <= ym && ym <= (y_raw+dy_raw) ){
	    #ifdef DEBUG
      	wm.logf( (char*)"OK-Panel::isMouseOver() = %s", getExtraString().c_str() );
        wm.log_tab(false);
	    #endif
	    return this;
	}
	else {
	    #ifdef DEBUG
       	wm.logf( (char*)"NOK-Panel::isMouseOver(%d,%d)", xm, ym );
        wm.log_tab(false);
	    #endif
	    return NULL;
	}

#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Panel* Panel::isMouseOverBorder(int xm, int ym)	{
//#define DEBUG
	#ifdef DEBUG
	cout << "Panel::isMouseOverBorder()" << x_raw <<", "<< y_raw <<", "<< dx_raw <<", "<< dy_raw << endl;
	#endif
    
    mouseOverBorder = MOB_FREE;
    
	return NULL;
//#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Panel::clickUp(int xm, int ym)	{
	if ( parent )		parent->clickUp( x, y );
	return;
	int nb = childs.size();
	
	for( int i=0; i<nb; i++ )	{
		childs[i]->clickUp( x, y );
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Panel::clickDown(int xm, int ym)	{
	if ( parent )		parent->clickDown( x, y );
	return;
	int nb = childs.size();
	for( int i=0; i<nb; i++ )	{
		childs[i]->clickDown( x, y );
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Panel::updatePos()	{
//#define DEBUG

	x_raw = x;
	y_raw = y;
	Panel* current = parent;
	#ifdef DEBUG
	cout << "Panel::updatePos() ..."<<"ID="<< ID <<" raw("<< x_raw <<","<< y_raw <<")"<< endl;
	#endif
	while( current != NULL )	{
    	#ifdef DEBUG
		cout << "  parent ID="<< current->getID() <<" rel("<< current->getPosX() <<","<< current->getPosY() <<")"<< endl;
    	#endif
		x_raw += current->getPosX();
		y_raw += current->getPosY();
    	#ifdef DEBUG
		cout << "  calcul  raw("<< x_raw <<","<< y_raw <<")"<< endl;
    	#endif
		current = current->getParent();
	}

	dx_raw = dx;
	dy_raw = dy;

	#ifdef DEBUG
	cout << "  ID="<< ID <<" raw("<< x_raw <<","<< y_raw <<") delta("<< dx <<","<< dy <<")"<< endl;
	#endif
	int nb = childs.size();
	for( int i=0; i<nb; i++ )	{
		childs[i]->updatePos();
	}
//#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Panel::idle(float elapsedtime)	{
	int nb = childs.size();
	for( int i=0; i<nb; i++ )	{
		childs[i]->idle( elapsedtime );
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Panel::displayGL()	{


	int nb = childs.size();
	
	for( int i=0; i<nb; i++ )	{
		childs[i]->displayGL();
	}

	if (displayGL_cb)   (*displayGL_cb)();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool Panel::isVisible()	{
	//_ResourceManager& res = _ResourceManager::getInstance();
	Panel * p = this;
	while( p!= NULL )
	{
    	if ( !p->getVisible() )         return false;
    	p = p->getParent();
	}

    return true;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------

