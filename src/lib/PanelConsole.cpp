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

	cb_cmd = NULL;
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

void PanelConsole::setCallBackCmd( CB_CMD f )		{
	cb_cmd = f;
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
	incCursor();
}











void PanelConsole::delChar() {
	if ( currentLine >= texts.size() )	{
		return;
	}
	if ( currentPos <= 0  )	{
		cout << "PanelConsole::delChar() : currentPos NULL" << endl;;
		return;
	}
	string str = texts[currentLine]->getText();
	cout << "PanelConsole::delChar() : \""<< str <<"\""<< endl;;
	#ifdef DEBUG
	#endif
	
	char buff[2048];
	
	
	
	if ( prompt.size() >= str.size() )	{
		cout << "PanelConsole::delChar() : *** Debut de ligne *** "<< endl;
		return;
	}
	
	// prompt lenght
	int pl = prompt.size();
	// command lenght
	int cl = str.size() - pl;
	
	string * val;

	


	if ( currentPos == cl )	{
		str.copy( buff, str.size()-1, 0);
		cout << "PanelConsole::delChar() : \""<< buff <<"\""<< endl;;
		buff[str.size()-1] = 0;
		val = new string(buff);
		cout << "PanelConsole::delChar() : \""<< *val <<"\""<< endl;;
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
		cout << "PanelConsole::delChar()  left  : \""<< left <<"\""<< endl;;
		cout << "PanelConsole::delChar()  right : \""<< right <<"\""<< endl;;
		
		char buff[2048];
		sprintf( buff, "%s%s", left, right );
		val = new string(buff);
	}


	texts[currentLine]->changeText( *val, PanelText::NORMAL_FONT, true );
	delete val;
	decCursor();

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
		char buff[2048];

		str.copy( buff, str.size()-1, 0);
		cout << "PanelConsole::supChar() : \""<< buff <<"\""<< endl;;
		buff[str.size()-1] = 0;
		val = new string(buff);
		cout << "PanelConsole::supChar() : \""<< *val <<"\""<< endl;;
	}
	else	{
		char left[2048];
		char right[2048];
		char buff[2048];
		
		int ll = pl+currentPos;
		int rl = str.size()-(pl+currentPos) -1;

		str.copy( left, ll, 0 );
		str.copy( right, rl, pl+currentPos )+1;

		left[ll] = 0;
		right[rl] = 0;
		cout << "PanelConsole::supChar()  left  : \""<< left <<"\""<< endl;;
		cout << "PanelConsole::supChar()  right : \""<< right <<"\""<< endl;;
		
		sprintf( buff, "%s%s", left, right );
		val = new string(buff);
	}
	
	

	
	texts[currentLine]->changeText( *val, PanelText::NORMAL_FONT, true );
	delete val;
	decCursor();

}




void PanelConsole::incCursor()	{
	// prompt lenght
	int pl = prompt.size();
	// command lenght
	int cl = texts[currentLine]->getText().size() - pl;

	if ( currentPos < cl )	{
		currentPos++;
		moveCursor();
	}
}



void PanelConsole::decCursor()	{
	if ( currentPos > 0 )	{
		currentPos--;
		moveCursor();
	}
}


int PanelConsole::posWordSuiv()	{
	int prevCursor = currentPos;
	string * pT = texts[currentLine];
	int pl = prompt.size();
	int l = pT->getText().size();
	char cmd[2048];

	// extrait la partie commande
	// string cmd	
	pT->getText().copy( cmd, l-pl, pl );
	cmd[l-pl] = 0;

	int i = currentPos;
	if ( (i+1) >= (l-pl) )		return l-pl;

	// tant que le caratere suivant est ' ' ou '\t'
	while( cmd[i+1] == ' ' || cmd[i+1] == '\t' )	{
		i++;
		if ( (i+1) >= (l-pl) )	return i;
	}
	// tant que le caratere suivant n'est pas ' ' et '\t'
	while( cmd[i+1] != ' ' && cmd[i+1] != '\t' )	{
		i++;
		if ( (i+1) >= (l-pl) )	return i;
	}
	
	return i;
}



int PanelConsole::posWordPrec()	{
	int prevCursor = currentPos;
	string * pT = texts[currentLine];
	int pl = prompt.size();
	int l = pT->getText().size();
	char cmd[2048];

	// extrait la partie commande
	// string cmd	
	pT->getText().copy( cmd, l-pl, pl );
	cmd[l-pl] = 0;

	int i = currentPos;
	if ( (i-1) <  0 )		return 0;

	// tant que le caratere suivant est ' ' ou '\t'
	while( cmd[i-1] == ' ' || cmd[i-1] == '\t' )	{
		i--;
		if ( (i-1) < 0 )	return i;
	}
	// tant que le caratere suivant n'est pas ' ' et '\t'
	while( cmd[i-1] != ' ' && cmd[i-1] != '\t' )	{
		i--;
		if ( (i-1) < 0 )	return i;
	}
	
	return i;
}



