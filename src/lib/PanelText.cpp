#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "PanelText.h"
#include "wm.h"

//#define DEBUG
//#define DEBUG_CONST

#ifdef DEBUG_WM
#	define DEBUG
#endif


#ifdef DEBUG
#	define DEBUG_CONST
#endif


using namespace std;
//--------------------------------------------------------------------------------------------------------------------
//
//						Constructeurs ....
//
//--------------------------------------------------------------------------------------------------------------------
PanelText::PanelText()	{
	init();
}
//--------------------------------------------------------
PanelText::PanelText( string str )	{
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelText( "<< str <<" )" << endl;
	#endif

	init();

	color = 0xffffffff;
	changeText( str );
}
//--------------------------------------------------------
PanelText::PanelText( string str, FONT type )	{
	typeFont = type;
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelText( "<< str <<", "<< strFont() <<" )" << endl;
	#endif

	init();

	color = 0xffffffff;

	changeText( str, type, true );
}
//--------------------------------------------------------
PanelText::PanelText( string str, FONT type, int x, int y )	{
	typeFont = type;
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelText( "<< str <<", "<< strFont() <<", "<< x <<", "<< y <<" )" << endl;
	#endif

	init();

	color = 0xffffffff;

	setPos(x, y);
	changeText( str, type );
}
//--------------------------------------------------------
PanelText::PanelText( string str, FONT type, int x, int y, unsigned int c )	{
	typeFont = type;
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelText( "<< str <<", "<< strFont() <<", "<< x <<", "<< y <<" )" << endl;
	#endif
	
	init();

	color = c;
	setPos(x, y);
	changeText( str, type );
}
//--------------------------------------------------------
PanelText::PanelText( char * cstr, FONT type, int x, int y )	{
	typeFont = type;
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelText( "<< cstr <<", "<< strFont() <<", "<< x <<", "<< y <<" )" << endl;
	#endif

	init();

	color = 0xffffffff;

	setPos(x, y);
	changeText( cstr, type );
	buildString();
}
//--------------------------------------------------------
PanelText::PanelText( char * cstr, FONT type, int x, int y, unsigned int c )	{
	typeFont = type;
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelText( "<< cstr <<", "<< strFont() <<", "<< x <<", "<< y <<" )" << endl;
	#endif

	init();

	color = c;
	
	setPos(x, y);
	changeText( cstr, type );
	buildString();
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
void PanelText::init() {
	#ifdef DEBUG_CONST
	cout << "PanelText::init() ..." << endl;
	#endif

	text		= "";
	textUtil	= WindowsManager::getInstance().getTextUtil();
	setPos( 0, 0 );
	align		= LEFT;
	pTextGL		= NULL;
	bChange		= false;
	ID			= 9000;
	tabSize		= 40;
	color		= 0xffffffff;
	alpha		= 0.0;
	//bFantome = true;
}
//--------------------------------------------------------------------------------------------------------------------
// ChangeText() functions
//--------------------------------------------------------------------------------------------------------------------
string PanelText::strFont() {
	string retString = "";
	
	switch( typeFont )	{
		case NORMAL_FONT :
			retString = "NORMAL_FONT";
			break;
		case SMALL_FONT :
			retString = "SMALL_FONT";
			break;
		case LARGE_FONT :
			retString = "LARGE_FONT";
			break;
		default:
			retString = "OTHER_FONT";
			break;
	}	
	return retString;	
}
//--------------------------------------------------------------------------------------------------------------------
// ChangeText() functions
//--------------------------------------------------------------------------------------------------------------------
void PanelText::changeText( char* cstr )	{
	#ifdef DEBUG
	cout << "PanelText::changeText( "<< cstr <<", "<< strFont() <<" )" <<  endl;
	#endif
	
	if ( text.compare(cstr) == 0 )		{
		bChange = false;
		return;
	}

	text = cstr;
	bChange = true;
}
void PanelText::changeText( char* cstr, FONT type )	{
	#ifdef DEBUG
	cout << "PanelText::changeText( "<< cstr  <<", "<< strFont() <<" )" << endl;
	#endif
	
	if ( (text.compare(cstr) == 0) && (type == typeFont) )		{
		bChange = false;
		return;
	}

	typeFont = type;
	changeText( cstr );
}
//--------------------------------------------------------
void PanelText::changeText( string str )	{
	#ifdef DEBUG
	cout << "PanelText::changeText( "<< str <<", "<< strFont() <<" )" <<  endl;
	#endif
	
	if ( text.compare(str) == 0 )		{
		bChange = false;
		return;
	}

	text = str;
	bChange = true;
}
//--------------------------------------------------------
void PanelText::changeText( string str, FONT type )	{
	#ifdef DEBUG
	cout << "PanelText::changeText( "<< str  <<", "<< strFont() <<" )" << endl;
	#endif
	
	if ( (text.compare(str) == 0) && (type == typeFont) )		{
		bChange = false;
		return;
	}

	typeFont = type;
	changeText( str );
}
//--------------------------------------------------------
void PanelText::changeText( string str, bool build )	{
	#ifdef DEBUG
	cout << "PanelText::changeText( "<< str <<", "<< strFont()  <<" )" << endl;
	#endif
	
	if ( text.compare(str) == 0 )		{
		bChange = false;
		return;
	}

	bChange = true;

	text = str;
	bChange = true;
	
	if (build)		buildString();
}
//--------------------------------------------------------
void PanelText::changeText( string str, FONT type, bool build )	{
	#ifdef DEBUG
	cout << "PanelText::changeText( \""<< str <<"\", "<< type <<" )" << endl;
	#endif
	
	if ( str.size() == 0 )	{
		text = str;
	}
	else if ( (type == typeFont) && text.compare(str) == 0 )	{
		#ifdef DEBUG
		cout << "   Identique" << endl;
		#endif
		bChange = false;
		return;
	}
	
	//cout << "PanelText::changeText( \""<< str <<"\", "<< typeFont <<" : "<< strFont() <<" )" << endl;
	typeFont = type;
	text = str;

	if (build)		buildString();
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
void PanelText::eraseText()	{
	#ifdef DEBUG
	cout << "PanelText::eraseText()"<< endl;
	#endif
	if ( pTextGL == NULL )	{
		#ifdef DEBUG
		cout << "PanelText::eraseText() text = NULL"<< endl;
		#endif
		return;
	}
	
	textUtil->DeleteTextObj( pTextGL );
	#ifdef DEBUG
	cout << "  delete pTextGL"<< endl;
	#endif
	pTextGL = NULL; 

	bChange = false;
		
	#ifdef DEBUG
	cout << "PanelText::eraseText() text = \""<< text <<"\", type = "<< strFont() << endl;
	#endif
	
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
void PanelText::buildString()	{
#ifdef DEBUG
	cout << "PanelText::buildString() font = "<< strFont() <<"  texte = \""<< text <<"\""<< endl;
#endif
	if ( text.compare("") == 0 )	{ 
		if (pTextGL != NULL )	{
			textUtil->DeleteTextObj( pTextGL );
			#ifdef DEBUG
			cout << "  delete pTextGL"<< endl;
			#endif
		}
		#ifdef DEBUG
		cout << "  pTextGL = NULL"<< endl;
		#endif
		pTextGL = NULL; 
		return; 
	}
	
	textUtil = WindowsManager::getInstance().getTextUtil();

	color32 c		= COLOR32_WHITE;
	color32 c_bg	= COLOR32_RED;

	//if (pTextGL != NULL )	textUtil->DeleteTextObj( pTextGL );
	textUtil->setTabSize( tabSize );
	pTextGL = textUtil->NewTextObj();


#ifdef DEBUG
	cout << "PanelText::buildString() \""<< text <<"\", "<< typeFont <<" : "<< strFont() <<" )" << endl;
#endif
	switch (typeFont )	{
	case NORMAL_FONT :
	    {
    	//cout << "NORMAL_FONT : "<< typeFont  << endl;
		textUtil->GenFont( DefaultNormalFont, color );
		textUtil->BuildText( pTextGL, &(text), &c, &c_bg, 1,  DefaultNormalFont, 0xffffff00, 0xff0000ff);
		}
		break;
	case SMALL_FONT :
		{
    	//cout << "SMALL_FONT : "<< typeFont << endl;
		textUtil->GenFont( DefaultSmallFont, color );
		textUtil->BuildText( pTextGL, &(text), &c, &c_bg, 1,  DefaultSmallFont, 0xffffff00, 0xff0000ff);
		}
		break;
	case LARGE_FONT :
		{
    	//cout << "LARGE_FONT : "<< typeFont << endl;
		textUtil->GenFont( DefaultLargeFont, color );
		//textUtil->BuildText( pTextGL, &(text), &c, &c_bg, 1,  DefaultLargeFont, 0xffffff00, 0xff0000ff);
		textUtil->BuildText( pTextGL, &(text), NULL, NULL, 1,  DefaultLargeFont, 0xffffff00, 0xff0000ff);
		}
		break;
	}
	bChange = false;
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
int PanelText::getTextLenght()	{
	CTexFont * defaultFont;
	switch (typeFont )	{
		case NORMAL_FONT :		defaultFont = DefaultNormalFont;		break;
		case SMALL_FONT :		defaultFont = DefaultSmallFont;			break;
		case LARGE_FONT :		defaultFont = DefaultLargeFont;			break;
	}
	return( textUtil->lenght( pTextGL, &text, defaultFont ) );
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
int PanelText::getTextLenght( int nbChar )	{
	CTexFont * defaultFont;
	switch (typeFont )	{
		case NORMAL_FONT :		defaultFont = DefaultNormalFont;		break;
		case SMALL_FONT :		defaultFont = DefaultSmallFont;			break;
		case LARGE_FONT :		defaultFont = DefaultLargeFont;			break;
	}
	return( textUtil->lenght( pTextGL, &text, defaultFont, nbChar ) );
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
void PanelText::setTabSize( int t ) {
	#ifdef DEBUG
	cout << "PanelText::setTabSize() : " << t << endl;
	#endif
	textUtil->setTabSize( t );
	tabSize = t;
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
void PanelText::setColor( int l)
{
    color = l>>8 | ((l&0xff)<<24);
    //bChange = true;
	//cout << "PanelText::setColor(" << hex << nColor << ")" << endl;
}
//--------------------------------------------------------------------------------------------------------------------
//
//						Virtuals fonctions....
//
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
void PanelText::updatePos() {
	/*
	#ifdef DEBUG
	cout << "PanelText::updatePos()" << boolalpha << bChange;
	cout << ", typeFont = "<< strFont();
	cout << ", addr str = "<< &text;
	cout << endl;
	#endif
	*/
	Panel::updatePos();

	if ( pTextGL == NULL )	{
		if ( text.compare("") == 0 )	{
			bChange = false;
			return;	
		}
		else	{
			bChange = true;
		}
	}


	if ( bChange )	{
		switch( typeFont )	{
		case NORMAL_FONT :
		case SMALL_FONT :
		case LARGE_FONT :
			buildString();
			break;
		}
	}



	switch (align)	{
	case LEFT:
		break;
	case CENTER:
		if ( parent && bChange == false )	{
			int l;
			switch( typeFont )	{
				case NORMAL_FONT :		l = textUtil->lenght( pTextGL, &text, DefaultNormalFont );		break;
				case SMALL_FONT :		l = textUtil->lenght( pTextGL, &text, DefaultSmallFont );		break;
				case LARGE_FONT :		l = textUtil->lenght( pTextGL, &text, DefaultLargeFont );		break;
			}
			#ifdef DEBUG
				cout << "Align : l="<< l << " String=\""<< text <<"\"" << endl;
				cout << "Align : x="<< x_raw << endl;
			#endif
			x_raw += parent->getDX()/2 - l/2;
			#ifdef DEBUG
				cout << "Align : x="<< x_raw << endl;
			#endif
		}
		break;
	case RIGHT:
		if ( parent )	{
			int l;
			switch( typeFont )	{
				case NORMAL_FONT :		l = textUtil->lenght( pTextGL, &text, DefaultNormalFont );		break;
				case SMALL_FONT :		l = textUtil->lenght( pTextGL, &text, DefaultSmallFont );		break;
				case LARGE_FONT :		l = textUtil->lenght( pTextGL, &text, DefaultLargeFont );		break;
			}
			#ifdef DEBUG
				cout << "Align : l="<< l << " String=\""<< text <<"\"" << endl;
				cout << "Align : x="<< x_raw << endl;
			#endif
			x_raw += parent->getDX() - l - x;
			#ifdef DEBUG
				cout << "Align : x="<< x_raw << endl;
			#endif
		}
		break;
	}
	
	

}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
void PanelText::displayGLInternal()	{
	WindowsManager& wm = WindowsManager::getInstance();
	float width  = (float)wm.getWidth();
	float height = (float)wm.getHeight();

	int iProgId;
	glGetIntegerv(GL_CURRENT_PROGRAM, &iProgId);
	glUseProgramObjectARB(0);
		
	textUtil->BeginGL();
	
	int scx, scy, scdx, scdy;
	scx  = getParent()->getX();
	scy  = height - getParent()->getDY() - getParent()->getY();
	scdx = getParent()->getDX();
	scdy = getParent()->getDY();

#ifdef DEBUG
	cout << "    PT:" << "-------------------" << endl;
	cout << "    PT:" << "-----Scissor-------" << endl;
	cout << "    PT:" << scx <<", "<< scy <<", "<< scdx <<", "<< scdy << endl;
#endif
	//glScissor( scx, scy, scdx, scdy );
	//glEnable( GL_SCISSOR_TEST );
	//glColor4f( 0.0f, 0.0f, 0.0f, 0.0f );
	//glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );
	//glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	int iTex;	
	int slot = wm.getSlot();
	//slot = 1;

    glEnable(GL_TEXTURE_2D);
	switch( typeFont )	{
	case NORMAL_FONT :
		#ifdef DEBUG
		cout << "    PT: NORMAL_FONT"  << endl;
		#endif
		//iTex = textUtil->BindFont( DefaultNormalFont );
		textUtil->BindFont( DefaultNormalFont, slot );
		break;
	case SMALL_FONT :
		#ifdef DEBUG
		cout << "    PT: SMALL_FONT"  << endl;
		#endif
		//iTex = textUtil->BindFont( DefaultSmallFont );
		textUtil->BindFont( DefaultSmallFont, slot );
		break;
	case LARGE_FONT :
		#ifdef DEBUG
		cout << "    PT: LARGE_FONT"  << endl;
		#endif
		//iTex = textUtil->BindFont( DefaultLargeFont );
		textUtil->BindFont( DefaultLargeFont, slot );
		break;
	}

	//cout << "PanelText::displayGLInternal() color =" << hex << color << "  text='"<< text <<"'"<< endl;
	textUtil->DrawText( pTextGL, alpha, getX(), getY(), color, 0 );

	textUtil->UnbindFont( slot );

	//glDisable( GL_SCISSOR_TEST );
	textUtil->EndGL();

	glUseProgramObjectARB( iProgId );
		
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
Panel* PanelText::isMouseOver(int xm, int ym)	{
//#define DEBUG
	#ifdef DEBUG
	if ( x_raw <= xm && xm <= (x_raw+dx_raw) && y_raw <= ym && ym <= (y_raw+dy_raw) )
    	cout << "PanelText::isMouseOver()" << x_raw <<", "<< y_raw <<", "<< dx_raw <<", "<< dy_raw << endl;
	#endif
	return NULL;

	if ( x_raw <= xm && xm <= (x_raw+dx_raw) && y_raw <= ym && ym <= (y_raw+dy_raw) )		return this;
	else																					return NULL;
//#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
void PanelText::displayGL() {
	if (visible == false)			return;
	if ( pTextGL == NULL )			return;	

#ifdef DEBUG
	WindowsManager& wm = WindowsManager::getInstance();
	cout << "    PT:" << "-------------------" << endl;
	int parentID = -1;
	if ( this->getParent() )	parentID = this->getParent()->getID();
	cout << "PT displayGL ... fenetre id : " << getID() <<"  parnet ID : "<< parentID << endl;
	cout << "        " << "-------------------" << endl;
	cout << "   text:" << text << endl;
	cout << "     PT:" << getX() <<", "<< getY() <<", "<< getDX() <<", "<< getDY() << endl;
	cout << "     WM:" << wm.getWidth() <<", "<< wm.getHeight() << endl;
#endif
	
	if ( alpha != 0.0 )	{
		mat4 mRot, mTrPlus, mTrMoins;
		mRot.rotate_z( DEG2RAD(alpha) );
		vec3 tr = vec3( x_raw, y_raw, 0.0 );
		mTrPlus.translate( tr );
		mTrMoins.translate( -tr );
		

		mat4 mModelView = mTrPlus * mRot * mTrMoins; 
		
	
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();	
		glLoadMatrixf (mModelView );
		//cout << "PT displayGL alpha="<< alpha <<" ... text: " << text << endl;
	}
    /*
    if ( color != 0xffffffff )      cout << "displa1" <<hex <<color<< endl;

	unsigned char a = (color&0xff000000)>>24;
	unsigned char r = (color&0x00ff0000)>>16;
	unsigned char g = (color&0x0000ff00)>>8;
	unsigned char b = (color&0x000000ff);

    if ( color != 0xffffffff )      cout << "displa2" << hex <<color<< endl;
	glColor4f( r,g,b,a );
	*/

	switch( typeFont )	{
		case NORMAL_FONT :
		case SMALL_FONT :
		case LARGE_FONT :
			displayGLInternal();
			break;
	}	


	if ( alpha != 0.0 )	{
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();	
	}
	
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------

