#ifndef WINDOWSMANAGER_H
#define WINDOWSMANAGER_H

#include <vector>
#include <string>
#include <map>


class _Texture2D;
class Panel;
class Font;
class TextUtil;

#define color32 unsigned int



/*
*/
 
class Font 	{
	public:
		enum FONT { ARIAL, UBUNTU_B, UBUNTU_RI, UBUNTU_R, DEJA_VU_SANS_MONO };
		Font();

		void				print(Font::FONT, int, int, std::string );
		void				print(Font::FONT, int, int, char* );

		/*
		static CTexFont *DefaulttSmallFont = NULL;
		static CTexFont *DefaulttNormalFont = NULL;
		static CTexFont *DefaulttLargeFont = NULL;
		*/

	private:
		//std::map<int, freetype::font_data *>*		pFonts;
		std::map<int, void *>*		pFonts;
};


struct CTexFont
{
    unsigned char * m_TexBytes;
	unsigned int	m_TexID;
    int             m_TexWidth;     // power of 2
    int             m_TexHeight;    // power of 2
    float           m_CharU0[256];
    float           m_CharV0[256];
    float           m_CharU1[256];
    float           m_CharV1[256];
    int             m_CharWidth[256];
    int             m_CharHeight;
    int             m_NbCharRead;

    CTexFont();
    ~CTexFont();
};


CTexFont *TwGenerateFont(const unsigned char *_Bitmap, int _BmWidth, int _BmHeight);



extern CTexFont *DefaultSmallFont;
extern CTexFont *DefaultNormalFont;
extern CTexFont *DefaultLargeFont;

void wmcglGenerateDefaultFonts();
void wmcglDeleteDefaultFonts();

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






//  ---------------------------------------------------------------------------



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
	GLuint 				GenFont( CTexFont *, color32 );
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






class Panel {
	public:
		//-----------------------------------------------------------------------------------
							Panel();
		//-----------------------------------------------------------------------------------
		void 				init();
		void				add( Panel* );
		void				sup( Panel* );
		
		virtual void		clickLeft( int, int)							{;}
		virtual void		releaseLeft( int, int)							{;}

		virtual void		clickUp( int, int);
		virtual void		clickDown( int, int);

		virtual void		clickRight( int, int)							{;}
		virtual void		releaseRight( int, int)							{;}

		virtual Panel*		isMouseOver( int, int);
		virtual void		haveFocus()										{;}
		virtual void		lostFocus()										{;}
		virtual void		displayGL();
		virtual void		updatePos();
		virtual void		idle(float)										{;};
		virtual void		cb_keyboard( unsigned char ) 					{;};
		virtual void		cb_keyboard_special( unsigned char )			{;};
		virtual void		cb_keyboard_special_up( unsigned char )			{;};
		virtual void		debug( bool )									{;};
		
		inline void			setParent( Panel* p )							{parent = p;};
		inline Panel*		getParent()										{return parent;};
		inline void			setPosAndSize(int x0, int y0, int dx0, int dy0)	{x=x0; y=y0; dx=dx0 ;dy=dy0;};
		inline void			setPos(int x0, int y0)							{x=x0; y=y0;};
		inline void			setSize(int dx0, int dy0)						{dx=dx0 ;dy=dy0;};
		inline int			getX()											{return x_raw;};
		inline int			getY()											{return y_raw;};
		inline int			getDX()											{return dx_raw;};
		inline int			getDY()											{return dy_raw;};
		inline void			setID(int id)									{ID = id;};
		inline int			getID()											{return ID;};
		
		inline void			setPosX(int i)									{x = i;};
		inline void			setPosY(int i)									{y = i;};
		inline void			setPosDX(int i)									{dx = i;};
		inline void			setPosDY(int i)									{dy = i;};
		inline int			getPosX()										{return x;};
		inline int			getPosY()										{return y;};
		inline int			getPosDX()										{return dx;};
		inline int			getPosDY()										{return dy;};
	
		inline bool			getVisible()									{return visible;};
		inline void			setVisible(bool b)								{visible=b;};
		
		
		inline 	std::vector<Panel*> getChilds()								{return childs;};
		
		
		//-----------------------------------------------------------------------------------
	protected:
		//-----------------------------------------------------------------------------------
		int					ID;
		int					x_raw;
		int					y_raw;
		int					dx_raw;
		int					dy_raw;
	
		int					x;
		int					y;
		int					dx;
		int					dy;
	
		int					startX;
		int					startY;
	
		bool				visible;
		bool				canMove;
		bool				mouseVisible;
	
		Panel* 				parent;
		std::vector<Panel*> childs;
		
		bool				bDebug;
};








class PanelText : public Panel	{

