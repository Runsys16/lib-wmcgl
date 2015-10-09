#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "PanelScrollY.h"
#include "PanelText.h"
#include "ResourceManager.h"
#include "wm.h"
//#include "../include/WindowsManager.h"


//#define DEBUG
//#define DEBUG_CONST

#ifdef DEBUG_WM
#	define DEBUG
#endif


#ifdef DEBUG
#	define DEBUG_CONST
#endif


using namespace std;

//#define NB	10
//static char*		tab[NB];

/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
PanelScrollY::PanelScrollY() : PanelSimple()	{	
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelScrollY ..." << endl;
	#endif
	y_scroll = 0;
	y_delta = 10;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelScrollY::clickUp( int x, int y ) {
	if ( y_scroll != 0 )	{
		y_scroll += y_delta;
		setPosY( getPosY() + y_delta );
		updatePos();
	}
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelScrollY::clickDown( int x, int y ) {
	y_scroll -= y_delta;
	setPosY( getPosY() - y_delta );
	updatePos();
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelScrollY::updatePos() {
	//if ( bTextOK == false )			buildText();

	if ( bDebug )	{
		if ( pPtDebug != NULL )	{
			WindowsManager& wm = WindowsManager::getInstance();
			char	str[255];
		
			sprintf( str, "ID=%d, X=%d, Y=%d, DX=%d, DY=%d order=%d", getID(), getX(), getY(), getDX(), getDY(), wm.getOrder(this) );
			sDebug = string(str);
			pPtDebug->changeText( sDebug, PanelText::NORMAL_FONT, true );
		}
	}

	//-----------------------------------------
	// Calcul des positions
	Panel::updatePos();
	//-----------------------------------------
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
Panel* PanelScrollY::isMouseOver(int xm, int ym)	{
	#ifdef DEBUG
	cout << "Panel::isMouseOver()" << x_raw <<", "<< y_raw <<", "<< dx_raw <<", "<< dy_raw << endl;
	#endif
	if ( !visible )			return NULL;

	if ( x_raw <= xm && xm <= (x_raw+dx_raw) && (y_raw-y_scroll) <= ym && ym <= (y_raw-y_scroll+dy_raw) )	{
		int nb = childs.size();
		for( int i=0; i<nb; i++ )	{
			Panel * p = childs[i]->isMouseOver(xm, ym);
			if ( p )		return p;
		}
		return this;
	}
	
	return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelScrollY::displayGL() {
	
	//if (texBackground == null)			return;
	//cout << "PanelScrollY::displayGL()" << endl;
	if (visible == false)			return;
	//if ( bTextOK == false )			return;

	WindowsManager& wm = WindowsManager::getInstance();
	
	float width  = (float)wm.getWidth();
	float height = (float)wm.getHeight();
	
	float X = getX();
	float Y = getY() - y_scroll;
	float DX = getDX();
	float DY = getDY();
	
#ifdef DEBUG
	cout << "PS displayGL ... fenetre id : " << getID() << endl;
	cout << "    PS:" << "-------------------" << endl;
	cout << "    PS:" << getX() <<", "<< getY() <<", "<< getDX() <<", "<< getDY() << endl;
	cout << "    PS:" << X <<", "<< Y <<", "<< DX <<", "<< DY << endl;
	cout << "    PS:" << wm.getWidth() <<", "<< wm.getHeight() << endl;
#endif
	
	/*
	X = -0.9f;
	Y =  -1.9f;
	DX = 1.0f;
	DY = 1.0f;
	*/

	//m_pTexBackground->Bind(wm.getSlot());
	//glBlendColor( 0.0, 0.0, 0.0, 0.2);
	if ( m_pTexBackground )		{
		m_pTexBackground->Bind( 0 );
	
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
		glBegin(GL_QUADS);
	
		glColor4f( 1.0, 1.0, 1.0, 1.0 );

		glTexCoord2f(1, 1);
		glVertex2f(X + DX, Y + DY);

		glTexCoord2f(1, 0);
		glVertex2f(X + DX, Y);

		glTexCoord2f(0, 0);
		glVertex2f(X, Y);

		glTexCoord2f(0, 1);
		glVertex2f(X, Y + DY);

		glEnd();

		m_pTexBackground->Unbind( 0 );
	}

	//m_pTexBackground->Unbind(wm.getSlot());




	
	
	int scx, scy, scdx, scdy;
	scx  = getX();
	scy  = height - getDY() - (getY() - y_scroll);
	scdx = getDX();
	scdy = getDY();
#ifdef DEBUG
	cout << "    PS:" << "-------------------" << endl;
	cout << "    PS:" << "-----Scissor-------" << endl;
	cout << "    PS:" << scx <<", "<< scy <<", "<< scdx <<", "<< scdy << endl;
#endif
	glScissor( scx, scy, scdx, scdy );
	glEnable( GL_SCISSOR_TEST );
	

	// display	with scissor
	Panel::displayGL();


	glDisable( GL_SCISSOR_TEST );
	
	if ( bDebug && pPsDebug )		{
		pPsDebug->displayGL();
		//pPtDebug->displayGL();
	}
}





