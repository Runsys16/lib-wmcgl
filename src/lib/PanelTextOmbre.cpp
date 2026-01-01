#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "PanelTextOmbre.h"
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
PanelTextOmbre::PanelTextOmbre()	{
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelTextOmbre() ..." << endl;
	#endif
	
	init();
}


PanelTextOmbre::PanelTextOmbre( string str )	{
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelTextOmbre( "<< str <<" )" << endl;
	#endif

	init();
	color = 0xffffffff;

	changeText( str );
}


PanelTextOmbre::PanelTextOmbre( string str, FONT type )	{
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelTextOmbre( "<< str <<", "<< strFont() <<" )" << endl;
	#endif

	init();

	typeFont = type;
	color = 0xffffffff;

	changeText( str, type, true );
	pTextOmbre.setColor( 0x000000ff );
	pTextOmbre.changeText( str, type, true );
}


PanelTextOmbre::PanelTextOmbre( string str, FONT type, int x, int y )	{
	typeFont = type;
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelTextOmbre( "<< str <<", "<< strFont() <<", "<< x <<", "<< y <<" )" << endl;
	#endif

	PanelTextOmbre();
	color = 0xffffffff;

	setPos(x, y);
	changeText( str, type );
	
	pTextOmbre.setPos(x+1, y+1);
	pTextOmbre.setColor( 0x000000ff );
	pTextOmbre.changeText( str, type );
}


PanelTextOmbre::PanelTextOmbre( string str, FONT type, int x, int y, unsigned int c )	{
	typeFont = type;
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelTextOmbre( "<< str <<", "<< strFont() <<", "<< x <<", "<< y <<" )" << endl;
	#endif
	
	init();

	color = c;
	setPos(x, y);
	changeText( str, type );

	pTextOmbre.setPos(x+1, y+1);
	pTextOmbre.setColor( 0x000000ff );
	pTextOmbre.changeText( str, type );
}


PanelTextOmbre::PanelTextOmbre( char * cstr, FONT type, int x, int y )	{
	typeFont = type;
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelTextOmbre( "<< cstr <<", "<< strFont() <<", "<< x <<", "<< y <<" )" << endl;
	#endif

	init();

	setPos(x, y);
	changeText( cstr, type );
	buildString();

	pTextOmbre.setPos(x+1, y+1);
	pTextOmbre.setColor( 0x000000ff );
	pTextOmbre.changeText( cstr, type );
	pTextOmbre.buildString();
}

PanelTextOmbre::PanelTextOmbre( char * cstr, FONT type, int x, int y, unsigned int c )	{
	typeFont = type;
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelTextOmbre( "<< cstr <<", "<< strFont() <<", "<< x <<", "<< y <<" )" << endl;
	#endif

	init();
	color = c;
	
	setPos(x, y);
	changeText( cstr, type );
	buildString();

	pTextOmbre.setPos(x+1, y+1);
	pTextOmbre.setColor( 0x000000ff );
	pTextOmbre.changeText( cstr, type );
	pTextOmbre.buildString();
}
//--------------------------------------------------------------------------------------------------------------------
// ChangeText() functions
//--------------------------------------------------------------------------------------------------------------------
void PanelTextOmbre::init()	{
	text = "";
	textUtil = WindowsManager::getInstance().getTextUtil();
	setPos( 0, 0 );
	align = LEFT;
	pTextGL = NULL;
	bChange = false;
	ID = 9001;
	color = 0xffffffff;
	pTextOmbre.setColor( 0x000000ff );
	pTextOmbre.setPos( 1, 1 );
	pTextOmbre.setAlign( align );
}

/*
//--------------------------------------------------------------------------------------------------------------------
string PanelTextOmbre::strFont() {
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
*/


//--------------------------------------------------------------------------------------------------------------------
// ChangeText() functions
//--------------------------------------------------------------------------------------------------------------------
void PanelTextOmbre::changeText( char* cstr )	{
	PanelText::changeText( cstr );
	pTextOmbre.changeText( cstr );
}



void PanelTextOmbre::changeText( char* cstr, FONT type )	{
	#ifdef DEBUG
	cout << "PanelTextOmbre::changeText( "<< cstr  <<", "<< strFont() <<" )" << endl;
	#endif
	
	if ( (text.compare(cstr) == 0) && (type == typeFont) )		{
		bChange = false;
		return;
	}

	typeFont = type;
	changeText( cstr );
	pTextOmbre.changeText( cstr, type );
}


void PanelTextOmbre::changeText( string str )	{
	#ifdef DEBUG
	cout << "PanelTextOmbre::changeText( "<< str <<", "<< strFont() <<" )" <<  endl;
	#endif
	
	if ( text.compare(str) == 0 )		{
		bChange = false;
		return;
	}

	text = str;
	pTextOmbre.changeText( str );
	bChange = true;
}



