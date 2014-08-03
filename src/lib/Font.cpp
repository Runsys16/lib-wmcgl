#include <stdlib.h>
#include <stdio.h>
#include <iostream>


#include "Font.h"
#include "wm.h"
//#include "../include/WindowsManager.h"

//#define DEBUG
#define DEBUG_CONST

#ifdef DEBUG_WM
#	define DEBUG
#endif


#ifdef DEBUG
#	define DEBUG_CONST
#endif





using namespace std;

Font::Font()	{
	#ifdef DEBUG_CONST
	cout << "Font::Font : Constructeur FONT..." << endl;
	#endif
	freetype::font_data * pFont;


	//pFonts = new std::map<int, freetype::font_data *>();
	pFonts = new std::map<int, void *>();

	
	pFont = new freetype::font_data;
	pFont->init("/usr/share/fonts/truetype/msttcorefonts/Arial.ttf", 10);
	(*pFonts)[(int)ARIAL] = pFont;
	
	pFont = new freetype::font_data;
	pFont->init("/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-B.ttf", 10);
	(*pFonts)[(int)UBUNTU_B] = pFont;
	
	pFont = new freetype::font_data;
	pFont->init("/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-RI.ttf", 10);
	(*pFonts)[(int)UBUNTU_RI] = pFont;
	
	pFont = new freetype::font_data;
	pFont->init("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf", 12);
	(*pFonts)[(int)UBUNTU_R] = pFont;
	//fonts[(int)3] = pFont;
	
	pFont = new freetype::font_data;
	pFont->init("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 10);
	(*pFonts)[(int)DEJA_VU_SANS_MONO] = pFont;
	
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

void Font::print( Font::FONT F, int x, int y, string str)	{
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
		freetype::print( *pFont, x, y, str.c_str() );
	}
}



void Font::print( Font::FONT F, int x, int y, char * cStr)	{
	#ifdef DEBUG
	std::cout <<"void Font::print( Font::FONT : " << (int)F << ", int : "<< x <<", int : "<< y << ", char * : "<< cStr << ")" << std::endl;
	//std::cout << "Nombre de pFonts : " << pFonts->size() << std::endl;
	#endif
	//map<int, freetype::font_data *>::iterator it;
	map<int, void *>::iterator it;
	WindowsManager& wm = WindowsManager::getInstance();
	/*
	#ifdef DEBUG
	std::cout << "Font::print   Nombre de pFonts : " << std::endl;
	std::cout << "Font::print   Adresse de  pFonts : " << &pFonts << std::endl;
	std::cout << "Font::print   Nombre de pFonts : " << pFonts->size() << std::endl;
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
		freetype::print( *pFont, x, y, cStr );
	}
}





