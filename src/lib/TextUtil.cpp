//  ---------------------------------------------------------------------------
//
//  @file       TwOpenGL.cpp
//  @author     Philippe Decaudin - http://www.antisphere.com
//  @license    This file is part of the AntTweakBar library.
//              For conditions of distribution and use, see License.txt
//
//  ---------------------------------------------------------------------------

//#include "TwPrecomp.h"
//#include "LoadOGL.h"
//#include "TwOpenGL.h"
//#include "TwMgr.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


#include <string>


#include "TextUtil.h"
#include "wm.h"
//#include "../include/WindowsManager.h"


//#define DEBUG

#ifdef DEBUG_WM
#	define DEBUG
#endif

using namespace std;

const char *g_ErrCantLoadOGL    = "Cannot load OpenGL library dynamically";
const char *g_ErrCantUnloadOGL  = "Cannot unload OpenGL library";

GLuint g_SmallFontTexID = 0;
GLuint g_NormalFontTexID = 0;
GLuint g_LargeFontTexID = 0;

//  ---------------------------------------------------------------------------
//  Extensions
/*
typedef void (APIENTRY * PFNGLBindBufferARB)(GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLBindProgramARB)(GLenum target, GLuint program);
typedef GLuint (APIENTRY * PFNGLGetHandleARB)(GLenum pname);
typedef void (APIENTRY * PFNGLUseProgramObjectARB)(GLuint programObj);
typedef void (APIENTRY * PFNGLTexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * PFNGLActiveTextureARB)(GLenum texture);
typedef void (APIENTRY * PFNGLClientActiveTextureARB)(GLenum texture);
typedef void (APIENTRY * PFNGLBlendEquation)(GLenum mode);
typedef void (APIENTRY * PFNGLBlendEquationSeparate)(GLenum srcMode, GLenum dstMode);
typedef void (APIENTRY * PFNGLBlendFuncSeparate)(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
PFNGLBindBufferARB glBindBufferARB = NULL;
PFNGLBindProgramARB glBindProgramARB = NULL;
PFNGLGetHandleARB glGetHandleARB = NULL;
PFNGLUseProgramObjectARB glUseProgramObjectARB = NULL;
PFNGLTexImage3D glTexImage3D = NULL;
PFNGLActiveTextureARB glActiveTextureARB = NULL;
PFNGLClientActiveTextureARB glClientActiveTextureARB = NULL;
PFNGLBlendEquation glBlendEquation = NULL;
PFNGLBlendEquationSeparate glBlendEquationSeparate = NULL;
PFNGLBlendFuncSeparate glBlendFuncSeparate = NULL;
#ifndef GL_ARRAY_BUFFER_ARB
#   define GL_ARRAY_BUFFER_ARB 0x8892
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER_ARB
#   define GL_ELEMENT_ARRAY_BUFFER_ARB 0x8893
#endif
#ifndef GL_ARRAY_BUFFER_BINDING_ARB
#   define GL_ARRAY_BUFFER_BINDING_ARB 0x8894
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB
#   define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB 0x8895
#endif
#ifndef GL_VERTEX_PROGRAM_ARB
#   define GL_VERTEX_PROGRAM_ARB 0x8620
#endif
#ifndef GL_FRAGMENT_PROGRAM_ARB
#   define GL_FRAGMENT_PROGRAM_ARB 0x8804
#endif
#ifndef GL_PROGRAM_OBJECT_ARB
#   define GL_PROGRAM_OBJECT_ARB 0x8B40
#endif
#ifndef GL_TEXTURE_3D
#   define GL_TEXTURE_3D 0x806F
#endif
#ifndef GL_TEXTURE0_ARB
#   define GL_TEXTURE0_ARB 0x84C0
#endif
#ifndef GL_ACTIVE_TEXTURE_ARB
#   define GL_ACTIVE_TEXTURE_ARB 0x84E0
#endif
#ifndef GL_CLIENT_ACTIVE_TEXTURE_ARB
#   define GL_CLIENT_ACTIVE_TEXTURE_ARB 0x84E1
#endif
#ifndef GL_MAX_TEXTURE_UNITS_ARB
#   define GL_MAX_TEXTURE_UNITS_ARB 0x84E2
#endif
#ifndef GL_MAX_TEXTURE_COORDS
#   define GL_MAX_TEXTURE_COORDS 0x8871
#endif
#ifndef GL_TEXTURE_RECTANGLE_ARB
#   define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#endif
#ifndef GL_FUNC_ADD
#   define GL_FUNC_ADD 0x8006
#endif
#ifndef GL_BLEND_EQUATION
#   define GL_BLEND_EQUATION 0x8009
#endif
#ifndef GL_BLEND_EQUATION_RGB
#   define GL_BLEND_EQUATION_RGB GL_BLEND_EQUATION
#endif
#ifndef GL_BLEND_EQUATION_ALPHA
#   define GL_BLEND_EQUATION_ALPHA 0x883D
#endif
#ifndef GL_BLEND_SRC_RGB
#   define GL_BLEND_SRC_RGB 0x80C9
#endif
#ifndef GL_BLEND_DST_RGB
#   define GL_BLEND_DST_RGB 0x80C8
#endif
#ifndef GL_BLEND_SRC_ALPHA
#   define GL_BLEND_SRC_ALPHA 0x80CB
#endif
#ifndef GL_BLEND_DST_ALPHA
#   define GL_BLEND_DST_ALPHA 0x80CA
#endif
*/
//  ---------------------------------------------------------------------------

