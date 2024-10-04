#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "PanelEditText.h"
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
PanelEditText::PanelEditText()	{
	#ifdef DEBUG_CONST
	cout << "Constructeur PanelEditText() ..." << endl;
	#endif
	
	PanelSimple();

	tabSize = 40;
	prompt = "";
	//currentLine = 0;
	currentPos = 0;
	//currentCmd = 0;
	cursorTime = -1.0f;

	cb_cmd = NULL;
	ppccb = NULL;
	bIns = false;
	bCtrl = false;
	bRightCtrl = false;
	bLeftCtrl = false;
	bFocus = false;
	bHideCursor = false;

	heightLine = 13;
			
	
	WindowsManager::getInstance().call_back_keyboard( this );
	//cursor.setPos( currentPos, 15*currentLine );

    text =  new PanelText();
    add(text);
	text->changeText((char*)"");
	cursor.changeText( "|", PanelText::NORMAL_FONT, true );
	this->add( &cursor );
	moveCursor();
	
	ID = 9010;

}
//--------------------------------------------------------------------------------------------------------------------
//
//						fonctions....
//
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::setCallBackCmd( CB_CMD f )		{
	cb_cmd = f;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::setCallBackCmd( PanelEditTextCallBack* p )		{
	ppccb = p;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::moveCursor()	{
	int pl = prompt.size();
	int x = text->getTextLenght(pl+currentPos);
	int y = text->getPosY();//TextLenght(pl+currentPos);
	//int y = currentLine*heightLine;

	cursor.setPos( x, y );
}
//--------------------------------------------------------------------------------------------------------------------
//
//						Callback keyboard functions....
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::setColor( long c ) {
    color = c;
    
    text->setColor( c );
}
//--------------------------------------------------------------------------------------------------------------------
//
//						fonctions d'editions
//
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::addChar( char c ) {
	#ifdef DEBUG
	cout <<"PanelEditText::addChar() "<< endl;
	cout <<"  currentPos   : \""<< currentPos <<"\""<< endl;;
	#endif

	string str = text->getText();
	// prompt lenght
	int pl = prompt.size();
	pl = 0;
	// command lenght
	int cl = str.size() - pl;
	
	string * val;
	if ( currentPos == cl )	{
		char buff[2048];
		sprintf( buff, "%s%c", str.c_str(), c );
		val = new string(buff);
	}
	else	{
		char left[1000];
		char right[1000];
		
		int ll = pl+currentPos;
		int rl = str.size()-(pl+currentPos);

		str.copy( left, ll, 0);
		str.copy( right, rl, pl+currentPos);

		left[ll] = 0;
		right[rl] = 0;
		cout << "  left  : \""<< left <<"\""<< endl;;
		cout << "  right : \""<< right <<"\""<< endl;;
		
		char buff[2048];
		snprintf( buff, sizeof(buff), "%s%c%s", left, c, right );
		val = new string(buff);
	}
	
	
	text->changeText( *val, PanelText::NORMAL_FONT, true );
    #ifdef DEBUG
	cout <<"PanelEditText::addChar() "<< *val << endl;
	#endif
	//cout <<"  val             : \""<< *val <<"\""<< endl;;
	//cout <<"  text  : \""<< text->getText() <<"\""<< endl;;
	delete val;
	incCursor();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::delChar() {
	#ifdef DEBUG
	cout << "PanelEditText::delChar()" << endl;
	#endif

	if ( currentPos <= 0  )	{
		cout << "  currentPos NULL" << endl;;
		return;
	}
	string str = text->getText();

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
		char left[1000];
		char right[1000];
		
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
		snprintf( buff, sizeof(buff), "%s%s", left, right );
		val = new string(buff);
	}


	text->changeText( *val, PanelText::NORMAL_FONT, true );
	delete val;
	decCursor();

}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::supChar() {
#define DEBUG
	#ifdef DEBUG
	cout << "PanelEditText::supChar()" << endl;
	#endif

	string str = text->getText();

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
		char left[1000];
		char right[1000];
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
		
		snprintf( buff, sizeof(buff), "%s%s", left, right );
		val = new string(buff);
	}
	
	

	
	text->changeText( *val, PanelText::NORMAL_FONT, true );
	delete val;
	//decCursor();
#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::incCursor()	{
	// prompt lenght
	int pl = prompt.size();
	// command lenght
	int cl = text->getText().size() - pl;

	if ( currentPos < cl )	{
		currentPos++;
		moveCursor();
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::decCursor()	{
	if ( currentPos > 0 )	{
		currentPos--;
		moveCursor();
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
int PanelEditText::posWordSuiv()	{
	int prevCursor = currentPos;
	string  pT = text->getText();
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
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
int PanelEditText::posWordPrec()	{
	int prevCursor = currentPos;
	string pT = text->getText();
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
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::wordPrec()	{
	currentPos = posWordPrec();
	moveCursor();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::wordSuiv()	{
	currentPos = posWordSuiv();
	moveCursor();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::supWord()	{
    #ifdef DEBUG
	cout << "PanelEditText::supWord()" << endl;
	#endif
	
	int rightPos = posWordSuiv();

	string str = text->getText();

	#ifdef DEBUG
	cout << "  str : \""<< str <<"\""<< endl;;
	#endif
	
	int pl = prompt.size();							// prompt size
	int cl = str.size() - pl;						// command size
	
	string *	val;
	char		left[1000];
	char		right[1000];
	
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
	snprintf( buff, sizeof(buff), "%s%s", left, right );

	val = new string(buff);
	text->changeText( *val, PanelText::NORMAL_FONT, true );
	delete val;
}
//	moveCursor();


void PanelEditText::delWord()	{
	currentPos = posWordPrec();
	supWord();
	moveCursor();
}
//--------------------------------------------------------------------------------------------------------------------
//
//						Callback keyboard functions....
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::cb_keyboard( unsigned char key ) {
//#define DEBUG
	//if ( !bVisible )		return;
	#ifdef DEBUG
	cout << "PanelEditText::cb_keyboard( "<< (unsigned int) key <<" ) "<< endl;;
	#endif
    
    WindowsManager& wm = WindowsManager::getInstance();
	Panel* parent = wm.getParentRoot(this);

	#ifdef DEBUG
	cout << " bFocus"<< (bFocus ? "True" : "False") << endl;;
	#endif

    if (!bFocus)                    return;
    if (!parent->getVisible())      return;
    
    
	static int n=0;
	switch(key){ 
	case '\r':		{
		if ( cb_cmd || ppccb )	{
			string* str = new string(text->getText());
			char		cmd[2048];
			int l = str->size()-prompt.size();
			str->copy( cmd, str->size()-prompt.size(), prompt.size() );
			cmd[l] = 0;

    	#ifdef DEBUG
			cout << "PanelEditText::cb_keyboard( "<< (unsigned int) key <<" ) "<< endl;;
			cout << "  str : \""<< *str <<"\""<< endl;
			cout << "  Size(s) str : "<< str->size() <<" prompt :"<< prompt.size() << endl;
			cout << "  Exec command "<< cmd << endl;
	    #endif
			
			delete str;
		}
		else {
			//addLine();
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
		
		//affiche( Affichage ); 
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
		if ( key >= ' ')		addChar(  key ); 
		}		
		break;
	}
//#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::cb_keyboard_special( unsigned char key ) {
	//if ( !bVisible )		return;
	#ifdef DEBUG
	cout << "PanelEditText::cb_keyboard_special( "<< (unsigned int) key <<" ) "<< endl;;
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
    /*
	case 101:	{
			if ( currentCmd == cmds.size() )	{
				svgCmd = text->getText();
			}
	
			if ( (currentCmd) > 0 )	{
				currentCmd--;
				string newCmd = prompt + *(cmds[currentCmd]);
				text->changeText( newCmd, PanelText::NORMAL_FONT, true );
				currentPos = 0;
				moveCursor();
			}
        	#ifdef DEBUG
			cout << "  CurrentCmd : "<< currentCmd<<"/"<< cmds.size() << endl;;
			#endif
		}
		break;
	*/
	// right
	case 102:	{
		if ( bCtrl )	wordSuiv();
		else			incCursor();
		}
		break;
	// down
	/*
	case 103:	{
			if ( (currentCmd) != cmds.size() )					currentCmd++;
			
			if ( (currentCmd) == cmds.size() )	{
				text->changeText( svgCmd, PanelText::NORMAL_FONT, true );
				currentPos = svgCmd.size()-prompt.size();
				moveCursor();
			}
			else{
				//currentCmd++;
				string newCmd = prompt + *(cmds[currentCmd]);
				text->changeText( newCmd, PanelText::NORMAL_FONT, true );
				currentPos = 0;
				moveCursor();
			}
        	#ifdef DEBUG
			cout << "  CurrentCmd : "<< currentCmd <<"/"<< cmds.size() << endl;;
			#endif
		}	
		break;
	*/
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
		currentPos = text->getText().size() - prompt.size();
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
	


void PanelEditText::cb_keyboard_special_up( unsigned char key ) {
	//if ( !bVisible )		return;
	#ifdef DEBUG
	cout << "PanelEditText::cb_keyboard_special_up( "<< (unsigned int) key <<" ) "<< endl;;
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
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::setTabSize( int t ) {
	text->setTabSize( t );
	tabSize = t;
}
//--------------------------------------------------------------------------------------------------------------------
//
//						Virtuals fonctions....
//
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::idle(float elapsedTime) {

	//cout <<"PanelEditText::idle( "<< elapsedTime <<" )"<< endl;
	//cout << "cursorTime  : "<< cursorTime << endl;
	if ( bHideCursor  )	{
		cursor.setVisible( false );
		return;
	}
	if ( !bFocus )
	{
	    cursor.setVisible( false );
	    return;
	}

	//-----------------------------------------------------------
	cursorTime += elapsedTime;
	if ( cursorTime >= 0.5f )		{
		cursor.setVisible( !cursor.getVisible() );
		cursorTime -= 0.5f;
	}

}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::updatePos() {
	#ifdef DEBUG
	cout << "PanelEditText::updatePos()" << endl;;
	#endif
	PanelSimple::updatePos();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::displayGL() {
	if (visible == false)			return;

	PanelSimple::displayGL();

#ifdef DEBUG
	cout << "PanelEditText::displayGL()" << endl;;
	WindowsManager& wm = WindowsManager::getInstance();
	cout << "  PC displayGL ... fenetre id : " << getID() << endl;
	cout << "  PC:" << "-------------------" << endl;
	cout << "  PC:" << getX() <<", "<< getY() <<", "<< getDX() <<", "<< getDY() << endl;
	cout << "  PC:" << wm.getWidth() <<", "<< wm.getHeight() << endl;
#endif
	
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------



