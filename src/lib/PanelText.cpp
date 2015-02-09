#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "PanelText.h"
#include "wm.h"

//#define DEBUG
#define DEBUG_CONST

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
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelText ..." << endl;
	#endif

	text = "";
	textUtil = WindowsManager::getInstance().getTextUtil();
	setPos( 0, 0 );
	align = LEFT;
	pTextGL = NULL;
	bChange = false;
	ID = 999;
	tabSize = 40;
}


PanelText::PanelText( string str )	{
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelText( "<< str <<" )" << endl;
	#endif

	PanelText();

	changeText( str );
}


PanelText::PanelText( string str, FONT type )	{
	typeFont = type;
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelText( "<< str <<", "<< strFont() <<" )" << endl;
	#endif
	PanelText();

	changeText( str, type, true );
}


PanelText::PanelText( string str, FONT type, int x, int y )	{
	typeFont = type;
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelText( "<< str <<", "<< strFont() <<", "<< x <<", "<< y <<" )" << endl;
	#endif

	PanelText();

	setPos(x, y);
	changeText( str, type );
}


PanelText::PanelText( char * cstr, FONT type, int x, int y )	{
	typeFont = type;
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelText( "<< cstr <<", "<< strFont() <<", "<< x <<", "<< y <<" )" << endl;
	#endif

	PanelText();

	setPos(x, y);
	changeText( cstr, type );
	buildString();
}


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
	
	typeFont = type;
	text = str;

	if (build)		buildString();
}



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

	color32 color		= COLOR32_BLACK;
	color32 color_bg	= COLOR32_WHITE;

	//if (pTextGL != NULL )	textUtil->DeleteTextObj( pTextGL );
	textUtil->setTabSize( tabSize );
	pTextGL = textUtil->NewTextObj();


	switch (typeFont )	{
	case NORMAL_FONT :
		textUtil->GenFont( DefaultNormalFont );
		textUtil->BuildText( pTextGL, &(text), &color, &color_bg, 1,  DefaultNormalFont, 0, 0);
		break;
	case SMALL_FONT :
		textUtil->GenFont( DefaultSmallFont );
		textUtil->BuildText( pTextGL, &(text), &color, &color_bg, 1,  DefaultSmallFont, 0, 0);
		break;
	case LARGE_FONT :
		textUtil->GenFont( DefaultLargeFont );
		textUtil->BuildText( pTextGL, &(text), &color, &color_bg, 1,  DefaultLargeFont, 0, 0);
		break;
	}
	bChange = false;
}



int PanelText::getTextLenght()	{
	CTexFont * defaultFont;
	switch (typeFont )	{
		case NORMAL_FONT :		defaultFont = DefaultNormalFont;		break;
		case SMALL_FONT :		defaultFont = DefaultSmallFont;			break;
		case LARGE_FONT :		defaultFont = DefaultLargeFont;			break;
	}
	return( textUtil->lenght( pTextGL, &text, defaultFont ) );
}

int PanelText::getTextLenght( int nbChar )	{
	CTexFont * defaultFont;
	switch (typeFont )	{
		case NORMAL_FONT :		defaultFont = DefaultNormalFont;		break;
		case SMALL_FONT :		defaultFont = DefaultSmallFont;			break;
		case LARGE_FONT :		defaultFont = DefaultLargeFont;			break;
	}
	return( textUtil->lenght( pTextGL, &text, defaultFont, nbChar ) );
}

void PanelText::setTabSize( int t ) {
	#ifdef DEBUG
	cout << "PanelText::setTabSize() : " << t << endl;
	#endif
	textUtil->setTabSize( t );
	tabSize = t;
}


//--------------------------------------------------------------------------------------------------------------------
//
//						Virtuals fonctions....
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


void PanelText::displayGLInternal()	{
	WindowsManager& wm = WindowsManager::getInstance();
	float width  = (float)wm.getWidth();
	float height = (float)wm.getHeight();

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
	glColor4f( 0.0f, 0.0f, 0.0f, 0.0f );
	//glColor4f( 1.0f, 0.0f, 0.0f, 0.0f );
	//glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	int iTex;	
	int slot = wm.getSlot();
	
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
	textUtil->DrawText( pTextGL, getX(), getY(), COLOR32_WHITE, COLOR32_RED );
	//textUtil->DrawText( pTextGL, 20, 30, COLOR32_WHITE, COLOR32_RED );
	textUtil->UnbindFont( slot );

	//glDisable( GL_SCISSOR_TEST );
	textUtil->EndGL();
}
	


void PanelText::displayGL() {
	if (visible == false)			return;
	if (bChange)					return;
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
	

	switch( typeFont )	{
		case NORMAL_FONT :
		case SMALL_FONT :
		case LARGE_FONT :
			displayGLInternal();
			break;
	}	
	
}




