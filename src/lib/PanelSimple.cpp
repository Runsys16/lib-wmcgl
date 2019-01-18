#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "PanelSimple.h"
#include "PanelText.h"
#include "ResourceManager.h"
#include "wm.h"
//#include "../include/WindowsManager.h"


#define DEBUG
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

PanelSimple::PanelSimple()	{	
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelSimple ..." << endl;
	#endif

    m_pTexBackground = NULL;
	/*
	_ResourceManager& res = _ResourceManager::getInstance();
	m_pTexBackground = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, "background.tga") );
	textUtil = WindowsManager::getInstance().getTextUtil();

	/*
	for ( int i=0; i<NB; i++ )	{
		//char * cStr = new char[255];
		//sprintf( cStr, "%d - Panel ... etc ...", i );
		tab[i] = NULL;
	}

	
	if ( DefaultNormalFont == NULL )	{
		cout << "ERREUR : Font null ..." << endl;
	}
	*/
	
	//bTextOK = false;
	
	pPsDebug = NULL;
	pPtDebug = NULL;
	bDebug = false;
}

/*
void PanelSimple::buildText()	{
	cTextObj = textUtil->NewTextObj();
	char cStr[255];
	sprintf( cStr, "--- ID %d ---", getID() );
	//str = new str::string()[30];
	str.push_back( cStr );
	str.push_back( cStr );
	str.push_back( cStr );
	str.push_back( cStr );

	str[1] = "essai de string dynamqiue de tres lognue string ...";
	str[2] = "TEST TEST ...";
	str[3] = "Ca fonctionne ...";

	color32 color		= COLOR32_WHITE;
	color32 color_bg	= COLOR32_WHITE;
	textUtil->GenFont( DefaultNormalFont );
	textUtil->BuildText( cTextObj, &str[0], &color, &color_bg, str.size(),  DefaultNormalFont, 10, 0);

	bTextOK = true;
}
*/

void PanelSimple::updatePos() {
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





void PanelSimple::displayGL() {
	//if (texBackground == null)			return;
	//cout << "PanelSimple::displayGL()" << endl;
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



void PanelSimple::debug( bool b )	{
	bDebug = b;
	
	
	if ( b && pPsDebug == NULL )	{
		pPsDebug = new PanelSimple();
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






void PanelSimple::setBackground( char * str_background )	{
	if ( str_background == NULL )		{
		m_pTexBackground = NULL;
		return;
	}
	_ResourceManager& res = _ResourceManager::getInstance();

	_Texture2D* ret = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, str_background) );

	if ( ret )
	    m_pTexBackground = ret;
	else
		std::cout << "  [Error] WM - PanelSimple::setBackground( \""<< str_background <<"\" )"<< std::endl;
}



void PanelSimple::setBackground( GLubyte* ptr, unsigned int w, unsigned int h, unsigned int d  )	{
    m_pTexBackground = NULL;

	m_pTexBackground = new _Texture2D();
	m_pTexBackground->Load( ptr, w, h, d );

}

void PanelSimple::deleteBackground()	{
	_ResourceManager& res = _ResourceManager::getInstance();
    
    if ( m_pTexBackground == NULL )         return;

	if (!res.Delete((void *)m_pTexBackground))
	{
		std::cout << "  [Error] WM - PanelSimple::deletetBackground"<< std::endl;
	    delete m_pTexBackground;
	    m_pTexBackground = NULL;
	}

    m_pTexBackground = NULL;
}




