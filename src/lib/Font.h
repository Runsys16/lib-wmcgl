#ifndef FONT_H
#define FONT_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <map>
#include <string>

#include "FreeType.h"
/*
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
*/
 
class Font 	{
	public:
		enum FONT { ARIAL, UBUNTU_B, UBUNTU_RI, UBUNTU_R, DEJA_VU_SANS_MONO };
		Font();

		void				print(Font::FONT, int, int, std::string );
		void				print(Font::FONT, int, int, char* );

	private:
		//std::map<int, freetype::font_data *>*		pFonts;
		std::map<int, void *>*		pFonts;
};


#endif