	public:
		enum FONT { ARIAL, UBUNTU_B, UBUNTU_RI, UBUNTU_R, DEJA_VU_SANS_MONO, NORMAL_FONT, SMALL_FONT, LARGE_FONT };
		enum ALIGN { LEFT, RIGHT, CENTER };


							PanelText();
							PanelText( std::string );
							PanelText( std::string, FONT );
							PanelText( std::string, FONT, int, int );
							PanelText( std::string, FONT, int, int, unsigned int );
							PanelText( char*, FONT, int, int );
							PanelText( char*, FONT, int, int, unsigned int );

		void 				buildString();
		
		inline std::string	getText()										{return text;};
		int					getTextLenght();
		int					getTextLenght(int);

		void 				changeText( char* );
		void 				changeText( char*, FONT );
		void 				changeText( std::string );
		void 				changeText( std::string, FONT );
		void 				changeText( std::string, bool );
		void 				changeText( std::string, FONT, bool );
		void 				eraseText( );
		void				setTabSize( int );
		void				setColor( unsigned long l)						{ color = l; bChange = true; }
		
		virtual Panel*		isMouseOver( int, int);
		virtual void		displayGL();
		virtual void		updatePos();
		
		inline void 		setAlign( ALIGN a )								{align = a;};
		
		//----------------- functions
		std::string			strFont();		
		void				displayGLInternal();

		//----------------- members
		ALIGN				align;

		TextUtil*			textUtil;
		FONT				typeFont;
		std::string			text;
		unsigned int		color;
		//std::string			cmdLine;
		bool				bChange;
		
		void*				pTextGL;
		int					tabSize;
	private:
	
};







class PanelTextOmbre : public PanelText	{

	public:

							PanelTextOmbre();
							PanelTextOmbre( std::string );
							PanelTextOmbre( std::string, FONT );
							PanelTextOmbre( std::string, FONT, int, int );
							PanelTextOmbre( std::string, FONT, int, int, unsigned int );
							PanelTextOmbre( char*, FONT, int, int );
							PanelTextOmbre( char*, FONT, int, int, unsigned int );

		void 				buildString();
		
		inline std::string	getText()										{return text;};
		int					getTextLenght();
		int					getTextLenght(int);

		void 				changeText( char* );
		void 				changeText( char*, FONT );
		void 				changeText( std::string );
		void 				changeText( std::string, FONT );
		void 				changeText( std::string, bool );
		void 				changeText( std::string, FONT, bool );
		void 				eraseText( );
		void				setTabSize( int );
		void				setColor( unsigned long l);
		
		virtual Panel*		isMouseOver( int, int);
		virtual void		displayGL();
		virtual void		updatePos();
		
		inline void 		setAlign( ALIGN a )								{align = a;};
		
		//----------------- functions
		std::string			strFont();		
		void				displayGLInternal();

	/*
		//----------------- members
		ALIGN				align;

		TextUtil*			textUtil;
		FONT				typeFont;
		std::string			text;
		unsigned int		color;
		//std::string			cmdLine;
		bool				bChange;
		
		void*				pTextGL;
		int					tabSize;
		*/
	protected:
		PanelText			pTextOmbre;
};





class PanelSimple : public Panel {
	public:
		PanelSimple();
		
		//void				buildText();

		virtual void		displayGL();
		virtual void		updatePos();
		
		void				debug(bool);
		void				setBackground( char *);

//	private:
	protected:
		_Texture2D*		m_pTexBackground;
		
		bool							bTextOK;
		void * 							cTextObj;
		TextUtil*						textUtil;
		std::vector<std::string>		str;

		//bool							bDebug;				in Panel.h
		PanelSimple*					pPsDebug;
		PanelText*						pPtDebug;
		std::string						sDebug;
};







/*
typedef void (* CB_CMD)(std::string);

class PanelScrollTextCallBack	{
	public :
	virtual void					callback_cmd(std::string)=0;
};
*/

class PanelScrollText : public PanelSimple	{
	public:

									PanelScrollText();
									PanelScrollText( int, int );
		
		void						setPrompt( std::string );

		virtual void				displayGL();
		virtual void				updatePos();
		virtual void				idle(float);

		
		
		void						scroll();
		void						rotateBuffer();
		void						addLine();
		void						eraseTexts();
		void						clear();
		void						affiche( std::string * );
		void						affiche( std::string );
		void						addChar( char );
		
		int							posWordPrec();
		int							posWordSuiv();
		void						wordPrec();
		void						wordSuiv();
		void						setTabSize( int );
		
	private:
		int							maxCmd;
		std::vector<PanelText *>	texts;
		std::string					prompt;
		
		int							heightLine;
		int							tabSize;
		
		int 						currentLine;
		int							currentPos;
		int 						currentCmd;
		std::string					svgCmd;
		
		bool						bIns;
		bool						bRightCtrl;
		bool						bLeftCtrl;
		bool						bCtrl;
};





