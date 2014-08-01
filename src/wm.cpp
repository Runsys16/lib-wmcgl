#include <stdlib.h>
#include <stdio.h>
#include <iostream>



#include "wm.h"
//#include "../include/WindowsManager.h"
#include "Font.h"
#include "InternalFonts.h"


#define DEBUG_CONST
//#define DEBUG

#ifdef DEBUG_WM
#	define DEBUG
#endif


#ifdef DEBUG
#	define DEBUG_CONST
#endif

using namespace std;

//void * cTextObj;


WindowsManager::WindowsManager()	{
	#ifdef DEBUG_CONST
	cout << "Constructeur WindowsManager ..." << endl;
	#endif
	init();
	//Font * 	font = new Font();
};


WindowsManager::WindowsManager( int w, int h )	{
	#ifdef DEBUG_CONST
	cout << "Constructeur WindowsManager( w,  h ) ..." << endl;
	#endif
	setScreenSize( w, h );
	init();
}

WindowsManager::~WindowsManager()	{
	#ifdef DEBUG_CONST
	cout << "Destructeur WindowsManager ..." << endl;
	#endif
};




void WindowsManager::init()	{
	#ifdef DEBUG_CONST
	cout << "WindowsManager::init()" << endl;
	#endif
	xm_old = -1;	
	ym_old = -1;	
	
	fonts = new Font();

	TwGenerateDefaultFonts();
	cTextObj = textUtil.NewTextObj();
	
	
	if ( g_DefaultNormalFont == NULL )	{
		cout << "ERREUR : Font null ..." << endl;
	}


	str[0] = "Bonjour c'est un essai de texte";
	str[1] = "Sur 2 lignes ...";
	str[2] = "Bonjour c'est un essai de texte";
	str[3] = "Sur 4 lignes ...";
	str[4] = "Bonjour c'est un essai de texte";
	str[5] = "Sur 6 lignes ...";
	str[6] = "Bonjour c'est un essai de texte";
	str[7] = "Sur 8 lignes ...";
	str[8] = "Bonjour c'est un essai de texte";
	str[9] = "Sur 10 lignes ...";

//void TextUtil::BuildText(void *_TextObj, const std::string *_TextLines, color32 *_LineColors, color32 *_LineBgColors, int _NbLines, const CTexFont *_Font, int _Sep, int _BgWidth)
	//twFont.BuildText( cTextObj, &str, 0xffffffff, 0xffffffff, 1,  g_DefaultNormalFont, int _Sep, int _BgWidth)
	color32 color		= COLOR32_WHITE;
	color32 color_bg	= COLOR32_WHITE;
	textUtil.BuildText( cTextObj, str, &color, &color_bg, 10,  g_DefaultNormalFont, 2, 100);



}




void WindowsManager::setScreenSize( int w, int h )	{
	#ifdef DEBUG
	cout << "WindowsManager::setScreenSize( " << w << ", " << h << " );" << endl;
	#endif
	width = w;
	height = h;
}


void WindowsManager::add( Panel * p )	{
	#ifdef DEBUG
	cout << "WindowsManager::add() " << endl;
	#endif
	
	if ( p->getID() == -1 )	p->setID( getFreeID() );
	
	childs.push_back( p );
	p->setParent( NULL );
}


void WindowsManager::sup( Panel * p )	{
	#ifdef DEBUG
	cout << "WindowsManager::sup() " << endl;
	#endif

	int nb = childs.size();
	int id = p->getID();
	
	for ( int i=0; i<nb; i++ )	{
		if ( childs[i]->getID() == id )	{
			childs.erase( childs.begin()+i );
			break;
		}
	}
}


void WindowsManager::supByID( int id )	{
	#ifdef DEBUG
	cout << "WindowsManager::supByID( " << id << " )" << endl;
	#endif

	int nb = childs.size();
	
	for ( int i=0; i<nb; i++ )	{
		if ( childs[i]->getID() == id )	{
			childs.erase( childs.begin()+i );
			#ifdef DEBUG
			cout << "Found ..." << endl;
			#endif
			break;
		}
	}
}


int WindowsManager::getFreeID()	{
	#ifdef DEBUG
	cout << "WindowsManager::getFreeID() : ";// << endl;
	#endif
	
	int nb = childs.size();
	int i;
	for ( i=0; i<nb; i++ )	{
		bool bID = true;
		for ( int j=0; j<nb; j++ )	{
			if ( i == childs[j]->getID() )	{
				bID = false;
			}
		}
		
		if ( bID )	{
			break;
		}
	}
	
	#ifdef DEBUG
	cout << i << endl;
	#endif
	return i;
}





Panel * WindowsManager::findPanelMouseOver( int xm, int ym)	{
	int nb = childs.size();
	
	for ( int i=nb-1; i>=0; i-- )	{
		if ( childs[i]->isMouseOver( xm, ym ) )	{
			return childs[i];
		}
	}
	
	return NULL;
}


