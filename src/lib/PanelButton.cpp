#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "PanelButton.h"
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
PanelButton::PanelButton()	: PanelSimple(){	
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelButton ..." << endl;
	#endif

	init();
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
PanelButton::PanelButton( PanelButtonCallBack* pCB)	: PanelSimple(){	
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelButton ..." << endl;
	#endif

	//PanelButton::PanelButton();
	init();
	pPanelButtonCallBack = pCB;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelButton::init(){	
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelButton ..." << endl;
	#endif

	m_pTexUp = NULL;
	m_pTexDown = NULL;
	m_pTexOver = NULL;
	
	m_pTexCurrent = NULL;

	pCallBackUp   = NULL;
	pCallBackDown = NULL;
	pCallBackOver = NULL;
	
	pPsDebug = NULL;
	pPtDebug = NULL;
	bDebug = false;
	pPanelButtonCallBack = NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelButton::updatePos() {
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
void PanelButton::haveCapture()	{
	#ifdef DEBUG_MOUSEOVER
	cout << "PanelButton::haveCapture( "<< endl;
	#endif
	m_pTexCurrent = m_pTexOver;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelButton::lostCapture()	{
	#ifdef DEBUG_MOUSEOVER
	cout << "PanelButton::LostCapture( "<< endl;
	#endif
	m_pTexCurrent = m_pTexUp;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelButton::clickLeft( int xm, int ym)	{
	if( pCallBackDown )				(*pCallBackDown)(this);
	if ( pPanelButtonCallBack )		pPanelButtonCallBack->cb_button_mouse_down(this);
	PanelSimple::clickLeft( xm, ym);
	texDown();
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelButton::releaseLeft( int xm, int ym)	{
	if( pCallBackUp )				(*pCallBackUp)(this);
	if ( pPanelButtonCallBack )		pPanelButtonCallBack->cb_button_mouse_up(this);
	PanelSimple::releaseLeft( xm, ym);
	texUp();
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
Panel* PanelButton::isMouseOver(int xm, int ym)	{
	#ifdef DEBUG_MOUSEOVER
	cout << "PanelButton::isMouseOver( "<< xm <<", "<< ym <<" ) " << x_raw <<", "<< y_raw <<", "<< dx_raw <<", "<< dy_raw << endl;
	#endif
	if ( !visible )			return NULL;

	if ( x_raw <= xm && xm <= (x_raw+dx_raw) && y_raw <= ym && ym <= (y_raw+dy_raw) )	{
    	texOver();
		#ifdef DEBUG_MOUSEOVER
		cout << "PanelButton::isMouseOver)  OVER" << endl;
		#endif
		if( pCallBackOver )				(*pCallBackOver)(this);
		if ( pPanelButtonCallBack )		pPanelButtonCallBack->cb_button_mouse_over(this);

		return this;
	}
	else	{
	    texUp();
		return NULL;
	}
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelButton::texOver()	{
	#ifdef DEBUG_MOUSEOVER
	cout << "PanelButton::haveCapture( "<< endl;
	#endif
	m_pTexCurrent = m_pTexOver;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelButton::texDown()	{
	#ifdef DEBUG_MOUSEOVER
	cout << "PanelButton::haveCapture( "<< endl;
	#endif
	m_pTexCurrent = m_pTexDown;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelButton::texUp()	{
	#ifdef DEBUG_MOUSEOVER
	cout << "PanelButton::haveCapture( "<< endl;
	#endif
	m_pTexCurrent = m_pTexUp;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelButton::displayGL() {
	//if (texBackground == null)			return;
	//cout << "PanelButton::displayGL()" << endl;
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
	
	unsigned char a = (c&0xff000000)>>24;
	unsigned char r = (c&0x00ff0000)>>16;
	unsigned char g = (c&0x0000ff00)>>8;
	unsigned char b = (c&0x000000ff);

	glColor4f( r,g,b,a );



	//cout << "    PS:"<< c <<" : "<< r <<","<< g <<","<< b <<","<< a << endl;

	if ( m_pTexBackground )	{
		m_pTexBackground->Bind(0);
		glBegin(GL_QUADS);
	
		//glColor4f( 1.0, 1.0, 1.0, 1.0 );

		glTexCoord2f(1, 1);
		glVertex2f(X + DX, Y + DY);

		glTexCoord2f(1, 0);
		glVertex2f(X + DX, Y);

		glTexCoord2f(0, 0);
		glVertex2f(X, Y);

		glTexCoord2f(0, 1);
		glVertex2f(X, Y + DY);

		glEnd();

		m_pTexBackground->Unbind(0);
	}

	if ( m_pTexCurrent == NULL )	m_pTexCurrent = m_pTexUp;

	if ( m_pTexCurrent )		{
		m_pTexCurrent->Bind( 0 );
	
	
		glBegin(GL_QUADS);
	
		//glColor4f( 1.0, 1.0, 1.0, 1.0 );

		glTexCoord2f(1, 1);
		glVertex2f(X + DX, Y + DY);

		glTexCoord2f(1, 0);
		glVertex2f(X + DX, Y);

		glTexCoord2f(0, 0);
		glVertex2f(X, Y);

		glTexCoord2f(0, 1);
		glVertex2f(X, Y + DY);

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
	//glScissor( scx, scy, scdx, scdy );
	//glEnable( GL_SCISSOR_TEST );
	

	// display	with scissor
	Panel::displayGL();


	//glDisable( GL_SCISSOR_TEST );
	
	if ( bDebug && pPsDebug )		{
		pPsDebug->displayGL();
		//pPtDebug->displayGL();
	}
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelButton::debug( bool b )	{
	bDebug = b;
	
	
	if ( b && pPsDebug == NULL )	{
		pPsDebug = new PanelButton();
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
void PanelButton::setUp( char * str_background )	{
	if ( str_background == NULL )		{
		m_pTexUp = NULL;
		return;
	}
	_ResourceManager& res = _ResourceManager::getInstance();

	_Texture2D* pResource = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, str_background) );

	if ( pResource )	m_pTexUp = pResource;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelButton::setDown( char * str_background )	{
	if ( str_background == NULL )		{
		m_pTexDown = NULL;
		return;
	}
	_ResourceManager& res = _ResourceManager::getInstance();

	_Texture2D* pResource = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, str_background) );

	if ( pResource )	m_pTexDown = pResource;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/
void PanelButton::setOver( char * str_background )	{
	if ( str_background == NULL )		{
		m_pTexOver = NULL;
		return;
	}
	_ResourceManager& res = _ResourceManager::getInstance();

	_Texture2D* pResource = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, str_background) );

	if ( pResource )	m_pTexOver = pResource;
}
/*----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------*/