class PanelScrollY : public PanelSimple {
	public:
		PanelScrollY();
		
		inline void			setDelta( int d )				{ y_delta = d; }
		//void				buildText();
		virtual void		clickUp( int, int);
		virtual void		clickDown( int, int);
		virtual Panel*		isMouseOver( int, int);
		

		virtual void		displayGL();
		virtual void		updatePos();

//	private:
	protected:
		int					y_scroll;
		int					y_delta;
		
};







typedef void (* CB_CMD)(std::string);

class PanelConsoleCallBack	{
	public :
	virtual void					callback_cmd(std::string)=0;
};


class PanelConsole : public PanelSimple	{
	public:

									PanelConsole();
									PanelConsole( int, int );
		
		void						setPrompt( std::string );
		void						setCallBackCmd( CB_CMD );
		void						setCallBackCmd( PanelConsoleCallBack * );

		virtual void				displayGL();
		virtual void				updatePos();
		virtual void				idle(float);

		void						addCmd(std::string cmd);
		
		virtual void				cb_keyboard( unsigned char );
		virtual void				cb_keyboard_special( unsigned char );
		virtual void				cb_keyboard_special_up( unsigned char );
		
		void						moveCursor();
		void						scroll();
		void						rotateBuffer();
		void						addLine();
		void						eraseTexts();
		void						clear();
		void						affiche( std::string * );
		void						affiche( std::string );
		void						addChar( char );
		void						supChar();
		void						delChar();
		void						supWord();
		void						delWord();
		
		void						incCursor();
		void						decCursor();
		int							posWordPrec();
		int							posWordSuiv();
		void						wordPrec();
		void						wordSuiv();
		void						setTabSize( int );
		
	private:
		int							maxCmd;
		std::vector<std::string*> 	cmds;
		std::vector<PanelText *>	texts;
		std::string					prompt;
		PanelText					cursor;
		CB_CMD						cb_cmd;
		PanelConsoleCallBack*		ppccb;
		
		int							heightLine;
		int							tabSize;
		
		int 						currentLine;
		int							currentPos;
		int 						currentCmd;
		std::string					svgCmd;
		
		float						cursorTime;
		bool						bIns;
		bool						bRightCtrl;
		bool						bLeftCtrl;
		bool						bCtrl;
};






class PanelButton;

typedef void (* CALLBACK_MOUSE)(PanelButton*);
//typedef void (* CALLBACK_DOWN)(PanelButton*);

class PanelButtonCallBack	{
	public :
	virtual void					cb_button_mouse_over(PanelButton*)				{;};
	virtual void					cb_button_mouse_down(PanelButton*)				{;};
	virtual void					cb_button_mouse_up(PanelButton*)				{;};
};


class PanelButton : public PanelSimple {
	public:
		PanelButton();
		PanelButton( PanelButtonCallBack* );
		void init();
		
		
		virtual void		haveFocus();
		virtual void		lostFocus();
		virtual void		clickLeft( int, int);
		virtual void		releaseLeft( int, int);
		virtual Panel*		isMouseOver( int, int);
		virtual void		displayGL();
		virtual void		updatePos();
		void				debug(bool);
		void				setUp( char *);
		void				setDown( char *);
		void				setOver( char *);
		
		void				setCallBackOver( CALLBACK_MOUSE p)		{ pCallBackOver = p; }
		void				setCallBackUp( CALLBACK_MOUSE p)		{ pCallBackUp = p; }
		void				setCallBackDown( CALLBACK_MOUSE p)		{ pCallBackDown = p; }

	protected:
		PanelButtonCallBack*	pPanelButtonCallBack;
		_Texture2D*				m_pTexUp;
		_Texture2D*				m_pTexDown;
		_Texture2D*				m_pTexOver;

		_Texture2D*				m_pTexCurrent;
		
		CALLBACK_MOUSE			pCallBackDown;
		CALLBACK_MOUSE			pCallBackUp;
		CALLBACK_MOUSE			pCallBackOver;;
};







typedef void (* CALLBACK_TRUE)();
typedef void (* CALLBACK_FALSE)();




class PanelCheckBox : public PanelSimple {
	public:
		PanelCheckBox();
		
		
		//void				buildText();

		virtual void		clickLeft( int, int);
		virtual Panel*		isMouseOver( int, int);
		virtual void		displayGL();
		virtual void		updatePos();
		void				debug(bool);
		void				setFalse( char *);
		void				setTrue( char *);
		
		void				setCallBackTrue( CALLBACK_TRUE p)		{ pCallBackTrue  = p; }
		void				setCallBackFalse( CALLBACK_FALSE p)		{ pCallBackFalse = p; }
		
