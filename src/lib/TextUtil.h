//  ---------------------------------------------------------------------------
//
//  @file       TwOpenGL.h
//  @brief      OpenGL graph functions
//  @author     Philippe Decaudin - http://www.antisphere.com
//  @license    This file is part of the AntTweakBar library.
//              For conditions of distribution and use, see License.txt
//
//  notes:      Private header
//              TAB=4
//
//  ---------------------------------------------------------------------------


#if !defined ANT_TW_OPENGL_INCLUDED
#define ANT_TW_OPENGL_INCLUDED

//#include "TwGraph.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <string>
#include <vector>


#include "InternalFonts.h"
//  ---------------------------------------------------------------------------

#define color32 unsigned int


const color32 COLOR32_BLACK     = 0xff000000;   // Black 
const color32 COLOR32_WHITE     = 0xffffffff;   // White 
const color32 COLOR32_ZERO      = 0x00000000;   // Zero 
const color32 COLOR32_RED       = 0xffff0000;   // Red 
const color32 COLOR32_GREEN     = 0xff00ff00;   // Green 
const color32 COLOR32_BLUE      = 0xff0000ff;   // Blue 
   


class TextUtil  {

public:

    void *      NewTextObj();
    void        DeleteTextObj(void *_TextObj);
    void        BuildText(void *_TextObj, const std::string *_TextLines, color32 *_LineColors, color32 *_LineBgColors, int _NbLines,  CTexFont *_Font, int _Sep, int _BgWidth);

    int			lenght(void *_TextObj, const std::string *_TextLines, CTexFont * );
    int			lenght(void *_TextObj, const std::string *_TextLines, CTexFont *, int );
    void		BeginGL();
    void		EndGL();
    void        DrawText(void *_TextObj, int _X, int _Y, color32 _Color, color32 _BgColor);

	void				ChangeViewport(int, int, int, int, int, int);

	GLuint 				GenFont( CTexFont * );
	void 				BindFont( const CTexFont * );
	void 				BindFont( const CTexFont *, int );
	void 				UnbindFont( int );
	void 				UnbindFont();
	void				setTabSize( int );



protected:
    bool                m_Drawing;
    GLuint              m_FontTexID;
    const CTexFont *    m_FontTex;
    GLfloat             m_PrevLineWidth;
    GLint               m_PrevTexEnv;
    GLint               m_PrevPolygonMode[2];
    GLint               m_MaxClipPlanes;
    GLint               m_PrevTexture;
    GLint               m_PrevArrayBufferARB;
    GLint               m_PrevElementArrayBufferARB;
    GLboolean           m_PrevVertexProgramARB;
    GLboolean           m_PrevFragmentProgramARB;
    GLuint              m_PrevProgramObjectARB;
    GLboolean           m_PrevTexture3D;
    enum EMaxTextures   { MAX_TEXTURES = 128 };
    GLboolean           m_PrevActiveTexture1D[MAX_TEXTURES];
    GLboolean           m_PrevActiveTexture2D[MAX_TEXTURES];
    GLboolean           m_PrevActiveTexture3D[MAX_TEXTURES];
    GLboolean           m_PrevClientTexCoordArray[MAX_TEXTURES];
    GLint               m_PrevActiveTextureARB;
    GLint               m_PrevClientActiveTextureARB;
    bool                m_SupportTexRect;
    GLboolean           m_PrevTexRectARB;
    GLint               m_PrevBlendEquation;
    GLint               m_PrevBlendEquationRGB;
    GLint               m_PrevBlendEquationAlpha;
    GLint               m_PrevBlendSrcRGB;
    GLint               m_PrevBlendDstRGB;
    GLint               m_PrevBlendSrcAlpha;
    GLint               m_PrevBlendDstAlpha;
    GLint               m_ViewportInit[4];
    GLfloat             m_ProjMatrixInit[16];
    int                 m_WndWidth;
    int                 m_WndHeight;
    int                 m_tab_size;

    struct Vec2         { GLfloat x, y; Vec2(){} Vec2(GLfloat _X, GLfloat _Y):x(_X),y(_Y){} Vec2(int _X, int _Y):x(GLfloat(_X)),y(GLfloat(_Y)){} };
    struct CTextObj
    {
        std::vector<Vec2>   m_TextVerts;
        std::vector<Vec2>   m_TextUVs;
        std::vector<Vec2>   m_BgVerts;
        std::vector<color32>m_Colors;
        std::vector<color32>m_BgColors;
    };
};

//  ---------------------------------------------------------------------------


#endif // !defined ANT_TW_OPENGL_INCLUDED