void WindowsManager::movePanel( int xm, int ym)	{
	#ifdef DEBUG
	cout << "WindowsManager::movePanel( " << xm << ", " << ym << " )" << endl;
	#endif
	Panel * p;
	
	p = findPanelMouseOver( xm, ym );
	if ( p != NULL )	{
		#ifdef DEBUG
		cout << "WindowsManager::Mouse Over Panel.ID = " << p->getID() << endl;
		#endif
		sup( p );
		add( p );
		
		if ( xm_old != -1 && ym_old != -1 )	{
			int dx = xm - xm_old;
			int dy = ym - ym_old;
			
			p->setPosX( p->getPosX() + dx );
			p->setPosY( p->getPosY() + dy );
		}
		xm_old = xm;
		ym_old = ym;
	}
}



void WindowsManager::swapVisible()	{
	int nb = childs.size();
	for (int i=0; i< nb; i++ )	{
		childs[i]->setVisible( !childs[i]->getVisible() );
	}
}


/*
void	WindowsManager::setWidth( int w)				{width=w;}
int		WindowsManager::getWidth()						{return width;}
void	WindowsManager::setHeight( int h )				{height=h;}
int		WindowsManager::getHeight()						{return height;}
Font*	WindowsManager::getFonts()						{return fonts;}
int		WindowsManager::getOffsetX()					{return 0;}
int		WindowsManager::getOffsetY()					{return 0;}
*/




//------------------------------------------------------------
//               opengl function	s
//------------------------------------------------------------

void WindowsManager::idleGL()	{
	//cout << "WindowsManager::idleGL()" << endl;
	int nb = childs.size();
	
	for( int i=0; i<nb; i++ )	{
		childs[i]->updatePos();
	}


}



void WindowsManager::displayGL()	{
	//cout << "WindowsManager::displayGL()" << endl;
	/*
	int min_filter;
	int mag_filter;
	int wrap_s;
	int wrap_t;
	*/
	
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);		//Color.black.bind();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*

	min_filter	= glGetTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER);		
	mag_filter	= glGetTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER);		
	wrap_s		= glGetTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S);
	wrap_t		= glGetTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	*/

	int nb = childs.size();
	
	for( int i=0; i<nb; i++ )	{
		childs[i]->displayGL();
	}
	
	//fonts->print( Font::UBUNTU_B, 50, 500, (string)"Essai numero 1" );
	//fonts->print( Font::UBUNTU_B, 50, 530, (string)"Essai numero 2" );

	#ifdef DEBUG
	cout << "WindowsManager::displayGL()  Draw Texte" << endl;
	#endif
	/*
	textUtil.BeginGL();
	int iTex = textUtil.BindFont( g_DefaultNormalFont );
	textUtil.DrawText( cTextObj, 100, 100, COLOR32_WHITE, COLOR32_GREEN );
	textUtil.UnbindFont( iTex );
	textUtil.EndGL();
	*/
	/*
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);		

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	*/
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);		//Color.black.bind();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}


void WindowsManager::clearBufferGL()	{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
	
void WindowsManager::clearBufferGL( GLbitfield bitField )	{
	glClear(bitField);
}


//------------------------------------------------------------
//               glut callback function	
//------------------------------------------------------------
void WindowsManager::passiveMotionFunc(int x, int y)	{
	//cout << "WindowsManager::passiveMotionFunc( " << x << ", " << y << " )" << endl;
	xm_old = -1;
	ym_old = -1;
}

void WindowsManager::motionFunc(int x, int y)	{
	//cout << "WindowsManager::motionFunc( " << x << ", " << y << " )" << endl;
	movePanel( x, y );
}

void WindowsManager::mouseFunc(int button, int state, int x, int y)	{
	//cout << "WindowsManager::mouseFunc( " << button << ", " << state << ", " << x << ", " << y << " )" << endl;
	
	if ( button == 2 && state == 1 )	{
		swapVisible();
	}
}



void WindowsManager::keyboardFunc( unsigned char key, int x, int y)	{
	//cout << "WindowsManager::keyboardFunc( " << key << ", " << x << ", " << y << " )" << endl;
}

void WindowsManager::keyboardUpFunc( unsigned char key, int x, int y)	{
	//cout << "WindowsManager::keyboardUpFunc( " << key << ", " << x << ", " << y << " )" << endl;
}



void WindowsManager::keyboardSpecialFunc( unsigned char key, int x, int y)	{
	//cout << "WindowsManager::keyboardSpecialFunc( " << key << ", " << x << ", " << y << " )" << endl;
}

void WindowsManager::keyboardSpecialUpFunc( unsigned char key, int x, int y)	{
	//cout << "WindowsManager::keyboardSpecialUpFunc( " << key << ", " << x << ", " << y << " )" << endl;
}
//------------------------------------------------------------



