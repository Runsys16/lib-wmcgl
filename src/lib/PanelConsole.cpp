#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "PanelConsole.h"
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
PanelConsole::PanelConsole(int nb)	{
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelConsole( "<< nb << " ) ..." << endl;
	#endif
	
	PanelSimple();

	prompt = "";
	currentLine = 0;
	currentPos = 0;
	cursorTime = -1.0f;

	bIns = false;
		
	for( int i=0; i<nb; i++ )	{
		texts.push_back( new PanelText() );
		texts[i]->setPos( 0, i*15 );
		this->add( texts[i] );
	}
	
	WindowsManager::getInstance().call_back_keyboard( this );
	//cursor.setPos( currentPos, 15*currentLine );

	cursor.changeText( "_", PanelText::NORMAL_FONT, true );
	this->add( &cursor );
	moveCursor();
}


//--------------------------------------------------------------------------------------------------------------------
//
//						fonctions....
//
//--------------------------------------------------------------------------------------------------------------------
void PanelConsole::setPrompt( std::string str )		{
	if ( str.size() == 0 )		return;
	prompt = str;
	texts[currentLine]->changeText( prompt, PanelText::NORMAL_FONT, true );
	moveCursor();
}



void PanelConsole::addLine() {
	currentLine++;
	currentPos = 0;
	//cursor.setPos( currentPos, 15*currentLine );
	if ( currentLine >= texts.size() )	{
		return;
	}
	
	texts[currentLine]->changeText( prompt, PanelText::NORMAL_FONT, true );
	moveCursor();
}

void PanelConsole::affiche( string* str ) {
	if ( currentLine >= texts.size() )	{
		return;
	}
	
	texts[currentLine]->changeText(*str, PanelText::NORMAL_FONT, true );
	currentLine++;
	currentPos = 0;
	//cursor.setPos( currentPos, 15*currentLine );
	texts[currentLine]->changeText( prompt, PanelText::NORMAL_FONT, true );
	moveCursor();
}




void PanelConsole::addChar( char c ) {
	cout << "PanelConsole::addChar()  currentPos   : \""<< currentPos <<"\""<< endl;;
	cout << "PanelConsole::addChar()  currentLine  : \""<< currentLine <<"\""<< endl;;
	if ( currentLine >= texts.size() )	{
		return;
	}
	string str = texts[currentLine]->getText();
	// prompt lenght
	int pl = prompt.size();
	// command lenght
	int cl = str.size() - pl;
	
	string * val;
	if ( currentPos == cl )	{
		char buff[2048];
		sprintf( buff, "%s%c", str.c_str(), c );
		val = new string(buff);
	}
	else	{
		char left[2048];
		char right[2048];
		
		int ll = pl+currentPos;
		int rl = str.size()-(pl+currentPos);

		str.copy( left, ll, 0);
		str.copy( right, rl, pl+currentPos);

		left[ll] = 0;
		right[rl] = 0;
		cout << "PanelConsole::addChar()  left  : \""<< left <<"\""<< endl;;
		cout << "PanelConsole::addChar()  right : \""<< right <<"\""<< endl;;
		
		char buff[2048];
		sprintf( buff, "%s%c%s", left, c, right );
		val = new string(buff);
	}
	
	
	texts[currentLine]->changeText( *val, PanelText::NORMAL_FONT, true );
	delete val;
	currentPos++;

	moveCursor();	
}

void PanelConsole::supChar() {
	if ( currentLine >= texts.size() )	{
		return;
	}
	if ( currentPos <= 0  )	{
		cout << "PanelConsole::supChar() : currentPos NULL" << endl;;
		return;
	}
	string str = texts[currentLine]->getText();
	cout << "PanelConsole::supChar() : \""<< str <<"\""<< endl;;
	#ifdef DEBUG
	#endif
	
	char buff[2048];
	
	
	
	if ( prompt.size() >= str.size() )	{
		cout << "PanelConsole::supChar() : *** Debut de ligne *** "<< endl;
		return;
	}
	
	// prompt lenght
	int pl = prompt.size();
	// command lenght
	int cl = str.size() - pl;
	
	string * val;

	


	if ( currentPos == cl )	{
		str.copy( buff, str.size()-1, 0);
		cout << "PanelConsole::supChar() : \""<< buff <<"\""<< endl;;
		buff[str.size()-1] = 0;
		val = new string(buff);
		cout << "PanelConsole::supChar() : \""<< *val <<"\""<< endl;;
	}
	else	{
		char left[2048];
		char right[2048];
		
		int ll = pl+currentPos - 1;
		int rl = str.size()-(pl+currentPos);

		str.copy( left, ll, 0);
		str.copy( right, rl, pl+currentPos);

		left[ll] = 0;
		right[rl] = 0;
		cout << "PanelConsole::supChar()  left  : \""<< left <<"\""<< endl;;
		cout << "PanelConsole::supChar()  right : \""<< right <<"\""<< endl;;
		
		char buff[2048];
		sprintf( buff, "%s%s", left, right );
		val = new string(buff);
	}
	
	

	
	texts[currentLine]->changeText( *val, PanelText::NORMAL_FONT, true );
	delete val;
	currentPos--;
	moveCursor();

}