#ifdef _DEBUG
    static void CheckGLError(const char *file, int line, const char *func)
    {
        int err=0;
        char msg[256];
        while( (err=glGetError())!=0 )
        {
            sprintf(msg, "%s(%d) : [%s] GL_ERROR=0x%x\n", file, line, func, err);
            #ifdef ANT_WINDOWS
                OutputDebugString(msg);
            #endif
            fprintf(stderr, msg);
        }
    }
#   ifdef __FUNCTION__
#       define CHECK_GL_ERROR CheckGLError(__FILE__, __LINE__, __FUNCTION__)
#   else
#       define CHECK_GL_ERROR CheckGLError(__FILE__, __LINE__, "")
#   endif
#else
#   define CHECK_GL_ERROR ((void)(0))
#endif

GLuint TextUtil::GenFont( CTexFont *_Font )	{
	return GenFont( _Font, (color32) 0xffffffff );
}
//  ---------------------------------------------------------------------------

GLuint TextUtil::GenFont( CTexFont *_Font, color32 color)
{
	//cout << "TextUtil::GenFont()" << endl;
	//m_tab_size = 40;
	if ( _Font->m_TexID != 0 )		return _Font->m_TexID;
	
    GLuint TexID = 0;
    glGenTextures(1, &(_Font->m_TexID) );
    #ifdef DEBUG
	cout << "TextUtil::GenFont() Generate Texture ID : " << _Font->m_TexID << endl;;
	#endif
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);

    glBindTexture(GL_TEXTURE_2D, _Font->m_TexID);
    glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
    glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/*    
    */
    float a, r, v, b;

    a =  (float)((unsigned)(color&0xff000000) >>24 )/255.0;
    r =  (float)((unsigned)(color&0x00ff0000) >>16 )/255.0;
    v =  (float)((unsigned)(color&0x0000ff00) >>8  )/255.0;
    b =  (float)((unsigned)(color&0x000000ff) >>0  )/255.0;
	    
    #ifdef DEBUG
	cout << "TextUtil::GenFont() Generate Texture ID : ";
	cout << "color=" << color << endl;;
	    
	cout << "TextUtil::GenFont() Generate Texture ID : ";
	cout << "a=" << a <<" r=" << r <<" v="<< v <<" b="<< b << endl;;
    #endif
        
    glPixelTransferf(GL_ALPHA_SCALE, 1);
    glPixelTransferf(GL_ALPHA_BIAS, 0);
    glPixelTransferf(GL_RED_BIAS, 1);
    glPixelTransferf(GL_GREEN_BIAS, 1);
    glPixelTransferf(GL_BLUE_BIAS, 1 );
    /*
    glPixelTransferf(GL_ALPHA_SCALE, 1);
    glPixelTransferi(GL_ALPHA_BIAS, (color&0xff000000) >>24 );
    glPixelTransferi(GL_RED_BIAS,	(color&0x00ff0000) >>16);
    glPixelTransferi(GL_GREEN_BIAS,	(color&0x0000ff00) >>8);
    glPixelTransferi(GL_BLUE_BIAS,	(color&0x000000ff) >>0);
   	*/
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, _Font->m_TexWidth, _Font->m_TexHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, _Font->m_TexBytes);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPixelTransferf(GL_ALPHA_SCALE, 1);
    glPixelTransferf(GL_ALPHA_BIAS, 0);
    glPixelTransferf(GL_RED_BIAS,   0);
    glPixelTransferf(GL_GREEN_BIAS, 0);
    glPixelTransferf(GL_BLUE_BIAS,  0);
    
    
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

    return _Font->m_TexID;
}

