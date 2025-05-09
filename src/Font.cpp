#include <stdlib.h>
#include <stdio.h>
#include <iostream>


#include "Font.h"
#include "wm.h"
//#include "../include/WindowsManager.h"

//#define DEBUG
//#define DEBUG_CONST

#ifdef DEBUG_WM
#	define DEBUG
#endif


#ifdef DEBUG
#	define DEBUG_CONST
#endif

#include <sys/stat.h>

bool cfileexists(const char* filename){
    struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0)
        return true;
    else // -1
        return false;
}



using namespace std;

_Font::_Font()	{
	#ifdef DEBUG_CONST
	cout << "_Font::_Font : Constructeur FONT..." << endl;
	#endif
	freetype::font_data * pFont;


	//pFonts = new std::map<int, freetype::font_data *>();
	pFonts = new std::map<int, void *>();

	/*
	#define FILENAME0 "/usr/share/fonts/truetype/msttcorefonts/Arial.ttf"
	#define FILENAME1 "/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf"
	#define FILENAME2 "/usr/share/fonts/truetype/ubuntu/Ubuntu-RI.ttf"
	#define FILENAME3 "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf"
	#define FILENAME4 "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"

	//--------------------------------------------------------------
	std::cout << FILENAME0 << std::endl;
	pFont = new freetype::font_data;
	if ( cfileexists(FILENAME0) ) 
		pFont->init(FILENAME0, 10);
    else
		pFont->init(FILENAME0, 10);
    (*pFonts)[(int)ARIAL] = pFont;
	//--------------------------------------------------------------
	std::cout << FILENAME1 << std::endl;
	pFont = new freetype::font_data;
	if ( cfileexists(FILENAME1) ) 
    	pFont->init(FILENAME1, 10);
    else
    	pFont->init(FILENAME1, 10);
	(*pFonts)[(int)UBUNTU_B] = pFont;
	//--------------------------------------------------------------
	std::cout << FILENAME2 << std::endl;
	pFont = new freetype::font_data;
	if ( cfileexists(FILENAME2) ) 
    	pFont->init(FILENAME2, 10);
    else
    	pFont->init(FILENAME2, 10);
	(*pFonts)[(int)UBUNTU_RI] = pFont;
	//--------------------------------------------------------------
	std::cout << FILENAME3 << std::endl;
	pFont = new freetype::font_data;
	if ( cfileexists(FILENAME3) ) 
    	pFont->init(FILENAME3, 12);
    else
    	pFont->init(FILENAME3, 10);
	(*pFonts)[(int)UBUNTU_R] = pFont;
	//--------------------------------------------------------------
	std::cout << FILENAME4 << std::endl;
	pFont = new freetype::font_data;
	if ( cfileexists(FILENAME4) ) 
		pFont->init(FILENAME4, 10);
	(*pFonts)[(int)DEJA_VU_SANS_MONO] = pFont;
	//--------------------------------------------------------------
	*/
	#ifdef DEBUG
	std::cout << "pFonts->size() : " << pFonts->size() << std::endl;
	std::cout << "Adresse de pFonts : " << pFonts << std::endl;
	#endif
	/*
	enum FONT { ARIAL, UBUNTU_B, UBUNTU_RI, DEJA_VU_SANS_MONO };
    font_console_b.init("/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-B.ttf", 14);
    font_console_i.init("/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-RI.ttf", 14);
    font_console_test.init("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 10);
    */
	
}

void _Font::print( _Font::FONT F, int x, int y, string str)	{
	WindowsManager& wm = WindowsManager::getInstance();
	//map<int, freetype::font_data *>::iterator it;
	map<int, void *>::iterator it;

	freetype::font_data *  pFont;
	it = pFonts->find( (int)F );

	if ( it != pFonts->end() )	{
		pFont = (freetype::font_data*)it->second;

		x = x + wm.getOffsetX();
		y = wm.getHeight() - (y+20) + wm.getOffsetX();
		
		glColor3ub(0xff,0xff,0xff);
		unsigned c = 0xFFFFFFFF;
		freetype::print( *pFont, x, y, c, str.c_str() );
	}
}



void _Font::print( _Font::FONT F, int x, int y, char * cStr)	{
	#ifdef DEBUG
	std::cout <<"void _Font::print( _Font::FONT : " << (int)F << ", int : "<< x <<", int : "<< y << ", char * : "<< cStr << ")" << std::endl;
	//std::cout << "Nombre de pFonts : " << pFonts->size() << std::endl;
	#endif
	//map<int, freetype::font_data *>::iterator it;
	map<int, void *>::iterator it;
	WindowsManager& wm = WindowsManager::getInstance();
	/*
	#ifdef DEBUG
	std::cout << "_Font::print   Nombre de pFonts : " << std::endl;
	std::cout << "_Font::print   Adresse de  pFonts : " << &pFonts << std::endl;
	std::cout << "_Font::print   Nombre de pFonts : " << pFonts->size() << std::endl;
	#endif
	*/
	freetype::font_data *  pFont;
	//it = pFonts.find( (int)3 );
	it = pFonts->find( (int)F );

	if ( it != pFonts->end() )	{
		pFont = (freetype::font_data *)it->second;

		x = x + wm.getOffsetX();
		y = wm.getHeight() - (y+20) + wm.getOffsetX();
		
		glColor3ub(0xff,0xff,0xff);
		unsigned c = 0xFFFFFFFF;
		freetype::print( *pFont, x, y, c, cStr );
	}
}





