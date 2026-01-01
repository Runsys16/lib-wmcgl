#ifndef TRUETYPE_H
#define TRUETYPE_H

//FreeType Headers
#include <ft2build.h>
#include <freetype2/freetype/freetype.h>
#include <freetype2/freetype/ftglyph.h>
#include <freetype2/freetype/ftoutln.h>
#include <freetype2/freetype/fttrigon.h>

//OpenGL Headers 
#include <GL/gl.h>
#include <GL/glu.h>

//Some STL headers
#include <vector>
#include <string>

//Using the STL exception library increases the
//chances that someone else using our code will corretly
//catch any exceptions that we throw.
#include <stdexcept>

using namespace std;
#define MAX_CARAC 128
///Wrap everything in a namespace, that we can use common
///function names like "print" without worrying about
///overlapping with anyone else's code.
class  TrueType {

protected:
		float 			hFont;			///< Holds the height of the font.
		GLuint * 		textures;	///< Holds the texture id's 
		GLuint 			list_base;	///< Holds the first display list id
		
		unsigned		uCaracLenght[MAX_CARAC];	
		unsigned		uCaracHeight[MAX_CARAC];	

public:	
		TrueType();
		~TrueType();

		void			init(const char * fname, unsigned int h);
		void			clean();
		void			make_dlist ( FT_Face face, char ch, GLuint list_base, GLuint * tex_base );
		void 			print(const float x, float y, uint32_t color, const char *fmt, ...) ;
		unsigned		lenght( const char* text );
		unsigned		height( const char* text );
		unsigned		getGlyphCount( const char*);

};

#endif