void TextUtil::BindFont(const CTexFont *_Font)	{
	BindFont( _Font, 0 );
}

void TextUtil::BindFont(const CTexFont *_Font, int slot)	{
	glActiveTexture(GL_TEXTURE0 + slot);
	//cout << "Bind texture slot : "<< slot << endl;
    glBindTexture(GL_TEXTURE_2D, _Font->m_TexID);
}

void TextUtil::UnbindFont( int slot)	{
    glBindTexture(GL_TEXTURE_2D, slot);
}

void TextUtil::UnbindFont()	{
	UnbindFont( 0 );
}



//  ---------------------------------------------------------------------------

void *TextUtil::NewTextObj()
{
    return new CTextObj;
}

//  ---------------------------------------------------------------------------

void TextUtil::DeleteTextObj(void *_TextObj)
{
    //assert(_TextObj!=NULL);
    delete static_cast<CTextObj *>(_TextObj);
}

//  ---------------------------------------------------------------------------

void TextUtil::BuildText(void *_TextObj, const std::string *_TextLines, color32 *_LineColors, color32 *_LineBgColors, int _NbLines, CTexFont *_Font, int _Sep, int _BgWidth)
{
    //assert(m_Drawing==true);
    //assert(_TextObj!=NULL);
    //assert(_Font!=NULL);

    m_FontTexID = GenFont(_Font);
    m_FontTex = _Font;
    UnbindFont();
	/*
    if( _Font != m_FontTex )
    {
        UnbindFont(m_FontTexID);
        _Font->m_TexID = m_FontTexID;
	    m_FontTexID = GenFont(_Font);
    }
    */
    CTextObj *TextObj = static_cast<CTextObj *>(_TextObj);
    TextObj->m_TextVerts.resize(0);
    TextObj->m_TextUVs.resize(0);
    TextObj->m_BgVerts.resize(0);
    TextObj->m_Colors.resize(0);
    TextObj->m_BgColors.resize(0);

    int x, x1, y, y1, i, Len;
    unsigned char ch;
    const unsigned char *Text;
    color32 LineColor = COLOR32_RED;
    for( int Line=0; Line<_NbLines; ++Line )
    {
        x = 0;
        y = Line * (_Font->m_CharHeight+_Sep);
        y1 = y+_Font->m_CharHeight;
        Len = (int)_TextLines[Line].length();
        Text = (const unsigned char *)(_TextLines[Line].c_str());
        if( _LineColors!=NULL )
            LineColor = (_LineColors[Line]&0xff00ff00) | GLubyte(_LineColors[Line]>>16) | (GLubyte(_LineColors[Line])<<16);
		
		#ifdef DEBUG
		cout << "TextUtil::BuildText Nb caractere : " << Len << endl;
		cout << "TextUtil::BuildText sizeof Vec2 : " << sizeof(Vec2) << endl;
		#endif
        for( i=0; i<Len; ++i )
        {
            ch = Text[i];
            if ( ch == '\t' )	{
            	if ( m_tab_size == 0 )	m_tab_size = 40;
            	int pos = x / m_tab_size;
            	x = (pos+1) * m_tab_size;
            	continue;
            }

            x1 = x + _Font->m_CharWidth[ch];

            TextObj->m_TextVerts.push_back(Vec2(x , y ));
            TextObj->m_TextVerts.push_back(Vec2(x1, y ));
            TextObj->m_TextVerts.push_back(Vec2(x , y1));
            TextObj->m_TextVerts.push_back(Vec2(x1, y ));
            TextObj->m_TextVerts.push_back(Vec2(x1, y1));
            TextObj->m_TextVerts.push_back(Vec2(x , y1));

            TextObj->m_TextUVs.push_back(Vec2(_Font->m_CharU0[ch], _Font->m_CharV0[ch]));
            TextObj->m_TextUVs.push_back(Vec2(_Font->m_CharU1[ch], _Font->m_CharV0[ch]));
            TextObj->m_TextUVs.push_back(Vec2(_Font->m_CharU0[ch], _Font->m_CharV1[ch]));
            TextObj->m_TextUVs.push_back(Vec2(_Font->m_CharU1[ch], _Font->m_CharV0[ch]));
            TextObj->m_TextUVs.push_back(Vec2(_Font->m_CharU1[ch], _Font->m_CharV1[ch]));
            TextObj->m_TextUVs.push_back(Vec2(_Font->m_CharU0[ch], _Font->m_CharV1[ch]));

            if( _LineColors!=NULL )
            {
            	LineColor = 0xffFF00FF;
                TextObj->m_Colors.push_back(LineColor);
                TextObj->m_Colors.push_back(LineColor);
                TextObj->m_Colors.push_back(LineColor);
                TextObj->m_Colors.push_back(LineColor);
                TextObj->m_Colors.push_back(LineColor);
                TextObj->m_Colors.push_back(LineColor);
            }
            else	{
            	LineColor = 0xffFF00FF;
                TextObj->m_Colors.push_back(LineColor);
                TextObj->m_Colors.push_back(LineColor);
                TextObj->m_Colors.push_back(LineColor);
                TextObj->m_Colors.push_back(LineColor);
                TextObj->m_Colors.push_back(LineColor);
                TextObj->m_Colors.push_back(LineColor);
            }

            x = x1;
        }
        if( _BgWidth>0 )
        {
            TextObj->m_BgVerts.push_back(Vec2(-1        , y ));
            TextObj->m_BgVerts.push_back(Vec2(_BgWidth+1, y ));
            TextObj->m_BgVerts.push_back(Vec2(-1        , y1));
            TextObj->m_BgVerts.push_back(Vec2(_BgWidth+1, y ));
            TextObj->m_BgVerts.push_back(Vec2(_BgWidth+1, y1));
            TextObj->m_BgVerts.push_back(Vec2(-1        , y1));

            if( _LineBgColors!=NULL )
            {
				color32 LineBgColor = (_LineBgColors[Line]&0xff00ff00) | GLubyte(_LineBgColors[Line]>>16) | (GLubyte(_LineBgColors[Line])<<16);
                LineBgColor = 0xffFFff00;
                TextObj->m_BgColors.push_back(LineBgColor);
                TextObj->m_BgColors.push_back(LineBgColor);
                TextObj->m_BgColors.push_back(LineBgColor);
                TextObj->m_BgColors.push_back(LineBgColor);
                TextObj->m_BgColors.push_back(LineBgColor);
                TextObj->m_BgColors.push_back(LineBgColor);
            }
           	else
           	{
                color32 LineBgColor = 0xffFFff00;
                TextObj->m_BgColors.push_back(LineBgColor);
                TextObj->m_BgColors.push_back(LineBgColor);
                TextObj->m_BgColors.push_back(LineBgColor);
                TextObj->m_BgColors.push_back(LineBgColor);
                TextObj->m_BgColors.push_back(LineBgColor);
                TextObj->m_BgColors.push_back(LineBgColor);
           	}
        }
    }
}

