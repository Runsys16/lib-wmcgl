#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "PanelCheckBox.h"
#include "PanelText.h"
#include "ResourceManager.h"
#include "wm.h"
//#include "../include/WindowsManager.h"


//#define DEBUG
//#define DEBUG_MOUSEOVER
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
PanelCheckBox::PanelCheckBox()	: PanelSimple(){	
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelCheckBox ..." << endl;
	#endif
	//PanelSimple::PanelSimple();

	
	m_pTexTrue  = NULL;
	m_pTexFalse = NULL;
	
	pCallBackTrue  = NULL;
	pCallBackFalse = NULL;
	pCallBackMouse = NULL;
	
	pPsDebug = NULL;
	pPtDebug = NULL;
	bDebug = false;
	bVal = false;
	pVal = NULL;
	bUseInverse = false;
	percent = 1.0;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelCheckBox::setPercent( float f ) {
	percent = f;
	float fDX = percent * (float)getPosDX();
	float fDY = percent * (float)getPosDY();
	dx = (int) fDX;
	dy = (int) fDY;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelCheckBox::updatePos() {
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
	if ( pVal )		bVal = *pVal;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelCheckBox::clickLeft( int xm, int ym)	{
	bVal = !bVal;
	if ( pVal )			*pVal = bVal;
	if( bVal && pCallBackTrue )			(*pCallBackTrue)();
	else if( !bVal && pCallBackFalse )	(*pCallBackFalse)();
	
	if ( pCallBackMouse )       (*pCallBackMouse)(this);
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
Panel* PanelCheckBox::isMouseOver(int xm, int ym)	{
	#ifdef DEBUG_MOUSEOVER
	cout << "PanelCheckBox::isMouseOver( "<< xm <<", "<< ym <<" ) " << x_raw <<", "<< y_raw <<", "<< dx_raw <<", "<< dy_raw << endl;
	#endif
	if ( !visible )			return NULL;

	if ( x_raw <= xm && xm <= (x_raw+dx_raw) && y_raw <= ym && ym <= (y_raw+dy_raw) )	{
		#ifdef DEBUG_MOUSEOVER
		cout << "PanelCheckBox::isMouseOver)  OVER" << endl;
		#endif
		return this;
	}
	else	{
		return NULL;
	}
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelCheckBox::displayGL() {
	//if (texBackground == null)			return;
	//cout << "PanelCheckBox::displayGL()" << endl;
	if (visible == false)			return;
	//if ( bTextOK == false )			return;

	WindowsManager& wm = WindowsManager::getInstance();
	
	float width  = (float)wm.getWidth();
	float height = (float)wm.getHeight();
	
	float X = getX();
	float Y = getY();
	float DX = getDX();
	float DY = getDY();
	
#ifdef DEBUG
	cout << "PS displayGL ... fenetre id : " << getID() << endl;
	cout << "    PS:" << "-------------------" << endl;
	cout << "    PS:" << getX() <<", "<< getY() <<", "<< getDX() <<", "<< getDY() << endl;
	cout << "    PS:" << X <<", "<< Y <<", "<< DX <<", "<< DY << endl;
	cout << "    PS:" << wm.getWidth() <<", "<< wm.getHeight() << endl;
#endif
	
	_Texture2D*		m_pTexCurrent;
	if ( bVal )		m_pTexCurrent = m_pTexTrue;
	else			m_pTexCurrent = m_pTexFalse;
	
	if ( bUseInverse )	{
		if (m_pTexCurrent == m_pTexTrue)	m_pTexCurrent = m_pTexFalse;
		else								m_pTexCurrent = m_pTexTrue;
	}

	//m_pTexBackground->Bind(wm.getSlot());
	unsigned char a = (c&0xff000000)>>24;
	unsigned char r = (c&0x00ff0000)>>16;
	unsigned char g = (c&0x0000ff00)>>8;
	unsigned char b = (c&0x000000ff);

	glColor4f( r,g,b,a );

	if ( m_pTexBackground )		{
		m_pTexBackground->Bind( 0 );
	
		//unsigned	dx = m_pTexBackground->dx;
		//unsigned	dy = m_pTexBackground->dy;
		unsigned	dx = DX;
		unsigned	dy = DY;
	
		glBegin(GL_QUADS);
		
		
	
		//glColor4f( 1.0, 1.0, 1.0, 1.0 );

		glTexCoord2f(1, 1);
		glVertex2f(X + dx, Y + dy);

		glTexCoord2f(1, 0);
		glVertex2f(X + dx, Y);

		glTexCoord2f(0, 0);
		glVertex2f(X, Y);

		glTexCoord2f(0, 1);
		glVertex2f(X, Y + dy );

		glEnd();

		m_pTexBackground->Unbind( 0 );
	}

	if ( m_pTexCurrent )		{
		m_pTexCurrent->Bind( 0 );
	
		/*
		if ( percent >1.0 )	percent = 1.0;
		
		unsigned	dx = (unsigned)(float)m_pTexCurrent->dx * percent;
		unsigned	dy = (unsigned)(float)m_pTexCurrent->dy * percent;
        */
		unsigned	dx = DX;
		unsigned	dy = DY;
	
		glBegin(GL_QUADS);
	
		//glColor4f( 1.0, 1.0, 1.0, 1.0 );

		glTexCoord2f(1, 1);
		glVertex2f(X + dx, Y + dy);

		glTexCoord2f(1, 0);
		glVertex2f(X + dx, Y);

		glTexCoord2f(0, 0);
		glVertex2f(X, Y);

		glTexCoord2f(0, 1);
		glVertex2f(X, Y + dy);

		glEnd();

		m_pTexCurrent->Unbind( 0 );
	}

	//m_pTexBackground->Unbind(wm.getSlot());




	
	
	int scx, scy, scdx, scdy;
	scx  = getX();
	scy  = height - getDY() - getY();
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
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelCheckBox::debug( bool b )	{
	bDebug = b;
	
	
	if ( b && pPsDebug == NULL )	{
		pPsDebug = new PanelCheckBox();
		pPsDebug->setPosAndSize( 10, -20, 300, 20 );
	}		

	if ( b && pPtDebug == NULL )	{
		WindowsManager& wm = WindowsManager::getInstance();
		pPtDebug = new PanelText();
		char	str[255];
		
		sprintf( str, "ID=%d\n X=%d, Y=%d, DX=%d, DY=%d order=%d", getID(), getX(), getY(), getDX(), getDY(), wm.getOrder(this) );
		sDebug = string(str);
		cout <<" DebugPanel : "<< sDebug << endl;
		
		pPtDebug->changeText( sDebug, PanelText::NORMAL_FONT, true );
		pPtDebug->setPos(0, 0);
		pPsDebug->add( pPtDebug );
		add( pPsDebug );
	}
	/*
	if ( b )	{
		pPsDebug->setVisible( true );
	}
	else	{
		pPsDebug->setVisible( false );
	}
	*/
	pPsDebug->setVisible( bDebug );
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelCheckBox::setTrue( char * str_background )	{
	if ( str_background == NULL )		{
		m_pTexTrue = NULL;
		return;
	}
	_ResourceManager& res = _ResourceManager::getInstance();

	_Texture2D* pResource = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, str_background) );

	if ( pResource )	m_pTexTrue = pResource;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelCheckBox::setFalse( char * str_background )	{
	if ( str_background == NULL )		{
		m_pTexFalse = NULL;
		return;
	}
	_ResourceManager& res = _ResourceManager::getInstance();

	_Texture2D* pResource = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, str_background) );

	if ( pResource )	m_pTexFalse = pResource;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/




