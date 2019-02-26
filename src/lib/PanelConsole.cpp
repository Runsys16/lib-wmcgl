#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "PanelConsole.h"
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
PanelConsole::PanelConsole()	{
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelConsole() ..." << endl;
	#endif
	
	PanelSimple();
	tabSize = 40;
}


PanelConsole::PanelConsole(int nbLine, int max)	{
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelConsole( "<< nbLine << " ) ..." << endl;
	#endif
	
	PanelConsole();

	prompt = "";
	maxCmd = max;
	currentLine = 0;
	currentPos = 0;
	currentCmd = 0;
	cursorTime = -1.0f;

	cb_cmd = NULL;
	ppccb = NULL;
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
	
	WindowsManager::getInstance().call_back_keyboard( this );
	//cursor.setPos( currentPos, 15*currentLine );

	cursor.changeText( "|", PanelText::NORMAL_FONT, true );
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

void PanelConsole::setCallBackCmd( PanelConsoleCallBack* p )		{
	ppccb = p;
}




void PanelConsole::moveCursor()	{
	int pl = prompt.size();
	int x = texts[currentLine]->getTextLenght(pl+currentPos);
	int y = texts[currentLine]->getPosY();//TextLenght(pl+currentPos);
	//int y = currentLine*heightLine;

	cursor.setPos( x, y );
}





void PanelConsole::scroll() {
	if ( texts[currentLine]->getPosY() < (getDY()-heightLine+1) )	{
		return;
	}
	#ifdef DEBUG
	cout << "PanelConsole::scroll()"<< endl;
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


void PanelConsole::rotateBuffer() {
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


void PanelConsole::addLine() {
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
	
	moveCursor();
}


void PanelConsole::eraseTexts( ) {
	currentLine = 0;
	currentPos = 0;
	
	int nb = texts.size();
	for (int i=0; i<nb; i++ )	{
		texts[i]->eraseText();
	}
	texts[currentLine]->changeText( prompt, PanelText::NORMAL_FONT, true );
	moveCursor();
}




void PanelConsole::clear( ) {
	//currentLine = 0;
	//currentPos = 0;
	
	int deltaY = texts[currentLine]->getPosY();
	int nb = texts.size();
	for (int i=0; i<nb; i++ )	{
		int y = texts[i]->getPosY();
		
		texts[i]->setPosY(y-deltaY);
	}
		
	texts[currentLine]->changeText( prompt, PanelText::NORMAL_FONT, true );
	moveCursor();
}


//--------------------------------------------------------------------------------------------------------------------
//
//						Callback keyboard functions....
//
//--------------------------------------------------------------------------------------------------------------------

void PanelConsole::setColor( long c ) {
    color = c;
    
    for ( int i=0; i<texts.size(); i++ )
    {
	    texts[i]->setColor( c );
	}
	
}






void PanelConsole::affiche( string* str ) {
	texts[currentLine]->changeText(*str, PanelText::NORMAL_FONT, true );
	texts[currentLine]->setTabSize( tabSize );
	for( int i=0; i<texts.size(); i++ )	{
		texts[i]->setTabSize( tabSize );
		texts[i]->setColor( color );
	}
	addLine();
}




void PanelConsole::affiche( string str ) {
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
//						fonctions d'editions
//
//--------------------------------------------------------------------------------------------------------------------

void PanelConsole::addChar( char c ) {
	#ifdef DEBUG
	cout <<"PanelConsole::addChar() "<< endl;
	cout <<"  currentPos   : \""<< currentPos <<"\""<< endl;;
	cout <<"  currentLine  : \""<< currentLine <<"\""<< endl;;
	#endif
	if ( currentLine == texts.size() )	{
		#ifdef DEBUG
		cout <<"PanelConsole::addChar() *** max texts *** "<< endl;
		#endif
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
		cout << "  left  : \""<< left <<"\""<< endl;;
		cout << "  right : \""<< right <<"\""<< endl;;
		
		char buff[2048];
		sprintf( buff, "%s%c%s", left, c, right );
		val = new string(buff);
	}
	
	
	texts[currentLine]->changeText( *val, PanelText::NORMAL_FONT, true );
    #ifdef DEBUG
	cout <<"PanelConsole::addChar() "<< *val << endl;
	#endif
	//cout <<"  val             : \""<< *val <<"\""<< endl;;
	//cout <<"  texts[current]  : \""<< texts[currentLine]->getText() <<"\""<< endl;;
	delete val;
	incCursor();
}











void PanelConsole::delChar() {
	#ifdef DEBUG
	cout << "PanelConsole::delChar()" << endl;
	#endif

	if ( currentLine >= texts.size() )	{

    	#ifdef DEBUG
		cout <<"PanelConsole::delChar() *** max texts *** "<< endl;
    	#endif
		return;
	}
	if ( currentPos <= 0  )	{
		cout << "  currentPos NULL" << endl;;
		return;
	}
	string str = texts[currentLine]->getText();

	#ifdef DEBUG
	cout <<"  str =  \""<< str <<"\""<< endl;;
	#endif
	
	char buff[2048];
	
	
	
	if ( prompt.size() >= str.size() )	{

    	#ifdef DEBUG
		cout << "  *** Debut de ligne *** "<< endl;
    	#endif
		return;
	}
	
	// prompt lenght
	int pl = prompt.size();
	// command lenght
	int cl = str.size() - pl;
	
	string * val;

	


	if ( currentPos == cl )	{
		str.copy( buff, str.size()-1, 0);
    	#ifdef DEBUG
		cout << "  buff : \""<< buff <<"\""<< endl;;
    	#endif

		buff[str.size()-1] = 0;
		val = new string(buff);

    	#ifdef DEBUG
		cout << "  val : \""<< *val <<"\""<< endl;;
    	#endif
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

    	#ifdef DEBUG
		cout << "  left  : \""<< left <<"\""<< endl;;
		cout << "  right : \""<< right <<"\""<< endl;;
    	#endif
		
		char buff[2048];
		sprintf( buff, "%s%s", left, right );
		val = new string(buff);
	}


	texts[currentLine]->changeText( *val, PanelText::NORMAL_FONT, true );
	delete val;
	decCursor();

}




void PanelConsole::supChar() {
	#ifdef DEBUG
	cout << "PanelConsole::supChar()" << endl;
	#endif

	if ( currentLine >= texts.size() )	{

    	#ifdef DEBUG
		cout <<"PanelConsole::supChar() *** max texts *** "<< endl;
    	#endif

		return;
	}

	string str = texts[currentLine]->getText();

	#ifdef DEBUG
	cout << "  str : \""<< str <<"\""<< endl;;
	#endif
	
	
	
	
	if ( prompt.size() >= str.size() )	{
    	#ifdef DEBUG
		cout << "  *** Debut de ligne *** "<< endl;
    	#endif
		return;
	}
	
	// prompt lenght
	int pl = prompt.size();
	// command lenght
	int cl = str.size() - pl;
	
	string * val;

	
	#ifdef DEBUG
	cout << "  CurretnPos = "<< currentPos <<" , cl = "<< cl<< endl;
	#endif

	if ( prompt.size() >= str.size() )	{
        #ifdef DEBUG
		cout << " *** Debut de ligne *** "<< endl;
		#endif
		return;
	}
	

	if ( currentPos >= cl )	{
        #ifdef DEBUG
		cout << "  *** Fin de ligne *** "<< endl;
		#endif
		return;
	}
	else	{
		char left[2048];
		char right[2048];
		char buff[2048];
		
		int ll = pl+currentPos;
		int rl = str.size()-(pl+currentPos) -1;

		str.copy( left, ll, 0 );
		str.copy( right, rl, pl+currentPos+1 );

		left[ll] = 0;
		right[rl] = 0;


        #ifdef DEBUG
		cout << "  left  : \""<< left <<"\""<< endl;;
		cout << "  right : \""<< right <<"\""<< endl;;
		#endif
		
		sprintf( buff, "%s%s", left, right );
		val = new string(buff);
	}
	
	

	
	texts[currentLine]->changeText( *val, PanelText::NORMAL_FONT, true );
	delete val;
	//decCursor();

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
	string  pT = texts[currentLine]->getText();
	int pl = prompt.size();
	int l = pT.size();
	char cmd[2048];

	// extrait la partie commande
	// string cmd	
	pT.copy( cmd, l-pl, pl );
	cmd[l-pl] = 0;

	int i = currentPos;
	if ( (i) >= (l-pl) )		return l-pl;

	// tant que le caratere suivant est ' ' ou '\t'
	while( cmd[i] == ' ' || cmd[i] == '\t' )	{
		i++;
		if ( (i) >= (l-pl) )	return i;
	}
	// tant que le caratere suivant n'est pas ' ' et '\t'
	while( cmd[i] != ' ' && cmd[i] != '\t' )	{
		i++;
		if ( (i) >= (l-pl) )	return i;
	}
	
	return i;
}



int PanelConsole::posWordPrec()	{
	int prevCursor = currentPos;
	string pT = texts[currentLine]->getText();
	int pl = prompt.size();
	int l = pT.size();
	char cmd[2048];

	// extrait la partie commande
	// string cmd	
	pT.copy( cmd, l-pl, pl );
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
	currentPos = posWordSuiv();
	moveCursor();
}



void PanelConsole::supWord()	{
    #ifdef DEBUG
	cout << "PanelConsole::supWord()" << endl;
	#endif
	
	int rightPos = posWordSuiv();

	if ( currentLine >= texts.size() )	{
		return;
	}

	string str = texts[currentLine]->getText();

	#ifdef DEBUG
	cout << "  str : \""<< str <<"\""<< endl;;
	#endif
	
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

    #ifdef DEBUG
	cout << "  left  : \""<< left <<"\""<< endl;;
	cout << "  right : \""<< right <<"\""<< endl;;
	#endif
	
	char buff[2048];
	sprintf( buff, "%s%s", left, right );

	val = new string(buff);
	texts[currentLine]->changeText( *val, PanelText::NORMAL_FONT, true );
	delete val;
}
//	moveCursor();


void PanelConsole::delWord()	{
	currentPos = posWordPrec();
	supWord();
	moveCursor();
}


//--------------------------------------------------------------------------------------------------------------------
//
//						Callback keyboard functions....
//
//--------------------------------------------------------------------------------------------------------------------
void PanelConsole::addCmd( string cmd ) {
	#ifdef DEBUG
	cout << "PanelConsole::addCmd( \""<< cmd <<"\" ) "<< endl;;
	#endif
	if ( cmds.size() == maxCmd )	{

		#ifdef DEBUG
		cout << "  delete cmd[0]  max = "<< maxCmd << endl;;
		#endif

    	delete cmds[0];
		cmds.erase( cmds.begin()+0 );
		currentCmd--;
	}
	
	string * pStr = new string( cmd );
	cmds.push_back( pStr );

	#ifdef DEBUG
	cout << "  CurrentCmd : "<< currentCmd << endl;;
    #endif
    
	currentCmd++;

	#ifdef DEBUG
	cout << "  CurrentCmd : "<< currentCmd << endl;;
	#endif
}


void PanelConsole::cb_keyboard( unsigned char key ) {
	//if ( !bVisible )		return;
	#ifdef DEBUG
	cout << "PanelConsole::cb_keyboard( "<< (unsigned int) key <<" ) "<< endl;;
	#endif

	static int n=0;
	switch(key){ 
	case '\r':		{
		if ( cb_cmd || ppccb )	{
			string* str = new string(texts[currentLine]->getText());
			char		cmd[2048];
			int l = str->size()-prompt.size();
			str->copy( cmd, str->size()-prompt.size(), prompt.size() );
			cmd[l] = 0;

    	#ifdef DEBUG
			cout << "PanelConsole::cb_keyboard( "<< (unsigned int) key <<" ) "<< endl;;
			cout << "  str : \""<< *str <<"\""<< endl;
			cout << "  Size(s) str : "<< str->size() <<" prompt :"<< prompt.size() << endl;
			cout << "  Exec command "<< cmd << endl;
	    #endif
			
			addLine();
			currentCmd = cmds.size();
			addCmd( cmd );
			if ( cb_cmd )	(*cb_cmd)(cmd);
			if ( ppccb )	(ppccb->callback_cmd)(cmd);
			delete str;
		}
		else {
			addLine();
		}
		}	
		break;
	// ESC
	case 27:		{
		}		
		break;

	//² 
	case 178:		{
		char buff[80];
		sprintf( buff, "  Essai de console %d ...", n++ );
		std::string * Affichage = new std::string(buff);
		
		affiche( Affichage ); 
		}		
		break;

	//tab	
	/*
	case 9:		{
		WindowsManager::getInstance().swapVisible();
		}		
		break;
	*/
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
		#ifdef DEBUG
		std::cout << "  Value : "<< (int)key << std::endl;
		#endif
		addChar(  key ); 
		}		
		break;
	}
}
	

void PanelConsole::cb_keyboard_special( unsigned char key ) {
	//if ( !bVisible )		return;
	#ifdef DEBUG
	cout << "PanelConsole::cb_keyboard_special( "<< (unsigned int) key <<" ) "<< endl;;
	#endif

	switch(key){ 
	//  Right Ctrl
	case 250:	{
		bRightCtrl = true;
		}	
		break;
	//  Left Ctrl
	case 114:	{
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
			if ( currentCmd == cmds.size() )	{
				svgCmd = texts[currentLine]->getText();
			}
	
			if ( (currentCmd) > 0 )	{
				currentCmd--;
				string newCmd = prompt + *(cmds[currentCmd]);
				texts[currentLine]->changeText( newCmd, PanelText::NORMAL_FONT, true );
				currentPos = 0;
				moveCursor();
			}
        	#ifdef DEBUG
			cout << "  CurrentCmd : "<< currentCmd<<"/"<< cmds.size() << endl;;
			#endif
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
			if ( (currentCmd) != cmds.size() )					currentCmd++;
			
			if ( (currentCmd) == cmds.size() )	{
				texts[currentLine]->changeText( svgCmd, PanelText::NORMAL_FONT, true );
				currentPos = svgCmd.size()-prompt.size();
				moveCursor();
			}
			else{
				//currentCmd++;
				string newCmd = prompt + *(cmds[currentCmd]);
				texts[currentLine]->changeText( newCmd, PanelText::NORMAL_FONT, true );
				currentPos = 0;
				moveCursor();
			}
        	#ifdef DEBUG
			cout << "  CurrentCmd : "<< currentCmd <<"/"<< cmds.size() << endl;;
			#endif
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
			cursor.changeText( "_", PanelText::NORMAL_FONT, true );
		else
			cursor.changeText( "|", PanelText::NORMAL_FONT, true );
		}
		break;
	default :		{
    	#ifdef DEBUG
		std::cout << "  Value : "<< (int)key << std::endl;
		#endif
		}
		break;
	}
	
	if ( bRightCtrl || bLeftCtrl ) 	bCtrl = true;
	//else							bCtrl = false;
	#ifdef DEBUG
	cout <<"  bCtrl= "<< std::boolalpha << bCtrl <<" bRightCtrl="<< bRightCtrl <<" bLeftCtrl="<< bLeftCtrl << endl;
	#endif
}
	


void PanelConsole::cb_keyboard_special_up( unsigned char key ) {
	//if ( !bVisible )		return;
	#ifdef DEBUG
	cout << "PanelConsole::cb_keyboard_special_up( "<< (unsigned int) key <<" ) "<< endl;;
	#endif
	switch(key){ 
	//  Right Ctrl
	case 250:	{
		bRightCtrl = false;
		}	
		break;
	//  Left Ctrl
	case 114:	{
		bLeftCtrl = false;
		}	
		break;
	default :		{
		#ifdef DEBUG
		std::cout << "  Value : "<< (int)key << std::endl;
		#endif
		}
		break;
	}
	
	if ( bRightCtrl || bLeftCtrl ) 	bCtrl = true;
	else							bCtrl = false;

	#ifdef DEBUG
	cout <<"  bCtrl= "<< std::boolalpha << bCtrl <<" bRightCtrl="<< bRightCtrl <<" bLeftCtrl="<< bLeftCtrl << endl;
	#endif
}
	

void PanelConsole::setTabSize( int t ) {
	texts[currentLine]->setTabSize( t );
	tabSize = t;
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
	cout << "  PC displayGL ... fenetre id : " << getID() << endl;
	cout << "  PC:" << "-------------------" << endl;
	cout << "  PC:" << getX() <<", "<< getY() <<", "<< getDX() <<", "<< getDY() << endl;
	cout << "  PC:" << wm.getWidth() <<", "<< wm.getHeight() << endl;
#endif
	
}