//  ---------------------------------------------------------------------------

int TextUtil::lenght(void *_TextObj, const std::string *_TextLines, CTexFont *_Font )
//, const std::string *_TextLines, color32 *_LineColors, color32 *_LineBgColors, int _NbLines, CTexFont *_Font, int _Sep, int _BgWidth)
{
    CTextObj *TextObj = static_cast<CTextObj *>(_TextObj);

    int x, x1, y, y1, i, Len;
    unsigned char ch;
    const unsigned char *Text;
	int _NbLines =1;
	int _Sep = 0;

    for( int Line=0; Line<_NbLines; ++Line )
    {
        x = 0;
        y = Line * (_Font->m_CharHeight+_Sep);
        y1 = y+_Font->m_CharHeight;
        Len = (int)_TextLines[Line].length();
        Text = (const unsigned char *)(_TextLines[Line].c_str());
		
		#ifdef DEBUG
		cout << "TextUtil::BuildText Nb caractere : " << Len << endl;
		cout << "TextUtil::BuildText sizeof Vec2 : " << sizeof(Vec2) << endl;
		#endif
        for( i=0; i<Len; ++i )
        {
            ch = Text[i];
            x1 = x + _Font->m_CharWidth[ch];
            x = x1;
        }
    }
    
    return x;
}