void PanelTextOmbre::changeText( string str, FONT type )	{
	#ifdef DEBUG
	cout << "PanelTextOmbre::changeText( "<< str  <<", "<< strFont() <<" )" << endl;
	#endif
	
	if ( (text.compare(str) == 0) && (type == typeFont) )		{
		bChange = false;
		return;
	}

	typeFont = type;
	changeText( str );
	pTextOmbre.changeText( str, type );
}


void PanelTextOmbre::changeText( string str, bool build )	{
	#ifdef DEBUG
	cout << "PanelTextOmbre::changeText( "<< str <<", "<< strFont()  <<" )" << endl;
	#endif
	
	if ( text.compare(str) == 0 )		{
		bChange = false;
		return;
	}

	bChange = true;

	text = str;
	bChange = true;
	
	if (build)		buildString();

	pTextOmbre.changeText( str, build );
}



void PanelTextOmbre::changeText( string str, FONT type, bool build )	{
	#ifdef DEBUG
	cout << "PanelTextOmbre::changeText( \""<< str <<"\", "<< type <<" )" << endl;
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
	
	typeFont = type;
	text = str;

	if (build)		buildString();

	pTextOmbre.changeText( str, type, build );
}



//--------------------------------------------------------------------------------------------------------------------
void PanelTextOmbre::eraseText()	{
	#ifdef DEBUG
	cout << "PanelTextOmbre::eraseText()"<< endl;
	#endif
	PanelText::eraseText();
	pTextOmbre.eraseText();
}


//--------------------------------------------------------------------------------------------------------------------

void PanelTextOmbre::buildString()	{
	#ifdef DEBUG
	cout << "PanelTextOmbre::buildString() font = "<< strFont() <<"  texte = \""<< text <<"\""<< endl;
	#endif
	PanelText::buildString();
	pTextOmbre.buildString();
}



/*
int PanelTextOmbre::getTextLenght()	{
	CTexFont * defaultFont;
	switch (typeFont )	{
		case NORMAL_FONT :		defaultFont = DefaultNormalFont;		break;
		case SMALL_FONT :		defaultFont = DefaultSmallFont;			break;
		case LARGE_FONT :		defaultFont = DefaultLargeFont;			break;
	}
	pTextOmbre.getTextLenght();
	return( textUtil->lenght( pTextGL, &text, defaultFont ) );
}

int PanelTextOmbre::getTextLenght( int nbChar )	{
	CTexFont * defaultFont;
	switch (typeFont )	{
		case NORMAL_FONT :		defaultFont = DefaultNormalFont;		break;
		case SMALL_FONT :		defaultFont = DefaultSmallFont;			break;
		case LARGE_FONT :		defaultFont = DefaultLargeFont;			break;
	}
	pTextOmbre.getTextLenght( nbChar );
	return( textUtil->lenght( pTextGL, &text, defaultFont, nbChar ) );
}
*/
void PanelTextOmbre::setTabSize( int t ) {
	#ifdef DEBUG
	cout << "PanelTextOmbre::setTabSize() : " << t << endl;
	#endif
	PanelText::setTabSize( t );
	pTextOmbre.setTabSize( t );
}


void PanelTextOmbre::setColor( unsigned long c ) {
	#ifdef DEBUG
	cout << "PanelTextOmbre::setTabSize() : " << c << endl;
	#endif
	color = c;//PanelText::setColor( c );
	//pTextOmbre.setColor( 0x000000ff );
}


//--------------------------------------------------------------------------------------------------------------------
//
//						Virtuals fonctions....
//
//--------------------------------------------------------------------------------------------------------------------
void PanelTextOmbre::updatePos() {
	if ( pTextOmbre.getParent() != getParent() )
	{
        #ifdef DEBUG
        cout << "PanelTextOmbre::updatePos() ";
		cout << "setParent "<< endl;
		#endif
		pTextOmbre.setParent( getParent() );
	}	
	pTextOmbre.setPosX( x+1 );
	pTextOmbre.setPosY( y+1 );
	pTextOmbre.setAlign( align );
	
	
	PanelText::updatePos();

	//pTextOmbre.setPosAndSize(int x0, int y0, int dx0, int dy0)	{x=x0; y=y0; dx=dx0 ;dy=dy0;};

	pTextOmbre.updatePos();

	dy = dy_raw = getDY() + 1;
}


void PanelTextOmbre::displayGLInternal()	{
	PanelText::displayGLInternal();
	pTextOmbre.displayGLInternal();
}
	
Panel* PanelTextOmbre::isMouseOver(int xm, int ym)	{
	return PanelText::isMouseOver( xm,  ym);
}




void PanelTextOmbre::displayGL() {
	/*
1	cout << "DisplayGL() "<< x_raw <<", "<< y_raw << endl;
	cout << " "<< x <<", "<< y << endl;
	cout << " "<< pTextOmbre.getPosX() <<", "<< pTextOmbre.getPosY() << endl;
	cout << " "<< pTextOmbre.getX() <<", "<< pTextOmbre.getY() << endl;
	cout << " "<< pTextOmbre.color << endl;
	cout << " "<< pTextOmbre.getText() << endl;
	*/
	pTextOmbre.displayGL();
	PanelText::displayGL();
}




