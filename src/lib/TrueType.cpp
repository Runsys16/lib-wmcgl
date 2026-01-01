#ifndef TRUETYPE_CPP
#define TRUETYPE_CPP
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
/*
	A quick and simple opengl font library that uses GNU TrueType2, written
	and distributed as part of a tutorial for nehe.gamedev.net.
	Sven Olsen, 2003
*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "TrueType.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
TrueType::TrueType ()
{
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
TrueType::~TrueType ()
{
	clean();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
inline int next_p2 ( int a )
{
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool fexists(const char *filename)
{
  std::ifstream ifile(filename);
  return (bool)ifile;
}
//--------------------------------------------------------------------------------------------------------------------
//
//
//   `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@
//
//   $@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'. 
//  "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'. "
//
//--------------------------------------------------------------------------------------------------------------------
char ascii_art[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`\'.     ";
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
///Create a display list coresponding to the give character.
void TrueType::make_dlist ( FT_Face face, char ch, GLuint list_base, GLuint * tex_base ) {
//#define DEBUG_TRUE_TYPE		
//#define LUMINANCE

	//Load the Glyph for our character.
	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
		throw std::runtime_error("FT_Load_Glyph failed");

	//Move the face's glyph into a Glyph object.
    FT_Glyph glyph;
    if(FT_Get_Glyph( face->glyph, &glyph ))
		throw std::runtime_error("FT_Get_Glyph failed");

	//Convert the glyph to a bitmap.
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//This reference will make accessing the bitmap easier
	FT_Bitmap& bitmap=bitmap_glyph->bitmap;

	//Use our helper function to get the widths of
	//the bitmap data that we will need in order to create
	//our texture.
	int width = next_p2( bitmap.width );
	int height = next_p2( bitmap.rows );

	//Allocate memory for the texture data.
#ifdef LUMINANCE
	GLubyte* expanded_data = new GLubyte[ width * height];
#else
	GLubyte* expanded_data = new GLubyte[ 2 * width * height];
#endif			

	for(int j=0; j<height; j++) {
		for(int i=0; i<width; i++){
#ifdef LUMINANCE
			expanded_data[(i+j*width)] = 
				(i>=bitmap.width || j>=bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
				//0 : bitmap.buffer[i + (bitmap.width*bitmap.rows)-(bitmap.width*j)+1];
#else
			expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] = 
				(i>=bitmap.width || j>=bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
				//0 : bitmap.buffer[i + (bitmap.width*bitmap.rows)-(bitmap.width*(j+1))];
#endif			
		}
	}
	
#ifdef DEBUG_TRUE_TYPE			
	std::cout <<"---Caract : "<< (unsigned)ch <<"  "<< width <<"x"<<height <<" "<<sizeof(ascii_art)<< std::endl;
	for(int j=0; j <height;j++) {
		for(int i=0; i < width; i++){
			if (i>=bitmap.width || j>=bitmap.rows)  continue;

			char st[]="12345678";
			unsigned char cc = bitmap.buffer[i + bitmap.width*j];

			float ff = cc;
			ff = ff / 255.0 * 70.0;
			int CC = 70.0-ff; 
			if ( CC<0)		CC=0;
			if ( CC>70)		printf( "Erreur %d\n", CC );

			printf( "%c", ascii_art[CC] );
		}
		std::cout << std::endl;
	}
#endif


	//Now we just setup some texture paramaters.
    glBindTexture( GL_TEXTURE_2D, tex_base[(int)ch]);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	//Here we actually create the texture itself, notice
	//that we are using GL_LUMINANCE_ALPHA to indicate that
	//we are using 2 channel data.
	
#ifdef LUMINANCE
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		  0, GL_LUMINANCE, GL_UNSIGNED_BYTE, expanded_data );
#else
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		  0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );
#endif
	
	
	//With the texture created, we don't need to expanded data anymore
    delete [] expanded_data;

	//So now we can create the display list
	glNewList(list_base+ch,GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D,tex_base[(int)ch]);
	//printf( (char*)"glBindTexture(%d)\n",  tex_base[(int)ch] );

	glPushMatrix();
	glTranslatef(bitmap_glyph->left,0,0);

	float dy = (float)bitmap_glyph->top;//-bitmap.rows;
	glTranslatef(0, -dy,0);

	float	x=(float)bitmap.width / (float)width,
			y=(float)bitmap.rows / (float)height;

	glBegin(GL_QUADS);
	glTexCoord2d(0,0); glVertex2f(0,0);
	glTexCoord2d(0,y); glVertex2f(0,bitmap.rows);
	glTexCoord2d(x,y); glVertex2f(bitmap.width,bitmap.rows);
	glTexCoord2d(x,0); glVertex2f(bitmap.width,0);
	glEnd();
	glPopMatrix();
	glTranslatef(face->glyph->advance.x >> 6 ,0,0);
	

	glEndList();
	//uCaracLenght[ch] = bitmap.width + bitmap_glyph->left + int(face->glyph->advance.x >> 6 );//bitmap_glyph->left;
	uCaracLenght[ch] = int(face->glyph->advance.x >> 6 );//bitmap_glyph->left;
	//uCaracLenght[ch] = bitmap.width + x;
	uCaracHeight[ch] = int( y );//bitmap_glyph->left;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void TrueType::init(const char * fname, unsigned int h) {
	//Allocate some memory to store the texture ids.
	textures = new GLuint[128];

	this->hFont=h;

	//Create and initilize a TrueType font library.
	FT_Library library;
	if (FT_Init_FreeType( &library )) 
		throw std::runtime_error("FT_Init_TrueType failed");

	FT_Face face;

	if (FT_New_Face( library, fname, 0, &face )) {
		string error = string("FT_New_Face failed (there is probably a problem with your font file)") +fname;
		throw std::runtime_error(error.c_str());
	}
	//h pixels high, we need to request a size of h*64.
	//(h << 6 is just a prettier way of writting h*64)
	FT_Set_Char_Size( face, h << 6, h << 6, 96, 96);

	list_base=glGenLists(MAX_CARAC);
	glGenTextures( MAX_CARAC, textures );

	//This is where we actually create each of the fonts display lists.
	for(unsigned char i=0;i<128;i++)
		make_dlist(face, i, list_base, textures);

	FT_Done_Face(face);
	FT_Done_FreeType(library);
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void TrueType::clean() {
	glDeleteLists(list_base,128);
	glDeleteTextures(128,textures);
	delete [] textures;
}
//--------------------------------------------------------------------------------------------------------------------
/// A fairly straight forward function that pushes
/// a projection matrix that will make object world 
/// coordinates identical to window coordinates.
//
//--------------------------------------------------------------------------------------------------------------------
inline void pushScreenCoordinateMatrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	GLint	viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	//glLoadIdentity();
	//gluOrtho2D(viewport[0],viewport[1],viewport[2],viewport[3]);
	glPopAttrib();
}
//--------------------------------------------------------------------------------------------------------------------
/// Pops the projection matrix without changing the current
/// MatrixMode.
//
//--------------------------------------------------------------------------------------------------------------------
inline void pop_projection_matrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}
//--------------------------------------------------------------------------------------------------------------------
///Much like Nehe's glPrint function, but modified to work
///with TrueType fonts.
//
//--------------------------------------------------------------------------------------------------------------------
void TrueType::print( float x, float y, uint32_t color, const char *fmt, ...)  {
	
	// We want a coordinate system where things coresponding to window pixels.
	if ( fmt[0] == 0 )			return;
	
	pushScreenCoordinateMatrix();					
	
	GLuint font = list_base;
	float h = hFont/.63f;						//We make the height about 1.5* that of
	
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		*text=0;											// Do Nothing

	else {
	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text
	}


	const char *start_line=text;
	vector<string> lines;

	const char * c = text;;

	//for(const char *c=text;*c;c++) {
	for(;*c;c++) {
		if(*c=='\n') {
			string line;
			for(const char *n=start_line;n<c;n++) line.append(1,*n);
			lines.push_back(line);
			start_line=c+1;
		}
	}
	if(start_line) {
		string line;
		for(const char *n=start_line;n<c;n++) line.append(1,*n);
		lines.push_back(line);
	}

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	unsigned char a = (color&0xff000000)>>24;
	unsigned char r = (color&0x00ff0000)>>16;
	unsigned char g = (color&0x0000ff00)>>8;
	unsigned char b = (color&0x000000ff);
	//glColor4f(r, g, b, a);


    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // Linear Filtering

	glListBase(font);

	float modelview_matrix[16];	
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

	for(unsigned int i=0;i<lines.size();i++) {
		glPushMatrix();
		glLoadIdentity();
		//glTranslatef(x,y-h*i,0);
		glTranslatef(x,y+h*i,0);
		glMultMatrixf(modelview_matrix);
		
		glTranslatef( 0.0, hFont, 0.0 );

		glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());

		glPopMatrix();
	}

	glPopAttrib();		
	pop_projection_matrix();
}
//--------------------------------------------------------------------------------------------------------------------
///Much like Nehe's glPrint function, but modified to work
///with TrueType fonts.
//
//--------------------------------------------------------------------------------------------------------------------
unsigned TrueType::lenght(const char* text)
{
	unsigned l = 0;
	for(unsigned i=0; text[i]!=0; i++ )	l +=  uCaracLenght[ text[i] ];

	return l;
}	
//--------------------------------------------------------------------------------------------------------------------
///Much like Nehe's glPrint function, but modified to work
///with TrueType fonts.
//
//--------------------------------------------------------------------------------------------------------------------
unsigned TrueType::height(const char* text)
{
	unsigned l = 0;
	for(unsigned i=0; text[i]!=0; i++ )	
	{
		unsigned u = uCaracLenght[ text[i] ];
		
		if ( l < u  )		l = u;
	}

	return l;
}	
//--------------------------------------------------------------------------------------------------------------------
///Much like Nehe's glPrint function, but modified to work
///with TrueType fonts.
//
//--------------------------------------------------------------------------------------------------------------------
//unsigned TrueType::getGlyphCount( const char * fname, unsigned int h) {
unsigned TrueType::getGlyphCount( const char * fname )
{
	FT_Library library;
	if (FT_Init_FreeType( &library )) 
		throw std::runtime_error("FT_Init_TrueType failed");

	FT_Face face;

	if (FT_New_Face( library, fname, 0, &face )) {
		string error = string("FT_New_Face failed (there is probably a problem with your font file)") +fname;
		throw std::runtime_error(error.c_str());
	}

	FT_UInt   	gindex;
	FT_ULong  	charcode = FT_Get_First_Char( face, &gindex );
	int nb = 0;
	while ( gindex != 0 )
	{
		//	... do something with (charcode,gindex) pair ...

		charcode = FT_Get_Next_Char( face, charcode, &gindex );
		//printf( (char*)"%s : %d  char = 0x%08lX\n", fname, nb++, charcode );
	}
	return nb;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
#endif