int TextUtil::lenght(void *_TextObj, const std::string *_TextLines, CTexFont *_Font, int nbChar )
//, const std::string *_TextLines, color32 *_LineColors, color32 *_LineBgColors, int _NbLines, CTexFont *_Font, int _Sep, int _BgWidth)
{
    CTextObj *TextObj = static_cast<CTextObj *>(_TextObj);

    int x, x1, y, y1, i, Len;
    unsigned char ch;
    const unsigned char *Text;
	int _NbLines =1;
	int _Sep = 0;

    for( int Line=0; Line<_NbLines; ++Line )
    {
        x = 0;
        y = Line * (_Font->m_CharHeight+_Sep);
        y1 = y+_Font->m_CharHeight;
        Len = (int)_TextLines[Line].length();
        Text = (const unsigned char *)(_TextLines[Line].c_str());
		
		#ifdef DEBUG
		cout << "TextUtil::BuildText Nb caractere : " << Len << endl;
		cout << "TextUtil::BuildText sizeof Vec2 : " << sizeof(Vec2) << endl;
		#endif
        for( i=0; i<nbChar; ++i )
        {
            ch = Text[i];
            x1 = x + _Font->m_CharWidth[ch];
            x = x1;
        }
    }
    
    return x;
}


//  ---------------------------------------------------------------------------

void TextUtil::setTabSize( int t ) {
	#ifdef DEBUG
	cout << "TextUtil::setTabSize() : " << t << endl;
	#endif
	m_tab_size = t;
}


//  ---------------------------------------------------------------------------

void TextUtil::BeginGL()	{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	/*	
	int width  = WindowsManager::getInstance().getWidth();
	int height = WindowsManager::getInstance().getHeight();
	ChangeViewport( 0, 0, width, height, 0, 0 );
	*/
  	

    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
    glDisable(GL_CULL_FACE);
}


//  ---------------------------------------------------------------------------

void TextUtil::EndGL()	{
   glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}


//  ---------------------------------------------------------------------------