		bool				getVal()								{ return bVal; }
		void				setPercent( float );
		inline void			setVal( bool b )						{ bVal = b; }
		inline void			setListener( bool* p)					{ pVal = p; }
		inline void			setUseInverse( bool p)					{ bUseInverse = p; }

	protected:
		_Texture2D*		m_pTexTrue;
		_Texture2D*		m_pTexFalse;

		CALLBACK_TRUE	pCallBackTrue;;
		CALLBACK_FALSE	pCallBackFalse;
		
		bool			bVal;
		bool*			pVal;
		bool			bUseInverse;
		
		float			percent;
//	private:
		/*	
	protected:
		_Texture2D*		m_pTexBackground;
		
		bool							bTextOK;
		void * 							cTextObj;
		TextUtil*						textUtil;
		std::vector<std::string>		str;

		//bool							bDebug;				in Panel.h
		PanelSimple*					pPsDebug;
		PanelText*						pPtDebug;
		std::string						sDebug;
		*/
};






class PanelWindow : public PanelSimple {
	public:
		enum SKIN { STD, RED, BLACK };
	
		PanelWindow();
		
		void				loadSkin( std::string );
		void				loadSkin( SKIN );
		
		void				setPosAndSize(int, int, int, int);
		void				setPos(int, int);
		void				setdSize(int, int);

		inline void 		setBorderSize( int n )											{ borderSize = n; };
		inline int	 		getBorderSize()													{ return borderSize; };

		void				displayGLtex( _Texture2D* , float, float, float, float );

		virtual void		displayGL();
		virtual void		updatePos();
		
	private:
		_Texture2D*			m_tex_tl;
		_Texture2D*			m_tex_tr;
		_Texture2D*			m_tex_bl;
		_Texture2D*			m_tex_br;
		_Texture2D*			m_tex_t;
		_Texture2D*			m_tex_b;
		_Texture2D*			m_tex_l;
		_Texture2D*			m_tex_r;
		
		int					borderSize;
};













class WindowsManager {

public:


	void				init();
	void				setScreenSize(int, int);
	void				add(Panel *);
	void				sup(Panel *);
	void				supByID(int);
	Panel*				getByID(int);
	int					getFreeID();
	Panel*				getParentScroll(Panel*);
	Panel*				getParentRoot(Panel*);
	Panel*				findPanelMouseOver(int, int);
	int					getOrder(Panel*);
	void				movePanel( int, int);
	void				movePanel( int, int, Panel* );
	void				swapVisible();
	void				debug(bool);
	void				debug();
	
	/*
	void			setWidth( int);
	int				getWidth();
	void			setHeight( int );
	int				getHeight();
	Font*			getFonts();
	int				getOffsetX();
	int				getOffsetY();
	*/


	inline void			setDebug( bool b )				{debug(b);}
	inline int			getDebug()						{return bDebug;}
	inline void			setWidth( int w)				{width=w;}
	inline int			getWidth()						{return width;}
	inline void			setHeight( int h )				{height=h;}
	inline int			getHeight()						{return height;}
	inline Font*		getFonts()						{return fonts;}
	inline int			getOffsetX()					{return 0;}
	inline int			getOffsetY()					{return 0;}
	inline TextUtil*	getTextUtil()					{return &textUtil;}
	inline void			setSlot( int s )				{slot=s;}
	inline int			getSlot()						{return slot;}

	void				ChangeViewport(int, int, int, int, int, int);

	void				idleGL(float);
	void				displayGL();
	void				clearBufferGL();
	void				clearBufferGL( GLbitfield );
		
	void				passiveMotionFunc(int, int);
	void				motionFunc(int, int);
	void				mouseFunc(int, int, int, int);
	void				keyboardUpFunc( unsigned char, int, int );
	void				keyboardFunc( unsigned char, int, int );
	void				keyboardSpecialUpFunc( unsigned char, int, int );
	void				keyboardSpecialFunc( unsigned char, int, int );

	void				call_back_keyboard( Panel * );
	

	inline static WindowsManager&	getInstance()			{ if (!instance) instance = new WindowsManager();return *instance;}
	inline static void				Destroy()				{ if (instance) delete instance;instance=0;}
	
	WindowsManager();
	WindowsManager(int, int);
	~WindowsManager();

	int					iTest;

				
private:
	static WindowsManager*	instance;
	
	int 						width;
	int							height;
	std::vector<Panel*>			childs;
	//std::vector<Panel*>		panels;
	Font*						fonts;
	
	int							xm_old;
	int							ym_old;
				
	void * 						cTextObj;
	TextUtil					textUtil;
	std::string					str[10];

	bool						bMovePanel;
	Panel*						panelMove;
	Panel*						panelFocus;
	std::vector<Panel *>		panels_cbKey;
	
	bool						bDebug;
	int							slot;
	
private:
	void						changeFocus(Panel *);
};





#endif
