#define WM_CPP

#include <stdlib.h>
#include <stdio.h>
#include <iostream>



#include "wm.h"
//#include "../include/WindowsManager.h"
#include "Font.h"
#include "InternalFonts.h"


//#define DEBUG_CONST
//#define DEBUG
//#define DEBUG_DISPLAY

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
	// texture slot pour textutil
	slot = 0;
	
	xm_old = -1;	
	ym_old = -1;	
	
	fonts = new _Font();

	wmcglGenerateDefaultFonts();
	cTextObj = textUtil.NewTextObj();
	
	
	if ( DefaultNormalFont == NULL )	{
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
	//twFont.BuildText( cTextObj, &str, 0xffffffff, 0xffffffff, 1,  DefaultNormalFont, int _Sep, int _BgWidth)
	color32 color		= COLOR32_WHITE;
	color32 color_bg	= COLOR32_WHITE;
	//textUtil.BuildText( cTextObj, str, &color, &color_bg, 10,  DefaultNormalFont, 2, 100);


	panelMove = NULL;
	panelFocus = NULL;
	bDebug = false;
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


void WindowsManager::onTop( Panel * p )	{
	#ifdef DEBUG
	cout << "WindowsManager::add() " << endl;
	#endif
	
    if (p)    {
        sup(p);
        add(p);
    }
}


void WindowsManager::onBottom( Panel * p )	{
	#ifdef DEBUG
	cout << "WindowsManager::sup() " << endl;
	#endif

    if (p)    {
        sup(p);
        childs.insert( childs.begin(), p );
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


Panel* WindowsManager::getByID( int id )	{
	#ifdef DEBUG
	cout << "WindowsManager::getByID( " << id << " )" << endl;
	#endif

	int nb = childs.size();
	
	for ( int i=0; i<nb; i++ )	{
		if ( childs[i]->getID() == id )	{
			return childs[i];
		}
	}
	return NULL;
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





Panel * WindowsManager::getParentRoot( Panel* pChild)	{
	if ( pChild == NULL )			return NULL;
	Panel * pParent = pChild->getParent();
	if ( pParent )	{
		while( pParent->getParent() != NULL )	pParent = pParent->getParent();
		return pParent;
	}
	return pChild;
	
}


void WindowsManager::changeFocus( Panel* p )	{
	//cout << "WindowsManager::passiveMotionFunc( " << x << ", " << y << " )" << endl;
	if ( p != panelFocus )	{
		if ( p )				p->haveFocus();
		if (panelFocus )		panelFocus->lostFocus();
	}
	panelFocus = p;
}



Panel * WindowsManager::findPanelMouseOver( int xm, int ym)	{
	#ifdef DEBUG
	cout << "WindowsManager::findPanelMouseOver( " << xm << ", " << ym << " )";
	#endif
	#ifdef DEBUG
	#endif

	int nb = childs.size();
	
	for ( int i=nb-1; i>=0; i-- )	{
		Panel * p = childs[i]->isMouseOver( xm, ym );
		if ( p != NULL )	{
			if ( p->getID() != 999 )	{
				#ifdef DEBUG
				cout << " Panel ID : "<< p->getID()  << endl;
				#endif
				return p;
			}
		}
	}
	
	#ifdef DEBUG
	cout << " - Panel ID : NULL" << endl;
	#endif
	return NULL;
}



int WindowsManager::getOrder( Panel* p )	{
	int nb = childs.size();
	
	for ( int i=nb-1; i>=0; i-- )	{
		if ( childs[i] = p )		return i;
	}
	
	return -1;
}



void WindowsManager::movePanel( int xm, int ym)	{
	#ifdef DEBUG
	cout << "WindowsManager::movePanel( " << xm << ", " << ym << " )" << endl;
	#endif
	Panel * p;
	
	p = findPanelMouseOver( xm, ym );
	if ( p ) {
		p = getParentRoot( p );
		if ( p )	movePanel( xm, ym, p );
	}
}


void WindowsManager::movePanel( int xm, int ym, Panel* p )	{
	#ifdef DEBUG
	cout << "WindowsManager::movePanel( " << xm << ", " << ym << " )" << endl;
	#endif

	if ( p != NULL )	{
	    if ( !p->getCanMove() )     return;
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




void WindowsManager::debug( bool b )	{
	bDebug = b;
	int nb = childs.size();
	for (int i=0; i< nb; i++ )	{
		childs[i]->debug( b );
	}
}


void WindowsManager::debug()	{
	bDebug = !bDebug;
	int nb = childs.size();
	for (int i=0; i< nb; i++ )	{
		childs[i]->debug( bDebug );
	}
}




void WindowsManager::call_back_keyboard( Panel * p )	{
	panels_cbKey.push_back(p);
}




//------------------------------------------------------------
//               opengl function	s
//------------------------------------------------------------

void WindowsManager::idleGL(float elapsedTime)	{
	//cout << "WindowsManager::idleGL()" << endl;
	int nb = childs.size();
	
	for( int i=0; i<nb; i++ )	{
		childs[i]->idle( elapsedTime );
	}

	for( int i=0; i<nb; i++ )	{
		childs[i]->updatePos();
	}
}


void WindowsManager::ChangeViewport(int _X0, int _Y0, int _Width, int _Height, int _OffsetX, int _OffsetY)	{
	//return;
    if( _Width>0 && _Height>0 )
    {
        GLint vp[4];
        vp[0] = _X0;
        vp[1] = _Y0;
        vp[2] = _Width-1;
        vp[3] = _Height-1;
        //glViewport(vp[0], m_WndHeight-vp[1]-vp[3], vp[2], vp[3]);

        GLint matrixMode = 0;
        glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(_OffsetX, _OffsetX+vp[2], vp[3]-_OffsetY, -_OffsetY, -1, 1);
        glMatrixMode(matrixMode);
    }
}



void WindowsManager::displayGL()	{
	//cout << "WindowsManager::displayGL()" << endl;
	//return;	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	ChangeViewport( 0, 0, width, height, 0, 0 );

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);		//Color.black.bind();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int nb = childs.size();
	
	for( int i=0; i<nb; i++ )	{
		childs[i]->displayGL();
	}
	
	#ifdef DEBUG_DISPLAY
	cout << "WindowsManager::displayGL()  Draw Texte" << endl;
	#endif

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
	Panel * p = findPanelMouseOver(x, y);
	
	changeFocus( p );
	
	xm_old = -1;
	ym_old = -1;
}

void WindowsManager::motionFunc(int x, int y)	{
	//cout << "WindowsManager::motionFunc( " << x << ", " << y << " )" << endl;
	if ( bMovePanel && panelMove != NULL )	{
		movePanel( x, y, panelMove );
	}
}

static bool bClickLeft=false;

void WindowsManager::mouseFunc(int button, int state, int x, int y)	{
	#ifdef DEBUG
	cout << "WindowsManager::mouseFunc( " << button << ", " << state << ", " << x << ", " << y << " )" << endl;
	#endif
	Panel* p = findPanelMouseOver(x, y);
	changeFocus( p );
	bMovePanel = false;
	
	if ( button == 2 && state == 0 )	{
		panelMove = getParentRoot( p );
		if ( panelMove != NULL )	{
			bMovePanel = true;
			sup( panelMove );
			add( panelMove );
		}
		//swapVisible();
	}
	else if ( button == 2 && state == 1 )	{
		panelMove = NULL;
		xm_old = -1;
		ym_old = -1;
	}
	else if ( button == 0 && state == 1  && bClickLeft )	{
		xm_old = -1;
		ym_old = -1;
		if ( panelFocus )			panelFocus->releaseLeft( x, y );
	}
	else if ( button == 0 && state == 0 )	{
		Panel * pFocusParent = getParentRoot( p );
		if ( pFocusParent )	{
			sup( pFocusParent );
			add( pFocusParent );
		}
		bClickLeft = true;
		if ( panelFocus )			panelFocus->clickLeft( x, y );
	}
	else if ( button == 3 && state == 0 )	{
		if ( p )	p->clickUp( x, y );
	/*
		Panel * pFocusParent = getParentRoot( p );
		bClickLeft = true;
		if ( pFocusParent )			pFocusParent->clickUp( x, y );
	*/
	}
	else if ( button == 4 && state == 0 )	{
		if ( p )	p->clickDown( x, y );
	/*
		Panel * pFocusParent = getParentRoot( p );
		bClickLeft = true;
		if ( pFocusParent )			pFocusParent->clickDown( x, y );
	*/
	}
	
	int ID = -1;
	if ( panelMove )			ID = panelMove->getID();
	
	#ifdef DEBUG
	cout << "WindowsManager::mouseFunc Addr : " << panelMove <<" ID "<< ID <<", " << bMovePanel << endl;;
	#endif

}



void WindowsManager::keyboardFunc( unsigned char key, int x, int y)	{
	#ifdef DEBUG
	cout << "WindowsManager::keyboardFunc( " << (int)key << ", " << x << ", " << y << " )" << endl;
	#endif
	int nb = panels_cbKey.size();
	for( int i=0; i<nb; i++ )	{
		if ( panels_cbKey[i]->getVisible() )
			panels_cbKey[i]->cb_keyboard( key );
	}
}

void WindowsManager::keyboardUpFunc( unsigned char key, int x, int y)	{
	#ifdef DEBUG
	cout << "WindowsManager::keyboardUpFunc( " << (int)key << ", " << x << ", " << y << " )" << endl;
	#endif
}



void WindowsManager::keyboardSpecialFunc( unsigned char key, int x, int y)	{
	#ifdef DEBUG
	cout << "WindowsManager::keyboardSpecialFunc( " << (int)key << ", " << x << ", " << y << " )" << endl;
	#endif
	int nb = panels_cbKey.size();
	for( int i=0; i<nb; i++ )	{
		if ( panels_cbKey[i]->getVisible() )
			panels_cbKey[i]->cb_keyboard_special( key );
	}
}

void WindowsManager::keyboardSpecialUpFunc( unsigned char key, int x, int y)	{
	#ifdef DEBUG
	cout << "WindowsManager::keyboardSpecialUpFunc( " << (int)key << ", " << x << ", " << y << " )" << endl;
	#endif
	int nb = panels_cbKey.size();
	for( int i=0; i<nb; i++ )	{
		if ( panels_cbKey[i]->getVisible() )
			panels_cbKey[i]->cb_keyboard_special_up( key );
	}
}

static GLubyte* WindowsManager::OpenImage( const std::string& filename, unsigned int& w, unsigned int& h, unsigned int& d)
{
return _ImageTools::OpenImage(filename,w,h,d);
    
}
	
//------------------------------------------------------------



