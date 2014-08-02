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

	textUtil = WindowsManager::getInstance().getTextUtil();
	setPos( 0, 0 );
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
	bChange = true;

	changeText( str, type );
}


PanelText::PanelText( string str, FONT type, int x, int y )	{
	typeFont = type;
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelText( "<< str <<", "<< strFont() <<", "<< x <<", "<< y <<" )" << endl;
	#endif
	PanelText();
	bChange = true;

	changeText( str, type );
	setPos(x, y);
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



void PanelText::changeText( string str )	{
	#ifdef DEBUG
	cout << "PanelText::changeText( "<< str <<" )" << endl;
	#endif
	
	text = str;
	bChange = true;
}



void PanelText::changeText( string str, FONT type )	{
	#ifdef DEBUG
	cout << "PanelText::changeText( "<< str <<", "<< type <<" )" << endl;
	#endif
	
	typeFont = type;
	changeText( str );
}



void PanelText::buildString()	{
	#ifdef DEBUG
	cout << "PanelText::buildString() font = "<< strFont() << endl;
	#endif
	textUtil = WindowsManager::getInstance().getTextUtil();

	color32 color		= COLOR32_WHITE;
	color32 color_bg	= COLOR32_WHITE;

	pTextGL = textUtil->NewTextObj();
 

	switch (typeFont )	{
	case NORMAL_FONT :
		textUtil->BuildText( pTextGL, &(text), &color, &color_bg, 1,  g_DefaultNormalFont, 0, 0);
		break;
	case SMALL_FONT :
		textUtil->BuildText( pTextGL, &(text), &color, &color_bg, 1,  g_DefaultSmallFont, 0, 0);
		break;
	case LARGE_FONT :
		textUtil->BuildText( pTextGL, &(text), &color, &color_bg, 1,  g_DefaultLargeFont, 0, 0);
		break;
	}
	bChange = false;
}



//--------------------------------------------------------------------------------------------------------------------
//
//						Virtuals fonctions....
//
//--------------------------------------------------------------------------------------------------------------------
void PanelText::updatePos() {
	#ifdef DEBUG
	cout << "PanelText::updatePos()" << boolalpha << bChange;
	cout << ", typeFont = "<< strFont() << endl;
	#endif
	
	Panel::updatePos();
	
	if ( bChange )	{
		switch( typeFont )	{
		case NORMAL_FONT :
		case SMALL_FONT :
		case LARGE_FONT :
			buildString();
			break;
		}
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
	

	int iTex;	
	switch( typeFont )	{
	case NORMAL_FONT :
		iTex = textUtil->BindFont( g_DefaultNormalFont );
		break;
	case SMALL_FONT :
		iTex = textUtil->BindFont( g_DefaultSmallFont );
		break;
	case LARGE_FONT :
		iTex = textUtil->BindFont( g_DefaultLargeFont );
		break;
	}
	textUtil->DrawText( pTextGL, getX(), getY(), COLOR32_WHITE, COLOR32_RED );
	textUtil->UnbindFont( iTex );

	//glDisable( GL_SCISSOR_TEST );
	textUtil->EndGL();
}
	


void PanelText::displayGL() {
	if (visible == false)			return;

#ifdef DEBUG
	WindowsManager& wm = WindowsManager::getInstance();
	cout << "PT displayGL ... fenetre id : " << getID() << endl;
	cout << "    PT:" << "-------------------" << endl;
	cout << "    PT:" << getX() <<", "<< getY() <<", "<< getDX() <<", "<< getDY() << endl;
	cout << "    PT:" << wm.getWidth() <<", "<< wm.getHeight() << endl;
#endif
	

	switch( typeFont )	{
		case NORMAL_FONT :
		case SMALL_FONT :
		case LARGE_FONT :
			displayGLInternal();
			break;
	}	
	
}