void PanelConsole::incCursor()	{
		int max = texts[currentLine]->getText().size() - prompt.size();
		if ( currentPos < max )			currentPos++;
		
		moveCursor();
		//cursor.setPos( currentPos, 15*currentLine );
		
}



void PanelConsole::moveCursor()	{
	int pl = prompt.size();
	int x = texts[currentLine]->getTextLenght(pl+currentPos);
	int y = currentLine*15;

	cursor.setPos( x, y );
}
//--------------------------------------------------------------------------------------------------------------------
//
//						Callback functions....
//
//--------------------------------------------------------------------------------------------------------------------
void PanelConsole::cb_keyboard( unsigned char key ) {
	cout << "PanelConsole::cb_keyboard( "<< (unsigned int) key <<" ) "<< endl;;
	#ifdef DEBUG
	#endif
	static int n=0;
	switch(key){ 
	case '\r':		{
		addLine();
		}	break;
	
	case 178:		{
		char buff[80];
		sprintf( buff, "Essai de console %d ...", n++ );
		std::string * Affichage = new std::string(buff);
		
		affiche( Affichage ); 
		}		break;
	
	case 9:		{
		WindowsManager::getInstance().swapVisible();
		}		break;
	// DEL
	case 8:		{
		supChar();
		}		break;

	//SUP
	case 127:
	default :		{
		std::cout << (int)key << std::endl;
		addChar(  key ); 
		}		break;
	}
}
	

void PanelConsole::cb_keyboard_special( unsigned char key ) {
	cout << "PanelConsole::cb_keyboard_special( "<< (unsigned int) key <<" ) "<< endl;;
	#ifdef DEBUG
	#endif
	switch(key){ 
	// left
	case 100:	{
		if ( currentPos > 0 )			currentPos--;
		moveCursor();
	}	break;
	// up
	case 101:	{
	}	break;
	// right
	case 102:	{
		int max = texts[currentLine]->getText().size() - prompt.size();
		if ( currentPos < max )			currentPos++;
		moveCursor();
	}	break;
	// down
	case 103:	{
	}	break;
	// pgup
	case 104:	{
	}	break;
	// pgdown
	case 105:	{
	}	break;
	// home
	case 106:	{
		currentPos = 0;
		moveCursor();
	}	break;
	// end
	case 107:	{
		currentPos = texts[currentLine]->getText().size() - prompt.size();
		moveCursor();
	}	break;
	// ins
	case 108:	{
		bIns = ! bIns;
		if ( bIns )
			cursor.changeText( "|", PanelText::NORMAL_FONT, true );
		else
			cursor.changeText( "_", PanelText::NORMAL_FONT, true );
	}	break;
	default :		{
		std::cout << "Velue : "<< (int)key << std::endl;
		}		break;
	}
}
	


//--------------------------------------------------------------------------------------------------------------------
//
//						Virtuals fonctions....
//
//--------------------------------------------------------------------------------------------------------------------
void PanelConsole::idle(float elapsedTime) {
	//cout << "ElapsedTime : "<< elapsedTime << endl;
	//cout << "cursorTime  : "<< cursorTime << endl;
	
	if ( cursorTime >= 0.0 )	{
		cursorTime += elapsedTime;
		if ( cursorTime >= 0.2f )		{
			cursor.setVisible( !cursor.getVisible() );
			cursorTime -= 0.2f;
		}
	}
	else	{
		cursorTime = 0.0f;
	}
}
	


void PanelConsole::updatePos() {
	#ifdef DEBUG
	cout << "PanelConsole::updatePos()" << endl;;
	#endif
	PanelSimple::updatePos();
}
	


void PanelConsole::displayGL() {
	if (visible == false)			return;

	PanelSimple::displayGL();

#ifdef DEBUG
	cout << "PanelConsole::displayGL()" << endl;;
	WindowsManager& wm = WindowsManager::getInstance();
	cout << "PC displayGL ... fenetre id : " << getID() << endl;
	cout << "    PC:" << "-------------------" << endl;
	cout << "    PC:" << getX() <<", "<< getY() <<", "<< getDX() <<", "<< getDY() << endl;
	cout << "    PC:" << wm.getWidth() <<", "<< wm.getHeight() << endl;
#endif
	
}