void PanelConsole::wordPrec()	{
	currentPos = posWordPrec();
	moveCursor();
}


void PanelConsole::wordSuiv()	{
	currentPos = posWordPrec();
	moveCursor();
}



void PanelConsole::supWord()	{
	rightPos = posWordPrec();

	if ( currentLine >= texts.size() )	{
		return;
	}

	#ifdef DEBUG
	cout << "PanelConsole::supWord() : \""<< str <<"\""<< endl;;
	#endif
	
	string str = texts[currentLine]->getText();
	int pl = prompt.size();							// prompt size
	int cl = str.size() - pl;						// command size
	
	string *	val;
	char		left[2048];
	char		right[2048];
	
	int ll = pl+currentPos;
	int rl = str.size()-(pl+rightPos);

	str.copy( left, ll, 0 );
	str.copy( right, rl, pl+rightPos );

	left[ll] = 0;
	right[rl] = 0;
	cout << "PanelConsole::supWord()  left  : \""<< left <<"\""<< endl;;
	cout << "PanelConsole::supWord()  right : \""<< right <<"\""<< endl;;
	
	char buff[2048];
	sprintf( buff, "%s%s", left, right );

	val = new string(buff);
	texts[currentLine]->changeText( *val, PanelText::NORMAL_FONT, true );
	delete val;
}
	moveCursor();


void PanelConsole::delWord()	{
	currentPos = posWordPrec();
	moveCursor();
}



void PanelConsole::moveCursor()	{
	int pl = prompt.size();
	int x = texts[currentLine]->getTextLenght(pl+currentPos);
	int y = currentLine*15;

	cursor.setPos( x, y );
}
//--------------------------------------------------------------------------------------------------------------------
//
//						Callback keyboard functions....
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
		if ( cb_cmd )		*cb_cmd();
		}	
		break;
	//² 
	case 178:		{
		char buff[80];
		sprintf( buff, "Essai de console %d ...", n++ );
		std::string * Affichage = new std::string(buff);
		
		affiche( Affichage ); 
		}		
		break;

	//tab	
	case 9:		{
		WindowsManager::getInstance().swapVisible();
		}		
		break;
	// DEL
	case 8:		{
		if ( bCtrl )	delWord();
		else			delChar();
		}
		break;

	//SUP
	case 127:		{
		if ( bCtrl )	supWord();
		else			supChar();
		}
		break;
	default :		{
		std::cout << (int)key << std::endl;
		addChar(  key ); 
		}		
		break;
	}
}
	

void PanelConsole::cb_keyboard_special( unsigned char key ) {
	cout << "PanelConsole::cb_keyboard_special( "<< (unsigned int) key <<" ) "<< endl;;
	#ifdef DEBUG
	#endif
	switch(key){ 
	//  Right Ctrl
	case 100:	{
		bRightCtrl = true;
		}	
		break;
	//  Left Ctrl
	case 100:	{
		bLeftCtrl = true;
		}	
		break;
	// left
	case 100:	{
		if ( bCtrl )	wordPrec();
		else			decCursor();
		}	
		break;
	// up
	case 101:	{
		}
		break;
	// right
	case 102:	{
		if ( bCtrl )	wordSuiv();
		else			incCursor();
		}
		break;
	// down
	case 103:	{
		}	
		break;
	// pgup
	case 104:	{
		}
		break;
	// pgdown
	case 105:	{
		}	
		break;
	// home
	case 106:	{
		currentPos = 0;
		moveCursor();
		}
		break;
	// end
	case 107:	{
		currentPos = texts[currentLine]->getText().size() - prompt.size();
		moveCursor();
		}
		break;
	// ins
	case 108:	{
		bIns = ! bIns;
		if ( bIns )
			cursor.changeText( "|", PanelText::NORMAL_FONT, true );
		else
			cursor.changeText( "_", PanelText::NORMAL_FONT, true );
		}
		break;
	default :		{
		std::cout << "Value : "<< (int)key << std::endl;
		}
		break;
	}
	
	if ( bRightCtrl || bLeftCtrl ) 	bCtrl = true;
	//else							bCtrl = false;
}
	


void PanelConsole::cb_keyboard_special_up( unsigned char key ) {
	cout << "PanelConsole::cb_keyboard_special_up( "<< (unsigned int) key <<" ) "<< endl;;
	#ifdef DEBUG
	#endif
	switch(key){ 
	//  Right Ctrl
	case 100:	{
		bRightCtrl = false;
		}	
		break;
	//  Left Ctrl
	case 101:	{
		bLeftCtrl = false;
		}	
		break;
	default :		{
		std::cout << "Value : "<< (int)key << std::endl;
		}
		break;
	}
	
	if ( bRightCtrl || bLeftCtrl ) 	bCtrl = true;
	else							bCtrl = false;
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




