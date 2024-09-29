#define WM_CPP

#include <stdlib.h>
#include <stdio.h>
#include <iostream>



#include "wm.h"
//#include "../include/WindowsManager.h"
#include "Font.h"
#include "InternalFonts.h"
#include "ResourceManager.h"
#include "PanelEditText.h"
#include "PanelConsole.h"


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


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
WindowsManager::WindowsManager()	{
	#ifdef DEBUG_CONST
	cout << "Constructeur WindowsManager ..." << endl;
	#endif
	init();
	//Font * 	font = new Font();
};
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
WindowsManager::WindowsManager( int w, int h )	{
	#ifdef DEBUG_CONST
	cout << "Constructeur WindowsManager( w,  h ) ..." << endl;
	#endif
	setScreenSize( w, h );
	init();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
WindowsManager::~WindowsManager()	{
	#ifdef DEBUG_CONST
	cout << "Destructeur WindowsManager ..." << endl;
	#endif
};
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
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
	textUtil.setTabSize(40);
	
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


	panelMove           = NULL;
	panelCapture        = NULL;
	panelFocus          = NULL;
	panelResize         = NULL;
    panelMotionMiddle   = NULL;
    panelMotionLeft     = NULL;
    panelMotionRight     = NULL;

	bDebug        = false;
	bStopKeyboard = false;
	bResize       = false;
	bMotionMiddle = false;
	bMotionLeft   = false;

	panelCallBackKeys.clear();
	
	sTab = "";
	nb_tab = 0;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::log_tab( bool b)
{
    if ( b )            nb_tab++;
    else                nb_tab--;
    if ( nb_tab<0 )     nb_tab = 0;
    
    sTab = "";
    for( int i=0; i<nb_tab; i++ )
    {
        sTab = sTab + "|  ";
    }
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::log( char* chaine )
{
    string aff = sTab + string(chaine);
    
    printf( "libwmcgl : %s\n", aff.c_str() );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::logf(char *fmt, ...)
{
    char chaine[255];
    va_list arglist;

    va_start( arglist, fmt );
    vsprintf( chaine, fmt, arglist );
    va_end( arglist );
    
    log((char*)chaine);
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::setScreenSize( int w, int h )	{
	#ifdef DEBUG
	cout << "WindowsManager::setScreenSize( " << w << ", " << h << " );" << endl;
	#endif
	width = w;
	height = h;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::add( Panel * p )	{
	#ifdef DEBUG
	cout << "WindowsManager::add() ID="<< p->getID() <<" nb="<< childs.size() << endl;
	#endif
	//cout << "WindowsManager::add() "<< p->getID() << endl;
	
	if ( p->getID() == -1 )	p->setID( getFreeID() );
	
	childs.push_back( p );
	p->setParent( NULL );

	#ifdef DEBUG
	int nb = childs.size();
	for ( int i=0; i<nb; i++ )	{
		cout << i <<" - "<< childs[i]->getID() << endl;
	}


	//panelCapture = p;
	cout << "WindowsManager::add() nb="<< childs.size() << endl;
	#endif
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::supCapture( Panel * p )	{
//#define DEBUG
	#ifdef DEBUG
	cout << "WindowsManager::supCapture() ID="<< p->getID() <<" nb="<< childs.size()  << endl;
	#endif

    if ( panelCapture == p  ){
        changeCapture(NULL); 
        return;
    }

    std::vector<Panel*>& childs = p->getChilds();
    
    if ( p != NULL )
    {
        for( int i=0; i<childs.size(); i++ )
        {
            Panel* child = childs[i];
            if ( panelCapture == child  ){
                changeCapture(NULL); 
                return;
            }
        }
    }
//#undef DEBUG    
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::supFocus( Panel * p )	{
//#define DEBUG
	#ifdef DEBUG
	cout << "WindowsManager::supFocus() ID="<< p->getID() <<" nb="<< childs.size()  << endl;
	#endif

    if ( panelFocus == p  ){
        changeFocus(NULL); 
        return;
    }

    std::vector<Panel*>& childs = p->getChilds();
    
    if ( p != NULL )
    {
        for( int i=0; i<childs.size(); i++ )
        {
            Panel* child = childs[i];
            if ( panelFocus == child  ){
                changeFocus(NULL); 
                return;
            }
        }
    }
//#undef DEBUG    
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::sup( Panel * p )	{
//#define DEBUG
	#ifdef DEBUG
	cout << "WindowsManager::sup() ID="<< p->getID() <<" nb="<< childs.size()  << endl;
	#endif

    supCapture(p);
    supFocus(p);

	int nb = childs.size();
	int id = p->getID();
	
	for ( int i=0; i<nb; i++ )	{
		if ( childs[i]->getID() == id )	{
			childs.erase( childs.begin()+i );
			break;
		}
	}

	#ifdef DEBUG
	cout << "WindowsManager::sup() nb="<< childs.size()  << endl;
	#endif
//#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::onTop( Panel * p )	{
	#ifdef DEBUG
	//cout << "WindowsManager::onTop() ID="<< p->getID() <<" nb="<< childs.size() << endl;
	#endif
	
	int nb = childs.size();
	int id = p->getID();
	
	for ( int i=0; i<nb; i++ )	{
		if ( childs[i]->getID() == id )	{
			childs.erase( childs.begin()+i );
			childs.push_back(p);
			break;
		}
	}

	#ifdef DEBUG
	//cout << "WindowsManager::onTop() nb="<< childs.size() << endl;
	#endif
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::onBottom( Panel * p )	{
	#ifdef DEBUG
	cout << "WindowsManager::onBottom() ID="<< p->getID() <<" nb="<< childs.size() << endl;
	#endif
	Panel* pC_SVG;
	Panel* pF_SVG;
    
    pC_SVG = panelCapture;
    pF_SVG = panelFocus;
    
    if (p)    {
        sup(p);
        childs.insert( childs.begin(), p );
    }
    
    panelCapture = pC_SVG;
    panelFocus   = pF_SVG;
	#ifdef DEBUG
	cout << "WindowsManager::onBottom() nb="<< childs.size() << endl;
	#endif
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Panel * WindowsManager::getParentRoot( Panel* pChild)	{
	if ( pChild == NULL )			return NULL;
	Panel * pParent = pChild->getParent();
	if ( pParent )	{
		while( pParent->getParent() != NULL )	pParent = pParent->getParent();
		return pParent;
	}
	return pChild;
	
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::changeCapture( Panel* p )	{
//#define DEBUG
    #ifdef DEBUG
	cout << "WindowsManager::changeCapture( " << (p!=NULL?p->getID():0) << " )" << endl;
	#endif
	if ( panelCapture!=NULL )	    panelCapture->lostCapture();

	panelCapture = p;
	if ( panelCapture!=NULL)        panelCapture->haveCapture();
	
//#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::changeFocus( Panel* p )	{
//#define DEBUG
    #ifdef DEBUG
	cout << "WindowsManager::changeFocus( " << (p!=NULL?p->getID():0) << " )" << endl;
	#endif
    if ( panelFocus )       panelFocus->lostFocus();

	panelFocus = p;
	if ( panelFocus )       panelFocus->haveFocus();
//#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Panel * WindowsManager::findPanelMouseOver( int xm, int ym)	{
//#define DEBUG
	#ifdef DEBUG
	logf( (char*)"WindowsManager::findPanelMouseOver(%d, %d)", xm, ym );
	log_tab(true);
	#endif

	int nb = childs.size();
	
	for ( int i=nb-1; i>=0; i-- )	{
		Panel* p = childs[i];
		#ifdef DEBUG
		logf( (char*)"Appel de isMouseOver childs[%d] \"%s\"  ID=%d", i, p->getExtraString().c_str(), p->getID() );
	    log_tab(true);
		#endif

		Panel* r = p->isMouseOver( xm, ym );
		if ( r && !r->isVisible() )		r = NULL;

	    #ifdef DEBUG
	    log_tab(false);
	    #endif
		if ( r != NULL )	{
			//if ( p->getID() < 9000 )	{
				#ifdef DEBUG
				logf( (char*)"OK \"%s\"  ID=%d", r->getExtraString().c_str(), r->getID() );
            	log_tab(false);
            	logf( (char*)"WindowsManager::findPanelMouseOver(%d, %d)", xm, ym );
				#endif
				return r;
			//}
		}

		#ifdef DEBUG
		logf( (char*)"NOK childs[%d] \"%s\"  ID=%d", i, p->getExtraString().c_str(), p->getID() );
		#endif
	}
	
	#ifdef DEBUG
    logf( (char*)"NOK");
    log_tab(false);
	logf( (char*)"WindowsManager::findPanelMouseOver(%d, %d)", xm, ym );
	#endif
	return NULL;
#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
int WindowsManager::getOrder( Panel* p )	{
	int nb = childs.size();
	//cout << "WindowsManager::getOrder() nb="<< nb << endl;
	
	for ( int i=0; i<nb; i++ )	{
		if ( childs[i] == p )		return i;
	}
	
	return -1;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::movePanel( int xm, int ym, Panel* p )	{
	#ifdef DEBUG
	cout << "WindowsManager::movePanel( " << xm << ", " << ym << " )" << endl;
	#endif

	if ( p != NULL )	{
	    if ( !p->getCanMove() )     return;
		#ifdef DEBUG
		cout << "WindowsManager::Mouse Over Panel.ID = " << p->getID() << endl;
		#endif
		onTop( p );
		
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
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::swapVisible()	{
	int nb = childs.size();
	for (int i=0; i< nb; i++ )	{
		childs[i]->setVisible( !childs[i]->getVisible() );
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::debug( bool b )	{
	bDebug = b;
	int nb = childs.size();
	for (int i=0; i< nb; i++ )	{
		childs[i]->debug( b );
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::debug()	{
	bDebug = !bDebug;
	int nb = childs.size();
	for (int i=0; i< nb; i++ )	{
		childs[i]->debug( bDebug );
	}
}
//-----------------------------------------------
// Traitement des focntions callback pour
// la gestion des evenements souris
//-----------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::call_back_keyboard( Panel * p )	{
    //cout << "call_back_keyboard() Callback nb panel : "<< panelCallBackKeys.size() << endl;
	panelCallBackKeys.push_back(p);
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::sup_call_back_keyboard( Panel * p )	{
    //cout << "sup_call_back_keyboard() Callback nb panel : "<< panelCallBackKeys.size() << endl;
	for( int i=0; i<panelCallBackKeys.size(); i++) 
	{
	    if ( p == panelCallBackKeys[i] )
	    {
	        //cout << "sup_call_back_keyboard() panel trouve... SUPPRESSION" << endl;
	        panelCallBackKeys[i] = NULL;
	        panelCallBackKeys.erase(panelCallBackKeys.begin()+i);
	        return;
	    }
	}
    
    //cout << "sup_call_back_keyboard() [ERREUR] panel n existe pas..." << endl;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool WindowsManager::is_call_back_keyboard( Panel * p )	{
    //cout << "is_call_back_keyboard() Callback nb panel : "<< panelCallBackKeys.size() << endl;
	for( int i=0; i<panelCallBackKeys.size(); i++) 
	{
	    if ( p == panelCallBackKeys[i] )
	    {
	        //cout << "panel trouve..." << endl;
	        return true;
	    }
	}
    
    //cout << "is_call_back_keyboard() [ERREUR] panel non trouve..." << endl;
    return false;
}
//------------------------------------------------------------
//               opengl function	s
//------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::clearBufferGL()	{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::clearBufferGL( GLbitfield bitField )	{
	glClear(bitField);
}
//------------------------------------------------------------
//               glut callback function	
//------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::passiveMotionFunc(int x, int y)	{
//#define DEBUG
    mouseX = x;
    mouseY = y;
	#ifdef DEBUG
    logf( (char*)"WindowsManager::passiveMotionFunc(%d, %d)", x, y );
    //log_tab(true);
    #endif
//#undef DEBUG
	Panel * p = findPanelMouseOver(x, y);
	
	if ( p )    {
		changeCapture( p );
		p->passiveMotionFunc( x, y);
	}
	
	xm_old = -1;
	ym_old = -1;

    bool bOver = false;

	p = findPanelMouseOver(x, y);
	p = getParentRoot( p );
	
	panelResize = NULL;
	
    if ( p != NULL )
    {
	    Panel * pp = p->isMouseOverBorder( x, y );
	    if ( pp != NULL )	
	    {
		    //cout << " Resize  Panel ID : "<< pp->getID()  << endl;
		    bOver = true;
        	panelResize = pp;
        }
    }

	if ( !bOver )   	            glutSetCursor(0);
	
	
    log_tab(false);
	#ifdef DEBUG
    if ( p != NULL )
        logf( (char*)"WindowsManager::passiveMotionFunc() sur ID=%d \"%s\"",p->getID(), p->getExtraString().c_str() );
    else
        logf( (char*)"WindowsManager::passiveMotionFunc() NULL" );
    #endif
//#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::motionFunc(int x, int y)	{
//#define DEBUG
    mouseX = x;
    mouseY = y;
#ifdef DEBUG
    if ( panelMove != NULL )
    	cout << "WindowsManager::motionFunc( " << x << ", " << y << " )  ID "<< panelMove->getID() << endl;
	else
    	cout << "WindowsManager::motionFunc( " << x << ", " << y << " )  panelMove NULL " << endl;
#endif
//#undef DEBUG
    	
	if ( bMovePanel && panelMove != NULL )	{
    	#ifdef DEBUG
    	cout << "WindowsManager::motionFunc( " << x << ", " << y << " )  ID "<< panelMove->getID() << endl;
        #endif

		movePanel( x, y, panelMove );
		panelCapture = panelMove;
	}
	else
	if( bResize && panelResize != NULL)
	{
    	#ifdef DEBUG
    	cout << "WindowsManager::motionFunc( " << x << ", " << y << " )" << endl;
    	cout << "   ID = " << panelResize->getID() << endl;
    	cout << "   getMouseOverBorder=" << panelResize->getMouseOverBorder() << endl;
        #endif
        
                
        int xx   = panelResize->getPosX();
        int yy   = panelResize->getPosY();
        int dxx  = panelResize->getPosDX();
        int dyy  = panelResize->getPosDY();
        int delta;
        
    	switch( panelResize->getMouseOverBorder() )
    	{
    	    case MOB_UPPER_LEFT:
    	        delta = x - xm_old;
    	        xx   += delta;
    	        dxx  -= delta;
    	        panelResize->setPosX(xx);
    	        panelResize->setPosDX(dxx);
    	        delta = y - ym_old;
    	        yy   += delta;
    	        dyy  -= delta;
    	        panelResize->setPosY(yy);
    	        panelResize->setPosDY(dyy);
    	        break; 
    	    case MOB_UPPER_RIGHT:
    	        delta = x - xm_old;
    	        dxx  -= -delta;
    	        panelResize->setPosDX(dxx);
    	        delta = y - ym_old;
    	        yy   += delta;
    	        dyy  -= delta;
    	        panelResize->setPosY(yy);
    	        panelResize->setPosDY(dyy);
    	        break; 
    	    case MOB_BOTTOM_LEFT:
    	        delta = x - xm_old;
    	        xx   += delta;
    	        dxx  -= delta;
    	        panelResize->setPosX(xx);
    	        panelResize->setPosDX(dxx);
    	        delta = y - ym_old;
    	        yy   += -delta;
    	        dyy  -= -delta;
    	        //panelResize->setPosY(yy);
    	        panelResize->setPosDY(dyy);
    	        break; 
    	    case MOB_BOTTOM_RIGHT:
    	        delta = x - xm_old;
    	        xx   += -delta;
    	        dxx  -= -delta;
    	        //panelResize->setPosX(xx);
    	        panelResize->setPosDX(dxx);
    	        delta = y - ym_old;
    	        yy   += -delta;
    	        dyy  -= -delta;
    	        //panelResize->setPosY(yy);
    	        panelResize->setPosDY(dyy);
    	        break; 
    	    case MOB_UPPER:
    	        delta = y - ym_old;
    	        yy   += delta;
    	        dyy  -= delta;
    	        panelResize->setPosY(yy);
    	        panelResize->setPosDY(dyy);
    	        break; 
    	    case MOB_BOTTOM:
    	        delta = y - ym_old;
    	        dyy  += delta;
    	        panelResize->setPosDY(dyy);
    	        break; 
    	    case MOB_LEFT:
    	        delta = x - xm_old;
    	        xx   += delta;
    	        dxx  -= delta;
    	        panelResize->setPosX(xx);
    	        panelResize->setPosDX(dxx);
    	        break; 
    	    case MOB_RIGHT:
    	        delta = x - xm_old;
    	        dxx  += delta;
    	        panelResize->setPosDX(dxx);
    	        break; 
    	}
        xm_old = x;
        ym_old = y;
        //cout << "panelResize() ID = "<< panelResize->getID() << endl;
        panelResize->updatePos();
	}
	else
	if( bMotionMiddle && panelMotionMiddle != NULL)
	{
	    panelMotionMiddle->motionMiddle(x,y);
	}
	else
	if( bMotionLeft && panelMotionLeft != NULL)
	{
	    panelMotionLeft->motionLeft(x,y);
	}

	if( bMotionRight && panelMotionRight != NULL)
	{
	    panelMotionRight->motionRight(x,y);
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
//static bool bClickLeft=false;
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool WindowsManager::isPanelFocus(Panel*p)
{
    if ( typeid(*p) == typeid(PanelEditText) )      return true;
    if ( typeid(*p) == typeid(PanelConsole) )       return true;
    return false;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::mouseFunc(int button, int state, int x, int y)	{
//#define DEBUG
    mouseX = x;
    mouseY = y;
    iMouseButton[button] = state;
	#ifdef DEBUG
	logf( (char*)"WindowsManager::mouseFunc( %s, %s, x %d, y %d)", 	
									(button==0 ? "left" : (button==1 ? "middle":" right" )),
									(state==1 ? "up" : "down"), x, y );
	log_tab(true);
	//cout << "WindowsManager::mouseFunc( " << button << ", " << state << ", " << x << ", " << y << " )" << endl;
	#endif

	Panel* p = findPanelMouseOver(x, y);
	#ifdef DEBUG
	logf( (char*)"find panel \"%s\"", (p!=NULL? (char*)p->getExtraString().c_str():"NULL") );
    #endif
    	
	bMovePanel = false;

    //
    // Si appuie sur les 3 boutons (pas de wheel)
    //
	if ( button >= 0 && button <=2 && state == 0 )
	{
	    changeCapture( p );
	    //if ( isPanelFocus(p) )      changeFocus(p);
	    changeFocus(p);
	}
    //
	// bouton gauche appuye
    //
	if ( button == 0 && state == 0 )	{
    	panelMotionLeft = p;
    	bMotionLeft = true;

	    if ( panelResize != NULL )	    {
		    xm_old = x;
		    ym_old = y;
		    bResize = true;
			onTop( panelResize );
	    }
	    else	    {
		    Panel * pCaptureParent = getParentRoot( p );
		    if ( pCaptureParent )	{
			    onTop( pCaptureParent );
		    }
		}
		
	    #ifdef DEBUG
        logf( (char*)"panelCapture = \"%s\" adr=%016lX", (panelCapture!=NULL?panelCapture->getExtraString().c_str():(char*)"NULL"),
                        (unsigned long)panelCapture );
	    #endif
        if ( panelCapture ){
            panelCapture->clickLeft( x, y );
        }
	}
	// bouton gauche relache
	else if ( button == 0 && state == 1  )	{
    	bMotionLeft = false;

		xm_old = -1;
		ym_old = -1;
		
		if ( panelCapture )			panelCapture->releaseLeft( x, y );
    	panelMotionLeft = NULL;

	    if ( bResize )
	    {
	        panelResize->haveMove();
	        panelResize = NULL;
    		bResize = false;
    	    //cout << " Release Left resize "<< endl;
        }
	    #ifdef DEBUG
	    cout << "  Release gauche panelCapture="<< panelCapture << endl;
	    #endif
	}
	// bouton milieu appuye
	else 
	if ( button == 1 && state == 0 )	{
	#ifdef DEBUG
    	cout << "WindowsManager::mouseFunc  button: " << button << endl;;
	#endif
    	panelMotionMiddle = p;
    	bMotionMiddle = true;
		if ( p )	p->clickMiddle( x, y );
	}
	// bouton milieu relache
	else 
	if ( button == 1 && state == 1 )	{
	#ifdef DEBUG
    	cout << "WindowsManager::mouseFunc  button: " << button << endl;;
	#endif
		if ( panelMotionMiddle )	panelMotionMiddle->releaseMiddle( x, y );
		bMotionMiddle = false;
    	panelMotionMiddle = NULL;
	}
	else 
	// bouton droite appuye
	if ( button == 2 && state == 0 )	{
    	panelMotionRight = p;
    	bMotionRight = true;
		if ( p->getCanMove() == false )		panelMove = getParentRoot( p );
		else								panelMove = p;
		if ( panelMove != NULL )	{
			bMovePanel = true;
			onTop( panelMove );
		}
		if ( panelCapture )			panelCapture->clickRight( x, y );
		//swapVisible();
	}
	// bouton droite relache
	else if ( button == 2 && state == 1 )	{
    	panelMotionRight = NULL;
    	bMotionRight = false;
	    if ( panelMove != NULL)     panelMove->haveMove();
		panelMove = NULL;
		xm_old = -1;
		ym_old = -1;
		if ( panelCapture )			panelCapture->releaseRight( x, y );
    	//cout << " releaseRight : "<< panelCapture->getID() << endl;
	}
	else 
	if ( button == 3 && state == 0 )	{
	#ifdef DEBUG
    	cout << "WindowsManager::mouseFunc  button: " << button << endl;;
	#endif
		if ( p )	p->wheelUp( x, y );
		if ( p != NULL && p->getParent() != NULL )   p->getParent()->wheelUp( x, y );
	}
	else
	if ( button == 4 && state == 0 )	{
	#ifdef DEBUG
    	cout << "WindowsManager::mouseFunc  button: " << button << endl;;
	#endif
		if ( p )	p->wheelDown( x, y );
		if ( p != NULL && p->getParent() != NULL )   p->getParent()->wheelDown( x, y );
	}
	
	int ID = -1;
	if ( panelMove )			ID = panelMove->getID();
	
	#ifdef DEBUG
	//cout << "WindowsManager::mouseFunc Addr : " << panelMove <<" ID "<< ID <<", " << bMovePanel << endl;;
	log_tab(false);
	logf( (char*)"WindowsManager::mouseFunc()" );
	#endif
#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::keyboardFunc( unsigned char key, int x, int y)	{
//#define DEBUG
	#ifdef DEBUG
	cout << "WindowsManager::keyboardFunc( " << (int)key << ", " << x << ", " << y << " )" << endl;
	#endif
	if ( bStopKeyboard )        return;
	if ( panelFocus!= NULL )    panelFocus->cb_keyboard( key );
	return;
	
	int nb = panelCallBackKeys.size();
	for( int i=0; i<nb; i++ )	{
		if ( panelCallBackKeys[i]->isVisible() )
			panelCallBackKeys[i]->cb_keyboard( key );
	}
//#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::keyboardUpFunc( unsigned char key, int x, int y)	{
	#ifdef DEBUG
	cout << "WindowsManager::keyboardUpFunc( " << (int)key << ", " << x << ", " << y << " )" << endl;
	#endif


//	if ( panelFocus!= NULL )    panelFocus->cb_keyboard_special_up( key );
//	return;
	
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::keyboardSpecialFunc( unsigned char key, int x, int y)	{
//#define DEBUG
	#ifdef DEBUG
	cout << "WindowsManager::keyboardSpecialFunc( " << (int)key << ", " << x << ", " << y << " )" << endl;
	#endif
	if ( bStopKeyboard )        return;
	int nb = panelCallBackKeys.size();

	if ( panelFocus!= NULL )    panelFocus->cb_keyboard_special( key );
	return;
	
	#ifdef DEBUG
	cout << "WindowsManager::keyboardSpecialFunc( " << nb <<" fenetres panelCallBackKeys " << endl;
	#endif
	for( int i=0; i<nb; i++ )	{
		if ( panelCallBackKeys[i]->isVisible() )
		{
		    //if ( panelCallBackKeys[i]->cb_keyboard_special != NULL )
			panelCallBackKeys[i]->cb_keyboard_special( key );
	    }
	}
//#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::keyboardSpecialUpFunc( unsigned char key, int x, int y)	{
//#define DEBUG
	#ifdef DEBUG
	cout << "WindowsManager::keyboardSpecialUpFunc( " << (int)key << ", " << x << ", " << y << " )" << endl;
	#endif
	if ( bStopKeyboard )        return;

	if ( panelFocus!= NULL )    panelFocus->cb_keyboard_special_up( key );
	return;
	
	int nb = panelCallBackKeys.size();
	#ifdef DEBUG
	cout << "WindowsManager::keyboardSpecialFunc( " << nb <<" fenetres panelCallBackKeys " << endl;
	#endif
	for( int i=0; i<nb; i++ )	{
		if ( panelCallBackKeys[i]->isVisible() )
		{
			panelCallBackKeys[i]->cb_keyboard_special_up( key );
	    }
	}
//#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
GLubyte* WindowsManager::OpenImage( const std::string& filename, unsigned int& w, unsigned int& h, unsigned int& d)
{
    return _ImageTools::OpenImage(filename,w,h,d);
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::loadResourceImage( const std::string& filename )
{
	_ResourceManager& res = _ResourceManager::getInstance();

	res.LoadResource(_ResourceManager::TEXTURE2D, filename );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void WindowsManager::genereMipMap(bool b)
{
    _Texture::EnableGenerateMipmaps(b);
}
	
//------------------------------------------------------------



