//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
#include "PanelScrollText.h"
#include "wm.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
//#define DEBUG
//#define DEBUG_CONST

#ifdef DEBUG_WM
#	define DEBUG
#endif


#ifdef DEBUG
#	define DEBUG_CONST
#endif
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
using namespace std;
//--------------------------------------------------------------------------------------------------------------------
//
//						Constructeurs ....
//
//--------------------------------------------------------------------------------------------------------------------
PanelScrollText::PanelScrollText()	{
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelScrollText() ..." << endl;
	#endif
	
	PanelSimple();
	tabSize = 40;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
PanelScrollText::PanelScrollText(int nbLine, int max)	{
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelScrollText( "<< nbLine << " ) ..." << endl;
	#endif
	
	PanelScrollText();

	prompt = "";
	maxCmd = max;
	currentLine = 0;
	currentPos = 0;
	currentCmd = 0;
//	cursorTime = -1.0f;

	bIns = false;
	bCtrl = false;
	bRightCtrl = false;
	bLeftCtrl = false;

	heightLine = 13;
			
	for( int i=0; i<nbLine; i++ )	{
		texts.push_back( new PanelText() );
		texts[i]->setPos( 0, i*heightLine );
		texts[i]->setTabSize( tabSize );
		this->add( texts[i] );
	}
	
	//WindowsManager::getInstance().call_back_keyboard( this );
	//cursor.setPos( currentPos, 15*currentLine );
	/*
	cursor.changeText( "|", PanelText::NORMAL_FONT, true );
	this->add( &cursor );
	moveCursor();
	*/


}
//--------------------------------------------------------------------------------------------------------------------
//
//						fonctions....
//
//--------------------------------------------------------------------------------------------------------------------
void PanelScrollText::setPrompt( std::string str )		{
	if ( str.size() == 0 )		return;
	prompt = str;
	texts[currentLine]->changeText( prompt, PanelText::NORMAL_FONT, true );
//	moveCursor();
}

/*
void PanelScrollText::setCallBackCmd( CB_CMD f )		{
	cb_cmd = f;
}

void PanelScrollText::setCallBackCmd( PanelScrollTextCallBack* p )		{
	ppccb = p;
}
*/

/*

void PanelScrollText::moveCursor()	{
	int pl = prompt.size();
	int x = texts[currentLine]->getTextLenght(pl+currentPos);
	int y = texts[currentLine]->getPosY();//TextLenght(pl+currentPos);
	//int y = currentLine*heightLine;

	cursor.setPos( x, y );
}
*/
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelScrollText::scroll() {
	if ( texts[currentLine]->getPosY() < (getDY()+1) )	{
		return;
	}
	#ifdef DEBUG
	cout << "PanelScrollText::scroll()"<< endl;
	#endif
	
	int nb = texts.size();
	for ( int i=0; i< nb; i++ )	{
		int y = texts[i]->getPosY();
		y -= heightLine;
		texts[i]->setPosY( y );
	}
	//cout << "--- scroll  y(curretnLine)    = "<< texts[currentLine]->getPosY() << endl;
	//cout << "--- scroll  text(curretnLine) = "<< texts[currentLine]->getText() << endl;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelScrollText::rotateBuffer() {
	int y = texts[ texts.size()-1 ]->getPosY() + heightLine;

	#ifdef DEBUG
	cout << "--- rotate buffer y = "<< y << endl;
	#endif

	currentLine--;
	this->sup( texts[0] );
	texts.erase( texts.begin() + 0 );
	texts.push_back( new PanelText() );
	texts[ texts.size()-1 ]->setPosY( y );
	texts[ texts.size()-1 ]->setPosX( 0 );
	texts[ texts.size()-1 ]->setTabSize( tabSize );
	this->add( texts[ texts.size()-1 ] );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelScrollText::addLine() {
	currentLine++;
	currentPos = 0;
	//cursor.setPos( currentPos, 15*currentLine );
	if ( currentLine >= texts.size() )	{
	    #ifdef DEBUG
		cout << "--- EraseLine "<< endl;
	    #endif
		rotateBuffer();
	
	}
	texts[currentLine]->changeText( prompt, PanelText::NORMAL_FONT, true );

	scroll();
	
	//moveCursor();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelScrollText::eraseTexts( ) {
	currentLine = 0;
	currentPos = 0;
	
	int nb = texts.size();
	for (int i=0; i<nb; i++ )	{
		texts[i]->eraseText();
	}
	texts[currentLine]->changeText( prompt, PanelText::NORMAL_FONT, true );
	//moveCursor();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelScrollText::clear( ) {
	//currentLine = 0;
	//currentPos = 0;
	
	int deltaY = texts[currentLine]->getPosY();
	int nb = texts.size();
	for (int i=0; i<nb; i++ )	{
		int y = texts[i]->getPosY();
		
		texts[i]->setPosY(y-deltaY);
	}
		
	texts[currentLine]->changeText( prompt, PanelText::NORMAL_FONT, true );
	//moveCursor();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelScrollText::affiche( string* str ) {
	#ifdef DEBUG
	cout << "PanelScrollText::affiche( (string*)\"";
    printf( str->c_str() );
    printf( "\" )");
    printf( "\n" );
    #endif
        
	texts[currentLine]->changeText(*str, PanelText::NORMAL_FONT, true );
	texts[currentLine]->setTabSize( tabSize );
	for( int i=0; i<texts.size(); i++ )	{
		texts[i]->setTabSize( tabSize );
	    texts[i]->setColor( color );
	}
	addLine();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelScrollText::affiche( string str ) {
	#ifdef DEBUG
    printf( "PanelScrollText::affiche( (string)\"");
    printf( str.c_str() );
    printf( "\" )");
    printf( "\n" );
    #endif

	texts[currentLine]->changeText(str, PanelText::NORMAL_FONT, true );
	texts[currentLine]->setTabSize( tabSize );
	for( int i=0; i<texts.size(); i++ )	{
		texts[i]->setTabSize( tabSize );
	    texts[i]->setColor( color );
	}
	addLine();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------

void PanelScrollText::setTabSize( int t ) {
    #ifdef DEBUG
    cout <<"CurrentLine : "<< currentLine << endl;
    #endif
	//texts[currentLine]->setTabSize( t );
	tabSize = t;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelScrollText::setColor( long c ) {
    //printf( "%08X", c );
    color = c;
    for ( int i=0; i<texts.size(); i++ )
    {
	    texts[i]->setColor( c );
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//						Virtuals fonctions....
//
//--------------------------------------------------------------------------------------------------------------------
void PanelScrollText::idle(float elapsedTime) {
	//cout << "ElapsedTime : "<< elapsedTime << endl;
	//cout << "cursorTime  : "<< cursorTime << endl;
	oldDY = getPosDY();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelScrollText::updatePos() {
	#ifdef DEBUG
	cout << "PanelScrollText::updatePos()" << endl;;
	#endif

    if ( oldDY != getPosDY() )
    {
        int decal, y;
        decal = oldDY - getPosDY();
        //cout <<"decal : "<< decal << endl;
        int nbLigne = texts.size();
        
	    for( int i=0; i<nbLigne; i++ )	{
	        y = texts[i]->getPosY();
		    texts[i]->setPos( 0, y - decal );
	    }
    }	    
	PanelSimple::updatePos();

    oldDY = getPosDY();


}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelScrollText::displayGL() {
	if (visible == false)			return;

	PanelSimple::displayGL();

#ifdef DEBUG
	cout << "PanelScrollText::displayGL()" << endl;;
	WindowsManager& wm = WindowsManager::getInstance();
	cout << "  PC displayGL ... fenetre id : " << getID() << endl;
	cout << "  PC:" << "-------------------" << endl;
	cout << "  PC:" << getX() <<", "<< getY() <<", "<< getDX() <<"x"<< getDY() << endl;
	cout << "  PC:" << wm.getWidth() <<", "<< wm.getHeight() << endl;
	cout << "  PC: no ligne currrent : " << currentLine << endl;
#endif
	
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------