void TextUtil::DrawText(void *_TextObj, int _X, int _Y, color32 _Color, color32 _BgColor)
{
    //assert(m_Drawing==true);
    //assert(_TextObj!=NULL);
	#ifdef DEBUG
   	std::cout << "TextUtil::DrawText( " << _TextObj <<", "<< _X <<", "<< _Y <<", "<< _Color <<", "<< _BgColor <<" )" << std::endl;
	#endif
	/*
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	int width  = WindowsManager::getInstance().getWidth();
	int height = WindowsManager::getInstance().getHeight();
	ChangeViewport( 0, 0, width, height, 0, 0 );

  	*/
    CTextObj *TextObj = static_cast<CTextObj *>(_TextObj);

    if( !TextObj || (TextObj->m_TextVerts.size()<4 && TextObj->m_BgVerts.size()<4 )	   ){
		#ifdef DEBUG
    	std::cout << "Nothing to draw ..." << std::endl;
		#endif
        return; // nothing to draw
    }
	/*
    glDisable(GL_CULL_FACE);
    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
    */
    glLoadIdentity();
    glTranslatef((GLfloat)_X, (GLfloat)_Y, 0);
    //glTranslatef((GLfloat)0.1, (GLfloat)0.1, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    if( (_BgColor!=0 || TextObj->m_BgColors.size()==TextObj->m_BgVerts.size()) && TextObj->m_BgVerts.size()>=4 )
    {
        glDisable(GL_TEXTURE_2D);
        glVertexPointer(2, GL_FLOAT, 0, &(TextObj->m_BgVerts[0]));
        if( TextObj->m_BgColors.size()==TextObj->m_BgVerts.size() && _BgColor==0 )
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_UNSIGNED_BYTE, 0, &(TextObj->m_BgColors[0]));
        }
        else
        {
            glDisableClientState(GL_COLOR_ARRAY);
            glColor4ub(GLubyte(_BgColor>>16), GLubyte(_BgColor>>8), GLubyte(_BgColor), GLubyte(_BgColor>>24));
        }
		#ifdef DEBUG
		std::cout << "TextUtil::DrawText glDrawArrays() ... nb : "<<  (int)TextObj->m_BgVerts.size() << std::endl;
		#endif
        glDrawArrays(GL_TRIANGLES, 0, (int)TextObj->m_BgVerts.size());
    }
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, m_FontTexID);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);


    if( TextObj->m_TextVerts.size()>=4 )
    {
        glVertexPointer(2, GL_FLOAT, 0, &(TextObj->m_TextVerts[0]));
        glTexCoordPointer(2, GL_FLOAT, 0, &(TextObj->m_TextUVs[0]));
        if( TextObj->m_Colors.size()==TextObj->m_TextVerts.size() && _Color==0 )
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_UNSIGNED_BYTE, 0, &(TextObj->m_Colors[0]));
        }
        else
        {
            glDisableClientState(GL_COLOR_ARRAY);
            glColor4ub(GLubyte(_Color>>16), GLubyte(_Color>>8), GLubyte(_Color), GLubyte(_Color>>24));
        }

		#ifdef DEBUG
		std::cout << "TextUtil::DrawText TextVerts : " << (int)TextObj->m_TextVerts.size() << std::endl;
		#endif
        glDrawArrays(GL_TRIANGLES, 0, (int)TextObj->m_TextVerts.size());
    }
    
	glColor4ub( 255, 255, 255, 255 );
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    /*
    glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	*/
}


//  ---------------------------------------------------------------------------

void TextUtil::ChangeViewport(int _X0, int _Y0, int _Width, int _Height, int _OffsetX, int _OffsetY)	{
    if( _Width>0 && _Height>0 )
    {
        GLint vp[4];
        vp[0] = _X0;
        vp[1] = _Y0;
        vp[2] = _Width-1;
        vp[3] = _Height-1;
        //glViewport(vp[0], m_WndHeight-vp[1]-vp[3], vp[2], vp[3]);

        GLint matrixMode = 0;
        glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(_OffsetX, _OffsetX+vp[2], vp[3]-_OffsetY, -_OffsetY, -1, 1);
        glMatrixMode(matrixMode);
    }
}

//  ---------------------------------------------------------------------------

