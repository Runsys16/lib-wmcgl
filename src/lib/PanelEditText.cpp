#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "ResourceManager.h"
#include "PanelEditText.h"
#include "wm.h"
#include "macro.h"

//#define DEBUG
//#define DEBUG_CONST

#ifdef DEBUG_WM
#	define DEBUG
#endif


#ifdef DEBUG
#	define DEBUG_CONST
#endif

#define CURSOR_TIME 0.4f
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

	tabSize			= 40;
	prompt			= "";
	currentPos		= 0;
	cursorTime		= 0.0f;

	cb_cmd			= NULL;
	ppccb			= NULL;
	TextureInverse	= NULL;

	bIns			= false;
	bCtrl			= false;
	bRightCtrl		= false;
	bLeftCtrl		= false;
	bFocus			= false;
	bHideCursor		= false;
	bDxStr			= true;
	bCursor			= false;
	
	heightLine		= 13;
	
	WindowsManager::getInstance().call_back_keyboard( this );
	//cursor.setPos( currentPos, 15*currentLine );

    text =  new PanelText();
	text->changeText((char*)"");

	cursor.changeText( "|", PanelText::NORMAL_FONT, true );
	
	cursorInverse.setVisible(false);
	cursor.setVisible(false);

	add( &cursor );
    add(text);
	
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
void PanelEditText::setBackgroundInverse( char * str_background )	{
	if ( TextureInverse != NULL )		return;
	
	
	_ResourceManager& res = _ResourceManager::getInstance();

	_Texture2D* ret = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, str_background) );

	if ( ret )	TextureInverse = ret;
	else		logf_wm( (char*)"  [Error] WM - PanelSimple::setBackground( %s )", str_background );
		
	cursorInverse.setBackground( TextureInverse );

	add( &cursorInverse );
	cursor.onTop();
	text->onTop();
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
	cursorInverse.setPos( x, y );
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
//						Callback keyboard functions....
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::setColor( long c ) {
	#ifdef DEBUG
		logf_wm( (char*)"PanelEditText::setColor( 0x%8x ) : %s ", c, sExtra.c_str() );
	#endif

    color = c;
    
    text->setColor( c );
    cursor.setColor( c ^ 0xffFFff00 );
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
//#define DEBUG
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
//#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::incCursor()	{
	//logf_wm( (char*)"PanelEditText::incCursor()" );
	// prompt lenght
	int pl = prompt.size();
	// command lenght
	int cl = text->getText().size() - pl;

	if ( currentPos < cl )	{
		currentPos++;
		moveCursor();
	}
	
	cursorTime	= 0.0;
	bCursor		= true;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::decCursor()	{
	//logf_wm( (char*)"PanelEditText::decCursor()" );
	if ( currentPos > 0 )	{
		currentPos--;
		moveCursor();
	}
	cursorTime = 0.0;
	bCursor		= true;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::resetCursor()	{
	currentPos = 0;
	moveCursor();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
int PanelEditText::posWordSuiv()	{
    #ifdef DEBUG
		logf_wm( (char*)"PanelEditText::posWordSuiv() pl=%d", prompt.size() );
	#endif
	
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

	// tant que le caratere suivant est alphanum'
	//while( cmd[i] == ' ' || cmd[i] == '\t' )	{
	while( isAlphaNum(cmd[i+1]) )	{
		i++;
		if ( (i) >= (l-pl) )	return i;
	}
	i++;
	if ( (i) >= (l-pl) )	return i-1;

	return i;
	
	/*
	// tant que le caratere suivant n'est pas ' ' et '\t'
	//while( cmd[i] != ' ' && cmd[i] != '\t' )	{
	while( isAlphaNum(cmd[i+1]) )	{
		i++;
		if ( (i) >= (l-pl) )	return i;
	}
	
	return i;
	*/
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
int PanelEditText::posWordPrec()	{
    #ifdef DEBUG
		logf_wm( (char*)"PanelEditText::posWordPrec() pl=%d", prompt.size() );
	#endif

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

	// tant que le caratere precedant est alphanum'
	//while( cmd[i-1] == ' ' || cmd[i-1] == '\t' )	{
	while( isAlphaNum(cmd[i-1]) )	{
		i--;
		if ( (i-1) < 0 )	return i;
	}
	
	//if ( (i-1) < 0 )	return i+1;
	return i;
	/*
	// tant que le caratere suivant n'est pas ' ' et '\t'
	//while( cmd[i-1] != ' ' && cmd[i-1] != '\t' )	{
	while( isAlphaNum(cmd[i-1]) )	{
		i--;
		if ( (i-1) < 0 )	return i;
	}
	
	return i;
	*/
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
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::delWord()	{
	currentPos = posWordPrec();
	supWord();
	moveCursor();
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::majuscule()
{
	string& sText = text->getText();
	string newString;
	
	for( int i=0; i<sText.size(); i++ )
	{
		unsigned char c = sText[i]; 
		
		if ( isAlphaMin(c) )
		{
			c = c ^ 0x20;
			sText[i] = c;
		}
		
		newString = newString + sText[i];
	}

	text->changeText( newString );
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::minuscule()
{
	string& sText = text->getText();
	string newString;
	
	for( int i=0; i<sText.size(); i++ )
	{
		unsigned char c = sText[i]; 
		
		if ( isAlphaMaj(c) )
		{
			c = c | 0x20;
			sText[i] = c;
		}
		
		newString = newString + sText[i];
	}

	text->changeText( newString );
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
bool PanelEditText::isAlpha(unsigned char c)
{
	return ( (0x41<=c && c<=0x5A) || (0x61<=c && c<=0x7A) );
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
bool PanelEditText::isNum(unsigned char c)
{
	return (0x30<=c && c<=0x39);
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
bool PanelEditText::isAlphaNum(unsigned char c)
{
	return isAlpha(c) || isNum(c);
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
bool PanelEditText::isAlphaMaj(unsigned char c)
{
	return ( (0x41<=c && c<=0x5A) );
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------------------------------------
bool PanelEditText::isAlphaMin(unsigned char c)
{
	return ( (0x61<=c && c<=0x7A) );
}
//--------------------------------------------------------------------------------------------------------------------
//
//						Fonctions Virtuels
//
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::haveFocus()	
{
	bFocus = true;

	cursorInverse.setVisible(true);
	cursor.setVisible(true);
	
	cursorTime = 0.0;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::lostFocus()	
{
	bFocus = false;

	cursorInverse.setVisible(false);
	cursor.setVisible(false);
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Panel* PanelEditText::isMouseOver(int xm, int ym)
{
//#define DEBUG
	if ( !isVisible() || bFantome )		return NULL;

	
	#ifdef DEBUG
		logf_wm( (char*)"MouseOver '%s' = %d,%d %dx%d %s", sExtra.c_str(), x, y, dx_raw, dy_raw, BOOL2STR(bDxStr) );
	#endif


	#ifdef DEBUG
	if ( x_raw <= xm && xm <= (x_raw+dx_raw) && y_raw <= ym && ym <= (y_raw+dy_raw) )		
		{ logf_wm( (char*)"Mouse over '%s'", sExtra.c_str() ); return this; }
	#endif
	if ( x_raw <= xm && xm <= (x_raw+dx_raw) && y_raw <= ym && ym <= (y_raw+dy_raw) )		return this;
	else																					return NULL;
}
//--------------------------------------------------------------------------------------------------------------------
//
// Deplace le curseur sur un appuye souris
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::clickLeft( int x, int y ) 
{
	logf_wm( (char*)"PanelEditText::clickLeft(%d,%d)", x, y );
	logf_wm( (char*)"bCursor %s, TextSize %d, cursorTime %f)", BOOL2STR(bCursor), text->getText().size(), cursorTime );
	logf_wm( (char*)"bHideCursor %s, bFocus %s", BOOL2STR(bHideCursor), BOOL2STR(bFocus) );
	
	bCursor = true;
	
	if ( text->getText().size() == 0 )		{ currentPos = 0; return; }
	
	unsigned X = getX();		// x screen
	
	currentPos = text->getText().size();
	
	for( int i=0; i<text->getText().size(); i++ )
	{
		int l = text->getTextLenght(i);
		//logf_wm( (char*)"Comparaison %d ~ %d", X+l, x );
		if ( (X+l) > x )	
		{
			//logf_wm( (char*)"currentPos %d => %d", currentPos, i );
			
			//-----------------
			// resultat		
			if ( i >= 1 )				currentPos = i-1;
			else						currentPos = i;
					
			moveCursor();
			//-----------------
			break;
		}
	}
	
}
//--------------------------------------------------------------------------------------------------------------------
//
//						Callback keyboard functions....
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::cb_keyboard( unsigned char key ) {
	#ifdef DEBUG
		logf_wm( (char*)"PanelEditText::cb_keyboard( %d )", key );
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
	// CTRL+M	//\r
	case 13:		
		{
		majuscule();

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
	// CTRL+N
	case 14:		{
		minuscule();
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
			logf_wm( (char*)"key : %d - %c",  (int)key, key;
		#endif
		if ( key >= ' ')		addChar(  key ); 
		}		
		break;
	}
//#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::cb_keyboard_special( unsigned char key ) {
	#ifdef DEBUG
		logf_wm( (char*)"PanelEditText::cb_keyboard_special( %d )", key );
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
		//else			decCursor();
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
		//else			incCursor();
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
//--------------------------------------------------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::cb_keyboard_special_up( unsigned char key ) {
	//if ( !bVisible )		return;
	#ifdef DEBUG
		logf_wm( (char*)"PanelEditText::cb_keyboard_special_up( %d )", key );
	#endif
	
	switch(key){ 
	//  Left
	case 100:	{
		decCursor();
		}	
		break;
	//  Left
	case 102:	{
		incCursor();
		}	
		break;
	//  Fin
	case 107:	{
		incCursor();
		}	
		break;
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
void PanelEditText::idle(float elapsedTime) {
	if ( !isVisible() )			
	{
		//logf_wm( (char*)"PanelEditText::idle invisible %s", getExtraString().c_str() );
		return;
	}

	//cout <<"PanelEditText::idle( "<< elapsedTime <<" )"<< endl;
	//cout << "cursorTime  : "<< cursorTime << endl;
	if ( bHideCursor  )	{
		cursor.setVisible( false );
		//logf_wm( (char*)"idle %s LINE %d", sExtra.c_str(), __LINE__ );
		return;
	}
	/*
	*/
	if ( !bFocus )
	{
	    cursor.setVisible( false );
		//logf_wm( (char*)"idle %s LINE %d", sExtra.c_str(), __LINE__ );
		cursorTime = 0.0f;
	    return;
	}

	//-----------------------------------------------------------
	cursorTime += elapsedTime;
	if ( cursorTime >= CURSOR_TIME )		
	{
		bCursor =  !bCursor;

		//logf_wm( (char*)"idle %0.2f", cursorTime );
		cursorTime -= CURSOR_TIME;
		/*
		*/
	}

	if ( TextureInverse == NULL )
	{
		if (bCursor)	{ cursor.setVisible( true ); cursorInverse.setVisible( true ); }
		else			{ cursor.setVisible( false ); cursorInverse.setVisible( false ); }

	}
	else
	{
		if (bCursor)	{ cursor.setVisible( true ); cursorInverse.setVisible( true ); }
		else			{ cursor.setVisible( false ); cursorInverse.setVisible( false ); }
	}

}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelEditText::updatePos() {
	if ( !isVisible() )			return;
	
	#ifdef DEBUG
		logf_wm( (char*)"PanelEditText::updatePos()" );
	#endif

	PanelSimple::updatePos();

	if ( !getConstLarg() )	{	
		dx = dx_raw = text->getDX();
		dy = dy_raw = text->getDY();
	}
	if ( TextureInverse != NULL && cursor.getVisible() )
	{
		char c[] = "B";
		c[0] =  text->getText()[currentPos];
		cursor.changeText( c, true );
		
		int x  = text->getX();
		int y  = text->getY();
		int dx = cursor.getTextLenght(1);
		int dy = text->getDY();
		
		if ( text->getText().size() != 0 )
		{
			cursor.setPosAndSize( x, y, dx, dy );
			moveCursor();
			cursorInverse.setSize( dx, dy );

			#ifdef DEBUG
				logf_wm( (char*)"%s::updatePos() %d:'%s'", sExtra.c_str(), currentPos, cursor.getText().c_str() );
				logf_wm( (char*)"    %d, %d %dx%d", cursor.getX(),cursor.getY(),cursor.getDX(),cursor.getDY() );
			#endif
		}
		else
		{
			c[0] = ' ';
			cursor.changeText( c, true );
			dx = cursor.getTextLenght(1);
			cursorInverse.setSize( dx, dy );

			cursor.setPosAndSize( x, y, dx, dy );
			moveCursor();
			cursorInverse.setSize( dx, dy );

			#ifdef DEBUG
				logf_wm( (char*)"%s::updatePos() %d:'%s'", sExtra.c_str(), currentPos, cursor.getText().c_str() );
				logf_wm( (char*)"    text size = 0" );
				logf_wm( (char*)"    %d, %d %dx%d", cursor.getX(),cursor.getY(),cursor.getDX(),cursor.getDY() );
			#endif
		}
	}
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



